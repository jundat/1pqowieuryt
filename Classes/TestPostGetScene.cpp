#include "MenuScene.h"
#include "TestPostGetScene.h"
#include "Global.h"
#include "TextLoader.h"
#include "DataManager.h"

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
	labelPost1->setColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost1 = CCMenuItemLabel::create(labelPost1, this, menu_selector(TestPostGetScene::testPost1));
	itemPost1->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost1->setPosition(ccp(20, 1220));
	menuRequest->addChild(itemPost1);

	// testSendPlayerProfile
	CCLabelTTF *labelPost2 = CCLabelTTF::create("Send Friend List", "Arial", 48);
	labelPost2->setColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost2 = CCMenuItemLabel::create(labelPost2, this, menu_selector(TestPostGetScene::testPost2));
	itemPost2->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost2->setPosition(ccp(20, 1160));
	menuRequest->addChild(itemPost2);

	CCLabelTTF *labelPost3 = CCLabelTTF::create("Send Device Profile", "Arial", 48);
	labelPost3->setColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost3 = CCMenuItemLabel::create(labelPost3, this, menu_selector(TestPostGetScene::testPost3));
	itemPost3->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost3->setPosition(ccp(20, 1100));
	menuRequest->addChild(itemPost3);

	CCLabelTTF *labelPost4 = CCLabelTTF::create("Send Score", "Arial", 48);
	labelPost4->setColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost4 = CCMenuItemLabel::create(labelPost4, this, menu_selector(TestPostGetScene::testPost4));
	itemPost4->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost4->setPosition(ccp(20, 1040));
	menuRequest->addChild(itemPost4);

	CCLabelTTF *labelPost5 = CCLabelTTF::create("Get Score", "Arial", 48);
	labelPost5->setColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost5 = CCMenuItemLabel::create(labelPost5, this, menu_selector(TestPostGetScene::testPost5));
	itemPost5->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost5->setPosition(ccp(20, 980));
	menuRequest->addChild(itemPost5);

	CCLabelTTF *labelPost6 = CCLabelTTF::create("Get Player Facebook Profile", "Arial", 48);
	labelPost6->setColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost6 = CCMenuItemLabel::create(labelPost6, this, menu_selector(TestPostGetScene::testPost6));
	itemPost6->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost6->setPosition(ccp(20, 920));
	menuRequest->addChild(itemPost6);
// 
	CCLabelTTF *labelPost7 = CCLabelTTF::create("Get Device Profile", "Arial", 48);
	labelPost7->setColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost7 = CCMenuItemLabel::create(labelPost7, this, menu_selector(TestPostGetScene::testPost7));
	itemPost7->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost7->setPosition(ccp(20, 860));
	menuRequest->addChild(itemPost7);


	CCLabelTTF *labelPost8 = CCLabelTTF::create("Get Friend List", "Arial", 48);
	labelPost8->setColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost8 = CCMenuItemLabel::create(labelPost8, this, menu_selector(TestPostGetScene::testPost8));
	itemPost8->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost8->setPosition(ccp(20, 800));
	menuRequest->addChild(itemPost8);

    CCLabelTTF *labelPost9 = CCLabelTTF::create("Buy Item", "Arial", 48);
	labelPost9->setColor(ccc3(0,0,0));
	CCMenuItemLabel *itemPost9 = CCMenuItemLabel::create(labelPost9, this, menu_selector(TestPostGetScene::testPost9));
	itemPost9->setAnchorPoint(ccp(0.0f, 0.5f));
	itemPost9->setPosition(ccp(20, 720));
	menuRequest->addChild(itemPost9);
    

    
	this->setKeypadEnabled(true);

	GameClientManager::sharedGameClientManager()->setDelegate(this);
    return true;
}

//user profile
void TestPostGetScene::testPost1(CCObject *sender)
{
	string fbid = DataManager::sharedDataManager()->GetFbID();
	GameClientManager::sharedGameClientManager()->sendPlayerFbProfile(fbid.c_str(), "PhamLong", "PhamLong@gmail.com");
// 	GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("Fb2", "DinhDung", "DinhDung@gmail.com", string(G_APP_ID));
// 	GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("Fb3", "BuiHieu", "BuiHieu@gmail.com", string(G_APP_ID));
// 	GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("Fb4", "DangTran", "DangTran@gmail.com", string(G_APP_ID));
// 	GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("Fb5", "TrinhLinh", "TrinhLinh@gmail.com", string(G_APP_ID));
// 	GameClientManager::sharedGameClientManager()->sendPlayerFbProfile("Fb6", "NguyenChau", "NguyenChau@gmail.com", string(G_APP_ID));
}

