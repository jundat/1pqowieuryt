#ifndef __SCORE_SCENE_H__
#define __SCORE_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpClient.h"
#include "jansson/jansson.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "EziSocialObject.h"
#include "EziSocialDelegate.h"
#include "EziFacebookFriend.h"
#endif

USING_NS_CC;
USING_NS_CC_EXT;

class ScoreScene : public cocos2d::CCLayer, 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	public EziFacebookDelegate,
#endif
	public cocos2d::extension::CCTableViewDataSource, 
	public cocos2d::extension::CCTableViewDelegate
{
public:
	ScoreScene();
	virtual ~ScoreScene();
	virtual bool init();
	CREATE_FUNC(ScoreScene);

	static CCScene* scene() {
		CCScene *scene = CCScene::create();
		scene->addChild(ScoreScene::create());
		return scene;
	}


	void fbCallback(CCObject* pSender);
	virtual void keyBackClicked();
	void menuCallback(CCObject* pSender);

	void makeSingleData();

	//new delegate

	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

	//end new delegate

private:
	int m_listSize;

	CCLabelTTF* m_lbWaiting;
	CCLabelTTF* m_lbName;
	CCLabelTTF* m_lbScore;
	CCSprite* m_sprCell;
	CCTableView* m_tableView;

	CCArray* m_arrName;
	CCArray* m_arrScore;
	CCArray* m_arrPhoto; //

public:
	// Facebook //=========================================
	CCSprite* m_userSprite;
	CCMenuItem* m_fbItem;
	CCArray* m_friendList;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	void callSubmitScore();
	void callGetHighScores();
	virtual void fbHighScoresCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* highScores);

	// Facebook Callback methods...
	virtual void fbSessionCallback(int responseCode, const char* responseMessage);
	virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
	virtual void fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser);

#endif

	// Facebook //=========================================
};

#endif // __SCORE_SCENE_H__
