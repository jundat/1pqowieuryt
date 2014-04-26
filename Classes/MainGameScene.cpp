#include "MainGameScene.h"
#include "AudioManager.h"
#include "MenuScene.h"
#include "LoseDialog.h"
#include "ObjectLayer.h"
#include "DataManager.h"
#include <time.h>
#include "PauseDialog.h"
#include "GameClientManager.h"
#include "BreakLeaderboardDialog.h"
#include "InGameTutorial.h"

USING_NS_CC;

CCScene* MainGameScene::scene()
{
    CCScene *scene = CCScene::create();
    MainGameScene *layer = MainGameScene::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool MainGameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(195, 200, 201, 255)) )
    {
        return false;
    }

	m_isShowingPause = false;
	m_isShowingLose = false;
    
    m_isLoggedIn = false;
    string fbId = DataManager::sharedDataManager()->GetFbID();
    if (fbId.compare("NULL") == 0) {
        m_isLoggedIn = false;
    } else {
        m_isLoggedIn = true;
    }

	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	//this->setTouchEnabled(true);
	this->setKeypadEnabled(true);

	CCSpriteFrameCache* sprcache = CCSpriteFrameCache::sharedSpriteFrameCache();
	sprcache->addSpriteFramesWithFile("MainGame.plist");
	

    m_BackgroundLayer = BackgroundLayer::create();
	this->addChild(m_BackgroundLayer);

	m_ObjLayer = ObjectLayer::create();
	this->addChild(m_ObjLayer);

	CCMenuItemImage *itPause = CCMenuItemImage::create(
		"pause_0.png",
		"pause_1.png",
		this,
		menu_selector(MainGameScene::pauseCallback));
	itPause->setPosition(ccp(1.15f * itPause->getContentSize().width/2, G_DESIGN_HEIGHT - 1.15f * itPause->getContentSize().height/2));

	CCMenu* pMenu = CCMenu::create(itPause, /*itCheat, */ NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu);
	

	//////////////////////////////////////////////////////////////////////////

	// testSendPlayerProfile
	CCLabelTTF *labelPost1 = CCLabelTTF::create("AddScore", "Arial", 48);
	labelPost1->setColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost1 = CCMenuItemLabel::create(labelPost1, this, menu_selector(MainGameScene::addScoreCallback));
	itemPost1->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost1->setPosition(ccp(20, 1100));
	//pMenu->addChild(itemPost1); //DEBUG

	// testSendPlayerProfile
	CCLabelTTF *labelPost2 = CCLabelTTF::create("SubScore", "Arial", 48);
	labelPost2->setColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost2 = CCMenuItemLabel::create(labelPost2, this, menu_selector(MainGameScene::subScoreCallback));
	itemPost2->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost2->setPosition(ccp(20, 1000));
	//pMenu->addChild(itemPost2); //DEBUG

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//View tut first time
	bool isFirstTimeViewTut = DataManager::sharedDataManager()->GetFirstTimeViewTutorial();
	if (isFirstTimeViewTut)
	{
		DataManager::sharedDataManager()->SetFirstTimeViewTutorial(false);

		this->ShowTutorial();
	}


	//////////////////////////////////////////////////////////////////////////

	PLAY_BACKGROUND_MUSIC;

    return true;
}

void MainGameScene::onEnterTransitionDidFinish()
{
	//CCLOG("[FeedAPIScene]: Enter Transition Finished.");
	//m_BackgroundLayer->scheduleUpdate();
}

void MainGameScene::pauseCallback(CCObject* pSender)
{
	if (m_isShowingLose || m_isShowingPause || m_ObjLayer->getIsEndGame())
	{
		return;
	}

	PLAY_BUTTON_EFFECT;

	m_isShowingPause = true;

	m_BackgroundLayer->Pause();
	m_ObjLayer->Pause();
	PauseDialog* dialog = PauseDialog::create();
	this->addChild(dialog);
}

void MainGameScene::resumeCallback() 
{
	m_isShowingLose = false;
	m_isShowingPause = false;

	m_BackgroundLayer->Resume();
	m_ObjLayer->Resume();
}

void MainGameScene::showEndGame( int score, int killedEnemies )
{
	m_isShowingLose = true;
	
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	this->setTouchEnabled(false);

	//refresh life, if lastLife = 4, and when playing, lastLife increase to 5
	DataManager::sharedDataManager()->RefreshPlayerLife();
    
	//check if enough last_life
	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
    if (m_isLoggedIn == true) {
        lastLife--;
    } else {
        lastLife = G_MAX_PLAYER_LIFE;
    }
    DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);

	CCLOG("Last life: %d", lastLife);

	//if life = G_MAX_PLAYER_LIFE = 4
	//start counter
	if (lastLife == G_MAX_PLAYER_LIFE - 1)
	{
		CCLOG("SET LAST DEAD TIME VALUE");
		DataManager::sharedDataManager()->SetLastDeadTimeNow();
	}

	PLAY_GAME_OVER;

	int oldScore = DataManager::sharedDataManager()->GetHighScore();
	bool isBreakRecord = false;

	if (oldScore < score)
	{
		isBreakRecord = true;
	}
	
	LoseDialog* dialog = LoseDialog::create(score, isBreakRecord);
	this->addChild(dialog);

	//show memory info
	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();

	//save highscore after show dialog
	DataManager::sharedDataManager()->SetHighScore(score);

	//submit score
	if (DataManager::sharedDataManager()->GetFbID().compare("NULL") != 0)
	{
		GameClientManager::sharedGameClientManager()->sendScore(
			DataManager::sharedDataManager()->GetFbID(), 
			DataManager::sharedDataManager()->GetHighScore());
	}
}

//revived just 1 time for 1 life
void MainGameScene::reviveCallback()
{
	m_isShowingLose = false;
	m_isShowingPause = false;

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);

 	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
 	lastLife++;
 	DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);
	
	CCLOG("------------- REVIVE ------------ %d", lastLife);
	
	DataManager::sharedDataManager()->SetIsJustRevived(true);
	m_ObjLayer->Revive();
}

void MainGameScene::restartCallback()
{
	m_isShowingLose = false;
	m_isShowingPause = false;

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);

	DataManager::sharedDataManager()->SetIsJustRevived(false);
	m_ObjLayer->Restart();
	m_BackgroundLayer->Resume();
}

void MainGameScene::keyBackClicked()
{
	pauseCallback(NULL);
}

void MainGameScene::cheatCallback( CCObject* pSender )
{
	m_ObjLayer->setScore(m_ObjLayer->getScore() + 100000);
}

void MainGameScene::addScoreCallback( CCObject* pSender )
{
	m_ObjLayer->setScore(m_ObjLayer->getScore() + 100000);
}

void MainGameScene::subScoreCallback( CCObject* pSender )
{
	m_ObjLayer->setScore(m_ObjLayer->getScore() - 100000);
}

void MainGameScene::ShowTutorial()
{
	InGameTutorial* tut = InGameTutorial::create();
	this->addChild(tut, 11);

	//this->setKeypadEnabled(false);
	m_isShowingPause = true;
	m_BackgroundLayer->Pause();
	m_ObjLayer->Pause();
}