//friendlist
void TestPostGetScene::testPost2(CCObject *sender)
{
	CCArray* arrFr = new CCArray();
	arrFr->retain();
/*
	arrFr->addObject(new FacebookAccount("Fb2", "DinhDung", std::string("DinhDung@gmail.com"), 100));
	arrFr->addObject(new FacebookAccount("Fb3", "BuiHieu", std::string("BuiHieu@gmail.com"), 200));
	arrFr->addObject(new FacebookAccount("Fb4", "DangTran", std::string("DangTran@gmail.com"), 300));
	arrFr->addObject(new FacebookAccount("Fb5", "TrinhLinh", std::string("TrinhLinh@gmail.com"), 400));
	arrFr->addObject(new FacebookAccount("Fb6", "NguyenChau", std::string("NguyenChau@gmail.com"), 500));
*/
	CCLOG("SEND FRIEND FOR: Fb1");
	string fbid = DataManager::sharedDataManager()->GetFbID();
	GameClientManager::sharedGameClientManager()->sendFriendList(fbid.c_str(), arrFr);


// 	CCArray* arrNotFr = new CCArray();
// 	arrNotFr->retain();
// 
// 	arrNotFr->addObject(new FacebookAccount("Fb1", "PhamLong", std::string("PhamLong@gmail.com"), 500));
// 	arrNotFr->addObject(new FacebookAccount("Fb11", "ABC", std::string("ABC@gmail.com"), 110));
// 	arrNotFr->addObject(new FacebookAccount("Fb12", "DEF", std::string("DEF@gmail.com"), 220));
// 	arrNotFr->addObject(new FacebookAccount("Fb13", "GHI", std::string("GHI@gmail.com"), 330));
// 	arrNotFr->addObject(new FacebookAccount("Fb14", "JKL", std::string("JKL@gmail.com"), 440));
// 	arrNotFr->addObject(new FacebookAccount("Fb15", "MNO", std::string("MNO@gmail.com"), 550));
// 	
// 	CCLOG("SEND FRIEND FOR: Fb2");
// 	GameClientManager::sharedGameClientManager()->sendFriendList("Fb2", arrNotFr);
}

//sendDeviceProfile
void TestPostGetScene::testPost3(CCObject *sender)
{
	string fbid = DataManager::sharedDataManager()->GetFbID();
	GameClientManager::sharedGameClientManager()->sendDeviceProfile(
		fbid.c_str(), 
		"DeviceId1", 
		"DeviceToken1", 
		"DeviceConfig1", 
		"PhoneNumber1");
}

//sendScore
void TestPostGetScene::testPost4(CCObject *sender)
{
	string fbid = DataManager::sharedDataManager()->GetFbID();
	GameClientManager::sharedGameClientManager()->sendScore(fbid.c_str(), 5000);
// 	GameClientManager::sharedGameClientManager()->sendScore(G_APP_ID, "Fb2", 10000);
// 	GameClientManager::sharedGameClientManager()->sendScore(G_APP_ID, "Fb3", 15000);
// 	GameClientManager::sharedGameClientManager()->sendScore(G_APP_ID, "Fb4", 20000);
// 	GameClientManager::sharedGameClientManager()->sendScore(G_APP_ID, "Fb5", 25000);
// 	GameClientManager::sharedGameClientManager()->sendScore(G_APP_ID, "Fb6", 30000);
}

//getScore
void TestPostGetScene::testPost5(CCObject *sender)
{
	string fbid = DataManager::sharedDataManager()->GetFbID();
	GameClientManager::sharedGameClientManager()->getScore(fbid.c_str());
}

//getPlayerFbProfile
void TestPostGetScene::testPost6(CCObject *sender)
{
	string fbid = DataManager::sharedDataManager()->GetFbID();
	GameClientManager::sharedGameClientManager()->getPlayerFbProfile(fbid.c_str());
}

//getDeviceProfile
void TestPostGetScene::testPost7(CCObject *sender)
{
	string fbid = DataManager::sharedDataManager()->GetFbID();
	GameClientManager::sharedGameClientManager()->getDeviceProfile(fbid.c_str(), "DeviceId1");
}

//getFriendList
void TestPostGetScene::testPost8(CCObject *sender)
{
	string fbid = DataManager::sharedDataManager()->GetFbID();
	GameClientManager::sharedGameClientManager()->getFriendList(fbid.c_str());
}


//getFriendList
void TestPostGetScene::testPost9(CCObject *sender)
{
	string fbid = DataManager::sharedDataManager()->GetFbID();
    //if (fbid.compare("NULL") == 0) {
        fbid = "100006639370902";
        DataManager::sharedDataManager()->SetFbProfileID(fbid.c_str());
    //}
	
    //Buy Laze
    //GameClientManager::sharedGameClientManager()->buyItem(G_APP_ID, fbid.c_str(), string(G_ITEM_LAZE), 1, string("MuaLaze"));

    //Buy Revive
    //GameClientManager::sharedGameClientManager()->buyItem(fbid.c_str(), string(G_ITEM_REVIVE), 1, string("HoiSinh"));
    
    //Send Life
    GameClientManager::sharedGameClientManager()->sendItem("100006639370902", "100001986079146", "test_lazer", 1);
    
}

