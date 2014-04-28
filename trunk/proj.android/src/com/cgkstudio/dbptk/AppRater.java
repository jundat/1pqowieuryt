package com.cgkstudio.dbptk;

import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class AppRater {
	
	//public static String game_name, game_package, msg, rate, later, no;
	
	private final static String TAG = "APP_RATER";
    
    private final static int DAYS_UNTIL_PROMPT = 1; //3
    private final static int LAUNCHES_UNTIL_PROMPT = 1; //7
    
    public static void app_launched(Context mContext, final DBPTK parent) {
        SharedPreferences prefs = mContext.getSharedPreferences("apprater", 0);
        if (prefs.getBoolean("dontshowagain", false)) { return ; }
        
        SharedPreferences.Editor editor = prefs.edit();
        
        // Increment launch counter
        long launch_count = prefs.getLong("launch_count", 0) + 1;
        editor.putLong("launch_count", launch_count);

        // Get date of first launch
        Long date_firstLaunch = prefs.getLong("date_firstlaunch", 0);
        if (date_firstLaunch == 0) {
            date_firstLaunch = System.currentTimeMillis();
            editor.putLong("date_firstlaunch", date_firstLaunch);
        }
        
        // Wait at least n days before opening
        if (launch_count >= LAUNCHES_UNTIL_PROMPT) {
            if (System.currentTimeMillis() >= date_firstLaunch + 
                    (DAYS_UNTIL_PROMPT * 24 * 60 * 60 * 1000)) {
                showRateDialog(mContext, editor, parent, 
                		"Điện Biên Phủ Trên Không", 
                		"com.cgkstudio.dbptk", 
                		"Rate 5 stars for Điện Biên Phủ Trên Không", 
                		"Rate", 
                		"Later", 
                		"No, thanks");
            }
        }
        
        editor.commit();
    }   
    
    public static void showRateDialog(final Context mContext, final SharedPreferences.Editor editor, final DBPTK parent,
    		String game_name, final String game_package, String msg, String rate, String later, String no) {
        final Dialog dialog = new Dialog(mContext);
        dialog.setTitle(game_name);

        LinearLayout ll = new LinearLayout(mContext);
        ll.setOrientation(LinearLayout.VERTICAL);
        
        TextView tv = new TextView(mContext);
        tv.setText(msg);
        tv.setWidth(400);
        tv.setPadding(10, 0, 10, 10);
        ll.addView(tv);
        
        Button b1 = new Button(mContext);
        b1.setText(rate);
        b1.setWidth(400);
        b1.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                mContext.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + game_package)));
                dialog.dismiss();
                
                parent.onRateCompleted("RATE");
                Log.i(TAG, "Choose1: Rate ngay");
            }
        });        
        ll.addView(b1);

        
        
        if (later.length() > 0)
        {
            Button b2 = new Button(mContext);
            b2.setText(later);
            b2.setWidth(400);
            b2.setOnClickListener(new OnClickListener() {
                public void onClick(View v) {
                    dialog.dismiss();

                    parent.onRateCompleted("REMIND_ME_LATER");
                    Log.i(TAG, "Choose2: Để sau");
                }
            });
            ll.addView(b2);
        }
        

        Button b3 = new Button(mContext);
        b3.setText(no);
        b3.setWidth(400);
        b3.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                if (editor != null) {
                    editor.putBoolean("dontshowagain", true);
                    editor.commit();
                }
                dialog.dismiss();
                
                parent.onRateCompleted("NO_THANKS");
                Log.i(TAG, "Choose3: Không");
            }
        });
        ll.addView(b3);

        dialog.setContentView(ll);        
        dialog.show();        
    }
}