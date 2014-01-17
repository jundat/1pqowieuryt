#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

#define ENEMY_TYPE_1	1
#define ENEMY_TYPE_2	2
#define ENEMY_TYPE_3	3

#define ENEMY_SMALL_TYPE_1_1	1
#define ENEMY_SMALL_TYPE_1_2	2
#define ENEMY_SMALL_TYPE_1_3	3
#define ENEMY_SMALL_TYPE_1_4	4
#define ENEMY_SMALL_TYPE_1_5	5
#define ENEMY_SMALL_TYPE_1_6	6

#define ENEMY_SMALL_TYPE_2_1	7
#define ENEMY_SMALL_TYPE_2_2	8
#define ENEMY_SMALL_TYPE_2_3	9
#define ENEMY_SMALL_TYPE_2_4	10
#define ENEMY_SMALL_TYPE_2_5	11

#define ENEMY_SMALL_TYPE_3_1	12
#define ENEMY_SMALL_TYPE_3_2	13
#define ENEMY_SMALL_TYPE_3_3	14
#define ENEMY_SMALL_TYPE_3_4	15


class Enemy : public GameObject
{
public:
	Enemy(int _type, int _smallType, int _hp, float _vy);
	virtual ~Enemy();
	virtual bool init();

	static Enemy* create(int _type, int _smallType, int _hp, float _vy) {
		Enemy* en = new Enemy(_type, _smallType, _hp, _vy);
		if (en && en->init())
		{
			en->autorelease();
			return en;	
		}
		else
		{
			delete en;
			en = NULL;
			return NULL;
		}
	}
	
	CC_SYNTHESIZE(int, m_type, EnemyType); //1-2-3
	CC_SYNTHESIZE(int, m_smallType, EnemySmallType); //1-2-3
	CC_SYNTHESIZE(float, m_vx, Vx);
	CC_SYNTHESIZE(float, m_vy, Vy); //1
	CC_SYNTHESIZE(int, m_hp, Hp); //2
	CC_SYNTHESIZE(int, m_originHp, OriginHp); //2
	CC_SYNTHESIZE(int, m_damage, Damage); //3 ~ damage for bullet

	static int S_NUMBER_BIG;
	static int S_NUMBER_MED;

	CCRepeatForever* m_acFlying;
	CCSequence* m_acPreExplosion;
	CCSequence* m_acExplosion;

public:
	virtual void update(float delta);
	void Fire();
	void ScheduleFire(float dt);
	void HitBullet(int damage);
	void RemoveFromObjectLayer();
};

#endif //__ENEMY_H__