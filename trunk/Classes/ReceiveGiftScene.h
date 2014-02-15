#ifndef __RECEIVE_GIFT_SCENE_H__
#define __RECEIVE_GIFT_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

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

class ReceiveGiftScene : public cocos2d::CCLayer, 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	public EziFacebookDelegate,
#endif
	public cocos2d::extension::CCTableViewDataSource, 
	public cocos2d::extension::CCTableViewDelegate
{
public:
	virtual bool init();
	CREATE_FUNC(ReceiveGiftScene);

	static CCScene* scene() {
		CCScene *scene = CCScene::create();
		scene->addChild(ReceiveGiftScene::create());
		return scene;
	}


	CCMenuItemImage *m_fbLogOutItem;
	void fbLogOutCallback(CCObject* pSender);

	void fbLogInCallback(CCObject* pSender);

	virtual void keyBackClicked();
	void menuCallback(CCObject* pSender);

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

	void mailInCallback(CCObject* pSender);
	void mailOutCallback(CCObject* pSender);

	//end new delegate

private:
	int m_listSize;

	CCLabelTTF* m_lbInvite;
	CCLabelTTF* m_lbWaiting;
	CCLabelTTF* m_lbName;
	CCLabelTTF* m_lbScore;
	CCSprite* m_sprCell;
	CCTableView* m_tableView;

	CCArray* m_arrHighScores;


public:
	// Facebook //=========================================
	CCSprite* m_userSprite;
	CCMenuItem* m_fbLogInItem;
	CCArray* m_friendList;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	void callSubmitScore();
	void callGetHighScores();
	virtual void fbHighScoresCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* highScores);

	// Facebook Callback methods...
	virtual void fbSessionCallback(int responseCode, const char* responseMessage);
	virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
	virtual void fbUserDetailCallback(int responseCode, const char* responseMessage, EziFacebookUser* fbUser);

	virtual void fbSendRequestCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friendsGotRequests);

	//check incomming request
	void fbIncomingRequestCallback(int responseCode, const char* responseMessage, int totalIncomingRequests);
#endif

	// Facebook //=========================================
};

#endif // __RECEIVE_GIFT_SCENE_H__
