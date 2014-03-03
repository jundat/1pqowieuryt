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
	CCLabelTTF *labelPost1 = CCLabelTTF::create("testPost1", "Arial", 48);
	labelPost1->setFontFillColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost1 = CCMenuItemLabel::create(labelPost1, this, menu_selector(TestPostGetScene::testPost1));
	itemPost1->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost1->setPosition(ccp(20, 1240));
	menuRequest->addChild(itemPost1);

	// testSendPlayerProfile
	CCLabelTTF *labelPost2 = CCLabelTTF::create("testPost2", "Arial", 48);
	labelPost2->setFontFillColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost2 = CCMenuItemLabel::create(labelPost2, this, menu_selector(TestPostGetScene::testPost2));
	itemPost2->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost2->setPosition(ccp(20, 1200));
	menuRequest->addChild(itemPost2);


	GameClientManager::sharedGameClientManager()->setDelegate(this);
    return true;
}

void TestPostGetScene::testPost1(CCObject *sender)
{
	//GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("123456789", "Jundat Pham", "longpham.uit@gmail.com");
	GameClientManager::sharedGameClientManager()->sendScore("App12345", "Fb54321", 9999, 1234567);
	//GameClientManager::sharedGameClientManager()->getScore("App12345", "Fb54321");
}

void TestPostGetScene::testPost2(CCObject *sender)
{
	//GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("123456789", "Jundat Pham", "longpham.uit@gmail.com");
	//GameClientManager::sharedGameClientManager()->sendScore("App12345", "Fb54321", 9999, 1234567);
	GameClientManager::sharedGameClientManager()->getScore("App12345", "Fb54321");
}
