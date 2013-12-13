#include "MainGameScene.h"
#include "AudioManager.h"
#include "MenuScene.h"
#include "LoseDialog.h"
#include "ObjectLayer.h"

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
		menu_selector(MainGameScene::menuCallback));
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
		origin.y + visibleSize.height - pCloseItem->getContentSize().height/2));
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);
	
	//////////////////////////////////////////////////////////////////////////
	
    return true;
}

void MainGameScene::menuCallback(CCObject* pSender)
{
	CCScene *pScene = MenuScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void MainGameScene::showEndGame( int score )
{
	LoseDialog* lose = LoseDialog::create();
	this->addChild(lose);
}

void MainGameScene::okCallback()
{
	m_ObjLayer->ContinueGame();
}

void MainGameScene::cancelCallback()
{
	m_ObjLayer->RestartGame();
}
