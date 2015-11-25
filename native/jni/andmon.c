/*
 * Copyright (c) 2015 droidwolf(droidwolf2006@gmail.com)
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
#include "andmon.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <android/log.h>
#include <sys/system_properties.h>

#define INOTIFY_BUFF_SIZE       8
#define VERSION                 "0.0.1"
#define PROC_NAME               "droidwolf.mon"
#define STR_EQUAL(s1, s2)       (strcmp(s1,s2)==0)
#define F_EXITS(path)           (access(path,F_OK)==0)
#define STR_EMPTY(str)          (str==NULL||strlen(str)==0)
#define LOGI(...)               __android_log_print(ANDROID_LOG_INFO, PROC_NAME, __VA_ARGS__)
#define STRCAT(s1, s2)          strcat( strcpy((char *)malloc((strlen(s1) + strlen(s2))*sizeof(char) + sizeof(char)), s1),s2 )
#define SUMMARY                 "-log output log\
                            \n-pkg package full name\
                            \n-pid monitor proc pid[need set -svr]\
                            \n-url Uninstall launch broswer url\
                            \n-svr monitor service class full name"

extern char *__progname;
int _log = 0;
int _sdk_ver=0;
char *_pkg = NULL;
char *_url = NULL;
char *_svr = NULL;
char *_pidPath = NULL;
char *_pkgPath = NULL;
int _fd, _wd_pid, _wd_pkg;                         

#if defined(__aarch64__)||defined(__x86_64__)||defined(__mips64)
int is_cmm(const char *str) {
	char ch;
	while ((ch = *str++) != '\0') {
		if(ch=='#') {
			return 1;
		}
		if(ch == ' ' || ch == '\t'|| ch == '\r'){
			continue;
		}
		break;
	}
	return 0;
}

int get_sdk_ver() {
	if (_sdk_ver > 0) {
		return _sdk_ver;
	}
	FILE *fp = fopen("/system/build.prop", "r");
	if (fp != NULL) {
		int ver = 0;
		char buff[256] = {0};
		while (!feof(fp)) {
			if (fgets(buff, sizeof(buff), fp)==NULL || is_cmm(buff)){
				continue;
			}
			int n = sscanf(buff, "ro.build.version.sdk=%d", &ver);
			if (n > 0 && ver > 0) {
				_sdk_ver = ver;
				break;
			}
		}
		fclose(fp);
	}
	return _sdk_ver;
}
#else
int get_sdk_ver() {
	if (_sdk_ver < 1) {
		char buff[10] = {0};
		__system_property_get("ro.build.version.sdk", buff);
		_sdk_ver = strlen(buff) > 0 ? atoi(buff) : 0;
	}
	return _sdk_ver;
}
#endif

int main(int argc, char *argv[]) {
    if (argc < 1 || argv == NULL) {
        LOGI(SUMMARY);
        return 0;
    }
    get_args(argc, argv);
    if (STR_EMPTY(_pkg) || ((STR_EMPTY(_pidPath) || STR_EMPTY(_svr)) && STR_EMPTY(_url))) {
        LOGI(SUMMARY);
        return 0;
    }
    if (_log){
        LOGI("start andmon v%s...", VERSION);
    }
    if (!STR_EMPTY(_url)) {
        _pkgPath = STRCAT("/data/data/", _pkg);
    }

    if (daemon(1, 1) < 0) {
        if (_log){
            LOGI("main: daemon failed!");
        }
    }
    kill_prev(PROC_NAME);
    get_sdk_ver();
    mon(mon_cb);
}

void mon(mon_callback callback) {
    if (_fd <1 && (_fd = inotify_init()) < 1) {
       if (_log){
           LOGI("mon--_fd return %d", _fd);
       }
       return;
    }
    //uninstall
    if (!STR_EMPTY(_url)) {
        _wd_pkg = inotify_add_watch(_fd, _pkgPath, IN_DELETE);
       if (_wd_pkg<1 && _log){
           LOGI("mon--IN_DELETE return %d,%s", _wd_pkg, _pkgPath);
       }
    }

    //process
    if (!STR_EMPTY(_svr)) {
        _wd_pid = inotify_add_watch(_fd, _pidPath, IN_CLOSE_NOWRITE);
       if (_wd_pid <1 && _log){
           LOGI("mon--IN_CLOSE_NOWRITE return %d,%s", _wd_pid, _pidPath);
       }
    }
    if (_wd_pkg > 0 || _wd_pid > 0) {
        mon_ob(callback);
    }
    mon_close();
}

void mon_close(){
    if(_fd>0){
        if (_wd_pid > 0){
            inotify_rm_watch((int) _fd, (uint32_t) _wd_pid);
        }
        if (_wd_pkg > 0){
            inotify_rm_watch((int) _fd, (uint32_t) _wd_pkg);
        }
		close(_fd);
        _wd_pid=_wd_pkg=_fd=0;
	}
    if (_log){
        LOGI("mon--exit!");
    }
}

int mon_cb(struct inotify_event *event) {
    if (!((event->mask & IN_DELETE) == IN_DELETE || (event->mask & IN_CLOSE_NOWRITE) == IN_CLOSE_NOWRITE)){
        return 0;
    }
    sleep(3);
    if (_wd_pkg > 0 && !F_EXITS(_pkgPath)) {
        mon_close();
        start_bro(_url);
    } else if (_wd_pid > 0 && !F_EXITS(_pidPath)) {
        mon_close();
        char buf[256] = {0};
        sprintf(buf, "%s/%s", _pkg, _svr);
        start_svr(buf);
    }

	return (_wd_pid == 0 && _wd_pkg == 0)? 1:0;
}

void mon_ob(mon_callback callback) {
    char buff[sizeof(struct inotify_event) * INOTIFY_BUFF_SIZE]={0};
    for (;;) {
        int n = read(_fd, buff, sizeof(buff));
        if (n < sizeof(struct inotify_event)) {
            if (errno == EINTR){
                continue;
            }
            return;
        }
        int pos = 0;
        while (n >= sizeof(struct inotify_event)) {
            struct inotify_event *event = (struct inotify_event *) (buff + pos);
            if(callback(event)){
                return;
            }
            int size = sizeof(struct inotify_event) + event->len;
            n -= size;
            pos += size;
        }
    }
}
void get_args(int argc, char **argv) {
    char *pid=NULL;
    for (int i = 0; i < argc; i++) {
        char *arg = argv[i];
        if (arg[0] != '-'){
            continue;
        }
        if (STR_EQUAL("-log", arg)) {
            _log = 1;
        } else if (STR_EQUAL("-pid", arg)) {
            pid=argv[i + 1];
            _pidPath = STRCAT("/proc/", pid);
        } else if (STR_EQUAL("-pkg", arg)) {
            _pkg = strdup(argv[i + 1]);
        } else if (STR_EQUAL("-url", arg)) {
            _url = strdup(argv[i + 1]);
        } else if (STR_EQUAL("-svr", arg)) {
            _svr = strdup(argv[i + 1]);
        }
    }
    if (_log) {
        LOGI("get_args--pid=%s\n_pkg=%s\n_url=%s\n_svr=%s", pid, _pkg, _url, _svr);
    }
}
void start_svr(const char *svr) {
    if (_log){
        LOGI("start_svr--%s", svr);
    }
    if (get_sdk_ver() >= 17) {
        execlp("am", "am", "startservice", "--user", "0", "-n", svr, NULL);
    } else {
        execlp("am", "am", "startservice", "-n", svr, NULL);
    }
}

void start_bro(const char *url) {
    if (_log){
        LOGI("start_bro--%s", url);
    }
    if (get_sdk_ver() >= 17) {
        execlp("am", "am", "start", "--user", "0", "-a", "android.intent.action.VIEW", "-d", url,NULL);
    } else {
        execlp("am", "am", "start", "-a", "android.intent.action.VIEW", "-d", url, NULL);
    }
}
void kill_prev(const char *name) {
    char buff[512] = "ps|grep ";
    if (!STR_EMPTY(name)) {
        strcat(buff, name);
    } else if (!STR_EMPTY(__progname)) {
        strcat(buff, __progname);
    } else {
        return;
    }

    FILE *fp= popen(buff, "r");
    if (fp== NULL) {
        return;
    }
    pid_t cpid = getpid();
    while (fgets(buff, sizeof(buff), fp)) {
        int pid = 0, ppid = 0;
        char name[128] = {0};
        int n = sscanf(buff, "%*s %d %d %*s %*s %*s %*s %*s %s", &pid, &ppid, name);
        if (n < 3 || pid == cpid){
            continue;
        }
       if (_log){
           LOGI("kill_prev: pid=%d, ppi=%d, proc=%s", pid, ppid, name);
       }
       kill(pid, SIGKILL);
    }
    fclose(fp);
}