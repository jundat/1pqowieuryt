#include "MenuScene.h"
#include "MainGameScene.h"
#include "ScoreScene.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "WaitForLifeDialog.h"
#include "QuitDialog.h"
#include <time.h>
#include "TextLoader.h"
#include "LogOutDialog.h"
#include "TryPlayDialog.h"
#include "NotLoggedInMenuScene.h"
#include "WaitDialog.h"

USING_NS_CC;
USING_NS_CC_EXT;

static bool IS_SHOW_VERSION = false;
static int  WAIT_DIALOG_TAG = 100;


int MenuScene::s_beginFriendInd = 0;
int MenuScene::s_endFriendInd = 0;



CCScene* MenuScene::scene()
{
    CCScene *scene = CCScene::create();
    MenuScene *layer = MenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MenuScene::init()
{
	DataManager::sharedDataManager()->RefreshPlayerLife();
	GameClientManager::sharedGameClientManager()->setDelegate(this);
    
	//////////////////////////////////////////////////////////////////////////

    if ( !CCLayerColor::initWithColor(G_MENU_BG_COLOR) )
    {
        return false;
    }

	m_isShowDialog = false;
	this->setKeypadEnabled(true);
    m_waitDialog = NULL;

	//////////////////////////////////////////////////////////////////////////
	m_isLoggedIn = false;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	EziSocialObject::sharedObject()->setFacebookDelegate(this);
	
    string sfbid = DataManager::sharedDataManager()->GetFbID();
    if (sfbid.compare("NULL") != 0)
    //if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		m_isLoggedIn = true;
	}
    else
    {
        m_isLoggedIn = false;
    }
#endif

    /////////////////////////////
	
	string lang = DataManager::sharedDataManager()->GetLanguage();
	if (lang.compare("English") == 0)
	{
		m_bg = CCSprite::create(G_MENU_BG_EN);
	}
	else
	{
		m_bg = CCSprite::create(G_MENU_BG_VN);
	}	
	
	m_bg->setAnchorPoint(G_MENU_BG_ANCHORPOINT);
	m_bg->setPosition(G_MENU_BG_POS);
	this->addChild(m_bg, G_MENU_BG_Z);
	
	initLifeIcon();	

	//////////////////////////////////////////////////////////////////////////

	//
    if (IS_SHOW_VERSION) {
        CCString* s = CCString::createWithFormat("v%d", G_VERSION);
        CCLabelTTF* labelVersion = CCLabelTTF::create(s->getCString(), G_FONT_NORMAL, G_MENU_VERSION_TEXT_SIZE);
        labelVersion->setColor(G_MENU_VERSION_TEXT_COLOR);
        labelVersion->setPosition(G_MENU_VERSION_TEXT_POS);
        this->addChild(labelVersion);
    }
    
    //
    //new button border
    //
    m_sprNewButtonBorderOut = CCSprite::create("new_button_border_out.png");
    m_sprNewButtonBorderIn = CCSprite::create("new_button_border_in.png");
    
    m_sprNewButtonBorderOut->setPosition(G_MENU_NEW_BUTTON_POS);
    m_sprNewButtonBorderIn->setPosition(G_MENU_NEW_BUTTON_POS);
    
    this->addChild(m_sprNewButtonBorderOut);
    this->addChild(m_sprNewButtonBorderIn);
    
    
    //
    //disable play button animation in local mode
	//
	if (lang.compare("English") == 0)
	{
		m_playItem = CCMenuItemImage::create(
			G_MENU_NEW_BUTTON_SPR_NORMAL_EN,
			G_MENU_NEW_BUTTON_SPR_PRESS_EN,
			this,
			menu_selector(MenuScene::playCallback));
	} 
	else
	{
		m_playItem = CCMenuItemImage::create(
			G_MENU_NEW_BUTTON_SPR_NORMAL_VN,
			G_MENU_NEW_BUTTON_SPR_PRESS_VN,
			this,
			menu_selector(MenuScene::playCallback));
	}

	m_playItem->setPosition(G_MENU_NEW_BUTTON_POS);
    
	m_scoreItem = CCMenuItemImage::create(
		"score_button.png",
		"score_button_press.png",
		this,
		menu_selector(MenuScene::scoreCallback));
	m_scoreItem->setPosition(ccp(315, 1280-1176));

    //
	//rate
    //
	CCMenuItemImage *itRate = CCMenuItemImage::create(
		"rate.png",
		"rateDown.png",
		this,
		menu_selector(MenuScene::rateCallback));
	itRate->setPosition(ccp(502, 1280-1180));

	
	//////////////////////////////////////////////////////////////////////////
	//Setting Bar

	m_sprSettingBar = CCSprite::create("setting_bar.png");
	m_sprSettingBar->setAnchorPoint(ccp(0.5f, 0.0f));
	m_sprSettingBar->setPosition(ccp(108, 1280-1245));
	m_sprSettingBar->setVisible(false);


	m_itShowCharge = CCMenuItemImage::create("charge.png", "charge_down.png", "charge_down.png", this, menu_selector(MenuScene::showChargeCallback));
	m_itShowCharge->setPosition(ccp(70, m_sprSettingBar->getContentSize().height - 65 + 121));
    m_itShowCharge->setVisible(false);


	CCMenuItem* facebookOn = CCMenuItemImage::create("facebook_in.png", NULL, NULL);
	CCMenuItem* facebookOff = CCMenuItemImage::create("facebook_out.png", NULL, NULL);
	m_facebookItem = CCMenuItemToggle::createWithTarget(this,  menu_selector(MenuScene::facebookCallback), facebookOn, facebookOff, NULL);
	m_facebookItem->setPosition(ccp(70, m_sprSettingBar->getContentSize().height-65));
	if (m_isLoggedIn)
	{
		m_facebookItem->setSelectedIndex(1);
	} 
	else
	{
		m_facebookItem->setSelectedIndex(0);
	}

	CCMenuItem* soundOn = CCMenuItemImage::create("sound_on.png", NULL, NULL);
	CCMenuItem* soundOff = CCMenuItemImage::create("sound_off.png", NULL, NULL);
	m_soundItem = CCMenuItemToggle::createWithTarget(this,  menu_selector(MenuScene::soundCallback), soundOn, soundOff, NULL);
	m_soundItem->setPosition(ccp(81, m_sprSettingBar->getContentSize().height-181));
	if(AudioManager::sharedAudioManager()->IsEnableBackground())
	{
		m_soundItem->setSelectedIndex(0);
	}
	else
	{
		m_soundItem->setSelectedIndex(1);
	}

	CCMenu* settingMenu = CCMenu::create(m_facebookItem, m_soundItem, m_itShowCharge, NULL);
	settingMenu->setPosition(CCPointZero);
	
	m_sprSettingBar->addChild(settingMenu);

	this->addChild(m_sprSettingBar, 2);

	CCMenuItem* settingOn = CCMenuItemImage::create("setting.png", NULL, NULL);
	CCMenuItem* settingOff = CCMenuItemImage::create("settingDown.png", NULL, NULL);
	m_settingItem = CCMenuItemToggle::createWithTarget(this,  menu_selector(MenuScene::settingCallback), settingOn, settingOff, NULL);
	m_settingItem->setPosition(ccp(112, 1280-1178));

	CCMenu* menuForSetting = CCMenu::create(m_settingItem, NULL);
	menuForSetting->setPosition(CCPointZero);
	this->addChild(menuForSetting, 3);
	
	//setting bar
	//////////////////////////////////////////////////////////////////////////
	//Language bar

	m_sprLanguageBar = CCSprite::create("language_bar.png");
	m_sprLanguageBar->setAnchorPoint(ccp(0.5f, 0.0f));
	m_sprLanguageBar->setPosition(ccp(688, 1280-1253));
	m_sprLanguageBar->setVisible(false);

	m_englishItem = CCMenuItemImage::create("flag_english.png", "flag_english_down.png", "flag_english_down.png", this, menu_selector(MenuScene::englishCallback));
	m_englishItem->setPosition(ccp(71, m_sprLanguageBar->getContentSize().height-53)); //(703, 1280-947));

	m_vietnamItem = CCMenuItemImage::create("flag_vietnam.png", "flag_vietnam_down.png", "flag_vietnam_down.png", this, menu_selector(MenuScene::vietnamCallback));
	m_vietnamItem->setPosition(ccp(71, m_sprLanguageBar->getContentSize().height-53)); //703, 1280-1065));

	CCMenu* languageMenu = CCMenu::create(m_englishItem, m_vietnamItem, NULL);
	languageMenu->setPosition(CCPointZero);

	m_sprLanguageBar->addChild(languageMenu);

	this->addChild(m_sprLanguageBar, 2);

	//check current language before set
	if (lang.compare("English") == 0)
	{
		m_englishItem->setVisible(false);
		m_languageItem = CCMenuItemImage::create("flag_english.png", "flag_english_down.png", this, menu_selector(MenuScene::languageCallback));
	} 
	else
	{
		m_vietnamItem->setVisible(false);
		m_languageItem = CCMenuItemImage::create("flag_vietnam.png", "flag_vietnam_down.png", this, menu_selector(MenuScene::languageCallback));
	}
	m_languageItem->setPosition(ccp(690, 1280-1202));

	CCMenu* menuForLanguage = CCMenu::create(m_languageItem, NULL);
	menuForLanguage->setPosition(CCPointZero);
	this->addChild(menuForLanguage, 3);


	//Language bar
	//////////////////////////////////////////////////////////////////////////
    
    m_menu = CCMenu::create(m_playItem, m_scoreItem, itRate, NULL);
    m_menu->setPosition(CCPointZero);
    this->addChild(m_menu, 1);



	//check if life = 0 to show

	//int life = DataManager::sharedDataManager()->GetLastPlayerLife();
	//CCLOG("MenuScene: Last life = %d", life);
	//if (life < G_MAX_PLAYER_LIFE) //start counter when not full of life
	//{
	
    initTimer();
	
    //}

	if (DataManager::sharedDataManager()->GetRegistrationId().length() == 0)
	{
		GetRegistrationId();
	}

    //
    //refresh
    //
    if (m_isLoggedIn == true) {
        
        //set all user item from server
        this->getLife();
        this->getAllItems();
        
        GameClientManager::sharedGameClientManager()->getScore(DataManager::sharedDataManager()->GetFbID());
        
        this->checkRefreshFriendList();
    }
    
    
	STOP_BACKGROUND_MUSIC;
    return true;
}

