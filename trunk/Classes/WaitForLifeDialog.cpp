#include "WaitForLifeDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "DataManager.h"
#include <time.h>
#include "cocos-ext.h"
#include "TextLoader.h"
#include "MyMacro.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool WaitForLifeDialog::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCPoint pexit = ccp(250, G_DESIGN_HEIGHT - 810);
	CCPoint pask = ccp(G_DESIGN_WIDTH-250, G_DESIGN_HEIGHT - 810);

	CCScale9Sprite* dialog = CCScale9Sprite::create("dialog.png");
	dialog->setPosition(ccp(400, 640));
	dialog->setContentSize(CCSizeMake(680, 480));
	this->addChild(dialog, -2);

	MY_CREATE_MENU_BUTTON(exitButton, "button.png", "button_down.png", TXT("btn_exit"), 
		"Roboto-Medium.ttf", 48, ccBLACK, pexit, this, WaitForLifeDialog::exitCallback);

	MY_CREATE_MENU_BUTTON(askButton, "button.png", "button_down.png", TXT("btn_ask_friend"), 
		"Roboto-Medium.ttf", 48, ccBLACK, pask, this, WaitForLifeDialog::askFriendCallback);

	CCMenu* menu = CCMenu::create(exitButton, askButton, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	CCLabelTTF* labelTitle = CCLabelTTF::create(TXT("wait_lack_of_life"), 
		"Roboto-Medium.ttf", 
		48, 
		CCSizeMake(600, 0), 
		kCCTextAlignmentCenter, 
		kCCVerticalTextAlignmentCenter);
	
	labelTitle->setColor(ccc3(56, 56, 56));
	labelTitle->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 120));
	this->addChild(labelTitle);

    return true;
}

void WaitForLifeDialog::exitCallback( CCObject* pSender )
{
	MenuScene* parent = (MenuScene*)this->getParent();
	parent->setTouchEnabled(true);
	parent->onCloseDialog();
	this->removeFromParent();
}

void WaitForLifeDialog::askFriendCallback( CCObject* pSender )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		inviteFriends();
	}
	else //Connect facebook
	{
		bool needPublicPermission = true;
		EziSocialObject::sharedObject()->performLoginUsingFacebook(needPublicPermission); // Pass true if you need publish permission also
	}

#else //WIN 32

	DataManager::sharedDataManager()->SetLastPlayerLife(5);
	this->unschedule(schedule_selector(WaitForLifeDialog::ScheduleTick));

	MenuScene* parent = (MenuScene*)this->getParent();
	parent->setTouchEnabled(true);
	parent->onCloseDialog();
	this->removeFromParent();

#endif
}

void WaitForLifeDialog::fbSessionCallback(int responseCode, const char *responseMessage)
{
	CCLOG("fbSessionCallback");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL)
	{
		CCLOG("fbSessionCallback: SUCCESSFUL");
		
		//Auto fetchFBUserDetails, do not call it again
		//It make exception
		//CCLOG("call: fetchFBUserDetails");
		//EziSocialObject::sharedObject()->fetchFBUserDetails(true); //need email = true
	}
	else
	{
		CCLOG("fbSessionCallback: FAILED");
	}
#endif
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // fbUserDetailCallback
void WaitForLifeDialog::fbUserDetailCallback( int responseCode, const char* responseMessage, EziFacebookUser* fbUser )
{
	CCLOG("fbUserDetailCallback");
	if (fbUser != NULL)
	{
		CCLOG("fbUserDetailCallback: user != NULL");
		EziSocialObject::sharedObject()->setCurrentFacebookUser(fbUser);

		//save data
		std::string firstname = fbUser->getFirstName(); //getFirstName //getFullName
		std::string userName = fbUser->getUserName();
		std::string profileID = fbUser->getProfileID();
		std::string fullName = fbUser->getFullName();

		DataManager::sharedDataManager()->SetName(firstname.c_str());
		DataManager::sharedDataManager()->SetFbProfileID(profileID.c_str());
		DataManager::sharedDataManager()->SetFbUserName(userName.c_str());
		DataManager::sharedDataManager()->SetFbFullName(fullName.c_str());

		//send request to friend
		inviteFriends();
	}
}
#endif


