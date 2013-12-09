#include "Bullet.h"
#include "ObjectLayer.h"
#include "Enemi.h"
#include "Global.h"

USING_NS_CC;


Enemi::Enemi(int type) : GameObject()
{
	this->setEnemiType(type);
}

bool Enemi::init()
{
	if (!GameObject::init())
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////
	
	switch(m_type)
	{
	case ENEMI_TYPE_1:
		m_sprite = CCSprite::create("enemi_1.png");
		this->setVx(ENEMI_1_VX);
		this->setVy(ENEMI_1_VY);
		this->setHp(ENEMI_1_HP);
		break;

	case ENEMI_TYPE_2:
		m_sprite = CCSprite::create("enemi_2.png");
		this->setVx(ENEMI_2_VX);
		this->setVy(ENEMI_2_VY);
		this->setHp(ENEMI_2_HP);
		break;

	case ENEMI_TYPE_3:
		m_sprite = CCSprite::create("enemi_3.png");
		this->setVx(ENEMI_3_VX);
		this->setVy(ENEMI_3_VY);
		this->setHp(ENEMI_3_HP);
		break;

	default:
		m_sprite = CCSprite::create("enemi_1.png");
		break;
	}

	m_sprite->setPosition(CCPointZero);
	this->addChild(m_sprite);

	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();
	return true;
}

void Enemi::Fire()
{
	Bullet* bullet = Bullet::create(K_BULLET_ENEMI, this->getPosition());
	ObjectLayer* parent = (ObjectLayer*)this->getParent();
	parent->AddBullet(bullet);
}

void Enemi::update( float delta )
{
	this->setPositionX(m_vx * delta * 1000 + this->getPositionX());
	this->setPositionY(m_vy * delta * 1000 + this->getPositionY());
}

void Enemi::HitBullet(float timeBeforeRemove)
{
	m_hp--;

	if (m_hp <= 0)
	{
		m_vx = 0;
		m_vy = 0;

		CCAction* ac = CCSequence::create(
			CCDelayTime::create(timeBeforeRemove), 
			CCCallFunc::create(this, callfunc_selector(Enemi::RemoveSeft)),
			NULL);

		this->runAction(ac);
	}
}

void Enemi::RemoveSeft()
{
	this->removeFromParentAndCleanup(true);
}