void MenuScene::initLifeIcon()
{
	float w = CCSprite::create("oil.png")->getContentSize().width;
	float x = (800 - w * 5)/2 + w/2;
	float y = 1280 - 445;

	m_arrSprLife = new CCArray();
	m_arrSprLife->retain();

	//life icon
	for (int i = 0; i < G_MAX_PLAYER_LIFE; ++i)
	{
		CCSprite* _sprBlur = CCSprite::create("oil_blur.png");
		_sprBlur->setPosition(ccp(x + i * w, y));
		this->addChild(_sprBlur, 1); //////////////////////////////////new ////////////////////////////////////////

		CCSprite* _spr = CCSprite::create("oil.png");
		_spr->setPosition(ccp(x + i * w, y));
		_spr->setVisible(false);
		this->addChild(_spr, 1.1); ////////////////////////////////////new //////////////////////////////////////
		
		m_arrSprLife->addObject(_spr);
	}

	int life = DataManager::sharedDataManager()->GetLastPlayerLife();
	for (int i = 0; i < life; ++i)
	{
		CCSprite* _spr = (CCSprite*) m_arrSprLife->objectAtIndex(i);
		_spr->setVisible(true);
	}
}

void MenuScene::refreshLifeIcon()
{
    CCLOG("MenuScene::refreshLifeIcon");
    
	for (int i = 0; i < G_MAX_PLAYER_LIFE; ++i)
	{
		CCSprite* _spr = (CCSprite*) m_arrSprLife->objectAtIndex(i);
		_spr->setVisible(false);
	}

	////////////////////////////////////////////////////////////////////////////////////////////

	int life = DataManager::sharedDataManager()->GetLastPlayerLife();
	for (int i = 0; i < life; ++i)
	{
		CCSprite* _spr = (CCSprite*) m_arrSprLife->objectAtIndex(i);
		_spr->setVisible(true);
	}
}

