#include "ObjectLayer.h"
#include "Global.h"
#include "AudioManager.h"
#include "Enemi.h"
#include "EffectLayer.h"

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

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////

	m_arrEnemi = new CCArray();
	m_arrPlayerBullets = new CCArray();
	m_arrEnemiBullets = new CCArray();

	m_arrEnemi->retain();
	m_arrPlayerBullets->retain();
	m_arrEnemiBullets->retain();

	m_score = 0;

	m_pLabelScore = CCLabelTTF::create("0", "Marker Felt.ttf", 48);
	m_pLabelScore->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - m_pLabelScore->getContentSize().height));
	this->addChild(m_pLabelScore, 10);

	m_player = Ship::create();
	m_player->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.1));
	this->addChild(m_player);
	m_IsTouchedPlayer = false;

	m_Joystick = HSJoystick::create();
	m_player->setJoystick(m_Joystick);
	this->addChild(m_Joystick, 100);
	
	this->schedule(schedule_selector(ObjectLayer::SchedulePlayerFire), PLAYER_TIME_TO_FIRE);
	m_timeToGenerateEnemi = DEFAULT_TIME_TO_GENERATE_ENEMI;
	this->schedule(schedule_selector(ObjectLayer::ScheduleGenerateEnemi), m_timeToGenerateEnemi);
	
	m_EffectLayer = EffectLayer::create();
	this->addChild(m_EffectLayer, 10);
	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();

    return true;
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
	Enemi* enemi = Enemi::create(type);
	float w = enemi->boundingBox().size.width;

	//random position
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	float x = (int)(CCRANDOM_0_1() * (visibleSize.width - w));
	float y = visibleSize.height + enemi->boundingBox().size.height/2;
	enemi->setPosition(ccp(x + w/2, y));

	m_arrEnemi->addObject(enemi);
	this->addChild(enemi);
}

//Player or enemi call to add bullet when fire
void ObjectLayer::AddBullet(Bullet* bullet)
{
	switch(bullet->getBulletType())
	{
	case K_BULLET_PLAYER:
		m_arrPlayerBullets->addObject(bullet);
		this->addChild(bullet);
		break;

	case K_BULLET_ENEMI:
		m_arrEnemiBullets->addObject(bullet);
		this->addChild(bullet);
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
				m_arrPlayerBullets->removeObject(bullet);
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
				m_arrEnemiBullets->removeObject(bullet);
			}
		}
	}

	//Enemi
	CCARRAY_FOREACH(m_arrEnemi, it)
	{
		Enemi* enemi = dynamic_cast<Enemi*>(it);
	
		if (NULL != enemi)
		{
			//out of screen
			if (enemi->getPositionY() < - enemi->boundingBox().size.height)
			{
				this->removeChild(enemi);
				m_arrEnemi->removeObject(enemi);
			}
		}
	}

	//update collision
	_updateCollision();
}

//check collision every frame
void ObjectLayer::_updateCollision()
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
					
					if (enemiRect.intersectsRect(bulletRect))
					{
						this->removeChild(bullet);
						m_arrPlayerBullets->removeObject(bullet);
						this->removeChild(enemi);
						m_arrEnemi->removeObject(enemi);

						//sound
						AudioManager::sharedAudioManager()->PlayEffect("explosion.wav");
						m_EffectLayer->AddExploisionEff(enemi->getEnemiType(), enemi->getPosition());
						//enemi->HitBullet(TIME_BEFORE_REMOVE_ENEMI);

						//score
						//
						m_score += enemi->getEnemiType();
						CCString* sscore = CCString::createWithFormat("%d", m_score);
						m_pLabelScore->setString(sscore->getCString());
					}
				}
			}
		}
	}

}



