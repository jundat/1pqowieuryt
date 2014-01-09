#include "Bullet.h"
#include "Global.h"
USING_NS_CC;


Bullet::Bullet(int type, float vy, float damage, CCPoint position, int level) : GameObject()
{
	this->setBulletType(type);
	this->setVy(vy);
	this->setDamage(damage);
	this->setPosition(position);
	this->setLevel(level);
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

	this->setVx(0);

	if (m_level == 1)
	{
		m_sprite = CCSprite::createWithSpriteFrameName("bullet1.png");
	}
	else //2-3
	{
		m_sprite = CCSprite::createWithSpriteFrameName("bullet2.png");
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
