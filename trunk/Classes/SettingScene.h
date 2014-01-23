#ifndef __SETTING_SCENE_H__
#define __SETTING_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "EziSocialObject.h"
#include "EziSocialDelegate.h"
#include "EziFacebookFriend.h"
#endif

USING_NS_CC_EXT;
USING_NS_CC;

class SettingScene : public cocos2d::CCLayer
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	,public EziFacebookDelegate
#endif
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(SettingScene);

	CCMenuItemToggle* soundToggle;

	virtual void keyBackClicked();
    void menuCallback(CCObject* pSender);
	void soundCallback(CCObject* pSender);
	void fbCallback(CCObject* pSender);

	//////////////////////////////////////////////////////////////////////////
	CCControlSlider* sliderMusic;
	CCControlSlider* sliderFX;
	float curValue;

	void volumeMusicCallBack(CCObject* sender,CCControlEvent pEvent);
	void volumeFXCallBack(CCObject* sender,CCControlEvent pEvent);
	//////////////////////////////////////////////////////////////////////////
	
	//facebook /////////////////////////////////

	CCMenuItem* m_fbItem;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	virtual void fbSessionCallback(int responseCode, const char* responseMessage);
#endif

	//end facebook

};

#endif // __SETTING_SCENE_H__
