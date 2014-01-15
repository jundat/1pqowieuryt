#pragma once

#ifndef __LEVEL_LOADER_H__
#define __LEVEL_LOADER_H__

#include "Global.h"
#include "MyMacro.h"
#include "cocos2d.h"

USING_NS_CC;

class LevelData : public CCObject {
public:
	LevelData(int score, int hp1, int hp2, int hp3, float velocity1, float velocity2, float velocity3, float gentime,
		int small1, int small2, int small3, int small4, int small5, int small6, 
		int med1, int med2, int med3, int med4, int med5, 
		int lar1, int lar2, int lar3, int lar4) {

			m_score = score;
			m_hp1 = hp1;
			m_hp2 = hp2;
			m_hp3 = hp3;
			m_velocity1 = velocity1;
			m_velocity2 = velocity2;
			m_velocity3 = velocity3;
			m_genTime = gentime;

			m_small1 = small1;
			m_small2 = small2;
			m_small3 = small3;
			m_small4 = small4;
			m_small5 = small5;
			m_small6 = small6;

			m_med1 = med1;
			m_med2 = med2;
			m_med3 = med3;
			m_med4 = med4;
			m_med5 = med5;

			m_lar1 = lar1;
			m_lar2 = lar2;
			m_lar3 = lar3;
			m_lar4 = lar4;
	}

	static LevelData* create(int score, int hp1, int hp2, int hp3, float velocity1, float velocity2, float velocity3, float gentime,
		int small1, int small2, int small3, int small4, int small5, int small6, 
		int med1, int med2, int med3, int med4, int med5, 
		int lar1, int lar2, int lar3, int lar4) {

		LevelData* ld = new LevelData(score, hp1, hp2, hp3, velocity1, velocity2, velocity3, gentime,
			small1, small2, small3, small4, small5, small6, 
			med1, med2, med3, med4, med5, 
			lar1, lar2, lar3, lar4);

		ld->autorelease();
		return ld;
	}

	const char* ToString() {
		CCString* s = CCString::createWithFormat("(%d, %d, %d, %d, %f, %f, %f, %f, \t%d, %d, %d, %d, %d, %d, \t%d, %d, %d, %d, %d, \t%d, %d, %d, %d)", \
			m_score, m_hp1, m_hp2, m_hp3, m_velocity1, m_velocity2, m_velocity3, m_genTime, \
			m_small1, m_small2, m_small3, m_small4, m_small5, m_small6, \
			m_med1, m_med2, m_med3, m_med4, m_med5, \
			m_lar1, m_lar2, m_lar3, m_lar4);
		return s->getCString();
	}

	int m_score;
	int m_hp1;
	int m_hp2;
	int m_hp3;
	float m_velocity1;
	float m_velocity2;
	float m_velocity3;
	float m_genTime;

	int m_small1;
	int m_small2;
	int m_small3;
	int m_small4;
	int m_small5;
	int m_small6;

	int m_med1;
	int m_med2;
	int m_med3;
	int m_med4;
	int m_med5;

	int m_lar1;
	int m_lar2;
	int m_lar3;
	int m_lar4;
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