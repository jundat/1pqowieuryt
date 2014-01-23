#include "MenuScene.h"
#include "ScoreScene.h"
#include "ParseClient.h"
#include "cocos-ext.h"
#include "HttpClient.h"
#include "GameClient.h"
#include "DataManager.h"
#include "CustomTableViewCell.h"
#include "MyMacro.h"


USING_NS_CC;
USING_NS_CC_EXT;


ScoreScene::ScoreScene()
{
	
}

ScoreScene::~ScoreScene()
{
	//CCHttpClient::getInstance()->destroyInstance();
	//MY_RELEASE(m_dictFriends);
	MY_RELEASE(m_arrName);
	MY_RELEASE(m_arrScore);
}

bool ScoreScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	this->setKeypadEnabled(true);

	//////////////////////////////////////////////////////////////////////////

	m_listSize = 0;

	m_friendList = NULL;
	m_arrName = new CCArray(m_listSize);
	m_arrScore = new CCArray(m_listSize);
	m_arrName->retain();
	m_arrScore->retain();

	CCSprite* bg = CCSprite::create("bg_stars.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg, 0);

	CCSprite* score_top = CCSprite::create("setting_top.png");
	score_top->setPosition(ccp(400, 1280-100));
	this->addChild(score_top, 0);

	//facebook avatar
	m_userSprite = CCSprite::create("fb-profile.png");
	m_userSprite->setPosition(ccp(114, 1280-280));
	this->addChild(m_userSprite);

	std::string name = DataManager::sharedDataManager()->GetName();
	m_lbName = CCLabelTTF::create(name.c_str(), "Marker Felt.ttf", 50);
	m_lbName->setFontFillColor(ccc3(0, 0, 0));
	m_lbName->setColor(ccc3(0, 0, 0));
	m_lbName->setPosition(ccp(400, 1280-248));
	this->addChild(m_lbName);

	CCString* score = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetHighScore());
	m_lbScore = CCLabelTTF::create(score->getCString(), "Marker Felt.ttf", 50);
	m_lbScore->setFontFillColor(ccc3(0, 0, 0));
	m_lbScore->setColor(ccc3(0, 0, 0));
	m_lbScore->setPosition(ccp(400, 1280-320));
	this->addChild(m_lbScore);

	//
	CCMenuItemImage *backItem = CCMenuItemImage::create(
		"back.png",
		"back1.png",
		this,
		menu_selector(ScoreScene::menuCallback));
	backItem->setPosition(ccp(400, 1280-1180));

	//
	bool _isLoggedIn = true;
	m_fbItem = CCMenuItemImage::create("connect_facebook.png", "connect_facebook.png", this,  menu_selector(ScoreScene::fbCallback));
	m_fbItem->setPosition(ccp(400, 1280-960));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EziSocialObject::sharedObject()->setFacebookDelegate(this);

	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		callSubmitScore();

		_isLoggedIn = true;
		m_fbItem->setVisible(false);

		//get avatar
		EziSocialObject::sharedObject()->getProfilePicForID(this, DataManager::sharedDataManager()->GetProfileID().c_str(), // Profile ID of current user
			128.0f, 128.0f, // Size of the image
			false // force download it from server
			);

		callGetHighScores();
	}
	else //logged out stated
	{
		_isLoggedIn = false;
		m_fbItem->setVisible(true);
	}
#endif

	CCMenu* pMenu = CCMenu::create(backItem, m_fbItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	//table view
	m_sprCell = CCSprite::create("tablecell.png");
	m_sprCell->retain();
	CCSize cellsize = m_sprCell->getContentSize();
	CCSize tableSize = CCSizeMake(cellsize.width, cellsize.height * 6.0f);

	//vertical
	m_tableView = CCTableView::create(this, tableSize);
	m_tableView->setDirection(kCCScrollViewDirectionVertical);
	m_tableView->setAnchorPoint(CCPointZero);
	m_tableView->setPosition(ccp(400 - tableSize.width/2, 1280-720 - tableSize.height/2));
	m_tableView->setDelegate(this);
	m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	this->addChild(m_tableView);


	if (_isLoggedIn == false)
	{
		makeSingleData();
		m_tableView->reloadData();
	}

	m_lbWaiting = NULL;
	m_lbWaiting = CCLabelBMFont::create("...WAITING...", "Mia_64.fnt");
	m_lbWaiting->setColor(ccc3(0, 0, 0));
	m_lbWaiting->setAlignment(kCCTextAlignmentCenter);
	m_lbWaiting->setPosition(ccp(400, 640));
	this->addChild(m_lbWaiting);

	if (_isLoggedIn == false)
	{
		m_lbWaiting->setVisible(false);
	}

	return true;
}

void ScoreScene::menuCallback(CCObject* pSender)
{
	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void ScoreScene::keyBackClicked()
{
	menuCallback(NULL);
}

void ScoreScene::makeSingleData()
{
	m_arrName->removeAllObjects();
	m_arrScore->removeAllObjects();

	m_listSize = 1;
	
	m_arrName->addObject(CCString::create(DataManager::sharedDataManager()->GetName()));
	m_arrScore->addObject(CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetHighScore()));
}


//new delegate

void ScoreScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	//CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize ScoreScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_sprCell->getContentSize();
}

