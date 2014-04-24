#ifndef __LOG_OUT_DIALOG_H__
#define __LOG_OUT_DIALOG_H__

#include "cocos2d.h"

USING_NS_CC;

class LogOutDialog : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    CREATE_FUNC(LogOutDialog);

public:
	void yesCallback(CCObject* pSender);
	void noCallback( CCObject* pSender );
	void keyBackClicked();
};

#endif // __LOG_OUT_DIALOG_H__
