package com.lostpeter.opengles.opengles_001_triangle;

import android.app.NativeActivity;
import android.os.Bundle;
import android.content.Context;
import android.view.inputmethod.InputMethodManager;
import android.view.KeyEvent;
import java.util.concurrent.LinkedBlockingQueue;

public class MainActivity extends NativeActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
    
}