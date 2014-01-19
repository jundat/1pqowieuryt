//
//  FlyAndFire.java
//  EasyNDK-for-cocos2dx
//
//  Created by Jundat
//
//

package com.chimgokien.phicongbutchi;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import com.chimgokien.phicongbutchi.classes.AndroidNDKHelper;
import com.facebook.android.AsyncFacebookRunner;
import com.facebook.android.AsyncFacebookRunner.RequestListener;
import com.facebook.android.DialogError;
import com.facebook.android.Facebook;
import com.facebook.android.Facebook.DialogListener;
import com.facebook.android.FacebookError;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
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

public class PhiCongButChi extends Cocos2dxActivity
{
	
	// Your Facebook APP ID
    private static String APP_ID = "613622542044186"; // Replace your App ID here
 
    // Instance of Facebook Class
    private Facebook facebook;
	private AsyncFacebookRunner mAsyncRunner;
    String FILENAME = "AndroidSSO_data";
    private SharedPreferences mPrefs;
	
    
    
    
	private FrameLayout rootView = null;
	
	/* Helper method to get the hold of Cocos2dx Changable View,
	 * You can add others views using this view
	 */
	private FrameLayout GetRootView()
	{
		if (this.rootView == null)
		{
			this.rootView = (FrameLayout)this.getWindow().getDecorView().findViewById(android.R.id.content);
		}
		return this.rootView;
	}
	
	static
	{
        System.loadLibrary("cocos2dcpp");
    }
    
    private void AddButton()
    {
        Button fbLogInBtn = new Button(this);
        fbLogInBtn.setText("LogIn");
        fbLogInBtn.setLayoutParams(new LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT));
        
        fbLogInBtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v)
			{
				// TODO Auto-generated method stub
				PhiCongButChi.this.postMessageOnWall("This is a test message! :3"); // login(); //FbLogIn(); // FbGetProfileInformation(); // FbPostStt(); // 
			}
		});
        
        this.GetRootView().addView(fbLogInBtn, 1);
    }
	
    /** Called when the activity is first created. */
    @SuppressWarnings("deprecation")
	@Override
    public void onCreate(Bundle savedInstanceState)
    {
    	try  {

    	      PackageInfo info = getPackageManager().
    	           getPackageInfo(this.getPackageName(), PackageManager.GET_SIGNATURES);

    	      for (Signature signature : info.signatures) {

    	          MessageDigest md = MessageDigest.getInstance("SHA");
    	          md.update(signature.toByteArray());
    	          Log.d("====Hash Key===", Base64.encodeToString(md.digest(), Base64.DEFAULT));

						AlertDialog.Builder builder = new AlertDialog.Builder(this);
						builder
						.setTitle("Deleting a Contact No")
						.setMessage(Base64.encodeToString(md.digest(), Base64.DEFAULT))
						.setIcon(android.R.drawable.ic_dialog_alert)
						.setPositiveButton("Yes", new DialogInterface.OnClickListener() 
						{
						    public void onClick(DialogInterface dialog, int which) 
						    {       
						           //do some thing here which you need
						    }
						});             
						builder.setNegativeButton("No", new DialogInterface.OnClickListener() 
						{
						    public void onClick(DialogInterface dialog, int which) 
						    {   
						    	dialog.dismiss();
						    }
						});
						
						AlertDialog alert = builder.create();
						alert.show();
    	      }
    	  } catch (NameNotFoundException e) {
    	      e.printStackTrace();
    	  } catch (NoSuchAlgorithmException ex) {
    	      ex.printStackTrace();
    	  }
    	
    	
    	
        super.onCreate(savedInstanceState);
        AndroidNDKHelper.SetNDKReciever(this);
        
    	facebook = new Facebook(APP_ID);
        mAsyncRunner = new AsyncFacebookRunner(facebook);

        this.AddButton();
    }
    
    
    public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        // FlyAndFire should create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
        
        return glSurfaceView;
    }

    //Old code

    @SuppressWarnings("deprecation")
	public void FbLogIn()
    {
    	
    	mPrefs = getPreferences(MODE_PRIVATE);
        String access_token = mPrefs.getString("access_token", null);
        long expires = mPrefs.getLong("access_expires", 0);
     
        if (access_token != null) {
            facebook.setAccessToken(access_token);
        }
     
        if (expires != 0) {
            facebook.setAccessExpires(expires);
        }
     
        if (!facebook.isSessionValid()) {
            facebook.authorize(this,
                    new String[] { "email", "publish_stream", "read_friendlists", "publish_actions" },
                    new DialogListener() {
     
                        @Override
                        public void onCancel() {
                            // Function to handle cancel event
                        }
     
                        @Override
                        public void onComplete(Bundle values) {
                            // Function to handle complete event
                            // Edit Preferences and update facebook acess_token
                            SharedPreferences.Editor editor = mPrefs.edit();
                            editor.putString("access_token",
                                    facebook.getAccessToken());
                            editor.putLong("access_expires",
                                    facebook.getAccessExpires());
                            editor.commit();
                        }
     
                        @Override
                        public void onError(DialogError error) {
                            // Function to handle error
     
                        }
     
                        @Override
                        public void onFacebookError(FacebookError fberror) {
                            // Function to handle Facebook errors
     
                        }
                    });
        }
    }
    
    @SuppressWarnings("deprecation")
	public void FbPostStt() {

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
	public void FbGetProfileInformation() {
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

    @SuppressWarnings("deprecation")
	public void FbLogOut() {
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

    //End old code
    
    //New code
    

    @SuppressWarnings("deprecation")
	public void postMessageOnWall(String msg) {
	    if (facebook.isSessionValid()) {
	        Bundle parameters = new Bundle();
	        parameters.putString("message", msg);
	        try {
	            String response = facebook.request("me/feed", parameters, "POST");
	            System.out.println(response);
	        } catch (IOException e) {
	            e.printStackTrace();
	        }
	    } else {
	        login();
	    }
	}
    
    @SuppressWarnings("deprecation")
	public void login() {
        if (!facebook.isSessionValid()) {
        	
            facebook.authorize(this, new String[] { "email", "publish_stream", "read_friendlists", "publish_actions" },
            		Facebook.FORCE_DIALOG_AUTH, 
            		new DialogListener() {
		                @Override
		                public void onFacebookError(FacebookError e) {
		                }
		         
		                @Override
		                public void onError(DialogError e) {
		                }
		         
		                @Override
		                public void onComplete(Bundle values) {
		                	postMessageOnWall("This is a test message! :3");
		                }
		         
		                @Override
		                public void onCancel() {
		                }
		            }
            );
        }
    }
}
