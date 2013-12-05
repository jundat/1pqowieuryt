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
	CCRect rect = m_sprite->boundingBox();
	rect.origin = ccp(this->getPosition().x - rect.size.width/2, this->getPosition().y - rect.size.height/2) ;
	return rect;
}

