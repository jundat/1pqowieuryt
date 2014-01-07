#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"
#include "GameObject.h"
#include "Global.h"

USING_NS_CC;

class Bullet : public GameObject
{
public:
	Bullet(int type, float vy, float damage, CCPoint position, int level = 1);
	~Bullet();
	virtual bool init();
	static Bullet* create(int type, float vy, float damage, CCPoint position, int level = 1) {
		Bullet* b = new Bullet(type, vy, damage, position, level);
		b->init();
		b->autorelease();
		return b;
	}
	virtual void update(float delta);
	CC_SYNTHESIZE(int, m_type, BulletType);
	CC_SYNTHESIZE(int, m_level, Level);
	CC_SYNTHESIZE(float, m_vx, Vx);
	CC_SYNTHESIZE(float, m_vy, Vy);
	CC_SYNTHESIZE(int, m_damage, Damage); //
};

#endif //__BULLET_H__