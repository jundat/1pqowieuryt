#include "MenuScene.h"
#include "MainGameScene.h"
#include "ScoreScene.h"
#include "SettingScene.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "WaitForLifeDialog.h"
#include <time.h>
#include "NDKHelper/NDKHelper.h"
#include "GameClient.h"
#include "HttpClientTest.h"

USING_NS_CC;
USING_NS_CC_EXT;


CCScene* MenuScene::scene()
{
    CCScene *scene = CCScene::create();
    MenuScene *layer = MenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MenuScene::init()
{
	//pre proccess

	DataManager::sharedDataManager()->RefreshPlayerLife();

	//////////////////////////////////////////////////////////////////////////
	

    if ( !CCLayer::init() )
    {
        return false;
    }

	m_isShowDialog = false;
	this->setKeypadEnabled(true);

    /////////////////////////////

	CCSprite* bg = CCSprite::create("bg_stars.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg, 0);
	
	CCSprite* menuTop = CCSprite::create("menu_top.png");
	menuTop->setPosition(ccp(G_DESIGN_WIDTH/2, -50 - menuTop->getContentSize().height/2 + G_DESIGN_HEIGHT));
	this->addChild(menuTop, 0);

	//
	CCString* s = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetLastPlayerLife());
	m_labelLife = CCLabelBMFont::create(s->getCString(), "Mia_64.fnt");
	m_labelLife->setColor(ccc3(56, 56, 56));
	m_labelLife->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 100));
	this->addChild(m_labelLife);

	//

	//
	s = CCString::createWithFormat("v%d", 20);
	CCLabelBMFont* labelVersion = CCLabelBMFont::create(s->getCString(), "Mia_64.fnt");
	labelVersion->setColor(ccc3(56, 56, 56));
	labelVersion->setScale(0.5f);
	labelVersion->setPosition(ccp(labelVersion->getContentSize().width/4, G_DESIGN_HEIGHT - labelVersion->getContentSize().height/4));
	this->addChild(labelVersion);

	//

    CCMenuItemImage *playItem = CCMenuItemImage::create(
                                        "new_button.png",
                                        "new_button_press.png",
                                        this,
                                        menu_selector(MenuScene::playCallback));
    
	playItem->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 - 30));


	CCMenuItemImage *scoreItem = CCMenuItemImage::create(
		"score_button.png",
		"score_button_press.png",
		this,
		menu_selector(MenuScene::scoreCallback));

	scoreItem->setPosition(ccp(G_DESIGN_WIDTH/2,
		playItem->getPositionY() - playItem->getContentSize().height/2 - scoreItem->getContentSize().height/2 - 10));

	//

	CCMenuItemImage *settingItem = CCMenuItemImage::create(
		"setting_button.png",
		"setting_button_press.png",
		this,
		menu_selector(MenuScene::settingCallback));

	settingItem->setPosition(ccp(G_DESIGN_WIDTH/2,
		scoreItem->getPositionY() - scoreItem->getContentSize().height/2 - settingItem->getContentSize().height/2 - 10));

	//

	CCMenuItemImage *exitItem = CCMenuItemImage::create(
		"exit_button.png",
		"exit_button_press.png",
		this,
		menu_selector(MenuScene::exitCallback));

	exitItem->setPosition(ccp(G_DESIGN_WIDTH/2,
		settingItem->getPositionY() - settingItem->getContentSize().height/2 - exitItem->getContentSize().height/2 - 10));

	//

	m_isLoggedIn = false;
	CCMenuItem* fbLogIn = CCMenuItemImage::create("facebook-icon.png", NULL, NULL);
	CCMenuItem* fbLogOut = CCMenuItemImage::create("facebook-icon_off.png", NULL, NULL);
	m_fbItem = CCMenuItemToggle::createWithTarget(this,  menu_selector(MenuScene::fbCallback), fbLogIn, fbLogOut, NULL);
	m_fbItem->setSelectedIndex(0);
	m_fbItem->setPosition(ccp(exitItem->getPosition().x + exitItem->getContentSize().width/2 + m_fbItem->getContentSize().width/2 + 10,
		exitItem->getPosition().y));

