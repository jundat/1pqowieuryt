#include "EffectLayer.h"

USING_NS_CC;


bool EffectLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    return true;
}

float EffectLayer::AddExploisionEff(int enemySize, CCPoint p )
{
	int NUM_FRAME = 12;
	float TIME_ANIMATION = 0.1f;

	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("explosion_3.plist");

	CCString* strSpriteName = CCString::create("explosion_3 (1).png");
	CCSprite* m_pSprite1 = CCSprite::createWithSpriteFrameName(strSpriteName->getCString());

	m_pSprite1->setScale(enemySize/3.0f); //////////////////////////////////////////////////////////////////////////
	
	m_pSprite1->setPosition(p);
	this->addChild(m_pSprite1);

	CCArray* animFrames = CCArray::createWithCapacity(NUM_FRAME);
	for(int i = 1; i < NUM_FRAME; i++) 
	{
		strSpriteName = CCString::createWithFormat("explosion_3 (%d).png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName( strSpriteName->getCString() );
		animFrames->addObject(frame);
	}

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, TIME_ANIMATION);
	m_pSprite1->runAction(
		CCSequence::create(
			CCAnimate::create(animation), 
			CCCallFuncN::create(this, callfuncN_selector(EffectLayer::RemoveEffCallback)),
			NULL
	));

	return NUM_FRAME * TIME_ANIMATION;
}

void EffectLayer::RemoveEffCallback(CCNode* pSender)
{
	this->removeChild(pSender);
}