void MenuScene::playStartAnimation(int lastLife)
{
    //
    //animation
    //
    
    m_sprNewButtonBorderOut->runAction(
       CCSequence::createWithTwoActions(
        CCRotateTo::create(0.15f, 40),
        CCRotateTo::create(0.15f, 0)
        ));
    m_sprNewButtonBorderIn->runAction(
      CCSequence::createWithTwoActions(
       CCRotateTo::create(0.15f, -40),
       CCRotateTo::create(0.15f, 0)
       ));
    
	this->setKeypadEnabled(false);
	this->setTouchEnabled(false);

    
    CCSprite* spr = (CCSprite*)m_arrSprLife->objectAtIndex(lastLife - 1);
	CCSequence* seq = CCSequence::create(
        CCMoveTo::create(0.6f, ccp(spr->getPositionX(), 1280 + 200)),
		CCCallFunc::create(this, callfunc_selector(MenuScene::gotoMainGame)),
		NULL);
	spr->runAction(seq);
}

void MenuScene::gotoMainGame()
{
    PLAY_ENEMY1_DOWN_EFFECT;
    
    GameClientManager::sharedGameClientManager()->setDelegate(NULL);
    
    CCScene *pScene = CCTransitionFade::create(0.5, MainGameScene::scene());
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void MenuScene::playCallback(CCObject* pSender)
{
    PLAY_GUN_RELOAD_EFFECT;
	
    //check if last_player_life > 0
	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
	if (lastLife > 0) //enough life -> go play
	{
        playStartAnimation(lastLife);
        m_playItem->selected();
        
        //request to server
        GameClientManager::sharedGameClientManager()->useLife(DataManager::sharedDataManager()->GetFbID());
	}
	else //not enough life -> request life
	{
		WaitForLifeDialog* dialog = WaitForLifeDialog::create((float)G_PLAYER_TIME_TO_REVIVE);
		this->addChild(dialog, 100);
		this->setTouchEnabled(false);
		this->onShowDialog();
	}
}

void MenuScene::scoreCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	GameClientManager::sharedGameClientManager()->setDelegate(NULL);

	CCScene *pScene = CCTransitionFade::create(0.5, ScoreScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void MenuScene::keyBackClicked()
{
	exitCallback(NULL);
}

void MenuScene::onRateCompleted( CCNode *sender, void *data )
{
	if (data != NULL)
	{
		CCDictionary *convertedData = (CCDictionary *)data;
		CCString* s = (CCString*)convertedData->objectForKey("isSuccess");
		if (s->boolValue())
		{
			//CCLOG("CPP Rate Completed: TRUE");
			CCString* s = (CCString*)convertedData->objectForKey("responseType");
			//CCLOG("%s", s->getCString());
			if (s->compare("RATE") == 0)
			{
				CCMessageBox(TXT("menu_thank_rate"), TXT("menu_thank_caption"));
			}
		} 
		else
		{
			//CCLOG("CPP Rate Completed: FALSE");
		}

		NDKHelper::RemoveSelector("MENU", "onRateCompleted");
	}
}

void MenuScene::onShowDialog()
{
	m_menu->setEnabled(false);
	this->setKeypadEnabled(false);
}

void MenuScene::onCloseDialog()
{
	//DataManager::sharedDataManager()->RefreshPlayerLife();
    //this->refreshLifeIcon();
	
	m_menu->setEnabled(true);
	this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
}

void MenuScene::onCompletedWaiting()
{
	//get revive_life
	long lasttm = DataManager::sharedDataManager()->GetLastDeadTime();
	double seconds = static_cast<long int>(time(NULL)) - lasttm;

	int lastLife = (int)(seconds / G_PLAYER_TIME_TO_REVIVE);
	lastLife = (lastLife > G_MAX_PLAYER_LIFE) ? G_MAX_PLAYER_LIFE : lastLife;

	//CCLOG("Revive Last life: %d", lastLife);

	if (lastLife > 0)
	{
		DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);
		initLifeIcon();
	}
	else
	{
		CCLOG("Your code is failed!, F**k the coder!");
	}
}

void MenuScene::soundCallback( CCObject* pSender )
{
	if(AudioManager::sharedAudioManager()->IsEnableBackground())
	{
		PLAY_BUTTON_EFFECT;

		AudioManager::sharedAudioManager()->SetEnableBackground(false);
		AudioManager::sharedAudioManager()->SetEnableEffect(false);

		//
		AudioManager::sharedAudioManager()->StopBackground();
	}
	else
	{
		AudioManager::sharedAudioManager()->SetEnableBackground(true);
		AudioManager::sharedAudioManager()->SetEnableEffect(true);
	}

	//m_sprSettingBar->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.5f), CCHide::create()));
}

void MenuScene::rateCallback( CCObject* pSender )
{
	NDKHelper::AddSelector("MENU",
		"onRateCompleted",
		callfuncND_selector(MenuScene::onRateCompleted),
		this);

	CCDictionary* prms = CCDictionary::create();
	prms->setObject(CCString::create(TXT("game_name")), "game_name");
	prms->setObject(CCString::create(TXT("game_package")), "game_package");
	prms->setObject(CCString::create(TXT("rate_msg")), "msg");
	prms->setObject(CCString::create(TXT("rate_rate")), "rate");
	prms->setObject(CCString::create(""), "later");
	prms->setObject(CCString::create(TXT("rate_no")), "no");

	SendMessageWithParams(string("Rate"), prms);
}

void MenuScene::settingCallback( CCObject* pSender )
{
	if (m_sprSettingBar->isVisible())
	{
		m_sprSettingBar->setVisible(false);
	} 
	else
	{
		m_sprSettingBar->setVisible(true);
	}
}

void MenuScene::facebookCallback( CCObject* pSender )
{
    CCLOG("MenuScene::facebookCallback");
	PLAY_BUTTON_EFFECT;

    this->facebookLogOut();
}

void MenuScene::facebookLogOut()
{
    CCLOG("MenuScene::facebookLogOut");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    EziSocialObject::sharedObject()->perfromLogoutFromFacebook();
#endif
}

void MenuScene::facebookLogIn()
{
    
    CCLOG("MenuScene::facebookLogIn");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    bool needPublicPermission = true;
    EziSocialObject::sharedObject()->performLoginUsingFacebook(needPublicPermission); // Pass true if you need publish permission also
    
    //show wait dialog to ignore all touch
    this->showWaitDialog(TXT("wait_connect_server"));
#endif
    
}

void MenuScene::exitCallback( CCObject* pSender )
{
	QuitDialog* dialog = QuitDialog::create();
    dialog->setYesNoDialogParent(this);
	this->addChild(dialog, 10);
	this->onShowDialog();
}

void MenuScene::ScheduleTick( float dt )
{
    int life = DataManager::sharedDataManager()->GetLastPlayerLife();
    
    if (life < G_MAX_PLAYER_LIFE) {
        
        long lasttm = DataManager::sharedDataManager()->GetLastDeadTime();
        int diff = static_cast<long int>(time(NULL)) - lasttm;
        
        m_waitTime = 1000 * G_PLAYER_TIME_TO_REVIVE - diff;
        m_waitTime = (m_waitTime < 0) ? 0 : m_waitTime;
        m_waitTime %= G_PLAYER_TIME_TO_REVIVE;
        
        int mins = m_waitTime / 60;
        int seconds = m_waitTime % 60;
        
        if (seconds == 0 && mins == 0)
        {
            //get life from server
            CCSequence* seq = CCSequence::create(CCDelayTime::create(2.0f),
                               CCCallFunc::create(this, callfunc_selector(MenuScene::getLife)));
            m_lbTime->runAction(seq);
            CCLOG("Get life to refresh life after 1 seconds (s)....");
            
            DataManager::sharedDataManager()->RefreshPlayerLife();
            refreshLifeIcon();
        }
        
        CCString* s;
        if (seconds >= 10)
        {
            s = CCString::createWithFormat("0%d:%d", mins, seconds);
        }
        else
        {
            s = CCString::createWithFormat("0%d:0%d", mins, seconds);
        }
        
        m_lbTime->setVisible(true);
        m_lbTime->setString(s->getCString());
        
    } else {
        m_lbTime->setVisible(false);
    }
}

void MenuScene::initTimer()
{
	//CCLOG("Initing... timer! ...");

	int life = DataManager::sharedDataManager()->GetLastPlayerLife();
	long lasttm = DataManager::sharedDataManager()->GetLastDeadTime();
	long diff = static_cast<long int>(time(NULL)) - lasttm;

	m_waitTime = 1000 * G_PLAYER_TIME_TO_REVIVE - diff;
	m_waitTime %= G_PLAYER_TIME_TO_REVIVE;


	if (m_waitTime < 0 || life >= G_MAX_PLAYER_LIFE)
	{
		DataManager::sharedDataManager()->SetLastPlayerLife(G_MAX_PLAYER_LIFE);
		return;
	}
	
	int mins = m_waitTime / 60;
	int seconds = m_waitTime % 60;

	CCString* s;
	if (seconds >= 10)
	{
		s = CCString::createWithFormat("0%d:%d", mins, seconds);
	} 
	else
	{
		s = CCString::createWithFormat("0%d:0%d", mins, seconds);
	}
	
	m_lbTime = CCLabelTTF::create(s->getCString(), "Roboto-Medium", 48);
	m_lbTime->setPosition(ccp(400, 1280-531));
	m_lbTime->setColor(ccc3(56, 56, 56));
	this->addChild(m_lbTime);

	this->schedule(schedule_selector(MenuScene::ScheduleTick), 1);
}

void MenuScene::GetRegistrationId()
{
	//CCLOG("GetRegistrationId");
	NDKHelper::AddSelector("MENU",
		"onGetRegistrationIdCompleted",
		callfuncND_selector(MenuScene::onGetRegistrationIdCompleted),
		this);
	SendMessageWithParams(string("GetRegistrationId"), NULL);
}

void MenuScene::onGetRegistrationIdCompleted( CCNode *sender, void *data )
{
	//CCLOG("onGetRegistrationIdCompleted");
	if (data != NULL)
	{
		CCDictionary *convertedData = (CCDictionary *)data;
		CCString* s = (CCString*)convertedData->objectForKey("isSuccess");
		if (s->boolValue())
		{
			//CCLOG("CPP RegistrationId Completed: TRUE");
			CCString* s = (CCString*)convertedData->objectForKey("registrationId");
			CCLOG("CPP: RegistrationId: %s", s->getCString());

			DataManager::sharedDataManager()->SetRegistrationId(s->getCString());

			//send to server if fbId != NULL
			string fbid = DataManager::sharedDataManager()->GetFbID();
			if (fbid.compare("NULL") != 0)
			{
				GameClientManager::sharedGameClientManager()->sendDeviceProfile(
				DataManager::sharedDataManager()->GetFbID(),
				string(""),
				s->getCString(),
				string(""),
				string("")
				);
			}			
		} 
		else
		{
			//CCLOG("CPP RegistrationId Completed: FALSE");
		}

		NDKHelper::RemoveSelector("MENU", "onGetRegistrationIdCompleted");
	}
}



///////////////// FACEBOOK ////////////////////



void MenuScene::fbSessionCallback(int responseCode, const char *responseMessage)
{
	//CCLOG("fbSessionCallback");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL)
	{
		CCLOG("fbSessionCallback: INNNNN");
		m_isLoggedIn = true;
        m_facebookItem->setSelectedIndex(1);
        
        //
		//auto get profile, info
        //
        
        bool needsEmail = true;
        EziSocialObject::sharedObject()->fetchFBUserDetails(needsEmail);
	}
	else
	{
		CCLOG("fbSessionCallback: OUTTTT");
        
        if (m_isLoggedIn == false) { //truoc do chwa log in
            CCMessageBox(TXT("menu_error_server"), TXT("menu_error_caption"));
        }
        
		m_isLoggedIn = false;
        m_facebookItem->setSelectedIndex(0);
        
        //close wait dialog
        this->closeWaitDialog();
        
        //
        //Clear data
        //
        //set FbId = "NULL";
        DataManager::sharedDataManager()->ClearFbProfileID();
        
        //clear highscores
        DataManager::sharedDataManager()->SetHigherFriends(NULL);
        
        
        //go to NotLoggedInMenuSene
        GameClientManager::sharedGameClientManager()->setDelegate(NULL);
        
        CCScene *pScene = CCTransitionFade::create(0.5, NotLoggedInMenuScene::scene());
        CCDirector::sharedDirector()->replaceScene(pScene);
	}
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS) // fbUserDetailCallback

void MenuScene::fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser )
{
	//CCLOG("fbUserDetailCallback");
	if (fbUser != NULL)
	{
		//CCLOG("fbUserDetailCallback: user != NULL");
		EziSocialObject::sharedObject()->setCurrentFacebookUser(fbUser);

		//save data
		std::string firstname = fbUser->getFirstName(); //getFirstName //getFullName
		std::string userName = fbUser->getUserName();
		std::string profileID = fbUser->getProfileID();
		std::string fullName = fbUser->getFullName(); //getEmailID
		std::string emailID = fbUser->getEmailID();

		DataManager::sharedDataManager()->SetName(firstname.c_str());
		DataManager::sharedDataManager()->SetFbProfileID(profileID.c_str());
		DataManager::sharedDataManager()->SetFbUserName(userName.c_str());
		DataManager::sharedDataManager()->SetFbFullName(fullName.c_str());
		DataManager::sharedDataManager()->SetFbEmail(emailID.c_str());

		sendUserProfileToServer(profileID, fullName, emailID);

        this->closeWaitDialog();
		getFacebookFriends();

		EziSocialObject::sharedObject()->getProfilePicForID(this, fbUser->getProfileID(), G_AVATAR_SIZE, G_AVATAR_SIZE, false);
	}
}
#endif

void MenuScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
	//CCLOG("fbUserPhotoCallback");
	std::string sid = std::string(fbID);

	if ((strcmp(userPhotoPath, "") != 0))
	{
		DataManager::sharedDataManager()->SetPhotoPath(userPhotoPath);
	}
}

void MenuScene::getFacebookFriends()
{
    //DEBUG
	CCLOG("getFacebookFriends");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	MenuScene::s_beginFriendInd = 0;
	MenuScene::s_endFriendInd = G_NUMBER_FRIEND_TO_GET - 1;
	EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ALL_FRIENDS, MenuScene::s_beginFriendInd, MenuScene::s_endFriendInd);

	DataManager::sharedDataManager()->SetTimeRefreshFriendNow();
    
    this->showWaitDialog(TXT("wait_connect_server"));
#endif
}

void MenuScene::fbFriendsCallback( int responseCode, const char* responseMessage, cocos2d::CCArray* friends )
{
	//CCLOG("fbFriendsCallback");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	int count = friends->count();
    //CCLOG("GOT >>>>>>>>>>>>>> %d friends", count);
	if (count > 0)
	{
		CCArray* arrFriends = new CCArray();
		arrFriends->retain();

		CCObject* it;
		CCARRAY_FOREACH(friends, it)
		{
			EziFacebookFriend* fbFriend = dynamic_cast<EziFacebookFriend*>(it);
			if (NULL != it)
			{
				string fbId = fbFriend->getFBID();
				string fbName = fbFriend->getName();
                
                //(string _fbId, string _fbName, string _email, int _score, int _coin, long timeGetLaze, long timeSendLife)
                
				FacebookAccount* acc = new FacebookAccount(
                       fbId,
                       fbName,
                       std::string(""),
                       -1,
                       -1,
                       -1,
                       -1);

				arrFriends->addObject(acc);
			}
		}

		GameClientManager::sharedGameClientManager()->sendFriendList(DataManager::sharedDataManager()->GetFbID(), arrFriends);		

		MenuScene::s_beginFriendInd += G_NUMBER_FRIEND_TO_GET;
		MenuScene::s_endFriendInd += G_NUMBER_FRIEND_TO_GET;
		EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ALL_FRIENDS, MenuScene::s_beginFriendInd, MenuScene::s_endFriendInd);
	} else {
        
        //end of friends
        this->closeWaitDialog();
    }
