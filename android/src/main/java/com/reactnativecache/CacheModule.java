package com.reactnativecache;

import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;

import java.io.File;

@ReactModule(name = CacheModule.NAME)
public class CacheModule extends ReactContextBaseJavaModule {
    public static final String NAME = "Cache";
    private native void nativeInstall(long jsi, String path);

  public CacheModule(ReactApplicationContext reactContext) {
    super(reactContext);
    }

    @Override
    @NonNull
    public String getName() {
        return NAME;
    }


  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      System.loadLibrary("cpp");
      JavaScriptContextHolder jsContext = getReactApplicationContext().getJavaScriptContextHolder();
      if (jsContext.get() != 0) {
        String path = getReactApplicationContext().getFilesDir() + File.separator + "jsi_cache.db";
        nativeInstall(jsContext.get(), path);
        Log.i(NAME, "Installing cache Success");
        return true;
      } else {
        Log.i(NAME, "Failed Installing cache");
        return false;
      }
    } catch (Exception exception) {
      Log.e(NAME, "Failed to install cache JSI Bingding");
      return false;
    }
  }

}
