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

package com.droidwolf;

import android.app.Service;
import android.content.Context;
import android.text.TextUtils;

import java.io.File;

public final class libandmon {
    private static boolean sLibExists = false;
    private static String sSoPath = null;

    private static boolean loadSo(Context context) {
        if (!sLibExists) {
            try {
                final String libpath = System.getProperty("jni.libpath");
                if (libpath == null || libpath.length() < 1) {
                    sSoPath = context.getFilesDir().toString().replace("/files", "/lib/liband_mon.so");
                    System.loadLibrary("and_mon");
                } else {
                    sSoPath = libpath + "/liband_mon.so";
                    System.load(sSoPath);
                }
            } catch (Throwable e) {
            } finally {
                sLibExists = (sSoPath != null && sSoPath.length() > 0) ? new File(sSoPath).exists() : false;
            }
        }

        return sLibExists;
    }


    /**
     * @param context
     * @param pid
     * @param svr
     * @param feedbackUrl
     * @param outputLog
     * @return
     */
    public static boolean start(Context context, int pid, Class<? extends Service> svr, String feedbackUrl, boolean outputLog) {
        return start(context, context.getPackageName(), pid, svr, feedbackUrl, outputLog);
    }

    /**
     * @param context
     * @param pkgName
     * @param pid
     * @param svr
     * @param feedbackUrl
     * @param outputLog
     * @return
     */
    public static boolean start(Context context, String pkgName, int pid, Class<? extends Service> svr, String feedbackUrl, boolean outputLog) {
        return start(context, pkgName, pid, svr.getName(), feedbackUrl, outputLog);
    }

    private static boolean start(Context context, String pkgName, int pid, String svrName, String feedbackUrl, boolean outputLog) {
        if (!loadSo(context.getApplicationContext())) return false;
        try {
            final StringBuilder sb = new StringBuilder(sSoPath);
            if (!TextUtils.isEmpty(pkgName)) sb.append(" -pkg ").append(pkgName);
            if (pid > 0) sb.append(" -pid ").append(pid);

            if (!TextUtils.isEmpty(svrName)) sb.append(" -svr ").append(svrName);
            if (!TextUtils.isEmpty(feedbackUrl)) sb.append(" -url ").append(feedbackUrl);
            if (outputLog) sb.append(" -log");

            if (sb.length() > 0) {
                Runtime.getRuntime().exec(sb.toString());
                return true;
            }
        } catch (Throwable e) {
        }
        return false;
    }

}
