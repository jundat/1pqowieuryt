#ifndef __WAIT_FOR_LIFE_DIALOG_H__
#define __WAIT_FOR_LIFE_DIALOG_H__

#include "cocos2d.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "EziSocialObject.h"
#include "EziSocialDelegate.h"
#include "EziFacebookFriend.h"
#endif

USING_NS_CC;

class WaitForLifeDialog : public cocos2d::CCLayer
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	,public EziFacebookDelegate
#endif
{
public:
	WaitForLifeDialog(float timeInSeconds):CCLayer(){
		this->m_waitTime = timeInSeconds;
	};
    virtual bool init();
	static WaitForLifeDialog* create(float timeInSeconds) {
		WaitForLifeDialog* dig = new WaitForLifeDialog(timeInSeconds);
		dig->init();
		dig->autorelease();
		return dig;
	}

private:
	float m_waitTime;
	CCLabelTTF* m_lbTime;

public:
	void ScheduleTick(float dt);
	void exitCallback(CCObject* pSender);
	void askFriendCallback(CCObject* pSender);

	
	//facebook /////////////////////////////////

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	void onEnterTransitionDidFinish()
	{
		CCLOG("[FeedAPIScene]: Enter Transition Finished.");
		EziSocialObject::sharedObject()->setFacebookDelegate(this);
	}

	void onExit()
	{
		CCLayer::onExit();
		EziSocialObject::sharedObject()->setFacebookDelegate(NULL);
	}

	// Facebook Delegate Methods
	void fbUserDetailCallback( int responseCode, const char* responseMessage, EziFacebookUser* fbUser );

#endif

	void fbUserPhotoCallback(const char *userPhotoPath, const char* fbID);
	void fbSessionCallback(int responseCode, const char *responseMessage);
	virtual void fbSendRequestCallback(int responseCode, const char* responseMessage, cocos2d::CCArray* friendsGotRequests);
	virtual void fbMessageCallback(int responseCode, const char* responseMessage);
	void inviteFriends();

	//end facebook
};

#endif // __WAIT_FOR_LIFE_DIALOG_H__
