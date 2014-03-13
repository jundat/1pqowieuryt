#include "MenuScene.h"
#include "MainGameScene.h"
#include "ScoreScene.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "WaitForLifeDialog.h"
#include "QuitDialog.h"
#include <time.h>

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
	DataManager::sharedDataManager()->RefreshPlayerLife();

	//////////////////////////////////////////////////////////////////////////
	

    if ( !CCLayerColor::initWithColor(G_MENU_BG_COLOR) )
    {
        return false;
    }

	m_isShowDialog = false;
	this->setKeypadEnabled(true);

    /////////////////////////////

	CCSprite* bg = CCSprite::create(G_MENU_BG);
	bg->setAnchorPoint(G_MENU_BG_ANCHORPOINT);
	bg->setPosition(G_MENU_BG_POS);
	this->addChild(bg, G_MENU_BG_Z);
	
	initLifeIcon();	

	//diamon
	CCString* s = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetDiamon());
	CCLabelTTF* lbDiamon = CCLabelTTF::create(s->getCString(), G_FONT_NORMAL, G_MENU_DIAMON_TEXT_SIZE);
	lbDiamon->setColor(G_MENU_DIAMON_TEXT_COLOR);
	lbDiamon->setAnchorPoint(G_MENU_DIAMON_TEXT_ANCHOR);
	lbDiamon->setPosition(G_MENU_DIAMON_TEXT_POS);
	this->addChild(lbDiamon);


	CCSprite* sprDiamon = CCSprite::create("diamond.png");
	sprDiamon->setPosition(ccp(lbDiamon->getPositionX() - lbDiamon->getContentSize().width - sprDiamon->getContentSize().width/1.5f, lbDiamon->getPositionY()));
	this->addChild(sprDiamon);

	//////////////////////////////////////////////////////////////////////////

	//
// 	s = CCString::createWithFormat("v%d", G_VERSION);
// 	CCLabelTTF* labelVersion = CCLabelTTF::create(s->getCString(), G_FONT_NORMAL, G_MENU_VERSION_TEXT_SIZE);
// 	labelVersion->setColor(G_MENU_VERSION_TEXT_COLOR);
// 	labelVersion->setPosition(G_MENU_VERSION_TEXT_POS);
// 	this->addChild(labelVersion);

	//
    m_playItem = CCMenuItemImage::create(
                                        G_MENU_NEW_BUTTON_SPR_NORMAL,
                                        G_MENU_NEW_BUTTON_SPR_PRESS,
                                        this,
                                        menu_selector(MenuScene::playCallback));
	m_playItem->setPosition(G_MENU_NEW_BUTTON_POS);

	CCMenuItemImage *scoreItem = CCMenuItemImage::create(
		"score_button.png",
		"score_button_press.png",
		this,
		menu_selector(MenuScene::scoreCallback));
	scoreItem->setPosition(ccp(400, 1280-1175));

	
	//

	CCMenuItem* soundOn = CCMenuItemImage::create("sound_on.png", NULL, NULL);
	CCMenuItem* soundOff = CCMenuItemImage::create("sound_off.png", NULL, NULL);
	CCMenuItemToggle* soundToggle = CCMenuItemToggle::createWithTarget(this,  menu_selector(MenuScene::soundCallback), soundOn, soundOff, NULL);
	if(AudioManager::sharedAudioManager()->IsEnableBackground())
	{
		soundToggle->setSelectedIndex(0);
	}
	else
	{
		soundToggle->setSelectedIndex(1);
	}
	soundToggle->setPosition(ccp(121, 1280-1176));

	//rate
	CCMenuItemImage *itRate = CCMenuItemImage::create(
		"rate.png",
		"rateDown.png",
		this,
		menu_selector(MenuScene::rateCallback));
	itRate->setPosition(ccp(548, 1280-1181));

	//exit
	CCMenuItemImage *itExit = CCMenuItemImage::create(
		"exit.png",
		"exitDown.png",
		this,
		menu_selector(MenuScene::exitCallback));
	itExit->setPosition(ccp(692, 1280-1181));

    m_menu = CCMenu::create(m_playItem, scoreItem, soundToggle, /*itRate,*/ itExit, NULL);
    m_menu->setPosition(CCPointZero);
    this->addChild(m_menu, 1);

	//check if life = 0 to show

	int life = DataManager::sharedDataManager()->GetLastPlayerLife();
	CCLOG("MenuScene: Last life = %d", life);
	if (life < G_MAX_PLAYER_LIFE) //start counter when not full of life
	{
		initTimer();
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
		CCSprite* _sprBlur = CCSprite::create("oil.png");
		_sprBlur->setOpacity(50);
		_sprBlur->setPosition(ccp(x + i * w, y));
		this->addChild(_sprBlur);

		CCSprite* _spr = CCSprite::create("oil.png");
		_spr->setPosition(ccp(x + i * w, y));
		_spr->setVisible(false);
		this->addChild(_spr);

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
	//CCTransitionProgessVertical
	//CCTransitionMoveInT
	//CCTransitionSlideInT
	
// 	CCSprite* spr = (CCSprite*)m_arrSprLife->objectAtIndex(lastLife - 1);
// 	float dy = 600;
// 	float time = dy / 320.0f;
// 
// 	CCSpawn* spawn = CCSpawn::create(
// 			CCFadeOut::create(time),
// 			CCEaseBackIn::create(CCMoveBy::create(time, ccp(0, dy))),
// 			CCSequence::create(
// 				CCDelayTime::create(1.0f),
// 				CCCallFunc::create(this, callfunc_selector(MenuScene::gotoMainGame)),
// 				NULL
// 			),
// 			NULL
// 		);
// 
// 	CCSequence* seq = CCSequence::create(
// 		//CCDelayTime::create(0.5f),
// 		spawn,
// 		NULL
// 		);
// 	
// 	MainGameScene::scene();
// 
// 	spr->runAction(seq);

	CCSprite* spr = (CCSprite*)m_arrSprLife->objectAtIndex(lastLife - 1);
	CCSequence* seq = CCSequence::create(
		CCSpawn::create(
		//CCScaleTo::create(0.5f, 0.0f, 0.0f),
		//CCFadeOut::create(0.5f)
		CCMoveTo::create(0.5f, ccp(spr->getPositionX(), 1280 + 200)),
		NULL
		),
		CCCallFunc::create(this, callfunc_selector(MenuScene::gotoMainGame)),
		NULL);
	spr->runAction(seq);
}

