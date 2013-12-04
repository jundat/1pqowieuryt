#include "ObjectLayer.h"
#include "Global.h"
#include "AudioManager.h"
#include "Enemi.h"


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
	
	this->schedule(schedule_selector(ObjectLayer::SchedulePlayerFire), PLAYER_TIME_TO_FIRE);
	m_timeToGenerateEnemi = DEFAULT_TIME_TO_GENERATE_ENEMI;
	this->schedule(schedule_selector(ObjectLayer::ScheduleGenerateEnemi), m_timeToGenerateEnemi);
	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();

    return true;
}


#pragma region TOUCH HANDLER

bool ObjectLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	m_IsTouchedPlayer = false;

	if (m_player->boundingBox().containsPoint(m_player->convertToNodeSpace(pTouch->getLocation()))) //
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

void ObjectLayer::SchedulePlayerFire(float dt)
{
	m_player->Fire();
}


//schedule generate enemi 
void ObjectLayer::ScheduleGenerateEnemi( float dt )
{
	//ramdom type
	int type = (int)(CCRANDOM_0_1() * NUMBER_OF_ENEMI) + 1;
	Enemi* enemi = new Enemi(type);

	//random position
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	float x = (int)(CCRANDOM_0_1() * visibleSize.width);
	float y = visibleSize.height + enemi->boundingBox().size.height/2;

	//add
	enemi->setPosition(ccp(x, y));
	m_arrEnemi->addObject(enemi);
	this->addChild(enemi);
}

//Player or enemi call to add bullet when fire
void ObjectLayer::AddBullet(Bullet* bullet )
{
	this->addChild(bullet);

	switch(bullet->getBulletType())
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

	CCObject* it = NULL;

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

	//update enemi
	CCARRAY_FOREACH(m_arrEnemi, it)
	{
		Enemi* enemi = dynamic_cast<Enemi*>(it);
		if (NULL != enemi)
		{
			//out of screen
			if (enemi->getPositionY() < - enemi->boundingBox().size.height)
			{
				this->removeChild(enemi);
				m_arrEnemi->fastRemoveObject(it);
			}
		}
	}

	//update collision
	_UpdateCollision();
}

//check collision every frame
void ObjectLayer::_UpdateCollision()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCObject* it1 = NULL;

	//////////////////////////////////////////////////////////////////////////
	//player's bullet -----VS------ Enemi
	CCARRAY_FOREACH(m_arrPlayerBullets, it1)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it1);
		if (NULL != bullet)
		{
			CCRect bulletRect = bullet->boundingBox();

			CCObject* it2 = NULL;

			//foreach all enemi
			CCARRAY_FOREACH(m_arrEnemi, it2)
			{
				Enemi* enemi = dynamic_cast<Enemi*>(it2);
				if (NULL != enemi)
				{
					CCRect enemiRect = enemi->boundingBox();
					
					if (bulletRect.intersectsRect(enemiRect))
					{
						this->removeChild(bullet);
						m_arrPlayerBullets->removeObject(it1);
						this->removeChild(enemi);
						m_arrEnemi->removeObject(it2);

						//sound
						AudioManager::sharedAudioManager()->PlayEffect("explosion.wav");
					}
				}
			}
		}
	}

}



#pragma endregion TOUCH HANDLER

