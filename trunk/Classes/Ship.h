#ifndef __SHIP_H__
#define __SHIP_H__

#include "cocos2d.h"
#include "GameObject.h"
#include "EffectLayer.h"

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
	CC_SYNTHESIZE(int, m_bulletLevel, BulletLevel); //0-1-2

	float m_timeoutArmor;
	float m_timeToFire;
	bool m_isArmor;
	CCSprite* m_sprArmor;
	EffectLayer* m_EffectLayer;

public:
	void EnableArmor();
	void UpgradeBullet();
	void DowngradeBullet();

private:
	void DisableArmor();

public:
	virtual void update(float delta);
	void Fire();
	void HitBullet(int damage);
	void Dead();
	void Restart();
};

#endif //__SHIP_H__