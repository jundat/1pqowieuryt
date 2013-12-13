#ifndef __SETTING_SCENE_H__
#define __SETTING_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
USING_NS_CC;

class SettingScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(SettingScene);

	CCMenuItemToggle* soundToggle;
    
    void menuCallback(CCObject* pSender);
	void soundCallback(CCObject* pSender);

	//////////////////////////////////////////////////////////////////////////
	CCControlSlider* sliderMusic;
	CCControlSlider* sliderFX;
	float curValue;

	void volumeMusicCallBack(CCObject* sender,CCControlEvent pEvent);
	void volumeFXCallBack(CCObject* sender,CCControlEvent pEvent);
	//////////////////////////////////////////////////////////////////////////
	
};

#endif // __SETTING_SCENE_H__
