#ifndef __SCORE_SCENE_H__
#define __SCORE_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpClient.h"
#include "jansson/jansson.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ScoreScene : public cocos2d::CCLayer, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
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


	virtual void keyBackClicked();
	void menuCallback(CCObject* pSender);
	void onGetLeaderboardCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);
	void processData(std::string str);

	// New Virtual Method
	void fbCallback(CCObject* pSender);

	//new delegate

	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

	//end new delegate

private:
	int m_listSize;
	CCLabelBMFont* m_waiting;

	CCArray* m_arrName;
	CCArray* m_arrScore;

	CCSprite* m_sprCell;
	CCTableView* m_tableView;

	bool m_isLoader;
};

#endif // __SCORE_SCENE_H__
