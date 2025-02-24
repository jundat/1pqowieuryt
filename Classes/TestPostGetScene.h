#ifndef __TEST_POSTGET_SCENE_H__
#define __TEST_POSTGET_SCENE_H__

#include "cocos2d.h"
#include "HttpClient.h"
#include "cocos-ext.h"
#include "GameClientDelegate.h"
#include "GameClientManager.h"
#include "MenuScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TestPostGetScene : public cocos2d::CCLayerColor, public GameClientDelegate
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
	CREATE_FUNC(TestPostGetScene);

	virtual void keyBackClicked()
	{
		GameClientManager::sharedGameClientManager()->setDelegate(NULL);

		CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
		CCDirector::sharedDirector()->replaceScene(pScene);
	}

	void testPost1(CCObject *sender);
	void testPost2(CCObject *sender);
	void testPost3(CCObject *sender);
	void testPost4(CCObject *sender);
	void testPost5(CCObject *sender);
	void testPost6(CCObject *sender);
	void testPost7(CCObject *sender);
	void testPost8(CCObject *sender);
	void testPost9(CCObject *sender);
    

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
    
    virtual void onBuyItemCompleted(bool isSuccess, int newCoin, std::string itemType, int itemCount, std::string uniqueTag)
    {
        if (isSuccess) {
            CCLOG("ItemType= %s", itemType.c_str());
            CCLOG("Count= %d", itemCount);
            CCLOG("uniqueTag= %s", uniqueTag.c_str());
            
            CCMessageBox("Mua dok OK", "Kaka");
        } else {
            CCMessageBox("FAIL to Mua đồ", "Èo");
        }
    }
    
    virtual void onSendItemCompleted(bool isSuccess, string friendId, string itemId, int count)
    {
        if (isSuccess) {
            CCLOG("TRUE");
        } else {
            CCLOG("FALSE");
        }
        
        CCLOG("friendId: %s, itemId: %s, count: %d", friendId.c_str(), itemId.c_str(), count);
    }
    
    
    
    
};


#endif // __TEST_POSTGET_SCENE_H__
