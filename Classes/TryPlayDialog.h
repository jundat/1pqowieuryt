#ifndef __TRY_PLAY_DIALOG_H__
#define __TRY_PLAY_DIALOG_H__

#include "cocos2d.h"
USING_NS_CC;

class TryPlayDialog : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    CREATE_FUNC(TryPlayDialog);

public:
	void yesCallback(CCObject* pSender);
	void noCallback( CCObject* pSender );
};

#endif // __TRY_PLAY_DIALOG_H__
