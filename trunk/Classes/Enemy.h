#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;

class Enemy : public GameObject
{
public:
	Enemy(float difficulty, int MOVE_TYPE = G_MOVE_STRAINGH);
	~Enemy(){};
	virtual bool init();
	static Enemy* create(float difficulty, int MOVE_TYPE) {
		Enemy* en = new Enemy(difficulty, MOVE_TYPE);
		en->init();
		en->autorelease();
		return en;
	}
	
	CC_SYNTHESIZE(int, m_type, EnemyType); //not use
	CC_SYNTHESIZE(float, m_difficulty, Difficulty); //độ khó
	CC_SYNTHESIZE(float, m_vx, Vx);
	CC_SYNTHESIZE(float, m_vy, Vy); //1
	CC_SYNTHESIZE(int, m_moveType, MoveType); //1
	CC_SYNTHESIZE(int, m_hp, Hp); //2
	CC_SYNTHESIZE(int, m_damage, Damage); //3 ~ damage for bullet

	EffectLayer* m_EffectLayer;	

public:
	static void DifficultySplit(float difficulty, float& vy, int& hp, int& dm);

public:
	void Fire();
	void ScheduleFire(float dt);
	void HitBullet(int damage);
};

#endif //__ENEMY_H__