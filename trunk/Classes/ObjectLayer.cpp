#include "ObjectLayer.h"
#include "Global.h"

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
	
	//m_playerFireTimer = CCTimer::timerWithTarget(this, schedule_selector( ObjectLayer::PlayerFire), 0.5f);
	this->schedule(schedule_selector(ObjectLayer::PlayerFireCallback), PLAYER_TIME_TO_FIRE);
	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();

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

void ObjectLayer::PlayerFireCallback(float dt)
{
	m_player->Fire();
}

//Player or enemi call to add bullet when fire
void ObjectLayer::AddBullet(Bullet* bullet )
{
	this->addChild(bullet);

	switch(bullet->getType())
	{
	case K_BULLET_PLAYER:
		m_arrPlayerBullets->addObject(bullet);
		break;

	case K_BULLET_ENEMI:
		m_arrEnemiBullets->addObject(bullet);
		break;
	}
}

void ObjectLayer::update( float delta )
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCObject* it;

	//Player's bullets
	CCARRAY_FOREACH(m_arrPlayerBullets, it)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it);
		if (NULL != bullet)
		{
			//out of screen
			if (bullet->getPositionY() > visibleSize.height)
			{
				this->removeChild(bullet);
				m_arrPlayerBullets->fastRemoveObject(it);
			}
		}
	}

	//Enemi's bullets
	CCARRAY_FOREACH(m_arrEnemiBullets, it)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it);
		if (NULL != bullet)
		{
			//out of screen
			if (bullet->getPositionY() < 0)
			{
				this->removeChild(bullet);
				m_arrEnemiBullets->fastRemoveObject(it);
			}
		}
	}
}

#pragma endregion TOUCH HANDLER

