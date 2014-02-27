#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class IntroScene : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
	CREATE_FUNC(IntroScene);

	void menuCallback();
};

#endif // __INTRO_SCENE_H__
