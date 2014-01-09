#ifndef __SCORE_SCENE_H__
#define __SCORE_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ScoreScene : public cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
	CREATE_FUNC(ScoreScene);
	
	CCLabelTTF* m_lbTitle;
	CCLabelTTF* m_lbMsg;

	virtual void keyBackClicked();
	void menuCallback(CCObject* pSender);
	void onHttpRequestCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);
};

#endif // __SCORE_SCENE_H__
