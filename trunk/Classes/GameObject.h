#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "cocos2d.h"

USING_NS_CC;

class GameObject : public CCNode
{
public:
	GameObject();
	~GameObject();

public:
	virtual CCRect boundingBox();

protected:
	/* data */
	CCSprite* m_sprite;

};

#endif //__GAME_OBJECT_H__