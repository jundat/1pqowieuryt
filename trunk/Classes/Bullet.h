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
	Bullet(int type);
	~Bullet();

	virtual void update(float delta);
	void setVelocity(float vx, float vy);

private:
	float vx;
	float vy;
};

#endif //__BULLET_H__