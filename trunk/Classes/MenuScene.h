#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MenuScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(MenuScene);

    void onHttpRequestCompleted(CCHttpClient *sender, CCHttpResponse *response);
    void playCallback(CCObject* pSender);
	void settingCallback(CCObject* pSender);
	void exitCallback(CCObject* pSender);
};

#endif // __MENU_SCENE_H__
