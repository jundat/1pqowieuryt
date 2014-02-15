#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "MyMacro.h"

USING_NS_CC;

class MenuScene : public cocos2d::CCLayer
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
	void playStartAnimation(int lastLife);
	void gotoMainGame();
	
	virtual void keyBackClicked();
    void playCallback(CCObject* pSender);
	void scoreCallback(CCObject* pSender);
	void soundCallback(CCObject* pSender);
	void giftCallback(CCObject* pSender);
	

	void onCompletedWaiting();
	void onShowDialog();
	void onCloseDialog();

	bool m_isShowDialog;

	//life animation
	CCArray* m_arrSprLife;


	//timer
	float m_waitTime;
	CCLabelTTF* m_lbTime;

	void initTimer();
	void ScheduleTick(float dt);
};

#endif // __MENU_SCENE_H__
