#include "Ship.h"
#include "Bullet.h"
USING_NS_CC;


Ship::Ship() : GameObject()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_sprite = CCSprite::create("ship.png");
	m_sprite->setPosition(CCPointZero);
	this->addChild(m_sprite);

	//////////////////////////////////////////////////////////////////////////

	m_arrBullets = new CCArray();

	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();
}

Ship::~Ship()
{

}

void Ship::Fire()
{

}

void Ship::update( float delta )
{
	CCObject* it;

	CCARRAY_FOREACH(m_arrBullets, it)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it);
		if (NULL != bullet)
		{
			//auto update

		}
	}
}