CCTableViewCell* ScoreScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCString *order = CCString::createWithFormat("%d", idx + 1);
	CCString *score = (CCString*)m_arrScore->objectAtIndex(idx);
	CCString *name = (CCString*)m_arrName->objectAtIndex(idx);
	
	CCTableViewCell *cell = table->dequeueCell();
	if (!cell) {
		cell = new CustomTableViewCell();
		cell->autorelease();

		CCSprite *sprite = CCSprite::create("tablecell.png");
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(0, 0));
		cell->addChild(sprite);

		CCLabelBMFont *lbOrder = CCLabelBMFont::create(order->getCString(), "Mia_64.fnt");
		lbOrder->setScale(0.6f);
		lbOrder->setAlignment(kCCTextAlignmentLeft); //cocos2d::CCTextAlignment::
		lbOrder->setPosition(ccp(20, m_sprCell->getContentSize().height/2));
		lbOrder->setAnchorPoint(ccp(0.0f, 0.5f));
		lbOrder->setTag(100);
		cell->addChild(lbOrder);

		CCLabelTTF *lbName = CCLabelTTF::create(name->getCString(), "Marker Felt.ttf", 64);
		lbName->setFontFillColor(ccc3(0,0,0));
		lbName->setScale(0.6f);
		lbName->setHorizontalAlignment(kCCTextAlignmentRight);
		//lbName->setAlignment(kCCTextAlignmentRight); //cocos2d::CCTextAlignment::
		lbName->setPosition(ccp(m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));
		lbName->setAnchorPoint(ccp(1.0f, 0.5f));
		lbName->setTag(101);
		cell->addChild(lbName);

		CCLabelBMFont *lbScore = CCLabelBMFont::create(score->getCString(), "Mia_64.fnt");
		lbScore->setScale(0.6f);
		lbScore->setAlignment(kCCTextAlignmentCenter); //cocos2d::CCTextAlignment::
		lbScore->setPosition(ccp(m_sprCell->getContentSize().width/2, m_sprCell->getContentSize().height/2));
		lbScore->setAnchorPoint(ccp(0.5f, 0.5f));
		lbScore->setTag(102);
		cell->addChild(lbScore);
	}
	else
	{
		CCLabelBMFont *lbOrder = (CCLabelBMFont*)cell->getChildByTag(100);
		lbOrder->setString(order->getCString());
		lbOrder->setPosition(ccp(20, m_sprCell->getContentSize().height/2));

		CCLabelTTF *lbName = (CCLabelTTF*)cell->getChildByTag(101);
		lbName->setString(name->getCString());
		lbName->setPosition(ccp(m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));

		CCLabelBMFont *lbScore = (CCLabelBMFont*)cell->getChildByTag(102);
		lbScore->setString(score->getCString());
		lbScore->setPosition(ccp(m_sprCell->getContentSize().width/2, m_sprCell->getContentSize().height/2));
	}


	return cell;
}

unsigned int ScoreScene::numberOfCellsInTableView(CCTableView *table)
{
	return m_listSize;
}

//end new delegate


// Facebook //=========================================

void ScoreScene::fbCallback( CCObject* pSender )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_lbWaiting->setVisible(true);
	bool needPublicPermission = false;
	EziSocialObject::sharedObject()->performLoginUsingFacebook(needPublicPermission); // Pass true if you need publish permission also
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void ScoreScene::fbSessionCallback(int responseCode, const char *responseMessage)
{
	if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL)
	{
		m_fbItem->setVisible(false);
		EziSocialObject::sharedObject()->fetchFBUserDetails(true); //need email = true
	}
	else
	{
		m_fbItem->setVisible(true);
	}
}

void ScoreScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
	if ((strcmp(userPhotoPath, "") != 0))
	{
		CCSprite* userPhoto = CCSprite::create(userPhotoPath);
		userPhoto->setPosition(ccp(m_userSprite->getContentSize().width/2, m_userSprite->getContentSize().height/2));
		userPhoto->setScale(m_userSprite->getContentSize().width/userPhoto->getContentSize().width);
		m_userSprite->addChild(userPhoto);
	}
}

void ScoreScene::fbUserDetailCallback( int responseCode, const char* responseMessage, EziFacebookUser* fbUser )
{
	if (fbUser != NULL)
	{
		EziSocialObject::sharedObject()->setCurrentFacebookUser(fbUser);

		//save data
		EziFacebookUser* currentUser = EziSocialObject::sharedObject()->getCurrentFacebookUser();
		std::string fullname = fbUser->getFullName();
		std::string userName = currentUser->getUserName();
		std::string profileID = currentUser->getProfileID();

		DataManager::sharedDataManager()->SetName(fullname.c_str());
		DataManager::sharedDataManager()->SetProfileID(profileID.c_str());
		DataManager::sharedDataManager()->SetFbUserName(userName.c_str());

		m_lbName->setString(fullname.c_str());

		EziSocialObject::sharedObject()->getProfilePicForID(this, currentUser->getProfileID(), // Profile ID of current user
			128.0f, 128.0f, // Size of the image
			false // force download it from server
		);

		callGetHighScores();
	}
}

void ScoreScene::callSubmitScore()
{
	EziSocialObject::sharedObject()->postScore(DataManager::sharedDataManager()->GetHighScore());
}

void ScoreScene::callGetHighScores()
{
	EziSocialObject::sharedObject()->getHighScores();
}

void ScoreScene::fbHighScoresCallback( int responseCode, const char* responseMessage, cocos2d::CCArray* highScores )
{
	m_arrName->removeAllObjects();
	m_arrScore->removeAllObjects();

	m_listSize = highScores->count();

	CCObject* it;
	CCARRAY_FOREACH(highScores, it)
	{
		EziFacebookFriend* fbFriend = dynamic_cast<EziFacebookFriend*>(it);
		if (NULL != fbFriend)
		{
			CCLOG("%s => %d", fbFriend->getFBID(), (int)fbFriend->getScore());
			m_arrName->addObject(CCString::createWithFormat("%s", fbFriend->getName()));
			m_arrScore->addObject(CCString::createWithFormat("%d", (int)fbFriend->getScore()));
		}
	}

	m_lbWaiting->setVisible(false);
	m_tableView->reloadData();
}



#endif

// Facebook //=========================================
