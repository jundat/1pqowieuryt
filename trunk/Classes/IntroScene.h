#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;


#define G_LOGO								"logo.png"
#define G_LOGO_POSITION						ccp(400, 640)
#define G_INTRO_FADE_IN_TIME				1.5f
#define G_INTRO_DELAY_TIME					2.0f
#define G_INTRO_TRANSITION_MENU_TIME		1.5f


class IntroScene : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
	CREATE_FUNC(IntroScene);

	void menuCallback();
};

#endif // __INTRO_SCENE_H__