#ifndef WIN32
	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		m_isLoggedIn = true;
		m_fbItem->setSelectedIndex(1); //log out button 
	}
	else //logged out stated
	{
		m_isLoggedIn = false;
		m_fbItem->setSelectedIndex(0); //log in button
	}
#endif

	
	//

    m_menu = CCMenu::create(playItem, scoreItem, settingItem, exitItem, m_fbItem, NULL);
    m_menu->setPosition(CCPointZero);
    this->addChild(m_menu, 1);

	 
	m_userSprite = CCSprite::create("fb-profile.png");
	m_userSprite->setPosition(ccp(64, 64));
	this->addChild(m_userSprite);

	
	AudioManager::sharedAudioManager()->PlayBackground("background.ogg");

    return true;
}

void MenuScene::playCallback(CCObject* pSender)
{
	//check if last_player_life > 0
	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
	lastLife = (lastLife > G_MAX_PLAYER_LIFE) ? G_MAX_PLAYER_LIFE : lastLife;
	DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);

	CCLOG("Last life: %d", lastLife);

	if (lastLife > 0)
	{
		CCScene *pScene = CCTransitionFade::create(0.5, MainGameScene::scene());
		CCDirector::sharedDirector()->replaceScene(pScene);

		CCLOG("LastLife > 0 -> Play");
	}
	else
	{
		//get revive_life
		tm* lasttm = DataManager::sharedDataManager()->GetLastDeadTime();
		time_t lastTime = mktime(lasttm);
		time_t curTime = time(NULL);
		double seconds = difftime(curTime, lastTime);

		lastLife = (int)(seconds / G_PLAYER_TIME_TO_REVIVE);
		lastLife = (lastLife > G_MAX_PLAYER_LIFE) ? G_MAX_PLAYER_LIFE : lastLife;

		CCLOG("Revive Last life: %d", lastLife);

		if (lastLife > 0)
		{
			DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);

			CCScene *pScene = CCTransitionFade::create(0.5, MainGameScene::scene());
			CCDirector::sharedDirector()->replaceScene(pScene);

			CCLOG("Revive->LastLife > 0 -> Play");
		}
		else
		{
			WaitForLifeDialog* dialog = WaitForLifeDialog::create((float)(G_PLAYER_TIME_TO_REVIVE - seconds));
			this->addChild(dialog, 100);
			this->setTouchEnabled(false);
			onShowDialog();

			CCLOG("Revive->LastLife < 0 -> Can not play");
			return;
		}
	}
}

