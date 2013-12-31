#pragma once

#ifndef __LEVEL_LOADER_H__
#define __LEVEL_LOADER_H__

#include "Global.h"
#include "MyMacro.h"
#include "cocos2d.h"

USING_NS_CC;

class LevelData : public CCObject {
public:
	LevelData(int score, int hp, float velocity, float gentime) {
			m_score = score;
			m_hp = hp;
			m_velocity = velocity;
			m_genTime = gentime;
	}

	static LevelData* create(int score, int hp, float velocity, float gentime) {
		LevelData* ld = new LevelData(score, hp, velocity, gentime);
		ld->autorelease();
		return ld;
	}

	const char* ToString() {
		CCString* s = CCString::createWithFormat("(%d, %d, %f, %f)", m_score, m_hp, m_velocity, m_genTime);
		return s->getCString();
	}

	int m_score;
	int m_hp;
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