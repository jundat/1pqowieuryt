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
	ScoreScene();
	virtual ~ScoreScene();

	CREATE_FUNC(ScoreScene);

	virtual CCScene* scene() {
		CCScene *scene = CCScene::create();
		scene->addChild(this);
		return scene;
	}


	virtual void keyBackClicked();
	void menuCallback(CCObject* pSender);
	void onGetLeaderboardCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);

	// New Virtual Method
	void fbCallback(CCObject* pSender);

private:
	CCLabelTTF* m_lbTitle;
	CCLabelTTF* m_lbMsg;
};

#endif // __SCORE_SCENE_H__
