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

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    m_BackgroundLayer = BackgroundLayer::create();
	this->addChild(m_BackgroundLayer);

	m_ObjLayer = ObjectLayer::create();
	this->addChild(m_ObjLayer);

	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
		"btnHome.png",
		"btnHome.png",
		this,
		menu_selector(MainGameScene::pauseCallback));
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
		origin.y + visibleSize.height - pCloseItem->getContentSize().height/2));
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);
	
	//////////////////////////////////////////////////////////////////////////
	
    return true;
}

void MainGameScene::pauseCallback(CCObject* pSender)
{
	m_BackgroundLayer->Pause();
	m_ObjLayer->Pause();
	PauseDialog* dialog = PauseDialog::create();
	this->addChild(dialog);
}

void MainGameScene::resumeCallback() 
{
	m_BackgroundLayer->Resume();
	m_ObjLayer->Resume();
}

void MainGameScene::showEndGame( int score )
{
	//check if enough last_life
	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
	lastLife--;
	DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);
	
	CCLOG("Last life: %d", lastLife);

	if (lastLife == 0) //end game
	{
		time_t curTime = time(NULL);
		tm* _tm = localtime(&curTime);
		DataManager::sharedDataManager()->SetLastDeadTimeNow();
		DataManager::sharedDataManager()->SetIsJustRevived(false);
		CCLOG("Not Enough Life To Play -> End game");
		CCScene *pScene = MenuScene::scene();
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
// 	else if (lastLife == 0) //end game
// 	{
// 		CCLOG("Enough Life To Play -> Continue");
// 		bool isJustRevived = DataManager::sharedDataManager()->GetIsJustRevived();
// 		LoseDialog* lose = LoseDialog::create(! isJustRevived);
// 		this->addChild(lose);
// 
// 		if (isJustRevived == true)
// 		{
// 			CCLOG("Can not revived again...");
// 		}
// 
// 		DataManager::sharedDataManager()->SetIsJustRevived(false);
// 	}
	else
	{
		CCLOG("Enough Life To Play -> Continue");
		bool isJustRevived = DataManager::sharedDataManager()->GetIsJustRevived();
		LoseDialog* lose = LoseDialog::create(! isJustRevived);
		this->addChild(lose);

		if (isJustRevived == true)
		{
			CCLOG("Can not revived again...");
		}
		
		DataManager::sharedDataManager()->SetIsJustRevived(false);
	}
}

//revived just 1 time for 1 life
void MainGameScene::okCallback()
{
 	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
 	lastLife++;
 	DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);
	
	CCLOG("Revive, lastLife: %d", lastLife);
	
	DataManager::sharedDataManager()->SetIsJustRevived(true);
	m_ObjLayer->ContinueGame();
}

void MainGameScene::cancelCallback()
{
	DataManager::sharedDataManager()->SetIsJustRevived(false);
	m_ObjLayer->RestartGame();
}
