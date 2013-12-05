#include "Bullet.h"
#include "Global.h"
USING_NS_CC;


Bullet::Bullet(int type, CCPoint position) : GameObject()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_sprite = CCSprite::create("bullet.png");
	this->addChild(m_sprite);

	//////////////////////////////////////////////////////////////////////////
	this->m_type = type;

	switch (m_type)
	{
	case K_BULLET_PLAYER:
		m_vx = PLAYER_BULLET_1_VX;
		m_vy = PLAYER_BULLET_1_VY;
		break;

	case K_BULLET_ENEMI:
		m_vx = ENEMI_BULLET_1_VX;
		m_vy = ENEMI_BULLET_1_VY;
		break;
	}

	this->setPosition(position);

	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();
}

Bullet::~Bullet()
{

}

void Bullet::update( float delta )
{
	this->setPositionX(m_vx * delta * 1000 + this->getPositionX());
	this->setPositionY(m_vy * delta * 1000 + this->getPositionY());
}
