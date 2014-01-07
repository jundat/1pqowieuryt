#include "Bullet.h"
#include "Global.h"
USING_NS_CC;


Bullet::Bullet(int type, float vy, float damage, CCPoint position) : GameObject()
{
	this->setBulletType(type);
	this->setVy(vy);
	this->setDamage(damage);
	this->setPosition(position);
}

Bullet::~Bullet()
{

}

bool Bullet::init()
{
	if (!GameObject::init())
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////

	this->setVx(0);

// 	if (m_type == G_BULLET_ENEMY_ID)
// 	{
// 		m_sprite = CCSprite::create("bullet.png");
// 		m_sprite->setAnchorPoint(ccp(0.5f, 0.5f));
// 		m_sprite->setScaleY(-1);
// 	}
// 	else
	{
		m_sprite = CCSprite::createWithSpriteFrameName("bullet.png");
	}

	this->addChild(m_sprite);

	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();
	return true;
}

void Bullet::update( float delta )
{
	this->setPositionX(m_vx * delta * 1000 + this->getPositionX());
	this->setPositionY(m_vy * delta * 1000 + this->getPositionY());
}