#endif
}

void MenuScene::languageCallback( CCObject* pSender )
{
	if (m_sprLanguageBar->isVisible())
	{
		m_sprLanguageBar->setVisible(false);
	} 
	else
	{
		m_sprLanguageBar->setVisible(true);
	}
}

void MenuScene::englishCallback( CCObject* pSender )
{
	m_englishItem->setVisible(false);
	m_vietnamItem->setVisible(true);

    
    CCSprite* normalImage = (CCSprite*)m_englishItem->getNormalImage();
    CCSprite* newNormalImage = CCSprite::createWithTexture(normalImage->getTexture());
    
    CCSprite* selectedImage = (CCSprite*)m_englishItem->getSelectedImage();
    CCSprite* newSelectedImage = CCSprite::createWithTexture(selectedImage->getTexture());
    
	m_languageItem->setNormalImage(newNormalImage);
	m_languageItem->setSelectedImage(newSelectedImage);
    
	m_sprLanguageBar->setVisible(false);
	DataManager::sharedDataManager()->SetLanguage("English");
	TextLoader::shareTextLoader()->setCurrentLanguage(LANGUAGE_ENGLISH);

	//refresh UI
	refreshLanguageUI();
}

void MenuScene::vietnamCallback( CCObject* pSender )
{
	m_englishItem->setVisible(true);
	m_vietnamItem->setVisible(false);

    CCSprite* normalImage = (CCSprite*)m_vietnamItem->getNormalImage();
    CCSprite* newNormalImage = CCSprite::createWithTexture(normalImage->getTexture());
    
    CCSprite* selectedImage = (CCSprite*)m_vietnamItem->getSelectedImage();
    CCSprite* newSelectedImage = CCSprite::createWithTexture(selectedImage->getTexture());
    
	m_languageItem->setNormalImage(newNormalImage);
	m_languageItem->setSelectedImage(newSelectedImage);

	m_sprLanguageBar->setVisible(false);
	DataManager::sharedDataManager()->SetLanguage("Vietnamese");
	TextLoader::shareTextLoader()->setCurrentLanguage(LANGUAGE_VIETNAMESE);

	//refresh UI
	refreshLanguageUI();
}

