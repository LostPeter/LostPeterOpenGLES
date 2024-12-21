package com.lostpeter.opengles.opengles_003_texture;

import android.app.NativeActivity;
import android.os.Bundle;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.view.inputmethod.InputMethodManager;
import android.view.KeyEvent;
import java.util.concurrent.LinkedBlockingQueue;

public class MainActivity extends NativeActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    }
    
}