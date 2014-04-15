#import <UIKit/UIKit.h>
#import "iRate.h"


@interface RootViewController : UIViewController <iRateDelegate>{

}


//------------- rate ---------------//
- (void) Rate:(NSObject *)prms;
//no thanks
- (void)iRateUserDidDeclineToRateApp;
//remind
- (void)iRateUserDidRequestReminderToRateApp;
//after rate
- (void)iRateDidOpenAppStore;


//------ show charge webview --------//
- (void) ShowChargeWebView:(NSObject *)prms;


//------ status bar in iOS 7 --------//
- (void)viewDidLoad;
- (BOOL)prefersStatusBarHidden;





@end
