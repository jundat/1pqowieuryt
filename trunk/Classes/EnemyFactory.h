#ifndef __ENEMY_FACTORY_H__
#define __ENEMY_FACTORY_H__

#include "cocos2d.h"
#include "LevelLoader.h"
USING_NS_CC;

class EnemyFactory : public CCNode
{
public:
	EnemyFactory();
	CREATE_FUNC(EnemyFactory);
	virtual void update(float delta, int score);
	void Reset();

	float m_smallCounter;
	float m_bigCounter;

	LevelData m_currentLevel;
	LevelData m_savedLevel;
};

#endif //__ENEMY_FACTORY_H__