#pragma once

#ifndef __LEVEL_LOADER_H__
#define __LEVEL_LOADER_H__

#include "Global.h"
#include "MyMacro.h"
#include "cocos2d.h"

USING_NS_CC;

class LevelData : public CCObject {
public:
	LevelData(int score, int hp, float velocity) {
			m_score = score;
			m_hp = hp;
			m_velocity = velocity;
	}
	static LevelData* create(int score, int hp, float velocity) {
		LevelData* ld = new LevelData(score, hp, velocity);
		ld->autorelease();
		return ld;
	}

	const char* ToString() {
		CCString* s = CCString::createWithFormat("(%d, %d, %f)", m_score, m_hp, m_velocity);
		return s->getCString();
	}

	int m_score;
	int m_hp;
	float m_velocity;
};

class LevelLoader
{
public:
	LevelLoader(void);
	~LevelLoader(void){
		RELEASE(m_dict);
	}

	static CCString* s_levelFile;
	static LevelLoader* shareConfigLoader();

	CCDictionary* m_dict;

	LevelData* GetValueLowerThan(int keyScore);

protected:
	static LevelLoader* s_instance;
};


#endif //__LEVEL_LOADER_H__