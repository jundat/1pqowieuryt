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
	CCLabelTTF *labelPost1 = CCLabelTTF::create("Send User Profile", "Arial", 48);
	labelPost1->setFontFillColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost1 = CCMenuItemLabel::create(labelPost1, this, menu_selector(TestPostGetScene::testPost1));
	itemPost1->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost1->setPosition(ccp(20, 1220));
	menuRequest->addChild(itemPost1);

	// testSendPlayerProfile
	CCLabelTTF *labelPost2 = CCLabelTTF::create("Send Friend List", "Arial", 48);
	labelPost2->setFontFillColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost2 = CCMenuItemLabel::create(labelPost2, this, menu_selector(TestPostGetScene::testPost2));
	itemPost2->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost2->setPosition(ccp(20, 1160));
	menuRequest->addChild(itemPost2);

	CCLabelTTF *labelPost3 = CCLabelTTF::create("Send Device Profile", "Arial", 48);
	labelPost3->setFontFillColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost3 = CCMenuItemLabel::create(labelPost3, this, menu_selector(TestPostGetScene::testPost3));
	itemPost3->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost3->setPosition(ccp(20, 1100));
	menuRequest->addChild(itemPost3);

	CCLabelTTF *labelPost4 = CCLabelTTF::create("Send Score", "Arial", 48);
	labelPost4->setFontFillColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost4 = CCMenuItemLabel::create(labelPost4, this, menu_selector(TestPostGetScene::testPost4));
	itemPost4->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost4->setPosition(ccp(20, 1040));
	menuRequest->addChild(itemPost4);

	CCLabelTTF *labelPost5 = CCLabelTTF::create("Get Score", "Arial", 48);
	labelPost5->setFontFillColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost5 = CCMenuItemLabel::create(labelPost5, this, menu_selector(TestPostGetScene::testPost5));
	itemPost5->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost5->setPosition(ccp(20, 980));
	menuRequest->addChild(itemPost5);

	CCLabelTTF *labelPost6 = CCLabelTTF::create("Get Player Facebook Profile", "Arial", 48);
	labelPost6->setFontFillColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost6 = CCMenuItemLabel::create(labelPost6, this, menu_selector(TestPostGetScene::testPost6));
	itemPost6->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost6->setPosition(ccp(20, 920));
	menuRequest->addChild(itemPost6);
// 
	CCLabelTTF *labelPost7 = CCLabelTTF::create("Get Device Profile", "Arial", 48);
	labelPost7->setFontFillColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost7 = CCMenuItemLabel::create(labelPost7, this, menu_selector(TestPostGetScene::testPost7));
	itemPost7->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost7->setPosition(ccp(20, 860));
	menuRequest->addChild(itemPost7);


	CCLabelTTF *labelPost8 = CCLabelTTF::create("Get Friend List", "Arial", 48);
	labelPost8->setFontFillColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost8 = CCMenuItemLabel::create(labelPost8, this, menu_selector(TestPostGetScene::testPost8));
	itemPost8->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost8->setPosition(ccp(20, 800));
	menuRequest->addChild(itemPost8);

	this->setKeypadEnabled(true);


	GameClientManager::sharedGameClientManager()->setDelegate(this);
    return true;
}

//user profile
void TestPostGetScene::testPost1(CCObject *sender)
{
	GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("Fb2", "DinhDung", "DinhDung@gmail.com");
	GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("Fb3", "BuiHieu", "BuiHieu@gmail.com");
	GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("Fb4", "DangTran", "DangTran@gmail.com");
	GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("Fb5", "TrinhLinh", "TrinhLinh@gmail.com");
	GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("Fb6", "NguyenChau", "NguyenChau@gmail.com");
}

//friendlist
void TestPostGetScene::testPost2(CCObject *sender)
{
	CCArray* arrFr = new CCArray();
	arrFr->retain();

	arrFr->addObject(new FacebookAccount("Fb2", "DinhDung", std::string("DinhDung@gmail.com"), 1));
	arrFr->addObject(new FacebookAccount("Fb3", "BuiHieu", std::string("BuiHieu@gmail.com"), 2));
	arrFr->addObject(new FacebookAccount("Fb4", "DangTran", std::string("DangTran@gmail.com"), 3));
	arrFr->addObject(new FacebookAccount("Fb5", "TrinhLinh", std::string("TrinhLinh@gmail.com"), 4));
	arrFr->addObject(new FacebookAccount("Fb6", "NguyenChau", std::string("NguyenChau@gmail.com"), 5));

	GameClientManager::sharedGameClientManager()->sendFriendList("Fb1", arrFr);
}

//sendDeviceProfile
void TestPostGetScene::testPost3(CCObject *sender)
{
	GameClientManager::sharedGameClientManager()->sendDeviceProfile(
		"Fb1", 
		"DeviceId1", 
		"DeviceToken1", 
		"DeviceConfig1", 
		"PhoneNumber1");
}

//sendScore
void TestPostGetScene::testPost4(CCObject *sender)
{
	GameClientManager::sharedGameClientManager()->sendScore("App1", "Fb1", 1111);
	GameClientManager::sharedGameClientManager()->sendScore("App1", "Fb2", 2222);
	GameClientManager::sharedGameClientManager()->sendScore("App1", "Fb3", 3333);
	GameClientManager::sharedGameClientManager()->sendScore("App1", "Fb4", 4444);
	GameClientManager::sharedGameClientManager()->sendScore("App1", "Fb5", 5555);
	GameClientManager::sharedGameClientManager()->sendScore("App1", "Fb6", 6666);
}

//getScore
void TestPostGetScene::testPost5(CCObject *sender)
{
	GameClientManager::sharedGameClientManager()->getScore("App1", "Fb1");
}

//getPlayerFbProfile
void TestPostGetScene::testPost6(CCObject *sender)
{
	GameClientManager::sharedGameClientManager()->getPlayerFbProfile("Fb1");
}

//getDeviceProfile
void TestPostGetScene::testPost7(CCObject *sender)
{
	GameClientManager::sharedGameClientManager()->getDeviceProfile("Fb1", "DeviceId1");
}

//getFriendList
void TestPostGetScene::testPost8(CCObject *sender)
{
	GameClientManager::sharedGameClientManager()->getFriendList("App1", "Fb1");
}