#include "MainGameScene.h"

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
    
    m_BackgroundLayer = new BackgroundLayer();
	m_BackgroundLayer->init();
	this->addChild(m_BackgroundLayer);

	m_ObjLayer = new ObjectLayer();
	m_ObjLayer->init();
	this->addChild(m_ObjLayer);


	//////////////////////////////////////////////////////////////////////////

    return true;
}

