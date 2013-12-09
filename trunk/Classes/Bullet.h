#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

const static int K_BULLET_PLAYER = 2;
const static int K_BULLET_ENEMI = 1;

class Bullet : public GameObject
{
public:
	Bullet(int type, CCPoint position);
	~Bullet();
	virtual bool init();
	static Bullet* create(int type, CCPoint position) {
		Bullet* b = new Bullet(type, position);
		b->init();
		b->autorelease();
		return b;
	}
	virtual void update(float delta);
	CC_SYNTHESIZE(int, m_type, BulletType);
	CC_SYNTHESIZE(float, m_vx, Vx);
	CC_SYNTHESIZE(float, m_vy, Vy);
};

#endif //__BULLET_H__