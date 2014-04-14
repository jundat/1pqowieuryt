#ifndef __EFFECT_LAYER_H__
#define __EFFECT_LAYER_H__

#include "cocos2d.h"
#include "CCParallaxNodeExtras.h"
USING_NS_CC;

class EffectLayer : public cocos2d::CCLayer
{
public:
	EffectLayer():CCLayer(){};
	~EffectLayer(){
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	};

	virtual bool init();
	CREATE_FUNC(EffectLayer);

public:
	float AddExploisionEff(int enemySize, CCPoint p);

private: //callback
	void RemoveEffCallback(CCNode* pSender);
};

#endif // __EFFECT_LAYER_H__
