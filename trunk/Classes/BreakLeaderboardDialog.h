#ifndef __BREAK_LEADERBOARD_DIALOG_H__
#define __BREAK_LEADERBOARD_DIALOG_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameClientObjects.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "EziSocialObject.h"
#include "EziSocialDelegate.h"
#include "EziFacebookFriend.h"
#include "EziFBIncomingRequestManager.h"
#include "EziFBIncomingRequest.h"
#endif

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


class BreakLeaderboardDialog : public cocos2d::CCLayer
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	,public EziFacebookDelegate
#endif
{
public:
    virtual bool init();
	static BreakLeaderboardDialog* create(FacebookAccount* loser) {
		BreakLeaderboardDialog* dig = new BreakLeaderboardDialog();
		dig->m_loser = loser;
		dig->init();
		dig->autorelease();
		return dig;
	}

public:
	FacebookAccount* m_loser;

public:
	void menuCallBack(CCObject* pSender);
	void shareCallBack(CCObject* pSender);
	void postMessageToLoser( string loserName, int yourScore );
	void onBreakLeaderboardFinish();

	//facebook
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
	virtual void fbMessageCallback(int responseCode, const char* responseMessage);
	virtual void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
};

#endif // __BREAK_LEADERBOARD_DIALOG_H__