void WaitForLifeDialog::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
	CCLOG("fbUserPhotoCallback");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//CCLOG("Gotten avatar for %s", fbID);
	std::string sid = std::string(fbID);

	if ((strcmp(userPhotoPath, "") != 0))
	{
		CCLOG("fbUserPhotoCallback: userPhotoPath != NULL");
		if(sid == DataManager::sharedDataManager()->GetFbID())
		{
			CCLOG("fbUserPhotoCallback: this user");
			//CCLOG("Gotten avatar for user");

			//save 
			DataManager::sharedDataManager()->SetPhotoPath(userPhotoPath);

			//send request to friend
			inviteFriends();
		}
	}
#endif
}



//Post status  callback
void WaitForLifeDialog::fbMessageCallback(int responseCode, const char* responseMessage)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	if(responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_NORMAL_MESSAGE_PUBLISHED)
	{
		CCLOG("Message published successfully!");
		DataManager::sharedDataManager()->SetLastPlayerLife(5);
		this->unschedule(schedule_selector(WaitForLifeDialog::ScheduleTick));

		MenuScene* parent = (MenuScene*)this->getParent();
		parent->setTouchEnabled(true);
		parent->onCloseDialog();
		this->removeFromParent();
	}
	else
	{
		CCLOG("Message published failed!");
	}
#endif
}


void WaitForLifeDialog::fbSendRequestCallback( int responseCode, const char* responseMessage, cocos2d::CCArray* friendsGotRequests )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
// 	FB_REQUEST_SENDING_ERROR - In case if there is any error while sending the request
// 	FB_REQUEST_SENDING_CANCELLED – In case, user decides not to send the request.
// 	FB_REQUEST_SENT - If sending request is success

	if (EziSocialWrapperNS::RESPONSE_CODE::FB_REQUEST_SENT == responseCode)
	{
		int numFriends = friendsGotRequests->count();

		CCLOG("Request sent successfully to %d friends", numFriends);

		//numFriends = (numFriends > G_MAX_PLAYER_LIFE) ? G_MAX_PLAYER_LIFE : numFriends;
		//DataManager::sharedDataManager()->SetLastPlayerLife(numFriends);
		this->unschedule(schedule_selector(WaitForLifeDialog::ScheduleTick));

		MenuScene* parent = (MenuScene*)this->getParent();
		parent->setTouchEnabled(true);
		parent->onCloseDialog();
		this->removeFromParent();
	}
	else
	{
		CCLOG("Error, Request sent failed!");
	}

#endif
}


void WaitForLifeDialog::ScheduleTick( float dt )
{

	tm* lasttm = DataManager::sharedDataManager()->GetLastDeadTime();
	time_t lastTime = mktime(lasttm);
	time_t curTime = time(NULL);
	double _secondsElapsed = difftime(curTime, lastTime);

	m_waitTime = (float)(G_PLAYER_TIME_TO_REVIVE - _secondsElapsed);

	//m_waitTime -= dt;

	if (m_waitTime < 0)
	{
		this->unschedule(schedule_selector(WaitForLifeDialog::ScheduleTick));
		MenuScene* parent = (MenuScene*)this->getParent();
		parent->onCompletedWaiting();
	}
	else
	{
		int mins = (int)m_waitTime/60;
		int seconds = (int)m_waitTime % 60;

		CCString* s = CCString::createWithFormat("0%d:%d", mins, seconds);
		m_lbTime->setString(s->getCString());
	}
}

void WaitForLifeDialog::inviteFriends()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	CCString* str = CCString::createWithFormat(TXT("score_invite_friend"), 
		DataManager::sharedDataManager()->GetFbFullName().c_str());

	//invite friends
	EziSocialObject::sharedObject()->sendRequestToFriends(
		EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE,
		str->getCString(), 
		NULL,
		NULL,
		TXT("game_name"));
#endif
}



