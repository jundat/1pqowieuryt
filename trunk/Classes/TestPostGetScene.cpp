#include "MenuScene.h"
#include "TestPostGetScene.h"
#include "Global.h"

USING_NS_CC;

CCScene* TestPostGetScene::scene()
{
    CCScene *scene = CCScene::create();
    TestPostGetScene *layer = TestPostGetScene::create();
    scene->addChild(layer);
    return scene;
}

bool TestPostGetScene::init()
{
    if ( !CCLayerColor::initWithColor(G_CCWHITE4) )
    {
        return false;
    }

	CCMenu *menuRequest = CCMenu::create();
	menuRequest->setPosition(CCPointZero);
	addChild(menuRequest);
	
	// testSendPlayerProfile
	CCLabelTTF *labelPost = CCLabelTTF::create("testSendPlayerProfile", "Arial", 48);
	labelPost->setFontFillColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost = CCMenuItemLabel::create(labelPost, this, menu_selector(TestPostGetScene::testSendPlayerProfile));
	itemPost->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost->setPosition(ccp(10, 1270));
	menuRequest->addChild(itemPost);



	GameClientManager::sharedGameClientManager()->setDelegate(this);
    return true;
}

void TestPostGetScene::testSendPlayerProfile(CCObject *sender)
{
	//GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("123456789", "Jundat Pham", "longpham.uit@gmail.com");
	GameClientManager::sharedGameClientManager()->sendScore("App12345", "Fb54321", 9999, 1234567);
}
