#ifndef __NOT_LOGGED_IN_MENU_SCENE_H__
#define __NOT_LOGGED_IN_MENU_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"////
#include "MyMacro.h"
#include "YesNoDialogParent.h"


USING_NS_CC;
USING_NS_CC_EXT;


class NotLoggedInMenuScene :
    public cocos2d::CCLayerColor,
    public YesNoDialogParent
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(NotLoggedInMenuScene);
	

    CCSprite* m_bg;
    CCMenuItemImage *m_facebookItem;
    CCMenuItemImage *m_playItem;
    CCMenu* m_menu;

    void playStartAnimation(int lastLife);
    void gotoMainGame();
    void initCloud();

    virtual void keyBackClicked();
    void playCallback(CCObject* pSender);
    void facebookCallback(CCObject* pSender);
    void exitCallback(CCObject* pSender);
    virtual void onShowDialog();
    virtual void onCloseDialog();
    
    void scheduleTick(float dt);
    void jumpMenu();
};

#endif // __NOT_LOGGED_IN_MENU_SCENE_H__
