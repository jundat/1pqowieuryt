#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

class Enemy : public GameObject
{
public:
	Enemy(float difficulty);
	~Enemy(){
		m_acExplosion->release();
		switch(m_type)
		{
		case 1:
			S_NUM_ENEMY_1--;
			break;

		case 2:
			S_NUM_ENEMY_2--;
			break;

		case 3:
			S_NUM_ENEMY_3_1--;
			break;
		}
	};
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
	static float S_VELOCITY1;
	static float S_VELOCITY2;
	static float S_VELOCITY3;
	static float S_GENERATE_TIME;

	static int S_NUM_ENEMY_1_1;
	static int S_NUM_ENEMY_1_2;
	static int S_NUM_ENEMY_1_3;
	static int S_NUM_ENEMY_1_4;
	static int S_NUM_ENEMY_1_5;
	static int S_NUM_ENEMY_1_6;

	static int S_NUM_ENEMY_2_1;
	static int S_NUM_ENEMY_2_2;
	static int S_NUM_ENEMY_2_3;
	static int S_NUM_ENEMY_2_4;
	static int S_NUM_ENEMY_2_5;

	static int S_NUM_ENEMY_3_1;
	static int S_NUM_ENEMY_3_2;
	static int S_NUM_ENEMY_3_3;
	static int S_NUM_ENEMY_3_4;

	CCRepeatForever* m_acFlying;
	CCSequence* m_acPreExplosion;
	CCSequence* m_acExplosion;

public:
	static void DifficultySplit(float difficulty);

public:
	virtual void update(float delta);
	void Fire();
	void ScheduleFire(float dt);
	void HitBullet(int damage);
};

#endif //__ENEMY_H__