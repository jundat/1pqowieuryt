#ifndef __MAIN_GAME_SCENE_H__
#define __MAIN_GAME_SCENE_H__

#include "cocos2d.h"
#include "BackgroundLayer.h"
#include "ObjectLayer.h"
#include "EffectLayer.h"


USING_NS_CC;
using namespace std;


class MainGameScene : public cocos2d::CCLayerColor
{
public:
	~MainGameScene() {
		//CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	}

    virtual bool init();
    static cocos2d::CCScene* scene();
	CREATE_FUNC(MainGameScene);
	void pauseCallback(CCObject* pSender);
	void resumeCallback();
	virtual void keyBackClicked();

	//DEBUG
	void addScoreCallback(CCObject* pSender);
	void subScoreCallback(CCObject* pSender);

	void onEnterTransitionDidFinish();

	//cheat
	void cheatCallback(CCObject* pSender);

	bool m_isShowingPause;
	bool m_isShowingLose;

public:
	void showEndGame(int score, int killedEnemies);
	void reviveCallback();
	void restartCallback();
	void ShowTutorial();

private:
	BackgroundLayer* m_BackgroundLayer;
	ObjectLayer* m_ObjLayer;
    bool m_isLoggedIn;
};

#endif // __MAIN_GAME_SCENE_H__
