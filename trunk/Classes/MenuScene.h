#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MyMacro.h"
#include "NDKHelper/NDKHelper.h"
#include "GameClientDelegate.h"
#include "GameClientManager.h"
#include "GameClientObjects.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "EziSocialObject.h"
#include "EziSocialDelegate.h"
#include "EziFacebookFriend.h"
#include "EziFBIncomingRequestManager.h"
#include "EziFBIncomingRequest.h"
#endif


USING_NS_CC;
USING_NS_CC_EXT;


#define G_VERSION			77
#define G_MENU_BG_COLOR		ccc4(195, 200, 201, 255)
#define G_MENU_BG_VN			"vn_bg_menu.png"
#define G_MENU_BG_EN			"en_bg_menu.png"
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

#define G_MENU_NEW_BUTTON_SPR_NORMAL_VN		"vn_new_button.png"
#define G_MENU_NEW_BUTTON_SPR_PRESS_VN		"vn_new_button_press.png"

#define G_MENU_NEW_BUTTON_SPR_NORMAL_EN		"en_new_button.png"
#define G_MENU_NEW_BUTTON_SPR_PRESS_EN		"en_new_button_press.png"

#define G_MENU_NEW_BUTTON_POS			ccp(400, 1280-813)



class MenuScene : public cocos2d::CCLayerColor,
	public GameClientDelegate
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
	

	CCSprite* m_bg;
	CCMenuItemImage* m_itShowCharge;
	CCMenuItemToggle *m_soundItem;
	CCMenuItemToggle *m_facebookItem;
	CCMenuItemToggle *m_settingItem;
	CCMenuItemImage *m_languageItem;
	CCMenuItemImage *m_englishItem;
	CCMenuItemImage *m_vietnamItem;

	CCMenuItemImage *m_playItem;
	CCMenu* m_menu;

	CCSprite* m_sprSettingBar;
	CCSprite* m_sprLanguageBar;

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
	void facebookLogInOut();
	void exitCallback(CCObject* pSender);
	void showChargeCallback(CCObject* pSender);
	void languageCallback(CCObject* pSender);
	void englishCallback(CCObject* pSender);
	void vietnamCallback(CCObject* pSender);
	void refreshLanguageUI();

	void onEnterTransitionDidFinish()
	{
		CCSpriteFrameCache* sprcache = CCSpriteFrameCache::sharedSpriteFrameCache();
		sprcache->addSpriteFramesWithFile("MainGame.plist");

//		//DEBUG
// 		NDKHelper::AddSelector("MENU",
// 			"onPushNotification",
// 			callfuncND_selector(MenuScene::onPushNotification),
// 			this);
	}
	void onRateCompleted( CCNode *sender, void *data );
	void onPushNotification( CCNode *sender, void *data );
	void onShowChargeCompleted( CCNode *sender, void *data );

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
	void getUserProfile();
	virtual void onGetPlayerFbProfileCompleted(bool isSuccess, FacebookAccount* acc);
	void disableMoneytize();

	//////////////////////////////////////////////////////////////////////////
	//facebook
	
	void sendUserProfileToServer(string fbId, string fbName, string email);
	virtual void onSendPlayerFbProfileCompleted( bool isSuccess );
	void getFacebookFriends();

	virtual void fbSessionCallback(int responseCode, const char* responseMessage);
	virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
	virtual void fbFriendsCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friends);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	virtual void fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser);
#endif
};

#endif // __MENU_SCENE_H__
