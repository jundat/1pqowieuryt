#include "BackgroundLayer.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool BackgroundLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////
	
	// 1) Create the CCParallaxNode
	_backgroundNode = CCParallaxNodeExtras::node();
	this->addChild(_backgroundNode,-1);

	// 2) Create the sprites will be added to the CCParallaxNode
	_spacedust1 = CCSprite::create("bg_front_spacedust.png");
	_spacedust2 = CCSprite::create("bg_front_spacedust.png");
	_planetsunrise = CCSprite::create("bg_planetsunrise.png");
	_galaxy = CCSprite::create("bg_galaxy.png");
	_spacialanomaly = CCSprite::create("bg_spacialanomaly.png");
	_spacialanomaly2 = CCSprite::create("bg_spacialanomaly2.png");

	// 3) Determine relative movement speeds for space dust and background
	CCPoint dustSpeed = ccp(0.1, 0.1);
	CCPoint bgSpeed = ccp(0.05, 0.05);

	// 4) Add children to CCParallaxNode
	_backgroundNode->addChild(_spacedust1, 0, dustSpeed, ccp(visibleSize.width/2, 0)); // 2
	_backgroundNode->addChild(_spacedust2, 0, dustSpeed, ccp(visibleSize.width/2, _spacedust1->getContentSize().height));
	_backgroundNode->addChild(_galaxy, -1, bgSpeed, ccp(visibleSize.width * 0.7, 0));
	_backgroundNode->addChild(_planetsunrise, -1 , bgSpeed, ccp(visibleSize.width * 0,  600));
	_backgroundNode->addChild(_spacialanomaly, -1, bgSpeed, ccp(visibleSize.width * 0.3, 900));
	_backgroundNode->addChild(_spacialanomaly2, -1, bgSpeed, ccp(visibleSize.width * 0.9, 1500));

	//////////////////////////////////////////////////////////////////////////
	
	this->scheduleUpdate();
	//////////////////////////////////////////////////////////////////////////

    return true;
}

void BackgroundLayer::update(float dt) {
	CCPoint backgroundScrollVert = ccp(0, -1000);
	_backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(), ccpMult(backgroundScrollVert, dt)));

	//////////////////////////////////////////////////////////////////////////
	CCArray *spaceDusts = CCArray::createWithCapacity(2);
	spaceDusts->addObject(_spacedust1);
	spaceDusts->addObject(_spacedust2);

	for ( int ii = 0; ii < spaceDusts->count(); ii++ ) {
		CCSprite * spaceDust = (CCSprite *)(spaceDusts->objectAtIndex(ii));
		float yPosition = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).y;
		float size = spaceDust->getContentSize().height;
		if ( yPosition < - size/2 ) {
			_backgroundNode->incrementOffset(ccp(0, spaceDust->getContentSize().height*2),spaceDust); 
		}
	}

	CCArray *backGrounds = CCArray::createWithCapacity(4);
	backGrounds->addObject(_galaxy);
	backGrounds->addObject(_planetsunrise);
	backGrounds->addObject(_spacialanomaly);
	backGrounds->addObject(_spacialanomaly2);

	for ( int ii = 0; ii < backGrounds->count(); ii++ ) {
		CCSprite * background = (CCSprite *)(backGrounds->objectAtIndex(ii));
		float yPosition = _backgroundNode->convertToWorldSpace(background->getPosition()).y;
		float size = background->getContentSize().height;
		if ( yPosition < -size ) {
			_backgroundNode->incrementOffset(ccp(0, 2000),background); 
		}
	}
}
