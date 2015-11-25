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

#include <sys/inotify.h>
#ifndef ANDMON_H_
#define ANDMON_H_
typedef int(*mon_callback)(struct inotify_event *);
int is_cmm(const char *);
void kill_prev(const char *);
void get_args(int,char **);
void start_svr(const char *);
void start_bro(const char *);
void mon(mon_callback);
void mon_close();
int mon_cb(struct inotify_event *);
void mon_ob(mon_callback);
#endif