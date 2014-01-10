#ifndef __WAIT_FOR_LIFE_DIALOG_H__
#define __WAIT_FOR_LIFE_DIALOG_H__

#include "cocos2d.h"
USING_NS_CC;

class WaitForLifeDialog : public cocos2d::CCLayer
{
public:
	WaitForLifeDialog(int timeInSeconds):CCLayer(){
		this->m_waitTime = timeInSeconds;
	};
	~WaitForLifeDialog(){};
    virtual bool init();
	static WaitForLifeDialog* create(int timeInSeconds) {
		WaitForLifeDialog* dig = new WaitForLifeDialog(timeInSeconds);
		dig->init();
		dig->autorelease();
		return dig;
	}

private:
	int m_waitTime;

public:
	void exitCallback(CCObject* pSender);
	void askFriendCallback(CCObject* pSender);
};

#endif // __WAIT_FOR_LIFE_DIALOG_H__
