#include "MenuScene.h"
#include "IntroScene.h"

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
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite* bg = CCSprite::create("welcome.png");
	bg->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(bg, 0);
	
	bg->runAction(CCSequence::create(
		CCFadeIn::create(2.0f),
		CCDelayTime::create(1.0f),
		CCFadeOut::create(1.0f),
		CCCallFunc::create(this, callfunc_selector(IntroScene::MenuCallback)),
		NULL));

	CCScene *pScene = MenuScene::scene();
    return true;
}

void IntroScene::MenuCallback()
{
	CCScene *pScene = MenuScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}
