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
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
// Required to get hash-kay for Facebook app. Copy the generated hash key and add it to your Facebook App. Then you can delete the code related to hash key.
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.Toast;

// EziSocial Plugin
import com.ezibyte.social.EziSocialManager;
import com.facebook.Session;
import com.facebook.Session.NewPermissionsRequest;
import com.facebook.SessionState;
import com.facebook.Session.StatusCallback;
import com.facebook.android.AsyncFacebookRunner;
import com.facebook.android.AsyncFacebookRunner.RequestListener;
import com.facebook.android.DialogError;
import com.facebook.android.Facebook;
import com.facebook.android.Facebook.DialogListener;
import com.facebook.android.FacebookError;

public class PhiCongButChi extends Cocos2dxActivity implements StatusCallback{
	
	boolean isLogin = false;
	
	
	// Your Facebook APP ID
    private static String APP_ID = "613622542044186"; // Replace your App ID here
 
    private Session fbSession;
    
    // Instance of Facebook Class
    private Facebook facebook;
    private AsyncFacebookRunner mAsyncRunner;
    String FILENAME = "AndroidSSO_data";
    private SharedPreferences mPrefs;

	private FrameLayout rootView = null;

	@Override
	public void call(Session session, SessionState state, Exception exception) {
		// TODO Auto-generated method stub
		
	}
	
	
    @SuppressWarnings("deprecation")
	public void initFacebook() {
    	fbSession = new Session(PhiCongButChi.getContext());
    	fbSession.addCallback(this);
    	
    	
    	
    	facebook = new Facebook(APP_ID);
        mAsyncRunner = new AsyncFacebookRunner(facebook);
    }
    
    @SuppressWarnings("deprecation")
	public void loginToFacebook() {
    	fbSession.requestNewPublishPermissions(new NewPermissionsRequest(this, "publish_stream"));
    	
    	
    	
//        mPrefs = getPreferences(MODE_PRIVATE);
//        String access_token = mPrefs.getString("access_token", null);
//        long expires = mPrefs.getLong("access_expires", 0);
//     
//        if (access_token != null) {
//            Log.i("access_token != null", "");
//            facebook.setAccessToken(access_token);
//        }
//     
//        if (expires != 0) {
//            Log.i("expires != 0", "");
//            facebook.setAccessExpires(expires);
//        }
//     
//        Log.i("Log in here +================================", "Log In");
//        
//        if (!facebook.isSessionValid()) {
//            facebook.authorize(this,
//                    new String[] {"publish_stream"},
//                    new DialogListener() {
//     
//                        @Override
//                        public void onCancel() {
//                            // Function to handle cancel event
//                        	Log.i("On Cancel +================================", "Log In");
//                        }
//     
//                        @Override
//                        public void onComplete(Bundle values) {
//                        	
//                        	Log.i("On complete +================================", "Log In");
//                        	
//                            // Function to handle complete event
//                            // Edit Preferences and update facebook acess_token
//                            SharedPreferences.Editor editor = mPrefs.edit();
//                            editor.putString("access_token", facebook.getAccessToken());
//                            editor.putLong("access_expires", facebook.getAccessExpires());
//                            editor.commit();
//                        }
//     
//                        @Override
//                        public void onError(DialogError error) {
//                            // Function to handle error
//                        	Log.i("On Error +================================", "Log In");
//                        }
//     
//                        @Override
//                        public void onFacebookError(FacebookError fberror) {
//                            // Function to handle Facebook errors
//                        	Log.i("On Facebook Error +================================", "Log In");
//                        }
//     
//                    });
//        } else {
//        	Log.i("facebook.isSessionValid()", "");
//        }
    }
	
    @SuppressWarnings("deprecation")
	public void postToWall() {
        // post on user's wall.
        facebook.dialog(this, "feed", new DialogListener() {
     
            @Override
            public void onFacebookError(FacebookError e) {
            }
     
            @Override
            public void onError(DialogError e) {
            }
     
            @Override
            public void onComplete(Bundle values) {
            }
     
            @Override
            public void onCancel() {
            }
        });
     
    }
    
