#ifndef __SCORE_SCENE_H__
#define __SCORE_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CustomTableViewCell.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "EziSocialObject.h"
#include "EziSocialDelegate.h"
#include "EziFacebookFriend.h"
#include "EziFBIncomingRequestManager.h"
#include "EziFBIncomingRequest.h"
#endif

#include <algorithm>

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
	virtual bool init();
	CREATE_FUNC(ScoreScene);

	static CCScene* scene() {
		CCScene *scene = CCScene::create();
		scene->addChild(ScoreScene::create());
		return scene;
	}

	//Xephang
	CCMenuItemToggle* m_xephangToggle;
	CCMenuItemToggle* m_quatangToggle;
	void xephangCallback(CCObject* pSender);
	void quatangCallback(CCObject* pSender);
	


	//

	CCMenuItemImage *m_fbLogOutItem;
	void fbLogOutCallback(CCObject* pSender);

	void fbLogInCallback(CCObject* pSender);

	virtual void keyBackClicked();
	void menuCallback(CCObject* pSender);
	void addFriendCallback(CCObject* pSender);

	static int MyMoreScoreFunction(const CCObject* p1, const CCObject* p2)
	{
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			EziFacebookFriend* f1 = (EziFacebookFriend*)(p1);
			EziFacebookFriend* f2 = (EziFacebookFriend*)(p2);

			return (f1->getScore() > f2->getScore());
		#else
			return true;
		#endif
	}

	void MySortHighScore()
	{
		std::sort(m_arrHighScores->data->arr, 
			m_arrHighScores->data->arr + m_arrHighScores->data->num, MyMoreScoreFunction);
	}

	//new delegate

	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {};
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

	//my function
	CCTableViewCell* tableCellXepHangAtIndex(CCTableView *table, unsigned int idx);
	CCTableViewCell* tableCellQuatangAtIndex(CCTableView *table, unsigned int idx);


	void getBoomCallback(CCObject* pSender);
	void sendLifeCallback(CCObject* pSender);

	bool m_isXepHangView;
	void refreshView();
	void refreshUserDetail();

	void scheduleTimer(float dt);

	//end new delegate

private:
	bool m_isLoggedIn;
	int m_tableXepHangSize;
	int m_tableQuatangSize;

	CCLabelTTF* m_lbInvite;
	CCSprite* m_sprWaiting;
	CCLabelTTF* m_lbName;
	CCLabelTTF* m_lbScore;
	CCSprite* m_sprCell;


	CCTableView* m_tableXephang;
	CCTableView* m_tableQuatang;

	CCArray* m_arrHighScores;
	CCArray* m_arrRequests;

	CustomTableViewCell* m_friendCell;

public:
	// Facebook //=========================================
	CCSprite* m_userAvatar;

	CCSprite* m_sprLife;
	CCSprite* m_sprBoom;
	CCLabelTTF* m_lbLife;
	CCLabelTTF* m_lbBoom;

	CCMenuItem* m_fbLogInItem;
	CCArray* m_friendList;


	void callSubmitScore();
	void callGetHighScores();
	virtual void fbHighScoresCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* highScores);
	void postMessageToLoser(std::string loserName, std::string loserUserName, int yourScore);
	virtual void fbMessageCallback(int responseCode, const char* responseMessage);

	// Facebook Callback methods...
	virtual void fbSessionCallback(int responseCode, const char* responseMessage);
	virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	virtual void fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser);
#endif

	virtual void fbSendRequestCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friendsGotRequests);

	//check incomming request
	void fbIncomingRequestCallback(int responseCode, const char* responseMessage, int totalIncomingRequests);
	// Facebook //=========================================
};

#endif // __SCORE_SCENE_H__
