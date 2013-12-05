#ifndef __ENEMI_H__
#define __ENEMI_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

class Enemi : public GameObject
{
public:
	Enemi(int type);
	~Enemi(){};

	CC_SYNTHESIZE(int, m_type, EnemiType);
	CC_SYNTHESIZE(float, m_vx, Vx);
	CC_SYNTHESIZE(float, m_vy, Vy);
	CC_SYNTHESIZE(float, m_hp, Hp);

public:
	void Fire();
	void HitBullet(float delayTimeBeforeDie);
	void RemoveSeft();
	virtual void update(float delta);
};

#endif //__ENEMI_H__