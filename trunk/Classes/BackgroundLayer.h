#ifndef __GAME_BACKGROUND_LAYER_H__
#define __GAME_BACKGROUND_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

class BackgroundLayer : public cocos2d::CCLayer
{
public:
    virtual bool init();
	CREATE_FUNC(BackgroundLayer);
	void update(float dt);

	void Pause();
	void Resume();

private:
	CCSprite *bg1;
	CCSprite *bg2;
};

#endif // __GAME_BACKGROUND_LAYER_H__
