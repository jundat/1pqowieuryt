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

	m_friendList = NULL;

	m_isShowDialog = false;
	this->setKeypadEnabled(true);

    /////////////////////////////

	CCSprite* bg = CCSprite::create("bg_stars.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg, 0);
	
	CCSprite* menuTop = CCSprite::create("menu_top.png");
	menuTop->setPosition(ccp(436, 1280-210));
	this->addChild(menuTop, 0);

	//
	CCString* s = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetLastPlayerLife());
	m_labelLife = CCLabelBMFont::create(s->getCString(), "Mia_64.fnt");
	m_labelLife->setColor(ccc3(56, 56, 56));
	m_labelLife->setPosition(ccp(400, 1280-508));
	this->addChild(m_labelLife);

	//

	//
	s = CCString::createWithFormat("v%d", 24);
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
    
	playItem->setPosition(ccp(400, 1280-653));


	CCMenuItemImage *scoreItem = CCMenuItemImage::create(
		"score_button.png",
		"score_button_press.png",
		this,
		menu_selector(MenuScene::scoreCallback));

	scoreItem->setPosition(ccp(400, 1280-813));

	//

	CCMenuItemImage *settingItem = CCMenuItemImage::create(
		"setting_button.png",
		"setting_button_press.png",
		this,
		menu_selector(MenuScene::settingCallback));

	settingItem->setPosition(ccp(400, 1280-973));

	//

	CCMenuItemImage *exitItem = CCMenuItemImage::create(
		"exit_button.png",
		"exit_button_press.png",
		this,
		menu_selector(MenuScene::exitCallback));

	exitItem->setPosition(ccp(G_DESIGN_WIDTH/2,
		settingItem->getPositionY() - settingItem->getContentSize().height/2 - exitItem->getContentSize().height/2 - 10));
	exitItem->setVisible(false);
	exitItem->setEnabled(false);

	//facebook /////////////////////////////////////

	m_fbItem = CCMenuItemImage::create("connect_facebook.png", "connect_facebook.png", this, menu_selector(MenuScene::fbCallback));
	m_fbItem->setPosition(ccp(400, 1280-1220));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EziSocialObject::sharedObject()->setFacebookDelegate(this);

	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		//login button
		m_fbItem->setVisible(false);
	}
#endif

	//end facebook
	

    m_menu = CCMenu::create(playItem, scoreItem, settingItem, exitItem, m_fbItem, NULL);
    m_menu->setPosition(CCPointZero);
    this->addChild(m_menu, 1);

	STOP_BACKGROUND_MUSIC;

    return true;
}

void MenuScene::playCallback(CCObject* pSender)
{
	PLAY_BUTTON_EFFECT;

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
	PLAY_BUTTON_EFFECT;

	CCScene *pScene = CCTransitionFade::create(0.5, ScoreScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void MenuScene::settingCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	CCScene *pScene = CCTransitionFade::create(0.5, SettingScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void MenuScene::exitCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	CCDirector::sharedDirector()->end();
}

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

//facebook ///////////////////////////////

void MenuScene::fbCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	bool needPublicPermission = true;
	EziSocialObject::sharedObject()->performLoginUsingFacebook(needPublicPermission); // Pass true if you need publish permission also
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void MenuScene::fbSessionCallback(int responseCode, const char *responseMessage)
{
	if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL) /////////////////// logged in state
	{
		m_fbItem->setVisible(false); //log out button
		EziSocialObject::sharedObject()->fetchFBUserDetails(true); //need email = true
		//EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ONLY_INSTALLED, 0, NUM_REQUEST_FRIENDS);
	}
	else
	{
		m_fbItem->setVisible(true); //log out button
	}
}

void MenuScene::fbUserDetailCallback( int responseCode, const char* responseMessage, EziFacebookUser* fbUser )
{
	if (fbUser != NULL)
	{
		// Set the current user in EziSocialObject to use it in future. EziSocialObject is immune to CCScene lifecycle. It is singleton class and will live forever.
		EziSocialObject::sharedObject()->setCurrentFacebookUser(fbUser);

		//save data
		std::string fullname = fbUser->getFullName();
		std::string userName = fbUser->getUserName();
		std::string profileID = fbUser->getProfileID();

		DataManager::sharedDataManager()->SetName(fullname.c_str());
		DataManager::sharedDataManager()->SetProfileID(profileID.c_str());
		DataManager::sharedDataManager()->SetFbUserName(userName.c_str());
		
		//CCLOG("FullName: %s", fullname.c_str());
		//CCLOG("EmailID: %s", emailID.c_str());
		//CCLOG("Gender: %s", gender.c_str());
		//CCLOG("UserName: %s", userName.c_str());
		//CCLOG("ProfileID: %s", profileID.c_str());
		//CCLOG("AccessToken: %s", accessToken.c_str());
		
		scoreCallback(NULL);
	}
}

void MenuScene::fbFriendsCallback( int responseCode, const char* responseMessage, cocos2d::CCArray* friends )
{
	if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_FRIEND_GET_SUCCESS)
	{
		if (m_friendList != NULL)
		{
			m_friendList->release();
			m_friendList = NULL;
		}

		m_friendList = CCArray::createWithArray(friends);
		m_friendList->retain();
	}
}



#endif

//end facebook ///////////////////////////////