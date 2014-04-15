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

//----------- end rate -------------//
//------ show charge webview --------//


- (void) ShowChargeWebView:(NSObject *)prms;


@end
