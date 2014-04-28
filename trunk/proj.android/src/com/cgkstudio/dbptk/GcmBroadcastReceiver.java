/*
 * Copyright 2013 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.cgkstudio.dbptk;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Vibrator;
import android.support.v4.content.WakefulBroadcastReceiver;
import android.util.Log;


/**
 * This {@code WakefulBroadcastReceiver} takes care of creating and managing a
 * partial wake lock for your app. It passes off the work of processing the GCM
 * message to an {@code IntentService}, while ensuring that the device does not
 * go back to sleep in the transition. The {@code IntentService} calls
 * {@code GcmBroadcastReceiver.completeWakefulIntent()} when it is ready to
 * release the wake lock.
 */

public class GcmBroadcastReceiver extends WakefulBroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
    	
    	//Log.i("NOTIFY", "GcmBroadcastReceiver: onReceive");
    	
    	
        // Explicitly specify that GcmIntentService will handle the intent.
        ComponentName comp = new ComponentName(context.getPackageName(),
                GcmIntentService.class.getName());
        
        // Start the service, keeping the device awake while it is launching.
        startWakefulService(context, (intent.setComponent(comp)));
        setResultCode(Activity.RESULT_OK);
        
        ////////////////////////////////////////////////////////////////////////
        
        //check if app is running
        SharedPreferences sp = context.getSharedPreferences(DBPTK.SAVE_KEY, Context.MODE_PRIVATE);
        boolean isActive = sp.getBoolean(DBPTK.KEY_IS_ACTIVE, false);
        if (isActive) {
			//Log.i("PUSH", "App is active --- ");
		} else {
			//Log.i("PUSH", "App is NOT active --- ");
		}

    	//TEST SEND DATA FROM PUSH TO APP    
//        try {
//        	String jsonStr = "{\"isSuccess\" : true }";    
//        	Log.i("TAG_PUSH", "send push to CPP");
//        	JSONObject prmsToSend = new JSONObject(jsonStr);
//			AndroidNDKHelper.SendMessageWithParameters("onPushNotification", prmsToSend);
//		}
//        catch (Exception e) {
//			e.printStackTrace();
//			Log.i("TAG_PUSH", "Failed: Application is not running!");
//		}
    }
}
