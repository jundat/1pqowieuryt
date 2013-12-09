#include "Ship.h"
#include "Bullet.h"
#include "ObjectLayer.h"
#include "Global.h"

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

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_sprite = CCSprite::create("ship.png");
	m_sprite->setPosition(CCPointZero);
	this->addChild(m_sprite);

	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();
	return true;
}

void Ship::Fire()
{
	Bullet* bullet = Bullet::create(K_BULLET_PLAYER, this->getPosition());
	ObjectLayer* parent = (ObjectLayer*)this->getParent();
	parent->AddBullet(bullet);
}

void Ship::update( float delta )
{
	this->setPosition(ccpAdd(getPosition(), ccpMult(m_refJoystick->GetVelocity(), 10.0f)));

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