void MenuScene::refreshLanguageUI()
{
	string lang = DataManager::sharedDataManager()->GetLanguage();

	if (lang.compare("English") == 0)
	{
		//bg
		m_bg->setTexture(CCSprite::create(G_MENU_BG_EN)->getTexture());

		//start
		m_playItem->setNormalImage(CCSprite::create(G_MENU_NEW_BUTTON_SPR_NORMAL_EN));
		m_playItem->setSelectedImage(CCSprite::create(G_MENU_NEW_BUTTON_SPR_PRESS_EN));
	}
	else
	{
		//bg
        m_bg->setTexture(CCSprite::create(G_MENU_BG_VN)->getTexture());

		//start
		m_playItem->setNormalImage(CCSprite::create(G_MENU_NEW_BUTTON_SPR_NORMAL_VN));
		m_playItem->setSelectedImage(CCSprite::create(G_MENU_NEW_BUTTON_SPR_PRESS_VN));
	}
}

void MenuScene::onPushNotification( CCNode *sender, void *data )
{
	if (data != NULL)
	{
		CCDictionary *convertedData = (CCDictionary *)data;
		CCString* s = (CCString*)convertedData->objectForKey("isSuccess");
		if (s->boolValue())
		{
			//CCLOG("CPP onPushNotification Completed: TRUE");
			CCMessageBox("Push notification Success!", "Kaka");
		} 
		else
		{
			//CCLOG("CPP onPushNotification Completed: FALSE");
		}

		//NDKHelper::RemoveSelector("MENU", "onPushNotification");
	}
}

