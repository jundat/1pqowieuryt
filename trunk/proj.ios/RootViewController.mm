#import "RootViewController.h"
#include "IOSNDKHelper.h"
#include "iRate.h"


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



- (void) Rate:(NSObject *)prms
{
    NSLog(@"Rate called in Object-C");
    NSDictionary *parameters = (NSDictionary*)prms;
    NSLog(@"Passed params are : %@", parameters);
    
    //isSuccess
    //true
    //false
    
    //responseType
    //RATE
    //REMIND_ME_LATER
    //NO_THANKS
    
    //String jsonStr = "{\"isSuccess\" : true, " +
    //"\"responseType\": \"" + responseType + "\"" +
    //"}";
    
    /*
     
    NSString *jsonStr = @"{\"isSuccess\": true, \"responseType\": \"RATE\"}";
    NSData *jsonData = [jsonStr dataUsingEncoding:NSUTF8StringEncoding];
    NSError *e = nil;
    NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:jsonData options:nil error:&e];
    
    [IOSNDKHelper SendMessage:@"onRateCompleted" WithParameters:dict];
    
     */
    
    [[iRate sharedInstance] promptForRating]; //promptForRating //promptIfNetworkAvailable
}


//------------------- JNI FUNCTION --------------------//


@end
