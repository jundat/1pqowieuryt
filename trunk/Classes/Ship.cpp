#include "Ship.h"
#include "Bullet.h"
#include "ObjectLayer.h"
USING_NS_CC;


Ship::Ship() : GameObject()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_sprite = CCSprite::create("ship.png");
	m_sprite->setPosition(CCPointZero);
	this->addChild(m_sprite);

	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();
}

Ship::~Ship()
{

}

void Ship::Fire()
{
	Bullet* bullet = new Bullet(K_BULLET_PLAYER, this->getPosition());
	ObjectLayer* parent = (ObjectLayer*)this->getParent();
	parent->AddBullet(bullet);
}

void Ship::update( float delta )
{
}

