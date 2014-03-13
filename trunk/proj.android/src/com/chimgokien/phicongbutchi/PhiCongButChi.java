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
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.net.Uri;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;

import com.chimgokien.phicongbutchi.classes.AndroidNDKHelper;
import com.ezibyte.social.EziSocialManager;
// EziSocial Plugin

public class PhiCongButChi extends Cocos2dxActivity{

    ////////////////////////////////////////////////////////////////////////
	String APP_ID = "613622542044186";
	String TAG = "JAVA_CALL";
	
	public void Rate(JSONObject prms)
	{
		String link = null;
    	
    	try {
			link = prms.getString("link");
			Log.i(TAG, "link: " + link);
			
			Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(link));
			startActivity(browserIntent);
		} catch (JSONException e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
	
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
                                          APP_ID,
                                          true, // Set to see ezisocial plugin logs
                                          savedInstanceState
                                          );
    }
        
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
