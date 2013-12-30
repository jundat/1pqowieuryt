#ifndef __GAME_BACKGROUND_LAYER_H__
#define __GAME_BACKGROUND_LAYER_H__

#include "cocos2d.h"
#include "CCParallaxNodeExtras.h"
USING_NS_CC;

class BackgroundLayer : public cocos2d::CCLayer
{
public:
	BackgroundLayer():CCLayer(){};
	~BackgroundLayer(){};
    virtual bool init();
	CREATE_FUNC(BackgroundLayer);

private:
	CCParallaxNodeExtras *_backgroundNode;
// 	CCSprite *_spacedust1;
// 	CCSprite *_spacedust2;
	CCSprite *_planetsunrise;
	CCSprite *_galaxy;
	CCSprite *_spacialanomaly;
	CCSprite *_spacialanomaly2;
	
public:
	void update(float dt);
	void Pause();
	void Resume();
};

#endif // __GAME_BACKGROUND_LAYER_H__
