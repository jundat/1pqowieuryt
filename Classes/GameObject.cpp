#include "GameObject.h"
USING_NS_CC;



GameObject::~GameObject()
{
}

bool GameObject::init()
{
	if (! CCNode::init())
	{
		return false;
	}
	return true;
}

cocos2d::CCRect GameObject::boundingBox()
{
	CCRect rect = m_sprite->boundingBox();
	rect.origin = ccp(this->getPosition().x - rect.size.width/2, this->getPosition().y - rect.size.height/2) ;
	return rect;
}

cocos2d::CCSize GameObject::getContentSize()
{
	return boundingBox().size;
}

