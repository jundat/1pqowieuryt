#ifndef __LOSE_DIALOG_H__
#define __LOSE_DIALOG_H__

#include "cocos2d.h"
#include "CCParallaxNodeExtras.h"
USING_NS_CC;

class LoseDialog : public cocos2d::CCLayer
{
public:
	LoseDialog(int score, int killedEnemies):CCLayer(){
		this->m_score = score;
		this->m_killedEnemies = killedEnemies;
	};
	~LoseDialog(){};
    virtual bool init();
	static LoseDialog* create(int score, int killedEnemies) {
		LoseDialog* dig = new LoseDialog(score, killedEnemies);
		dig->init();
		dig->autorelease();
		return dig;
	}

private:
	int m_score;
	int m_killedEnemies;

public:
	void menuCallBack(CCObject* pSender);
};

#endif // __LOSE_DIALOG_H__
