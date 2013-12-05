#ifndef __EFFECT_LAYER_H__
#define __EFFECT_LAYER_H__

#include "cocos2d.h"
#include "CCParallaxNodeExtras.h"
USING_NS_CC;

class EffectLayer : public cocos2d::CCLayer
{
public:
	EffectLayer(){};

	~EffectLayer(){
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	};

	virtual bool init();
	void update(float dt);

public:
	void AddExploisionEff(int enemiSize, CCPoint p);

private: //callback
	void RemoveEffCallback(CCNode* pSender);
};

#endif // __EFFECT_LAYER_H__
