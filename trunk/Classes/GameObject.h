#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "cocos2d.h"

USING_NS_CC;

class GameObject : public CCNode
{
public:
	GameObject();
	~GameObject();

	CC_SYNTHESIZE(CCSprite*, m_sprite, Sprite);

public:
	virtual CCRect boundingBox();

};

#endif //__GAME_OBJECT_H__