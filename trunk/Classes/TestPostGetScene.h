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

	void testSendPlayerProfile(CCObject *sender);
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
};


#endif // __TEST_POSTGET_SCENE_H__
