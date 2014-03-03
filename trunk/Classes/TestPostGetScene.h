#ifndef __TEST_POSTGET_SCENE_H__
#define __TEST_POSTGET_SCENE_H__

#include "cocos2d.h"
#include "HttpClient.h"
#include "cocos-ext.h"
#include "GameClientDelegate.h"
#include "GameClientManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TestPostGetScene : public cocos2d::CCLayerColor, public GameClientDelegate
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
	CREATE_FUNC(TestPostGetScene);

	void testPost1(CCObject *sender);
	void testPost2(CCObject *sender);
	void testPost3(CCObject *sender);
	void testPost4(CCObject *sender);
	void testPost5(CCObject *sender);
	void testPost6(CCObject *sender);
	void testPost7(CCObject *sender);
	void testPost8(CCObject *sender);

	virtual void onSendPlayerFbProfileCompleted( bool isSuccess )
	{
		if (isSuccess)
		{
			CCLOG("TRUE");
		} 
		else
		{
			CCLOG("FALSE");
		}
	}

	virtual void onSendFriendListCompleted( bool isSuccess )
	{
		if (isSuccess)
		{
			CCLOG("TRUE");
		} 
		else
		{
			CCLOG("FALSE");
		}
	}

	virtual void onSendDeviceProfileCompleted( bool isSuccess )
	{
		if (isSuccess)
		{
			CCLOG("TRUE");
		} 
		else
		{
			CCLOG("FALSE");
		}
	}

	virtual void onSendScoreCompleted( bool isSuccess )
	{
		if (isSuccess)
		{
			CCLOG("TRUE");
		} 
		else
		{
			CCLOG("FALSE");
		}
	}

	virtual void onGetScoreCompleted( bool isSuccess, int score )
	{
		if (isSuccess)
		{
			CCLOG("TRUE: %d", score);
		} 
		else
		{
			CCLOG("FALSE");
		}
	}

	virtual void onGetFriendLeaderboardCompleted( bool isSuccess, CCArray* arrHighScore )
	{
		if (isSuccess)
		{
			CCLOG("TRUE");

			for (int i = 0; i < arrHighScore->count(); ++i)
			{
				FacebookAccount* fb = (FacebookAccount*) arrHighScore->objectAtIndex(i);
				CCLOG("%s", fb->toJson().c_str());
			}
		} 
		else
		{
			CCLOG("FALSE");
		}
	}

	virtual void onGetWorldLeaderboardCompleted( bool isSuccess, CCArray* arrHighScore )
	{
		if (isSuccess)
		{
			CCLOG("TRUE");

			for (int i = 0; i < arrHighScore->count(); ++i)
			{
				FacebookAccount* fb = (FacebookAccount*) arrHighScore->objectAtIndex(i);
				CCLOG("%s", fb->toJson().c_str());
			}
		} 
		else
		{
			CCLOG("FALSE");
		}
	}
};


#endif // __TEST_POSTGET_SCENE_H__