    @SuppressWarnings("deprecation")
	public void getProfileInformation() {
        mAsyncRunner.request("me", new RequestListener() {
            @Override
            public void onComplete(String response, Object state) {
                Log.d("Profile", response);
                String json = response;
                try {
                    JSONObject profile = new JSONObject(json);
                    // getting name of the user
                    final String name = profile.getString("name");
                    // getting email of the user
                    final String email = profile.getString("email");
     
                    runOnUiThread(new Runnable() {
     
                        @Override
                        public void run() {
                            Toast.makeText(getApplicationContext(), "Name: " + name + "\nEmail: " + email, Toast.LENGTH_LONG).show();
                        }
     
                    });
     
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
     
            @Override
            public void onIOException(IOException e, Object state) {
            }
     
            @Override
            public void onFileNotFoundException(FileNotFoundException e, Object state) {
            }
     
            @Override
            public void onMalformedURLException(MalformedURLException e, Object state) {
            }
     
            @Override
            public void onFacebookError(FacebookError e, Object state) {
            }
        });
    }
    
    @SuppressWarnings("deprecation")
	public void logoutFromFacebook() {
        mAsyncRunner.logout(this, new RequestListener() {
            @Override
            public void onComplete(String response, Object state) {
                Log.d("Logout from Facebook", response);
                if (Boolean.parseBoolean(response) == true) {
                    // User successfully Logged out
                }
            }
     
            @Override
            public void onIOException(IOException e, Object state) {
            }
     
            @Override
            public void onFileNotFoundException(FileNotFoundException e,
                    Object state) {
            }
     
            @Override
            public void onMalformedURLException(MalformedURLException e,
                    Object state) {
            }
     
            @Override
            public void onFacebookError(FacebookError e, Object state) {
            }
        });
    }
    
    private FrameLayout GetRootView()
	{
		if (this.rootView == null)
		{
			this.rootView = (FrameLayout)this.getWindow().getDecorView().findViewById(android.R.id.content);
		}
		return this.rootView;
	}
    
    public void AddButton()
    {
        Button tapButton = new Button(this);
        tapButton.setText("Log In");
        tapButton.setLayoutParams(new LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT));
        
        tapButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v)
			{
				if(isLogin == false) {
					
					Log.i("Call Log In", "Log In");
					
					PhiCongButChi.this.loginToFacebook();
					isLogin = true;
				} else {
					PhiCongButChi.this.getProfileInformation();
					isLogin = false;
				}
				
			}
		});
        
        this.GetRootView().addView(tapButton);
    }
    
    
    
    protected void onCreate(Bundle savedInstanceState)
    {
		super.onCreate(savedInstanceState);
        
        // Try to get your hash key
        try {
		    PackageInfo info = getPackageManager().getPackageInfo("com.chimgokien.phicongbutchi", PackageManager.GET_SIGNATURES);
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
        
        // Initialize EziSocial Plugin Manager for Android.
        // ToDo Make sure you add the Facebook App ID in strings.xml file.
        EziSocialManager.initWithActivity(this,
		                                  this.getApplicationContext().getString(R.string.app_id),
		                                  true, // Set to see ezisocial plugin logs
		                                  savedInstanceState
		                                  );
        initFacebook();
        this.AddButton();
	}
    
    public Cocos2dxGLSurfaceView onCreateView()
    {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// PhiCongButChi should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }
    
    @Override
	protected void onResume()
	{
		super.onResume();
		EziSocialManager.applicationResumed();
	}
    
	@Override
	public void onPause()
	{
		super.onPause();
		EziSocialManager.applicationPaused();
	}
    
	@Override
	public void onDestroy()
	{
		super.onDestroy();
		EziSocialManager.applicationDestroyed();
	}
    
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		super.onActivityResult(requestCode, resultCode, data);
		EziSocialManager.onActivityResult(requestCode, resultCode, data);
        
	}
    
    static
    {
        System.loadLibrary("cocos2dcpp");
    }

}
