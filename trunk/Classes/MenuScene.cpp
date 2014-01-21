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
		m_isLoggedIn = true;
		m_fbItem->setSelectedIndex(1); //log out button
		CCLOG("FB_LOGIN_SUCCESSFUL");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_FAILED:
		CCLOG("FB_LOGIN_FAILED");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGOUT_SUCCESSFUL:  /////////////////// logged out state
		m_isLoggedIn = false;
		m_fbItem->setSelectedIndex(0); //log in button
		CCLOG("FB_LOGOUT_SUCCESSFUL");
		break;
	}
#endif
}

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
