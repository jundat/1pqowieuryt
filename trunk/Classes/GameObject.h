#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "cocos2d.h"

USING_NS_CC;

class GameObject : public CCNode
{
public:
	GameObject():CCNode(){}
	~GameObject();
	virtual bool init();
	CREATE_FUNC(GameObject);
	CC_SYNTHESIZE(CCSprite*, m_sprite, Sprite);

public:
	virtual CCRect boundingBox();
	virtual CCSize getContentSize();
};

#endif //__GAME_OBJECT_H__