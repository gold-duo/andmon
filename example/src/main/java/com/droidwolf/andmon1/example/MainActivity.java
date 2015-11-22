package com.droidwolf.andmon1.example;
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

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;


public class MainActivity extends Activity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        findViewById(R.id.send).setOnClickListener(this);
    }


    @Override
    public void onClick(View v) {
//        read();
        startService(new Intent(this, MyService.class));
    }

    private void read() {
        BufferedReader bufferReader = null;
        try {
            bufferReader = new BufferedReader(new InputStreamReader(new FileInputStream("/system/build.prop")));
            String line = null;
            while ((line = bufferReader.readLine()) != null) {
                Log.d("droidwolf", line);
            }
        } catch (IOException e) {

        } finally {
            safeClose(bufferReader);
        }
    }

    public static void safeClose(Closeable closeable) {
        if (closeable != null) {
            try {
                closeable.close();
            } catch (IOException e) {
            }
        }
    }
}
