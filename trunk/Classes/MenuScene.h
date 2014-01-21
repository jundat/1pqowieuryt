#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"


#ifndef WIN32
#include "EziSocialObject.h"
#include "EziSocialDelegate.h"
#endif

USING_NS_CC;

class MenuScene : public cocos2d::CCLayer

#ifndef WIN32
	,public EziFacebookDelegate
#endif

{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MenuScene);

	CCSprite* m_userSprite;
	CCMenuItemToggle* m_fbItem;
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



	// Facebook //=========================================

	bool m_isLoggedIn;
	virtual void fbSessionCallback(int responseCode, const char* responseMessage);
	
#ifndef WIN32

	void buildScene();
	virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
	virtual void fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser);

	void onEnterTransitionDidFinish()
	{
		CCLog("[MenuScene]: Enter Transition Finished.");
		// Set the delegates to receive responses.
		EziSocialObject::sharedObject()->setFacebookDelegate(this);
	}

#endif

	// Facebook //=========================================
};

#endif // __MENU_SCENE_H__
