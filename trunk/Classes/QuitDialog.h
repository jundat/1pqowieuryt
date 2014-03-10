#ifndef __QUIT_DIALOG_H__
#define __QUIT_DIALOG_H__

#include "cocos2d.h"
USING_NS_CC;

class QuitDialog : public cocos2d::CCLayer
{
public:
    virtual bool init();
    CREATE_FUNC(QuitDialog);

public:
	void yesCallback(CCObject* pSender);
	void noCallback( CCObject* pSender );
};

#endif // __QUIT_DIALOG_H__
