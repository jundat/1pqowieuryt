#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

class MenuScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MenuScene);
    
    void playCallback(CCObject* pSender);
	void settingCallback(CCObject* pSender);
	void exitCallback(CCObject* pSender);
};

#endif // __MENU_SCENE_H__
