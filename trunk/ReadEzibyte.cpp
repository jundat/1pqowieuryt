EziSocialObject::sharedObject()->setEmailDelegate(this);
EziSocialObject::sharedObject()->setTwitterDelegate(this);
EziSocialObject::sharedObject()->setAutoCheckIncomingRequestsOnAppLaunch(true);



void HomeScene::tweetButtonPressed()
{
    EziSocialObject::sharedObject()->tweet("Welcome to the world of EziSocial Plugin. Try now at http://ezibyte.com", NULL);
}

void HomeScene::emailButtonPressed()
{
    EziSocialObject::sharedObject()->sendEmail("EziSocial Plugin Feedback",
                                               "<p><B>Please write your feedback here</B></p><br>",
                                               "support@ezibyte.com");
}

void BaseScene::onExit()
{
    CCLayer::onExit();
    EziSocialObject::sharedObject()->setFacebookDelegate(NULL);
}

void BaseScene::mailCallback(int responseCode)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::MAIL_SEND_ERROR:
            CCMessageBox("There is error while sending the email", "Email Delegate Response ");
            break;

        case EziSocialWrapperNS::RESPONSE_CODE::MAIL_SEND_SUCCESS:
            CCMessageBox("Email sent sucessfully", "Email Delegate Response ");
            break;
        
        case EziSocialWrapperNS::RESPONSE_CODE::MAIL_SEND_CANCLLED:
            CCMessageBox("You have cancelled email.", "Email Delegate Response ");
            break;

        case EziSocialWrapperNS::RESPONSE_CODE::MAIL_SEND_STORED:
            CCMessageBox("Your email is stored in draft.", "Email Delegate Response ");
            break;

        default:
            CCMessageBox("Unknown message code in email response. Please report back at support@ezibyte.com", "[Email]: Unknown Response.");
            break;
    }
}

void BaseScene::twitterCallback(int responseCode)
{   
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::TWIT_SEND:
            CCMessageBox("Tweeted! :)", "Tweet Delegate Response ");
            break;
            
        case EziSocialWrapperNS::RESPONSE_CODE::TWIT_CANCEL:
            CCMessageBox("Sorry! You have cancelled the tweet.", "Tweet Delegate Response ");
            break;
                        
        default:
            CCMessageBox("Unknown message code in email response. Please report back at support@ezibyte.com", "[Email]: Unknown Response.");
            break;

    }
}

void BaseScene::fbIncomingRequestCallback(int responseCode, const char* responseMessage, int totalIncomingRequests)
{
    if (totalIncomingRequests > 0)
    {
        int pendingRequest = EziFBIncomingRequestManager::sharedManager()->getPendingRequestCount();
        CCString* message = CCString::createWithFormat("Total new requests = %d\n Total pending requests = %d.\nGo to Request API Scene to see the list of incoming requets.", totalIncomingRequests, pendingRequest);
        CCMessageBox(message->getCString(), "fbIncomingRequestCallback");
    }
    else
    {
        CCLOG("No new incoming request...");
    }
}

void BaseScene::fbFriendsCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friends)
{
    if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_FRIEND_GET_SUCCESS)
    {
        for (int i=0; i<friends->count(); i++)
        {
            EziFacebookFriend* myFriend = (EziFacebookFriend*)(friends->objectAtIndex(i));
            CCString* friendKey = CCString::createWithFormat("Friend_ID_%d", (i+1), NULL);
            CCUserDefault::sharedUserDefault()->setStringForKey(friendKey->getCString(), myFriend->getFBID());
        }
        
        if (friends->count() > 0)
        {
            CCLOG("[EziSocial Log]: %d friends saved in DB for future use.", friends->count());
            CCUserDefault::sharedUserDefault()->setIntegerForKey("FRIENDS_COUNT", friends->count());
            
            // Save these friends for future use ;)
            CCUserDefault::sharedUserDefault()->flush();
        }
    }
}

void FBActionScene::fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser)
{
    switch (responseCode)
    {
        case EziSocialWrapperNS::RESPONSE_CODE::FB_USER_DETAIL_SUCCESS:
            showCurrentUserData();
            break;
            
        default:
            CCMessageBox(responseMessage, "Error - User details");
            break;
    }
}