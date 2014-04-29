#ifndef __SHIP_H__
#define __SHIP_H__

#include "cocos2d.h"
#include "GameObject.h"
#include "EffectLayer.h"

USING_NS_CC;

class Ship : public GameObject
{
public:
	~Ship() {
		m_acFlying->release();
		m_acExplosion->release();
		//m_acArmor->release();
	}
	virtual bool init();
	CREATE_FUNC(Ship); //static function: new, init, autorelease, return reference

public:
	CC_SYNTHESIZE(int, m_hp, Hp);
	CC_SYNTHESIZE(int, m_damage, Damage);
	CC_SYNTHESIZE(int, m_bulletLevel, BulletLevel); //1-2-3
	CC_SYNTHESIZE(bool, m_isArmor, ArmorStatus);

	float m_timeOutBulletLevel;
	float m_timeToFire;

	CCRepeatForever* m_acFlying; //0-1
	CCSequence* m_acExplosion; //2-3-4
	//CCSequence* m_acArmor; //5-6

public:
	void EnableArmor();
	void UpgradeBullet();

private:
	void DisableArmor();

public:
	virtual void update(float delta);
	CCRect collisionBox();
	CCRect bigcollisionBox();
	void Fire();
	void HitBullet(int damage);
	void AfterDeadEffectCallback();
	void Dead();
	void Restart();
    
    float getTimeoutBulletPercent();
};

#endif //__SHIP_H__