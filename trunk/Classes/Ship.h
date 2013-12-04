#ifndef __SHIP_H__
#define __SHIP_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

class Ship : public GameObject
{
public:
	Ship();
	~Ship();

public:
	void Fire();
	virtual void update(float delta);

};

#endif //__SHIP_H__