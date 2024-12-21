package com.lostpeter.opengles.opengles_000_window;


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

    // void showSoftInput() {
    //     InputMethodManager inputMethodManager = getSystemService(Context.INPUT_METHOD_SERVICE);
    //     inputMethodManager.showSoftInput(this.window.decorView, 0);
    // }

    // void hideSoftInput() {
    //     InputMethodManager inputMethodManager = getSystemService(Context.INPUT_METHOD_SERVICE);
    //     inputMethodManager.hideSoftInputFromWindow(this.window.decorView.windowToken, 0);
    // }

    // // Queue for the Unicode characters to be polled from native code (via pollUnicodeChar())
    // private LinkedBlockingQueue<Int> unicodeCharacterQueue = new LinkedBlockingQueue<int>();

    // // We assume dispatchKeyEvent() of the NativeActivity is actually called for every
    // // KeyEvent and not consumed by any View before it reaches here
    // @Override
    // Boolean dispatchKeyEvent(KeyEvent event) {
    //     if (event.action == KeyEvent.ACTION_DOWN) {
    //         unicodeCharacterQueue.offer(event.getUnicodeChar(event.metaState));
    //     }
    //     return super.dispatchKeyEvent(event);
    // }

    // int pollUnicodeChar() {
    //     return unicodeCharacterQueue.poll() ?: 0;
    // }

}