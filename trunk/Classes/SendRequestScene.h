#ifndef __SEND_REQUEST_SCENE_H__
#define __SEND_REQUEST_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class SendRequestScene : public cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
	CREATE_FUNC(SendRequestScene);

	void menuCallback();
};

#endif // __SEND_REQUEST_SCENE_H__
