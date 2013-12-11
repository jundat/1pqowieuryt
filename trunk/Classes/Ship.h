#ifndef __SHIP_H__
#define __SHIP_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

class Ship : public GameObject
{
public:
	Ship():GameObject(){}
	~Ship();
	virtual bool init();
	CREATE_FUNC(Ship); //static function: new, init, autorelease, return reference

private:
	CC_SYNTHESIZE(int, m_hp, Hp);
	CC_SYNTHESIZE(int, m_damage, Damage);

public:
	virtual void update(float delta);
	void Fire();
	void ScheduleFire(float dt);
	void HitBullet(int damage);
	void Restart();
};

#endif //__SHIP_H__