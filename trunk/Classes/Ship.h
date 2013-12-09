#ifndef __SHIP_H__
#define __SHIP_H__

#include "cocos2d.h"
#include "GameObject.h"
#include "HSJoystick.h"

USING_NS_CC;

class Ship : public GameObject
{
public:
	Ship():GameObject(){}
	~Ship();
	virtual bool init();
	CREATE_FUNC(Ship); //static function: new, init, autorelease, return reference
	CC_SYNTHESIZE(HSJoystick*, m_refJoystick, Joystick);

public:
	virtual void update(float delta);
	void Fire();
};

#endif //__SHIP_H__