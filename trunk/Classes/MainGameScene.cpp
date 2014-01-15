#include "MainGameScene.h"
#include "AudioManager.h"
#include "MenuScene.h"
#include "LoseDialog.h"
#include "ObjectLayer.h"
#include "DataManager.h"
#include <time.h>
#include "PauseDialog.h"

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
    if ( !CCLayer::init() )
    {
        return false;
    }

	m_isShowingPause = false;
	m_isShowingLose = false;

	this->setKeypadEnabled(true);

	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("MainGame_1.plist");
	cache->addSpriteFramesWithFile("MainGame_2.plist");

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
	CCMenu* pMenu = CCMenu::create(itPause, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);
	
	//////////////////////////////////////////////////////////////////////////
	
    return true;
}

void MainGameScene::pauseCallback(CCObject* pSender)
{
	if (m_isShowingLose || m_isShowingPause || m_ObjLayer->getIsEndGame())
	{
		return;
	}
	
	m_isShowingPause = true;

	m_BackgroundLayer->Pause();
	m_ObjLayer->Pause();
	PauseDialog* dialog = PauseDialog::create();
	this->addChild(dialog);
}

void MainGameScene::resumeCallback() 
{
	m_isShowingPause = false;

	m_BackgroundLayer->Resume();
	m_ObjLayer->Resume();
}

void MainGameScene::showEndGame( int score, int killedEnemies )
{
	m_isShowingLose = true;

	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	this->setTouchEnabled(false);
	
	//check if enough last_life
	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
	lastLife--;
	DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);
	
	CCLOG("Last life: %d", lastLife);

	time_t curTime = time(NULL);
	tm* _tm = localtime(&curTime);
	DataManager::sharedDataManager()->SetLastDeadTimeNow();
	DataManager::sharedDataManager()->SetIsJustRevived(false);
	
	LoseDialog* dialog = LoseDialog::create(score, killedEnemies);
	this->addChild(dialog);
}

//revived just 1 time for 1 life
void MainGameScene::reviveCallback()
{
	m_isShowingLose = false;

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);

 	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
 	lastLife++;
 	DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);
	
	CCLOG("Revive, lastLife: %d", lastLife);
	
	DataManager::sharedDataManager()->SetIsJustRevived(true);
	m_ObjLayer->ContinueGame();
}

void MainGameScene::restartCallback()
{
	m_isShowingLose = false;

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);

	DataManager::sharedDataManager()->SetIsJustRevived(false);
	m_ObjLayer->RestartGame();
}

void MainGameScene::keyBackClicked()
{
	pauseCallback(NULL);
}