void MenuScene::scoreCallback( CCObject* pSender )
{
	CCScene *pScene = CCTransitionFade::create(0.5, ScoreScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void MenuScene::settingCallback( CCObject* pSender )
{
	CCScene *pScene = CCTransitionFade::create(0.5, SettingScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void MenuScene::exitCallback( CCObject* pSender )
{
	CCDirector::sharedDirector()->end();
}

// Facebook //=========================================

void MenuScene::fbCallback( CCObject* pSender )
{
	CCLOG("Call to Log In/Out facebook");
#ifndef WIN32
	if (EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in
	{
		CCLOG("Log Out");
		EziSocialObject::sharedObject()->perfromLogoutFromFacebook();
	}
	else
	{
		CCLOG("Log In");
		bool needPublicPermission = false;
		EziSocialObject::sharedObject()->performLoginUsingFacebook(needPublicPermission); // Pass true if you need publish permission also
	}
#endif
}

void MenuScene::fbSessionCallback(int responseCode, const char *responseMessage)
{
#ifndef WIN32
	switch (responseCode)
	{
	case EziSocialWrapperNS::RESPONSE_CODE::ERROR_INTERNET_NOT_AVAILABLE:
		CCLOG("ERROR_INTERNET_NOT_AVAILABLE");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::ERROR_READ_PERMISSION_ERROR:
		CCLOG("ERROR_READ_PERMISSION_ERROR");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::ERROR_PUBLISH_PERMISSION_ERROR:
		CCLOG("ERROR_PUBLISH_PERMISSION_ERROR");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_USER_PERMISSION_REVOKED:
		CCLOG("FB_LOGIN_USER_PERMISSION_REVOKED");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_APP_NOT_ALLOWERD_TO_USE_FB:
		CCLOG("FB_LOGIN_APP_NOT_ALLOWERD_TO_USE_FB");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_PERMISSION_DENIED:
		CCLOG("FB_LOGIN_PERMISSION_DENIED");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL: /////////////////// logged in state
		{
			CCMessageBox("Log in success!", "Info");
			CCLOG("FB_LOGIN_SUCCESSFUL");

			m_isLoggedIn = true;
			m_fbItem->setSelectedIndex(1); //log out button

			//get profile information
			//bool needsEmail = false;
			EziSocialObject::sharedObject()->fetchFBUserDetails(false); //need email = false

			break;
		}
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_FAILED:
		CCLOG("FB_LOGIN_FAILED");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGOUT_SUCCESSFUL:  /////////////////// logged out state
		{
			CCLOG("FB_LOGOUT_SUCCESSFUL");
			CCMessageBox("Log out success!", "Info");

			m_isLoggedIn = false;
			m_fbItem->setSelectedIndex(0); //log in button
			break;
		}		
	}
#endif
}
//#undef WIN32
#ifndef WIN32

void MenuScene::buildScene()
{
	EziFacebookUser* currentUser = EziSocialObject::sharedObject()->getCurrentFacebookUser();
	std::string fullname = currentUser->getFullName();
	std::string emailID = currentUser->getEmailID();
	std::string gender = currentUser->getGender();
	std::string userName = currentUser->getUserName();
	std::string profileID = currentUser->getProfileID();
	std::string accessToken = currentUser->getAccessToken();

	CCLOG("FullName: %s", fullname.c_str());
	CCLOG("EmailID: %s", emailID.c_str());
	CCLOG("Gender: %s", gender.c_str());
	CCLOG("UserName: %s", userName.c_str());
	CCLOG("ProfileID: %s", profileID.c_str());
	CCLOG("AccessToken: %s", accessToken.c_str());

// 	CCLabelTTF* userName = CCLabelTTF::create(currentUser->getFullName(), "Arial", 64);
// 	userName->setAnchorPoint(ccp(1.0f, 1.0f));
// 	PlaceNode(userName, 0.98, 0.5);
// 	userName->setPositionY(_userSprite->getPositionY() - (_userSprite->getContentSize().height * _userSprite->getScaleY()));
// 
// 	CCLabelTTF* userEmailID = CCLabelTTF::create(currentUser->getEmailID(), "Arial", SCALED_VALUE(40.0f));
// 	userEmailID->setColor(ccc3(255, 255, 0));
// 	userEmailID->setAnchorPoint(ccp(1.0f, 1.0f));
// 	PlaceNode(userEmailID, 0.98, 0.5);
// 	userEmailID->setPositionY(userName->getPositionY() - (userName->getContentSize().height * userName->getScaleY()));
// 
//  std::string textToDisplay = ""; 
// 	textToDisplay.append("A '").append(gender).append("' member, who is using '").append(currentUser->getUserName()).append("' as username on Facebook, and lives in '").append(currentUser->getPresentLocation()).append("' but belongs to '").append(currentUser->getHomeTown()).append("'. Profile ID of this user is '").append(currentUser->getProfileID()).append("' and access token is ");
 
// 	CCLOG("Text to display = %s", textToDisplay.c_str());
 
// 	CCLabelTTF* userDetails = CCLabelTTF::create("", "Arial", SCALED_VALUE(20.0f));
// 	userDetails->setHorizontalAlignment(kCCTextAlignmentRight);
// 	userDetails->setDimensions(CCSizeMake(AppDelegate::SCREEN_WIDTH * 0.65, AppDelegate::SCREEN_HEIGHT * 0.13));
// 	userDetails->setString(textToDisplay.c_str());
// 	userDetails->setColor(ccc3(0, 255, 255));
// 	userDetails->setAnchorPoint(ccp(1.0f, 1.0f));
// 	PlaceNode(userDetails, 0.98, 0.5);
// 	userDetails->setPositionY(userEmailID->getPositionY() - (userEmailID->getContentSize().height * userEmailID->getScaleY()));
// 
// 	CCLabelTTF* accessToken = CCLabelTTF::create("", "Arial", SCALED_VALUE(15.0));
// 	accessToken->setHorizontalAlignment(kCCTextAlignmentRight);
// 	accessToken->setVerticalAlignment(kCCVerticalTextAlignmentTop);
// 	accessToken->setDimensions(CCSizeMake(AppDelegate::SCREEN_WIDTH * 0.70, AppDelegate::SCREEN_HEIGHT * 0.10));
// 	accessToken->setString(currentUser->getAccessToken());
// 	accessToken->setAnchorPoint(ccp(1.0f, 1.0f));
// 	PlaceNode(accessToken, 0.98, 0.5);
// 	accessToken->setPositionY(userDetails->getPositionY() - (userDetails->getContentSize().height * userDetails->getScaleY()));
// 
// 	this->addChild(userName);
// 	this->addChild(userEmailID);
// 	this->addChild(userDetails);
// 	this->addChild(accessToken);
// 
// 	EziSocialObject::sharedObject()->setFacebookDelegate(this);
// 	EziSocialObject::sharedObject()->getProfilePicForID(this, currentUser->getProfileID(),
// 		_userSprite->getContentSize().width * _userSprite->getScaleX(),
// 		_userSprite->getContentSize().height * _userSprite->getScaleY(), false);

}


void MenuScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
	const char* currentUserId = EziSocialObject::sharedObject()->getCurrentFacebookUser()->getProfileID();

	CCLOG("fbID = %s", fbID);
	CCLOG("userPhotoPath = %s", userPhotoPath);
	CCLOG("currentUserId = %s", currentUserId);

	if ((strcmp(userPhotoPath, "") != 0) && (strcmp(currentUserId, fbID) == 0))
	{
		CCSprite* userPhoto = CCSprite::create(userPhotoPath);
		userPhoto->setPosition(ccp(m_userSprite->getContentSize().width/2, m_userSprite->getContentSize().height/2));
		userPhoto->setScale(m_userSprite->getContentSize().width/userPhoto->getContentSize().width);
		m_userSprite->addChild(userPhoto);
	}
}

void MenuScene::fbUserDetailCallback( int responseCode, const char* responseMessage, EziFacebookUser* fbUser )
{
	if (fbUser != NULL)
	{
		// Set the current user in EziSocialObject to use it in future. EziSocialObject is immune to CCScene lifecycle. It is singleton class and will live forever.
		EziSocialObject::sharedObject()->setCurrentFacebookUser(fbUser);

		EziSocialObject::sharedObject()->getProfilePicForID(this, fbUser->getProfileID(), // Profile ID of current user
			128.0f, 128.0f, // Size of the image
			false // force download it from server
		);
	}

	// You can call method associated with EziFacebookUser and use the user details. For example:-
	this->buildScene();
}

#endif
// Facebook //=========================================



void MenuScene::keyBackClicked()
{
	exitCallback(NULL);
}

void MenuScene::onShowDialog()
{
	m_menu->setEnabled(false);
}

void MenuScene::onCloseDialog()
{
	DataManager::sharedDataManager()->RefreshPlayerLife();
	CCString* s = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetLastPlayerLife());
	m_labelLife->setString(s->getCString());
	m_menu->setEnabled(true);
}

void MenuScene::onCompletedWaiting()
{
	//get revive_life
	tm* lasttm = DataManager::sharedDataManager()->GetLastDeadTime();
	time_t lastTime = mktime(lasttm);
	time_t curTime = time(NULL);
	double seconds = difftime(curTime, lastTime);

	int lastLife = (int)(seconds / G_PLAYER_TIME_TO_REVIVE);
	lastLife = (lastLife > G_MAX_PLAYER_LIFE) ? G_MAX_PLAYER_LIFE : lastLife;

	CCLOG("Revive Last life: %d", lastLife);

	if (lastLife > 0)
	{
		DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);

		CCScene *pScene = CCTransitionFade::create(0.5, MainGameScene::scene());
		CCDirector::sharedDirector()->replaceScene(pScene);

		CCLOG("Revive->LastLife > 0 -> Play");
	}
	else
	{
		CCMessageBox("Your code is failed!", "F**k the coder!");
	}
}
