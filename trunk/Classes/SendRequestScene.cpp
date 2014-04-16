#include "MenuScene.h"
#include "SendRequestScene.h"
#include "Global.h"

USING_NS_CC;

CCScene* SendRequestScene::scene()
{
    CCScene *scene = CCScene::create();
    SendRequestScene *layer = SendRequestScene::create();
    scene->addChild(layer);
    return scene;
}

bool SendRequestScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCSprite* bg = CCSprite::create("bg_stars.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg);
	
// 	bg->runAction(CCSequence::create(
// 		CCFadeIn::create(2.0f),
// 		CCDelayTime::create(1.0f),
// 		CCFadeOut::create(1.0f),
// 		CCCallFunc::create(this, callfunc_selector(SendRequestScene::MenuCallback)),
// 		NULL));

	//CCMenuItemImage* itSent = 


	
    return true;
}

void SendRequestScene::menuCallback()
{
	CCScene *pScene = MenuScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}
