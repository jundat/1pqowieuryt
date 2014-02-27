#include "MenuScene.h"
#include "IntroScene.h"
#include "Global.h"

USING_NS_CC;

CCScene* IntroScene::scene()
{
    CCScene *scene = CCScene::create();
    IntroScene *layer = IntroScene::create();
    scene->addChild(layer);
    return scene;
}

bool IntroScene::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)) )
    {
        return false;
    }

	CCSprite* sprLogo = CCSprite::create("logo.png");
	sprLogo->setPosition(ccp(400, 640));
	sprLogo->runAction(CCSequence::create(
		CCFadeIn::create(1.5f),
		CCDelayTime::create(2.0f),
		CCCallFunc::create(this, callfunc_selector(IntroScene::menuCallback)),
		NULL));
	this->addChild(sprLogo);

	CCScene *pScene = MenuScene::scene();
    return true;
}

void IntroScene::menuCallback()
{
	CCScene *pScene = CCTransitionFade::create(1.5f, MenuScene::scene(), ccWHITE);
	CCDirector::sharedDirector()->replaceScene(pScene);
}