void MenuScene::showChargeCallback( CCObject* pSender )
{
	NDKHelper::AddSelector("MENU",
		"onShowChargeCompleted",
		callfuncND_selector(MenuScene::onShowChargeCompleted),
		this);

	CCDictionary* prms = CCDictionary::create();
	prms->setObject(CCString::create(TXT("charge_caption")), "charge_caption");
	prms->setObject(CCString::create(TXT("charge_close")), "charge_close");
	prms->setObject(CCString::create(TXT("charge_link")), "charge_link");

	SendMessageWithParams(string("ShowChargeWebView"), prms);
}

void MenuScene::onShowChargeCompleted( CCNode *sender, void *data )
{
	//CCLOG("CPP: onShowChargeCompleted");
	//m_sprSettingBar->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.5f), CCHide::create()));
}

void MenuScene::getUserProfile()
{
	string fbId = DataManager::sharedDataManager()->GetFbID();

	if (fbId.compare("NULL") != 0)
	{
		GameClientManager::sharedGameClientManager()->getPlayerFbProfile(fbId);
	}
	else
	{
		CCLOG("fbId == NULL, could not connect server to get diamond");
	}
}

void MenuScene::onGetPlayerFbProfileCompleted( bool isSuccess, FacebookAccount* acc )
{
	if (isSuccess)
	{
		CCLOG("DIAMOND: %d", acc->m_coin);
		DataManager::sharedDataManager()->SetDiamon(acc->m_coin);
	}
	else
	{
        //failed to connect server
		this->disableMoneytize();
	}
}

void MenuScene::sendUserProfileToServer(string fbId, string fbName, string email)
{
    this->showWaitDialog(TXT("wait_connect_server"));
    
	//CCLOG("sendUserProfileToServer");
	GameClientManager::sharedGameClientManager()->sendPlayerFbProfile(fbId, fbName, email);
	
	//send device token to server
	string regid = DataManager::sharedDataManager()->GetRegistrationId();
	if (regid.length() > 0)
	{
		GameClientManager::sharedGameClientManager()->sendDeviceProfile(
			DataManager::sharedDataManager()->GetFbID(),
			string(""),
			DataManager::sharedDataManager()->GetRegistrationId(),
			string(""),
			string("")
			);
	}
}


void MenuScene::onSendPlayerFbProfileCompleted( bool isSuccess )
{
    this->closeWaitDialog();
    
	if (isSuccess)
	{
		//get User Profile
		this->getUserProfile();
        
        //set all user item from server
        this->getLife();
        this->getAllItems();
        
        GameClientManager::sharedGameClientManager()->getScore(DataManager::sharedDataManager()->GetFbID());
	}
	else
	{
		//failed to connect server
		this->disableMoneytize();
	}
}


void MenuScene::disableMoneytize()
{
    this->closeWaitDialog();
    this->closeWaitDialog();
    this->closeWaitDialog();
    
    CCMessageBox(TXT("menu_error_server"), TXT("menu_error_caption"));
    
	////iOS
	//CCLOG("DISABLE MONEYTIZE...");
	//DataManager::sharedDataManager()->SetIsMoneytize(false);
	////remove chargeTutorial
	//m_itShowCharge->setVisible(false);

	//Android, Windows Phone
	CCLOG("...... Disable monitize, SET DIAMOND = DEFAULT......");
	DataManager::sharedDataManager()->SetDiamon(G_DEFAULT_DIAMON);
    
    
    
    facebookLogOut();
    
    //go to NotLoggedInMenuSene
    //GameClientManager::sharedGameClientManager()->setDelegate(NULL);
    
    //CCScene *pScene = CCTransitionFade::create(0.5, NotLoggedInMenuScene::scene());
    //CCDirector::sharedDirector()->replaceScene(pScene);
}


