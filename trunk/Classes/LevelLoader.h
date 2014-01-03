#pragma once

#ifndef __LEVEL_LOADER_H__
#define __LEVEL_LOADER_H__

#include "Global.h"
#include "MyMacro.h"
#include "cocos2d.h"

USING_NS_CC;

class LevelData : public CCObject {
public:
	LevelData(int score, int hp1, int hp2, int hp3, float velocity, float gentime) {
			m_score = score;
			m_hp1 = hp1;
			m_hp2 = hp2;
			m_hp3 = hp3;
			m_velocity = velocity;
			m_genTime = gentime;
	}

	static LevelData* create(int score, int hp1, int hp2, int hp3, float velocity, float gentime) {
		LevelData* ld = new LevelData(score, hp1, hp2, hp3, velocity, gentime);
		ld->autorelease();
		return ld;
	}

	const char* ToString() {
		CCString* s = CCString::createWithFormat("(%d, %d, %d, %d, %f, %f)", m_score, m_hp1, m_hp2, m_hp3, m_velocity, m_genTime);
		return s->getCString();
	}

	int m_score;
	int m_hp1;
	int m_hp2;
	int m_hp3;
	float m_velocity;
	float m_genTime;
};

class LevelLoader
{
public:
	LevelLoader(void);
	~LevelLoader(void){
		RELEASE(m_dict);
	}

	static CCString* s_levelFile;
	static LevelLoader* shareLevelLoader();

	CCDictionary* m_dict;

	LevelData* GetValueLowerThan(int keyScore);

protected:
	static LevelLoader* s_instance;
};


#endif //__LEVEL_LOADER_H__