void MenuScene::gotoMainGame()
{
// 	CCScene *pScene = CCTransitionSlideInT::create(1280.0f / 480.0f, MainGameScene::scene());
// 	CCDirector::sharedDirector()->replaceScene(pScene);

	CCScene *pScene = CCTransitionFade::create(0.5, MainGameScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void MenuScene::playCallback(CCObject* pSender)
{
	PLAY_ENEMY1_DOWN_EFFECT;

	this->setKeypadEnabled(false);
	this->setTouchEnabled(false);

	//check if last_player_life > 0
	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();

	CCLOG("GOTO PLAY: Lastlife: %d", lastLife);

	if (lastLife > 0)
	{
		playStartAnimation(lastLife);
		m_playItem->selected();
	}
	else
	{
		WaitForLifeDialog* dialog = WaitForLifeDialog::create((float)G_PLAYER_TIME_TO_REVIVE);
		this->addChild(dialog, 100);
		this->setTouchEnabled(false);
		onShowDialog();
	}
}

void MenuScene::scoreCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	CCScene *pScene = CCTransitionFade::create(0.5, ScoreScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
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
	//CCString* s = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetLastPlayerLife());
	//m_labelLife->setString(s->getCString());
	initLifeIcon();
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
}

void MenuScene::rateCallback( CCObject* pSender )
{
	CCDictionary* prms = CCDictionary::create();
	prms->setObject(CCString::create(G_URL_RATE), "link");

	SendMessageWithParams(string("Rate"), prms);
}

void MenuScene::exitCallback( CCObject* pSender )
{
	QuitDialog* dialog = QuitDialog::create();
	this->addChild(dialog, 10);
	this->onShowDialog();
}

void MenuScene::ScheduleTick( float dt )
{
	tm* lasttm = DataManager::sharedDataManager()->GetLastDeadTime();
	int diff = (int)difftime(time(NULL), mktime(lasttm));

	m_waitTime = 1000 * G_PLAYER_TIME_TO_REVIVE - diff;
	m_waitTime %= G_PLAYER_TIME_TO_REVIVE;

	int mins = m_waitTime / 60;
	int seconds = m_waitTime % 60;

	if (seconds == 0)
	{
		DataManager::sharedDataManager()->RefreshPlayerLife();
		refreshLifeIcon();

		int life = DataManager::sharedDataManager()->GetLastPlayerLife();
		CCLOG("REFRESH LIFE ICON, Life = %d", life);
		if (life >= G_MAX_PLAYER_LIFE)
		{
			this->unschedule(schedule_selector(MenuScene::ScheduleTick));
			m_lbTime->setVisible(false);
			return;
		}
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

	m_lbTime->setString(s->getCString());
}

void MenuScene::initTimer()
{
	CCLOG("Initing... timer! ...");

	int life = DataManager::sharedDataManager()->GetLastPlayerLife();
	tm* lasttm = DataManager::sharedDataManager()->GetLastDeadTime();
	int diff = (int)difftime(time(NULL), mktime(lasttm));

	m_waitTime = 1000 * G_PLAYER_TIME_TO_REVIVE - diff;
	m_waitTime %= G_PLAYER_TIME_TO_REVIVE;


	if (m_waitTime < 0)
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
