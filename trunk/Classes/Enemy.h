#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

#define ENEMY_TYPE_1_1		11
#define ENEMY_TYPE_1_2		12
#define ENEMY_TYPE_1_3		13
#define ENEMY_TYPE_1_4		14
#define ENEMY_TYPE_1_5		15
#define ENEMY_TYPE_1_6		16

#define ENEMY_TYPE_2_1		21
#define ENEMY_TYPE_2_2		22
#define ENEMY_TYPE_2_3		23
#define ENEMY_TYPE_2_4		24
#define ENEMY_TYPE_2_5		25

#define ENEMY_TYPE_3_1		31
#define ENEMY_TYPE_3_2		32
#define ENEMY_TYPE_3_3		33
#define ENEMY_TYPE_3_4		34


class Enemy : public GameObject
{
public:
	Enemy(float difficulty);
	~Enemy(){
		m_acExplosion->release();
		m_acFlying->release();
		m_acPreExplosion->release();

		switch(m_smallType)
		{
		case ENEMY_TYPE_1_1:
			S_COUNT_ENEMY_1_1--;
			break;
		case ENEMY_TYPE_1_2:
			S_COUNT_ENEMY_1_2--;
			break;
		case ENEMY_TYPE_1_3:
			S_COUNT_ENEMY_1_3--;
			break;
		case ENEMY_TYPE_1_4:
			S_COUNT_ENEMY_1_4--;
			break;
		case ENEMY_TYPE_1_5:
			S_COUNT_ENEMY_1_5--;
			break;
		case ENEMY_TYPE_1_6:
			S_COUNT_ENEMY_1_6--;
			break;

			//////////////////////////////////////////////////////////////////////////

		case ENEMY_TYPE_2_1:
			S_COUNT_ENEMY_2_1--;
			break;
		case ENEMY_TYPE_2_2:
			S_COUNT_ENEMY_2_2--;
			break;
		case ENEMY_TYPE_2_3:
			S_COUNT_ENEMY_2_3--;
			break;
		case ENEMY_TYPE_2_4:
			S_COUNT_ENEMY_2_4--;
			break;
		case ENEMY_TYPE_2_5:
			S_COUNT_ENEMY_2_5--;
			break;

			//////////////////////////////////////////////////////////////////////////

		case ENEMY_TYPE_3_1:
			S_COUNT_ENEMY_3_1--;
			break;
		case ENEMY_TYPE_3_2:
			S_COUNT_ENEMY_3_2--;
			break;
		case ENEMY_TYPE_3_3:
			S_COUNT_ENEMY_3_3--;
			break;
		case ENEMY_TYPE_3_4:
			S_COUNT_ENEMY_3_4--;
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
	CC_SYNTHESIZE(int, m_smallType, EnemySmallType); //1-2-3
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

	static int S_COUNT_ENEMY_1_1;
	static int S_COUNT_ENEMY_1_2;
	static int S_COUNT_ENEMY_1_3;
	static int S_COUNT_ENEMY_1_4;
	static int S_COUNT_ENEMY_1_5;
	static int S_COUNT_ENEMY_1_6;

	static int S_COUNT_ENEMY_2_1;
	static int S_COUNT_ENEMY_2_2;
	static int S_COUNT_ENEMY_2_3;
	static int S_COUNT_ENEMY_2_4;
	static int S_COUNT_ENEMY_2_5;

	static int S_COUNT_ENEMY_3_1;
	static int S_COUNT_ENEMY_3_2;
	static int S_COUNT_ENEMY_3_3;
	static int S_COUNT_ENEMY_3_4;

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