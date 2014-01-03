#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

class Enemy : public GameObject
{
public:
	Enemy(float difficulty);
	~Enemy(){};
	virtual bool init();
	static Enemy* create(float difficulty) {
		Enemy* en = new Enemy(difficulty);
		en->init();
		en->autorelease();
		return en;
	}
	
	CC_SYNTHESIZE(int, m_type, EnemyType); //1-2-3
	CC_SYNTHESIZE(float, m_difficulty, Difficulty); //độ khó
	CC_SYNTHESIZE(float, m_vx, Vx);
	CC_SYNTHESIZE(float, m_vy, Vy); //1
	CC_SYNTHESIZE(int, m_hp, Hp); //2
	CC_SYNTHESIZE(int, m_originHp, OriginHp); //2
	CC_SYNTHESIZE(int, m_damage, Damage); //3 ~ damage for bullet

	static int S_HP1;
	static int S_HP2;
	static int S_HP3;
	static float S_VELOCITY;
	static float S_GENERATE_TIME;

	EffectLayer* m_EffectLayer;	

public:
	static void DifficultySplit(float difficulty);

public:
	virtual void update(float delta);
	void Fire();
	void ScheduleFire(float dt);
	void HitBullet(int damage);
};

#endif //__ENEMY_H__