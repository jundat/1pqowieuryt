//
//  FlyAndFire.java
//  EasyNDK-for-cocos2dx
//
//  Created by Jundat
//
//

package com.chimgokien.flyandfire;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import com.chimgokien.flyandfire.classes.AndroidNDKHelper;

import android.app.AlertDialog;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.FrameLayout;

public class FlyAndFire extends Cocos2dxActivity
{
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
        Button tapButton = new Button(this);
        tapButton.setText("Tap to change text");
        tapButton.setLayoutParams(new LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT));
        
        tapButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v)
			{
				// TODO Auto-generated method stub
				FlyAndFire.this.ChangeSomethingInCocos();
			}
		});
        
        this.GetRootView().addView(tapButton);
    }
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        AndroidNDKHelper.SetNDKReciever(this);
        //this.AddButton();
    }
    
    
    public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        // FlyAndFire should create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
        
        return glSurfaceView;
    }


    public void ChangeSomethingInCocos()
    {
        // If you want to change anything that cocos handles, please run it on GLThread
        // Because cocos is a non threaded environment, it is required to queue stuff there
        // Every call on NDK opens up a new thread, hence making inconsistency in cocos and NDK
        
    	this.runOnGLThread(new Runnable() {
			@Override
			public void run()
			{
				// TODO Auto-generated method stub
				try {
					AndroidNDKHelper.SendMessageWithParameters("F1_callJavaToCpp", new JSONObject("{\"name\":12345}"));
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		});
    }
    
    public void SampleSelectorWithData(JSONObject prms)
    {
    	AlertDialog.Builder dlgAlert  = new AlertDialog.Builder(this);                      
        dlgAlert.setMessage("Hello world!!!");
        dlgAlert.setTitle("Title");
        dlgAlert.setPositiveButton("OK", null);
        dlgAlert.setCancelable(true);
        dlgAlert.create().show();
        finish(); 
        
        
    	Log.v("SampleSelector", "purchase something called");
    	Log.v("SampleSelector", "Passed params are : " + prms.toString());
    	
    	String CPPFunctionToBeCalled = null;
		try
		{
			CPPFunctionToBeCalled = prms.getString("to_be_called");
		}
		catch (JSONException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
    	AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage("This is a sample popup on Android").
        setTitle("Hello World!").
        setNeutralButton("OK", null).show();
        
        String jsonStr = "{\"sample_dictionary\":{\"sample_array\":[\"1\",\"2\",\"3\",\"4\",\"5\",\"6\",\"7\",\"8\",\"9\",\"10\",\"11\"],\"sample_integer\":1234,\"sample_float\":12.34,\"sample_string\":\"a string\"}}";
        JSONObject prmsToSend = null;
        
        try
        {
			prmsToSend = new JSONObject(jsonStr);
		}
        catch (JSONException e)
        {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
        if (prmsToSend != null)
        {
        	AndroidNDKHelper.SendMessageWithParameters(CPPFunctionToBeCalled, prmsToSend);
        }
        else
        {
        	AndroidNDKHelper.SendMessageWithParameters(CPPFunctionToBeCalled, null);
        }
    }
    
    public void SampleSelector(JSONObject prms)
    {
    	AlertDialog.Builder dlgAlert  = new AlertDialog.Builder(this);                      
        dlgAlert.setMessage("Hello world!!!");
        dlgAlert.setTitle("Title");
        dlgAlert.setPositiveButton("OK", null);
        dlgAlert.setCancelable(true);
        dlgAlert.create().show();
        finish(); 
    	
    	Log.v("SampleSelector", "purchase something called");
    	Log.v("SampleSelector", "Passed params are : " + prms.toString());
    	
    	String CPPFunctionToBeCalled = null;
		try
		{
			CPPFunctionToBeCalled = prms.getString("to_be_called");
		}
		catch (JSONException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
    	AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage("This is a sample popup on Android").
        setTitle("Hello World!").
        setNeutralButton("OK", null).show();
    	
    	AndroidNDKHelper.SendMessageWithParameters(CPPFunctionToBeCalled, null);
    }
}
