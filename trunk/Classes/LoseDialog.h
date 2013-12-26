#ifndef __LOSE_DIALOG_H__
#define __LOSE_DIALOG_H__

#include "cocos2d.h"
#include "CCParallaxNodeExtras.h"
USING_NS_CC;

class LoseDialog : public cocos2d::CCLayer
{
public:
	LoseDialog(bool canBeRevived):CCLayer(){
		this->m_canBeRevived = canBeRevived;
	};
	~LoseDialog(){};
    virtual bool init();
	static LoseDialog* create(bool canBeRevived) {
		LoseDialog* dig = new LoseDialog(canBeRevived);
		dig->init();
		dig->autorelease();
		return dig;
	}

private:
	bool m_canBeRevived;

public:
	void CancelCallBack(CCObject* pSender);
	void OkCallBack(CCObject* pSender);
};

#endif // __LOSE_DIALOG_H__
