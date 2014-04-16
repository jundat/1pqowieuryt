#import "RootViewController.h"
#include "IOSNDKHelper.h"
#include "iRate.h"
#import <AudioToolbox/AudioToolbox.h>


@implementation RootViewController


 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
        
        
        // Tell NDKHelper that RootViewController will respond to messages
        // Coming from C++
        [IOSNDKHelper SetNDKReciever:self];
        

    }
    return self;
}


/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
 
*/
// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


//------------------- JNI FUNCTION --------------------//


//------------- RATE --------------//

- (void) Rate:(NSObject *)prms
{
    NSDictionary *parameters = (NSDictionary*)prms;
    //NSLog(@"Passed params are : %@", parameters);
    
    NSString *game_name, *game_package, *msg, *rate, *later, *no;
    
    game_name = (NSString*)[parameters objectForKey:@"game_name"];
    game_package = (NSString*)[parameters objectForKey:@"game_package"];
    msg = (NSString*)[parameters objectForKey:@"msg"];
    rate = (NSString*)[parameters objectForKey:@"rate"];
    later = (NSString*)[parameters objectForKey:@"later"];
    no = (NSString*)[parameters objectForKey:@"no"];
    game_name = (NSString*)[parameters objectForKey:@"game_name"];
    
    [iRate sharedInstance].applicationBundleID = game_package;
    [iRate sharedInstance].messageTitle = game_name;
    [iRate sharedInstance].message = msg;
    [iRate sharedInstance].rateButtonLabel = rate;
    [iRate sharedInstance].remindButtonLabel = later;
    [iRate sharedInstance].cancelButtonLabel = no;
    
    [[iRate sharedInstance] promptForRating];
}



//no thanks
- (void)iRateUserDidDeclineToRateApp
{
    NSLog(@"no thanks");
    NSString *jsonStr = @"{\"isSuccess\": true, \"responseType\": \"NO_THANKS\"}";
    NSData *jsonData = [jsonStr dataUsingEncoding:NSUTF8StringEncoding];
    NSError *e = nil;
    NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:jsonData options:nil error:&e];
    
    [IOSNDKHelper SendMessage:@"onRateCompleted" WithParameters:dict];

}

//remind
- (void)iRateUserDidRequestReminderToRateApp
{
    NSLog(@"remind");
    NSString *jsonStr = @"{\"isSuccess\": true, \"responseType\": \"REMIND_ME_LATER\"}";
    NSData *jsonData = [jsonStr dataUsingEncoding:NSUTF8StringEncoding];
    NSError *e = nil;
    NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:jsonData options:nil error:&e];
    
    [IOSNDKHelper SendMessage:@"onRateCompleted" WithParameters:dict];
}

//after rate
- (void)iRateDidOpenAppStore
{
    NSLog(@"rated");
    NSString *jsonStr = @"{\"isSuccess\": true, \"responseType\": \"RATE\"}";
    NSData *jsonData = [jsonStr dataUsingEncoding:NSUTF8StringEncoding];
    NSError *e = nil;
    NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:jsonData options:nil error:&e];
    
    [IOSNDKHelper SendMessage:@"onRateCompleted" WithParameters:dict];
}

//-------- SHOW CHARGE WEB VIEW ----------//


- (void) ShowChargeWebView:(NSObject *)prms
{
    
    NSDictionary *parameters = (NSDictionary*)prms;
    //NSLog(@"Passed params are : %@", parameters);
    
    NSString *charge_caption, *charge_close, *charge_link;
    
    charge_caption = (NSString*)[parameters objectForKey:@"charge_caption"];
    charge_close = (NSString*)[parameters objectForKey:@"charge_close"];
    charge_link = (NSString*)[parameters objectForKey:@"charge_link"];

    CGRect screenRect = [[UIScreen mainScreen] bounds];
    CGFloat screenWidth = screenRect.size.width;
    CGFloat screenHeight = screenRect.size.height;
    
    //
    //FullView
    //Tag: 100
    CGRect applicationFrame = [[UIScreen mainScreen] applicationFrame];
    UIView *contentView = [[UIView alloc] initWithFrame:applicationFrame];
    contentView.backgroundColor = [UIColor whiteColor];
    [contentView setTag:100];
    [self.view addSubview:contentView];

    //
    //Close button
    //
    UIButton *roundRectButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [roundRectButton setFrame:CGRectMake(screenWidth/2 - 50, screenHeight - 27.5, 100, 25)];
    [roundRectButton setTitle:charge_close forState:UIControlStateNormal];
    [roundRectButton addTarget:self action:@selector(closeChargeWebView:) forControlEvents:UIControlEventTouchUpInside];
    [contentView addSubview:roundRectButton];
    
    //
    //WebView
    //
    UIWebView *webView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 0, screenWidth, screenHeight - 30)];
    [contentView addSubview:webView];
    NSURLRequest *urlRequest;
    NSURL *urlforWebView;
    urlforWebView=[NSURL URLWithString:charge_link];
    urlRequest=[NSURLRequest requestWithURL:urlforWebView];
    [webView loadRequest:urlRequest];
}

- (void) closeChargeWebView:(UIButton *)sender
{
    //NSLog(@"Close charge web view");
    [[self.view viewWithTag:100] removeFromSuperview];
}

//-------------- status bar ---------------//

- (void)viewDidLoad
{
    [super viewDidLoad];
    if ([self respondsToSelector:@selector(setNeedsStatusBarAppearanceUpdate)])
    {
        [self prefersStatusBarHidden];
        [self performSelector:@selector(setNeedsStatusBarAppearanceUpdate)];
    }
    else
    {
        // iOS 6
        [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationSlide];
    }
}


// Add this Method
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

//-------------- vibrate ---------------//

- (void) Vibrate: (NSObject *)prms
{
    NSLog(@"Vibrate in IOS");
    AudioServicesPlayAlertSound(kSystemSoundID_Vibrate); //kSystemSoundID_Vibrate
}



@end
