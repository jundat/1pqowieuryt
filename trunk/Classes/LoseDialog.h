#ifndef __LOSE_DIALOG_H__
#define __LOSE_DIALOG_H__

#include "cocos2d.h"
#include "CCParallaxNodeExtras.h"
USING_NS_CC;

class LoseDialog : public cocos2d::CCLayer
{
public:
	LoseDialog():CCLayer(){};
	~LoseDialog(){};
    virtual bool init();
	CREATE_FUNC(LoseDialog);

public:
	void CancelCallBack(CCObject* pSender);
	void OkCallBack(CCObject* pSender);
};

#endif // __LOSE_DIALOG_H__
