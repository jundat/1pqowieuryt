#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "MyMacro.h"

USING_NS_CC;


#define G_VERSION			58
#define G_MENU_BG_COLOR		ccc4(195, 200, 201, 255)
#define G_MENU_BG			"bg_menu.png"
#define G_MENU_BG_ANCHORPOINT	ccp(0.0f, 0.0f)
#define G_MENU_BG_POS			CCPointZero
#define G_MENU_BG_Z					0
#define G_MENU_DIAMON_TEXT_SIZE		48
#define G_MENU_DIAMON_TEXT_COLOR	ccBLACK
#define G_MENU_DIAMON_TEXT_ANCHOR	ccp(1.0f, 0.5f)
#define G_MENU_DIAMON_TEXT_POS		ccp(790, 1250)



class MenuScene : public cocos2d::CCLayerColor
{
public:
	~MenuScene() 
	{
		MY_RELEASE(m_arrSprLife);
	}

    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MenuScene);
	
	CCMenuItemImage *m_playItem;
	CCMenu* m_menu;

	void initLifeIcon();
	void refreshLifeIcon();
	void playStartAnimation(int lastLife);
	void gotoMainGame();
	
	virtual void keyBackClicked();
    void playCallback(CCObject* pSender);
	void scoreCallback(CCObject* pSender);
	void soundCallback(CCObject* pSender);
	
	void onEnterTransitionDidFinish()
	{
		CCSpriteFrameCache* sprcache = CCSpriteFrameCache::sharedSpriteFrameCache();
		sprcache->addSpriteFramesWithFile("MainGame.plist");
	}

	void onCompletedWaiting();
	void onShowDialog();
	void onCloseDialog();

	bool m_isShowDialog;

	//life animation
	CCArray* m_arrSprLife;


	//timer
	int m_waitTime;
	CCLabelTTF* m_lbTime;

	void initTimer();
	void ScheduleTick(float dt);
};

#endif // __MENU_SCENE_H__
