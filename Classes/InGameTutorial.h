#ifndef __INGAME_TUTORIAL_H__
#define __INGAME_TUTORIAL_H__

#include "cocos2d.h"
USING_NS_CC;

class InGameTutorial : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
	CREATE_FUNC(InGameTutorial);

private:
	CCMenuItemImage* m_itExitButton;

public:
	void menuCallBack(CCObject* pSender);
};

#endif // __INGAME_TUTORIAL_H__
