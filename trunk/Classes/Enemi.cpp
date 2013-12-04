#include "Bullet.h"
#include "ObjectLayer.h"
#include "Enemi.h"
#include "Global.h"

USING_NS_CC;


Enemi::Enemi(int type) : GameObject()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////
	
	this->setEnemiType(type);

	switch(m_type)
	{
	case ENEMI_TYPE_1:
		m_sprite = CCSprite::create("enemi_1.png");
		this->setVx(ENEMI_1_VX);
		this->setVy(ENEMI_1_VY);
		break;
		
	case ENEMI_TYPE_2:
		m_sprite = CCSprite::create("enemi_2.png");
		this->setVx(ENEMI_2_VX);
		this->setVy(ENEMI_2_VY);
		break;

	case ENEMI_TYPE_3:
		m_sprite = CCSprite::create("enemi_3.png");
		this->setVx(ENEMI_3_VX);
		this->setVy(ENEMI_3_VY);
		break;

	default:
		m_sprite = CCSprite::create("enemi_1.png");
		break;
	}

	m_sprite->setPosition(CCPointZero);
	this->addChild(m_sprite);

	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();
}

void Enemi::Fire()
{
	Bullet* bullet = new Bullet(K_BULLET_ENEMI, this->getPosition());
	ObjectLayer* parent = (ObjectLayer*)this->getParent();
	parent->AddBullet(bullet);
}

void Enemi::update( float delta )
{
	this->setPositionX(m_vx * delta * 1000 + this->getPositionX());
	this->setPositionY(m_vy * delta * 1000 + this->getPositionY());
}

