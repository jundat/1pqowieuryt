/****************************************************************************
 Copyright (c) 2010-2011 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
package com.chimgokien.phicongbutchi;

// Cocos2d-x
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.concurrent.atomic.AtomicInteger;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Vibrator;
import android.util.Base64;
import android.util.Log;
import android.view.View;

import com.chimgokien.phicongbutchi.classes.AndroidNDKHelper;
import com.ezibyte.social.EziSocialManager;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.gcm.GoogleCloudMessaging;
// Required to get hash-kay for Facebook app. Copy the generated hash key and add it to your Facebook App. Then you can delete the code related to hash key.
//NEW
//END NEW
// EziSocial Plugin

public class PhiCongButChi extends Cocos2dxActivity {

    public static final String EXTRA_MESSAGE = "message";
    public static final String PROPERTY_REG_ID = "registration_id";
    private static final String PROPERTY_APP_VERSION = "appVersion";
    private static final int PLAY_SERVICES_RESOLUTION_REQUEST = 9000;
    

    /**
     * Substitute you own sender ID here. This is the project number you got
     * from the API Console, as described in "Getting Started."
     */
    String SENDER_ID = "415130126994";// "1063175715204"; //Google Cloud Project Id
    String TAG_PUSH = "PUSH";

    GoogleCloudMessaging gcm;
    AtomicInteger msgId = new AtomicInteger();
    Context context;

    String regid;
    
    ////////////////////////////////////////////////////////////////////////
	
	String TAG = "JAVA_CALL";
	
	public void Rate(JSONObject prms)
	{
		String game_name, game_package, msg, rate, later, no;
		try {
			game_name = prms.getString("game_name");
			game_package = prms.getString("game_package");
			msg = prms.getString("msg");
			rate = prms.getString("rate");
			later = prms.getString("later");
			no = prms.getString("no");
						
			AppRater.showRateDialog(this, null, this, game_name, game_package, msg, rate, later, no);
			
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}
	
	public void onRateCompleted(String responseType) {
		String jsonStr = "{\"isSuccess\" : true, " +
        		"\"responseType\": \"" + responseType + "\"" +
        		"}";
        JSONObject prmsToSend = null;
        
        try {
			prmsToSend = new JSONObject(jsonStr);
		}
        catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
    	AndroidNDKHelper.SendMessageWithParameters("onRateCompleted", prmsToSend);
	}
	
	public void GetRegistrationId(JSONObject prms) {
		Log.i(TAG_PUSH, "GetRegistrationId");
		
		String jsonStr = "{\"isSuccess\" : true, " +
        		"\"registrationId\" : \"" + regid + "\"}";
        JSONObject prmsToSend = null;
        Log.i(TAG_PUSH, jsonStr);
        try {
			prmsToSend = new JSONObject(jsonStr);
		}
        catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.i(TAG_PUSH, "NULL params");
		}
        
    	AndroidNDKHelper.SendMessageWithParameters("onGetRegistrationIdCompleted", prmsToSend);
	}
	
	public void Vibrate(JSONObject prms) {
		Vibrator v = (Vibrator) this.context.getSystemService(Context.VIBRATOR_SERVICE);
		// Vibrate for 500 milliseconds
		v.vibrate(300);
	}
	
	///////////////////////////////////////////////////////////////
	
    protected void onCreate(Bundle savedInstanceState)
    {
		Log.i(TAG, "onCreate");
		super.onCreate(savedInstanceState);
        
        // Try to get your hash key
        try {
		    PackageInfo info = getPackageManager().getPackageInfo(this.getPackageName(), PackageManager.GET_SIGNATURES);
		    for (Signature signature : info.signatures) {
		        MessageDigest md = MessageDigest.getInstance("SHA");
		        md.update(signature.toByteArray());
		        Log.e("MY KEY HASH:", Base64.encodeToString(md.digest(), Base64.DEFAULT));
		    }
		}
        catch (NameNotFoundException e)
        {
            
		}
        catch (NoSuchAlgorithmException e)
        {
		}
        
        AndroidNDKHelper.SetNDKReciever(this);
        
        // Initialize EziSocial Plugin Manager for Android.
        // ToDo Make sure you add the Facebook App ID in strings.xml file.
        EziSocialManager.initWithActivity(this,
                                          this.getApplicationContext().getString(R.string.app_id),
                                          true, // Set to see ezisocial plugin logs
                                          savedInstanceState
                                          );
        

        /////////////////////////////////////////////////////////////////
        //PUSH
        
        context = getApplicationContext();

        // Check device for Play Services APK. If check succeeds, proceed with GCM registration.
        if (checkPlayServices()) {
            gcm = GoogleCloudMessaging.getInstance(this);
            regid = getRegistrationId(context);

            if (regid == null || regid == "") {
                registerInBackground();
            }
        } else {
            Log.i(TAG_PUSH, "No valid Google Play Services APK found.");
        }        
        /////////////////////////////////////////////////////////////////
	}
    

    /**
     * Check the device to make sure it has the Google Play Services APK. If
     * it doesn't, display a dialog that allows users to download the APK from
     * the Google Play Store or enable it in the device's system settings.
     */
    private boolean checkPlayServices() {
		Log.i(TAG_PUSH, "checkPlayServices");
        int resultCode = GooglePlayServicesUtil.isGooglePlayServicesAvailable(this);
        if (resultCode != ConnectionResult.SUCCESS) {
            if (GooglePlayServicesUtil.isUserRecoverableError(resultCode)) {
                GooglePlayServicesUtil.getErrorDialog(resultCode, this,
                        PLAY_SERVICES_RESOLUTION_REQUEST).show();
            } else {
                Log.i(TAG_PUSH, "This device is not supported.");
                finish();
            }
            return false;
        }
        return true;
    }

    /**
     * Stores the registration ID and the app versionCode in the application's
     * {@code SharedPreferences}.
     *
     * @param context application's context.
     * @param regId registration ID
     */
    private void storeRegistrationId(Context context, String regId) {
		Log.i(TAG_PUSH, "storeRegistrationId");
        final SharedPreferences prefs = getGcmPreferences(context);
        int appVersion = getAppVersion(context);
        Log.i(TAG_PUSH, "Saving regId on app version " + appVersion);
        SharedPreferences.Editor editor = prefs.edit();
        editor.putString(PROPERTY_REG_ID, regId);
        editor.putInt(PROPERTY_APP_VERSION, appVersion);
        editor.commit();
    }

    /**
     * Gets the current registration ID for application on GCM service, if there is one.
     * <p>
     * If result is empty, the app needs to register.
     *
     * @return registration ID, or empty string if there is no existing
     *         registration ID.
     */
    private String getRegistrationId(Context context) {
		Log.i(TAG_PUSH, "getRegistrationId");
        final SharedPreferences prefs = getGcmPreferences(context);
        String registrationId = prefs.getString(PROPERTY_REG_ID, "");
        if (registrationId == null || registrationId == "") {
            Log.i(TAG_PUSH, "Registration not found.");
            return "";
        }
        // Check if app was updated; if so, it must clear the registration ID
        // since the existing regID is not guaranteed to work with the new
        // app version.
        int registeredVersion = prefs.getInt(PROPERTY_APP_VERSION, Integer.MIN_VALUE);
        int currentVersion = getAppVersion(context);
        if (registeredVersion != currentVersion) {
            Log.i(TAG_PUSH, "App version changed.");
            return "";
        }
        
        Log.i("Registragion Id", registrationId);
        return registrationId;
    }

    /**
     * Registers the application with GCM servers asynchronously.
     * <p>
     * Stores the registration ID and the app versionCode in the application's
     * shared preferences.
     */
    private void registerInBackground() {
		Log.i(TAG_PUSH, "registerInBackground");
        new AsyncTask<Void, Void, String>() {
            @Override
            protected String doInBackground(Void... params) {
                String msg = "";
                try {
                    if (gcm == null) {
                        gcm = GoogleCloudMessaging.getInstance(context);
                    }
                    regid = gcm.register(SENDER_ID);
                    msg = "Device registered, registration ID=" + regid;

                    // You should send the registration ID to your server over HTTP, so it
                    // can use GCM/HTTP or CCS to send messages to your app.
                    sendRegistrationIdToBackend(regid);

                    // For this demo: we don't need to send it because the device will send
                    // upstream messages to a server that echo back the message using the
                    // 'from' address in the message.

                    // Persist the regID - no need to register again.
                    storeRegistrationId(context, regid);
                } catch (IOException ex) {
                    msg = "Error :" + ex.getMessage();
                    // If there is an error, don't just keep trying to register.
                    // Require the user to click a button again, or perform
                    // exponential back-off.
                }
                return msg;
            }

            @Override
            protected void onPostExecute(String msg) {
                Log.i(TAG_PUSH, "onPostExecute: " + msg);
            }
        }.execute(null, null, null);
    }

    // Send an upstream message.
    public void onClick(final View view) {
		Log.i(TAG_PUSH, "onClick");
    	new AsyncTask<Void, Void, String>() {
            @Override
            protected String doInBackground(Void... params) {
                String msg = "";
                try {
                    Bundle data = new Bundle();
                    data.putString("my_message", "Hello World");
                    data.putString("my_action", "com.google.android.gcm.demo.app.ECHO_NOW");
                    String id = Integer.toString(msgId.incrementAndGet());
                    
                    gcm.send(SENDER_ID + "@gcm.googleapis.com", id, data);
                    
                    msg = "Sent message";
                } catch (IOException ex) {
                    msg = "Error :" + ex.getMessage();
                }
                return msg;
            }

            @Override
            protected void onPostExecute(String msg) {
                Log.i(TAG_PUSH, "onPostExecute: " + msg);
            }
        }.execute(null, null, null);
    }


    /**
     * @return Application's version code from the {@code PackageManager}.
     */
    private static int getAppVersion(Context context) {
		Log.i("TAG_PUSH_STATIC", "getAppVersion");
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
            return packageInfo.versionCode;
        } catch (NameNotFoundException e) {
            // should never happen
            throw new RuntimeException("Could not get package name: " + e);
        }
    }

    /**
     * @return Application's {@code SharedPreferences}.
     */
    private SharedPreferences getGcmPreferences(Context context) {
		Log.i(TAG_PUSH, "getGcmPreferences");
        // This sample app persists the registration ID in shared preferences, but
        // how you store the regID in your app is up to you.
		
		Log.i(TAG_PUSH, "NAME: " + PhiCongButChi.class.getSimpleName());
		
        return getSharedPreferences(PhiCongButChi.class.getSimpleName(),
                Context.MODE_PRIVATE);
    }
    
    /**
     * Sends the registration ID to your server over HTTP, so it can use GCM/HTTP or CCS to send
     * messages to your app. Not needed for this demo since the device sends upstream messages
     * to a server that echoes back the message using the 'from' address in the message.
     */
    private void sendRegistrationIdToBackend(String regid) {
		
    }
    
    //END PUSH
    ////////////////////////////////////////////////////////////////////
    
    public Cocos2dxGLSurfaceView onCreateView()
    {
		Log.i(TAG, "onCreateView");
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// PhiCongButChi should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }
    
    @Override
	protected void onResume()
	{
    	Log.i(TAG, "onResume");
		super.onResume();
		EziSocialManager.applicationResumed();
		
		//PUSH
        // Check device for Play Services APK.
        checkPlayServices();
	}
    
	@Override
	public void onPause()
	{
		Log.i(TAG, "onPause");
		super.onPause();
		EziSocialManager.applicationPaused();
	}
    
	@Override
	public void onDestroy()
	{
		Log.i(TAG, "onDestroy");
		super.onDestroy();
		EziSocialManager.applicationDestroyed();
	}
    
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		Log.i(TAG, "onActivityResult");
		super.onActivityResult(requestCode, resultCode, data);
		EziSocialManager.onActivityResult(requestCode, resultCode, data);
	}
    
    static
    {
        System.loadLibrary("cocos2dcpp");
    }
}
