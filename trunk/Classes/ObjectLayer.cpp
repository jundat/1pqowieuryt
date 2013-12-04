#include "ObjectLayer.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool ObjectLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	//////////////////////////////////////////////////////////////////////////
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);
	//////////////////////////////////////////////////////////////////////////
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////

	m_player = new Ship();
	m_player->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.1));
	this->addChild(m_player);
	m_IsTouchedPlayer = false;
	
	//////////////////////////////////////////////////////////////////////////

    return true;
}


#pragma region TOUCH HANDLER

bool ObjectLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	m_IsTouchedPlayer = false;

	if (m_player->boundingBox().containsPoint(m_player->convertToNodeSpace(pTouch->getLocation())))
	{
		m_IsTouchedPlayer = true;
	}

	return true;
}

void ObjectLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if (m_IsTouchedPlayer)
	{
		m_player->setPosition(pTouch->getLocation());
	}
}

void ObjectLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	m_IsTouchedPlayer = false;
}

#pragma endregion TOUCH HANDLER
