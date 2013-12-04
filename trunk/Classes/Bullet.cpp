#include "Bullet.h"
#include "Global.h"
USING_NS_CC;


Bullet::Bullet(int type) : GameObject()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_sprite = CCSprite::create("bullet.png");
	m_sprite->setPosition(CCPointZero);
	this->addChild(m_sprite);

	//////////////////////////////////////////////////////////////////////////
	switch (type)
	{
	case K_BULLET_PLAYER:
		vx = PLAYER_BULLET_1_VX;
		vy = PLAYER_BULLET_1_VY;
		break;

	case K_BULLET_ENEMI:
		vx = ENEMI_BULLET_1_VX;
		vy = ENEMI_BULLET_1_VY;
		break;
	}

	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();
}

Bullet::~Bullet()
{

}

void Bullet::update( float delta )
{
	this->setPositionX(vx * delta + this->getPositionX());
	this->setPositionY(vy * delta + this->getPositionY());
}

void Bullet::setVelocity( float _vx, float _vy )
{
	vx = _vx;
	vy = _vy;
}
