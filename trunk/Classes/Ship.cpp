#include "Ship.h"
#include "Bullet.h"
#include "ObjectLayer.h"
#include "Global.h"
#include "MyMacro.h"

USING_NS_CC;


Ship::~Ship()
{

}

bool Ship::init()
{
	if (!GameObject::init())
	{
		return false;
	}

	this->setHp(G_PLAYER_HP);
	this->setDamage(G_PLAYER_DAMAGE);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_sprite = CCSprite::create("ship.png");
	m_sprite->setPosition(CCPointZero);
	this->addChild(m_sprite);

	this->schedule(schedule_selector(Ship::ScheduleFire), G_PLAYER_TIME_TO_FIRE);
	//////////////////////////////////////////////////////////////////////////
	
	this->scheduleUpdate();
	return true;
}

void Ship::Fire()
{
	Bullet* bullet = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), this->getPosition());
	ObjectLayer* parent = (ObjectLayer*)this->getParent();
	parent->AddBullet(bullet);
}

void Ship::ScheduleFire( float dt )
{
	this->Fire();
}

void Ship::update( float delta )
{
	//limit on screen
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	float x = getPositionX();
	float y = getPositionY();
	float w_2 = boundingBox().size.width/2;
	float h_2 = boundingBox().size.height/2;

	LIMIT_VALUE(x, origin.x + w_2, origin.x + visibleSize.width - w_2);
	LIMIT_VALUE(y, origin.y + h_2, origin.y + visibleSize.height - h_2);
	this->setPosition(x, y);
}

void Ship::HitBullet( int damage )
{
	m_hp -= damage;
}

void Ship::Restart()
{
	this->setHp(G_PLAYER_HP);
}

