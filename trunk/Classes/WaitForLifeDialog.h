#ifndef __WAIT_FOR_LIFE_DIALOG_H__
#define __WAIT_FOR_LIFE_DIALOG_H__

#include "cocos2d.h"
USING_NS_CC;

class WaitForLifeDialog : public cocos2d::CCLayer
{
public:
	WaitForLifeDialog(float timeInSeconds):CCLayer(){
		this->m_waitTime = timeInSeconds;
	};
    virtual bool init();
	static WaitForLifeDialog* create(float timeInSeconds) {
		WaitForLifeDialog* dig = new WaitForLifeDialog(timeInSeconds);
		dig->init();
		dig->autorelease();
		return dig;
	}

private:
	float m_waitTime;
	CCLabelTTF* m_lbTime;

public:
	void ScheduleTick(float dt);
	void exitCallback(CCObject* pSender);
	void askFriendCallback(CCObject* pSender);
};

#endif // __WAIT_FOR_LIFE_DIALOG_H__
