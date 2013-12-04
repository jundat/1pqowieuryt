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
	return m_sprite->boundingBox();
}
