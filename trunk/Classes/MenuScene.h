#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "EziSocialObject.h"
#include "EziSocialDelegate.h"
#include "EziFacebookFriend.h"
#endif


USING_NS_CC;

class MenuScene : public cocos2d::CCLayer
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	,public EziFacebookDelegate
#endif
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MenuScene);

	CCMenu* m_menu;
	CCLabelBMFont* m_labelLife;
	
	virtual void keyBackClicked();
    void playCallback(CCObject* pSender);
	void scoreCallback(CCObject* pSender);
	void settingCallback(CCObject* pSender);
	void exitCallback(CCObject* pSender);
	void fbCallback(CCObject* pSender);
	

	void onCompletedWaiting();
	void onShowDialog();
	void onCloseDialog();

	bool m_isShowDialog;

	//facebook /////////////////////////////////

	CCMenuItem* m_fbItem;
	CCArray* m_friendList;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	virtual void fbSessionCallback(int responseCode, const char* responseMessage);
	virtual void fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser);
	virtual void fbFriendsCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friends);
#endif

	//end facebook
};

#endif // __MENU_SCENE_H__
