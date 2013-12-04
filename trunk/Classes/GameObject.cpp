#include "GameObject.h"
USING_NS_CC;


GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

cocos2d::CCRect GameObject::boundingBox()
{
	//CCRect rect = m_sprite->boundingBox();
	//rect.origin = this->getPosition();
	return m_sprite->boundingBox(); //rect;
}