void MenuScene::getAllItems()
{
    //this->showWaitDialog(TXT("wait_connect_server"));
    
    CCLOG("MenuScene::getAllItems");
    GameClientManager::sharedGameClientManager()->getAllItem(DataManager::sharedDataManager()->GetFbID());
}


void MenuScene::onGetAllItemsCompleted(bool isSuccess, int laze, int coin)
{
    CCLOG("MenuScene::onGetAllItemsCompleted");
    CCLOG("laze: %d", laze);
    CCLOG("coin: %d", coin);
    
    //this->closeWaitDialog();

    if (isSuccess) {
        DataManager::sharedDataManager()->SetBoom(laze);
        DataManager::sharedDataManager()->SetDiamon(coin);
        
    } else {
        
        this->disableMoneytize();
    }
}


void MenuScene::showWaitDialog(string title)
{
    CCLOG("MenuScene::showWaitDialog");
    this->onShowDialog();
    
    if (m_waitDialog != NULL) {
        m_waitDialog->m_refCount++;

    } else {
        
        //time out to close waiting dialog
        
        CCLOG("Schedule connection timeout...");
        this->scheduleOnce(schedule_selector(MenuScene::connectionTimeOut), CONNECTION_TIMEOUT);
        
        
        m_waitDialog = WaitDialog::create();
        m_waitDialog->m_refCount = 1;
        m_waitDialog->setTitle(title);
        
        this->addChild(m_waitDialog, WAIT_DIALOG_TAG); // =1
    }
}


void MenuScene::closeWaitDialog()
{
    CCLOG("MenuScene::closeWaitDialog");
    
    if (m_waitDialog != NULL) {
        m_waitDialog->m_refCount--;
        
        if (m_waitDialog->m_refCount <= 0) {
            
            //unschedule close waitdialog
            CCLOG("Unschedule connection timeout...");
            this->unschedule(schedule_selector(MenuScene::connectionTimeOut));
            
            this->removeChild(m_waitDialog);
            m_waitDialog = NULL;
            this->onCloseDialog();
        }
    }
}


bool MenuScene::checkRefreshFriendList()
{
	CCLOG("~~~~~--- check Refresh Friend List ---~~~~~");
	//check
	//get boom
	tm* _lastTimeRefreshFriend = DataManager::sharedDataManager()->GetTimeRefreshFriend();
	if (_lastTimeRefreshFriend == NULL)
	{
		CCLOG("checkRefreshFriendList == first time");
		this->getFacebookFriends();
        return true;
	}
	else
	{
		time_t lastTime = mktime(_lastTimeRefreshFriend);
		time_t curTime = time(NULL);
		double elapsedTime = difftime(curTime, lastTime);
		
		if (elapsedTime > G_TIME_TO_REFRESH_FRIENDS)
		{
			CCLOG("checkRefreshFriendList == true");
			this->getFacebookFriends();
            return true;
		}
		else
		{
			CCLOG("NOT ENOUGH TIME TO REFRESH");
		}
	}
    
    return false;
}


void MenuScene::onUseLifeCompleted(bool isSuccess, int newLife, long lastTime_client)
{
    CCLOG("MenuScene::onUseLifeCompleted");
    
    CCLOG("life: %d, lastTime: %ld", newLife, lastTime_client);
    
    if (isSuccess) {
        CCLOG("Success");
        DataManager::sharedDataManager()->SetLastPlayerLife(newLife);
        DataManager::sharedDataManager()->SetLastDeadTime(lastTime_client);
        
    } else {
        CCLOG("Failed");
        
        //failed to connect server
		//this->disableMoneytize();
    }
}


void MenuScene::getLife()
{
    //this->showWaitDialog(TXT("wait_connect_server"));
    
    CCLOG("MenuScene::getLife");
    
    GameClientManager::sharedGameClientManager()->getLife(DataManager::sharedDataManager()->GetFbID());
}


void MenuScene::onGetLifeCompleted(bool isSuccess, int life, long lastTimeClient_Second)
{
    CCLOG("MenuScene::onGetLifeCompleted: %s, %d, %ld", BoolToStr(isSuccess), life, lastTimeClient_Second);
    
    //this->closeWaitDialog();
    
    if (isSuccess) {
        
        DataManager::sharedDataManager()->SetLastPlayerLife(life);
        DataManager::sharedDataManager()->SetLastDeadTime(lastTimeClient_Second);
        
        this->refreshLifeIcon();
    
    } else {
        CCLOG("FAILED TO CONNECT SERVER");
        
		this->disableMoneytize();
    }
}


void MenuScene::onGetScoreCompleted( bool isSuccess, int score, std::string time )
{
    CCLOG("MenuScene::onGetScoreCompleted");
    
    if (isSuccess) {
        DataManager::sharedDataManager()->SetHighScore(score);
    } else {
        CCLOG("FAILED TO GET SCORE");
        
		this->disableMoneytize();
    }
}


void MenuScene::connectionTimeOut()
{
    CCLOG("~~~~~~~~~~~~~~~~~~ MenuScene::connectionTimeOut ~~~~~~~~~~~~~~~~~~");
    
    this->disableMoneytize();
}



