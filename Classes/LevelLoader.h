#pragma once

#ifndef __LEVEL_LOADER_H__
#define __LEVEL_LOADER_H__

#include "Global.h"
#include "MyMacro.h"
#include "cocos2d.h"

USING_NS_CC;

#define NUM_ENEMY_TYPE 15

class LevelData : public CCObject {
public:
	LevelData(){};
	LevelData(int score, int hp1, int hp2, int hp3, float velocity1, float velocity2, float velocity3, float smallDelay, float bigDelay, int arrEnemy[NUM_ENEMY_TYPE]) {
		m_score = score;
		m_hp1 = hp1;
		m_hp2 = hp2;
		m_hp3 = hp3;
		m_velocity1 = velocity1;
		m_velocity2 = velocity2;
		m_velocity3 = velocity3;
			
		m_smallDelay = smallDelay;
		m_bigDelay = bigDelay;
			
		for (int i = 0; i < NUM_ENEMY_TYPE; ++i)
		{
			m_arrEnemy[i] = arrEnemy[i];
		}
	}

	LevelData Clone() {
		LevelData newLd;

		newLd.m_score = this->m_score;
		newLd.m_hp1 = this->m_hp1;
		newLd.m_hp2 = this->m_hp2;
		newLd.m_hp3 = this->m_hp3;
		newLd.m_velocity1 = this->m_velocity1;
		newLd.m_velocity2 = this->m_velocity2;
		newLd.m_velocity3 = this->m_velocity3;

		newLd.m_smallDelay = this->m_smallDelay;
		newLd.m_bigDelay = this->m_bigDelay;

		for (int i = 0; i < NUM_ENEMY_TYPE; ++i)
		{
			newLd.m_arrEnemy[i] = this->m_arrEnemy[i];
		}

		return newLd;
	}

	static LevelData* create(int score, int hp1, int hp2, int hp3, float velocity1, float velocity2, float velocity3, float smallDelay, float bigDelay, int arrEnemy[NUM_ENEMY_TYPE]) {
		LevelData* ld = new LevelData(score, hp1, hp2, hp3, velocity1, velocity2, velocity3, smallDelay, bigDelay, arrEnemy);
		ld->autorelease();
		return ld;
	}

	const char* ToString() {
		CCString* s = CCString::createWithFormat("(%d, %d, %d, %d, %f, %f, %f, %f, %f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)", \
			m_score, m_hp1, m_hp2, m_hp3, m_velocity1, m_velocity2, m_velocity3, m_smallDelay, m_bigDelay, \
			m_arrEnemy[0], m_arrEnemy[1], m_arrEnemy[2], m_arrEnemy[3], m_arrEnemy[4], m_arrEnemy[5], m_arrEnemy[6], \
			m_arrEnemy[7], m_arrEnemy[8], m_arrEnemy[9], m_arrEnemy[10], m_arrEnemy[11], m_arrEnemy[12], m_arrEnemy[13], m_arrEnemy[14]);
		return s->getCString();
	}

	int m_score;
	int m_hp1;
	int m_hp2;
	int m_hp3;
	
	float m_velocity1;
	float m_velocity2;
	float m_velocity3;
	
	float m_smallDelay;
	float m_bigDelay;

	int m_arrEnemy[NUM_ENEMY_TYPE];
};

class LevelLoader
{
public:
	LevelLoader(void);
	~LevelLoader(void){
		MY_RELEASE(m_dict);
	}

	static CCString* s_levelFile;
	static LevelLoader* shareLevelLoader();

	CCDictionary* m_dict;

	LevelData* GetValueLowerThan(int keyScore);

protected:
	static LevelLoader* s_instance;
};


#endif //__LEVEL_LOADER_H__