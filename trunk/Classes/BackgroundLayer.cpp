#include "BackgroundLayer.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool BackgroundLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(195, 200, 201, 255)))
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	bg1 = CCSprite::create("bg_stars.png");
	bg2 = CCSprite::create("bg_stars.png");
	CCSize size = bg1->getContentSize();
	
	bg1->setPosition(ccp(visibleSize.width/2, visibleSize.height/2));
	bg2->setPosition(ccp(visibleSize.width/2, visibleSize.height/2));

	this->addChild(bg1);
	this->addChild(bg2);

	this->scheduleUpdate();
    return true;
}

void BackgroundLayer::update(float dt) {
	bg1->setPositionY(bg1->getPositionY() - 120 * dt);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCSize size = bg1->getContentSize();

	if (bg1->getPositionY() <= - size.height/2)
	{
		bg1->setPositionY(bg1->getPositionY() + size.height * 2);
	}

	if (bg1->getPositionY() <= visibleSize.height/2)
	{
		bg2->setPositionY(bg1->getPositionY() + size.height);
	} 
	else
	{
		bg2->setPositionY(bg1->getPositionY() - size.height);
	}
}

void BackgroundLayer::Pause()
{
	this->unscheduleUpdate();
}

void BackgroundLayer::Resume()
{
	this->scheduleUpdate();
}
