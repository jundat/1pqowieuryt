#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MyMacro.h"
#include "NDKHelper/NDKHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "EziSocialObject.h"
#include "EziSocialDelegate.h"
#include "EziFacebookFriend.h"
#include "EziFBIncomingRequestManager.h"
#include "EziFBIncomingRequest.h"
#endif


USING_NS_CC;
USING_NS_CC_EXT;


#define G_VERSION			70
#define G_MENU_BG_COLOR		ccc4(195, 200, 201, 255)
#define G_MENU_BG			"bg_menu.png"
#define G_MENU_BG_ANCHORPOINT	ccp(0.0f, 0.0f)
#define G_MENU_BG_POS			CCPointZero
#define G_MENU_BG_Z					0
#define G_MENU_DIAMON_TEXT_SIZE		48
#define G_MENU_DIAMON_TEXT_COLOR	ccBLACK
#define G_MENU_DIAMON_TEXT_ANCHOR	ccp(1.0f, 0.5f)
#define G_MENU_DIAMON_TEXT_POS		ccp(790, 1250)

#define G_MENU_VERSION_TEXT_SIZE	32
#define G_MENU_VERSION_TEXT_COLOR	ccc3(56, 56, 56)
#define G_MENU_VERSION_TEXT_POS		ccp(20, 1280-20)

#define G_MENU_NEW_BUTTON_SPR_NORMAL	"new_button.png"
#define G_MENU_NEW_BUTTON_SPR_PRESS		"new_button_press.png"
#define G_MENU_NEW_BUTTON_POS			ccp(400, 1280-813)


class MenuScene : public cocos2d::CCLayerColor
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	,public EziFacebookDelegate
#endif
{
public:
	~MenuScene() 
	{
		MY_RELEASE(m_arrSprLife);
	}

    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MenuScene);
	


	CCMenuItemToggle *m_soundItem;
	CCMenuItemToggle *m_facebookItem;
	CCMenuItemToggle *m_settingItem;
	CCMenuItemImage *m_playItem;
	CCMenu* m_menu;

	int m_settingBarW;
	int m_settingBarH;
	CCSprite* m_sprSettingBar;

	void initLifeIcon();
	void refreshLifeIcon();
	void playStartAnimation(int lastLife);
	void gotoMainGame();
	
	virtual void keyBackClicked();
    void playCallback(CCObject* pSender);
	void scoreCallback(CCObject* pSender);
	void soundCallback(CCObject* pSender);
	void rateCallback(CCObject* pSender);
	void settingCallback(CCObject* pSender);
	void facebookCallback(CCObject* pSender);
	void exitCallback(CCObject* pSender);
	
	void onEnterTransitionDidFinish()
	{
		CCSpriteFrameCache* sprcache = CCSpriteFrameCache::sharedSpriteFrameCache();
		sprcache->addSpriteFramesWithFile("MainGame.plist");
	}

	void onRateCompleted( CCNode *sender, void *data );

	void onCompletedWaiting();
	void onShowDialog();
	void onCloseDialog();

	bool m_isShowDialog;
	bool m_isLoggedIn;

	//life animation
	CCArray* m_arrSprLife;


	//timer
	int m_waitTime;
	CCLabelTTF* m_lbTime;

	void initTimer();
	void ScheduleTick(float dt);

	//////////////////////////////////////////////////////////////////////////
	void GetRegistrationId();
	void onGetRegistrationIdCompleted( CCNode *sender, void *data );


	//////////////////////////////////////////////////////////////////////////
	//facebook
	void sendUserProfileToServer(string fbId, string fbName, string email);
	void getFacebookFriends();

	virtual void fbSessionCallback(int responseCode, const char* responseMessage);
	virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
	virtual void fbFriendsCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friends);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	virtual void fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser);
#endif
};

#endif // __MENU_SCENE_H__
