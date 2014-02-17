#include "MenuScene.h"
#include "ReceiveGiftScene.h"
#include "cocos-ext.h"
#include "DataManager.h"
#include "CustomTableViewCell.h"
#include "MyMacro.h"
#include "AudioManager.h"
#include "Global.h"


USING_NS_CC;
USING_NS_CC_EXT;


bool ReceiveGiftScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	this->setKeypadEnabled(true);

	//////////////////////////////////////////////////////////////////////////

	m_listSize = 0;

	m_friendList = NULL;
	m_arrHighScores = NULL;

	CCSprite* bg = CCSprite::create("bg_stars.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg);

	CCSprite* bg_table = CCSprite::create("bg_table.png");
	bg_table->setPosition(ccp(400, 1280-662));
	this->addChild(bg_table);

	CCSprite* score_top = CCSprite::create("setting_top.png");
	score_top->setPosition(ccp(400, 1280-100));
	this->addChild(score_top);

	//facebook avatar
	if(DataManager::sharedDataManager()->GetPhotoPath() != "NULL")
	{
		m_userSprite = CCSprite::create(DataManager::sharedDataManager()->GetPhotoPath().c_str());
	}
	else
	{
		m_userSprite = CCSprite::create("fb-profile.png");
	}

	m_userSprite->setPosition(ccp(88, 1280-276));
	this->addChild(m_userSprite);

	std::string name = DataManager::sharedDataManager()->GetName();
	m_lbName = CCLabelTTF::create(name.c_str(), "Roboto-Medium.ttf", 52);
	m_lbName->setFontFillColor(ccc3(0, 0, 0));
	m_lbName->setPosition(ccp(180, 1280-244)); //(ccp(400, 1280-250)); //248
	//m_lbName->setHorizontalAlignment(kCCTextAlignmentLeft);
	m_lbName->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbName);

	CCString* score = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetHighScore());
	m_lbScore = CCLabelTTF::create(score->getCString(), "Roboto-Medium.ttf", 48);
	m_lbScore->setFontFillColor(ccc3(0, 0, 0));
	m_lbScore->setPosition(ccp(178, 1280-316)); //ccp(400, 1280-320)); //320
	//m_lbScore->setHorizontalAlignment(kCCTextAlignmentLeft);
	m_lbScore->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbScore);

	//
	CCMenuItemImage *backItem = CCMenuItemImage::create(
		"back.png",
		"back1.png",
		this,
		menu_selector(ReceiveGiftScene::menuCallback));
	backItem->setPosition(ccp(178, 1280-1209));

	//
	m_fbLogOutItem = CCMenuItemImage::create(
		"disconnect_facebook.png",
		"disconnect_facebook.png",
		this,
		menu_selector(ReceiveGiftScene::fbLogOutCallback));
	m_fbLogOutItem->setPosition(ccp(639, 1280-1209));
	this->addChild(m_fbLogOutItem);

	//
	bool _isLoggedIn = true;
	m_fbLogInItem = CCMenuItemImage::create(
		"connect_facebook.png", 
		"connect_facebook.png", 
		this,  
		menu_selector(ReceiveGiftScene::fbLogInCallback));
	m_fbLogInItem->setPosition(ccp(400, 1280-838));

	m_lbInvite = CCLabelTTF::create("Connect Facebook\nfor more fun", "Roboto-Medium.ttf", 48);
	m_lbInvite->setFontFillColor(ccc3(0, 0, 0));
	m_lbInvite->setPosition(ccp(400, 1280-703)); //320
	this->addChild(m_lbInvite, 1); //samw menu

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EziSocialObject::sharedObject()->setFacebookDelegate(this);

	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		callSubmitScore();

		//check incoming request
		EziSocialObject::sharedObject()->checkIncomingRequest();



		_isLoggedIn = true;
		m_fbLogInItem->setVisible(false);
		m_lbInvite->setVisible(false);
		m_fbLogOutItem->setVisible(true);

		//get avatar
		EziSocialObject::sharedObject()->getProfilePicForID(this, DataManager::sharedDataManager()->GetProfileID().c_str(), // Profile ID of current user
			G_AVATAR_SIZE, G_AVATAR_SIZE, // Size of the image
			false // force download it from server
			);

		callGetHighScores();
	}
	else //logged out stated
	{
		_isLoggedIn = false;

		m_fbLogInItem->setVisible(true);
		m_lbInvite->setVisible(true);
		m_fbLogOutItem->setVisible(false);
	}
#endif

	CCMenu* pMenu = CCMenu::create(backItem, m_fbLogInItem, m_fbLogOutItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	//table view
	m_sprCell = CCSprite::create("tablecell.png");
	m_sprCell->retain();
	CCSize cellsize = m_sprCell->getContentSize();
	CCSize tableSize = CCSizeMake(768, 731); //CCSizeMake(cellsize.width, cellsize.height * 6.0f);

	//vertical
	m_tableView = CCTableView::create(this, tableSize);
	m_tableView->setDirection(kCCScrollViewDirectionVertical);
	m_tableView->setAnchorPoint(CCPointZero);
	m_tableView->setPosition(ccp(400 - tableSize.width/2, 1280-742 - tableSize.height/2));
	m_tableView->setDelegate(this);
	m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	this->addChild(m_tableView);

	m_lbWaiting = CCLabelTTF::create("...WAITING...", "Roboto-Medium.ttf", 64);
	m_lbWaiting->setColor(ccc3(0, 0, 0));
	m_lbWaiting->setHorizontalAlignment(kCCTextAlignmentCenter);
	m_lbWaiting->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	m_lbWaiting->setPosition(ccp(400, 640));

	this->addChild(m_lbWaiting);

	if (_isLoggedIn == false)
	{
		m_tableView->setVisible(false);
		m_lbWaiting->setVisible(false);
	}

	return true;
}

void ReceiveGiftScene::menuCallback(CCObject* pSender)
{
	PLAY_BUTTON_EFFECT;

	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void ReceiveGiftScene::keyBackClicked()
{
	menuCallback(NULL);
}

//new delegate

void ReceiveGiftScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	CCLOG("cell touched at index: %i", cell->getIdx());
	
	CustomTableViewCell* customCell = (CustomTableViewCell*) cell;
	
	int curLife = DataManager::sharedDataManager()->GetLastPlayerLife();
	int gift = DataManager::sharedDataManager()->GetGiftFromFriend(customCell->fbID.c_str());

	if (curLife < G_MAX_PLAYER_LIFE && gift > 0)
	{
		PLAY_GET_BOMB_EFFECT;

		curLife++;
		gift--;

		DataManager::sharedDataManager()->SetLastPlayerLife(curLife);
		DataManager::sharedDataManager()->DecreaseGiftFromFriend(customCell->fbID.c_str());

		//refresh
		m_tableView->reloadData();
	} 
	else
	{
		PLAY_OUT_PORP_EFFECT;
	}
}

CCSize ReceiveGiftScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_sprCell->getContentSize();
}

CCTableViewCell* ReceiveGiftScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	bool isMyScore = false;
	CCString *order = CCString::createWithFormat("%d", idx + 1);
	CCString *score = CCString::create("0");
	CCString *name = CCString::create(G_DEFAULT_NAME);
	CCString *photo = CCString::create("fb-profile.png");
	std::string friendId;
	CCString *gift = CCString::create("0");
	

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(m_arrHighScores == NULL)
	{
		CCMessageBox("m_arrHighScores == NULL", "Error");
	}
	else
	{
		EziFacebookFriend* fbFriend = dynamic_cast<EziFacebookFriend*>(m_arrHighScores->objectAtIndex(idx));
		score = CCString::createWithFormat("%d", (int)fbFriend->getScore());
		name  = CCString::createWithFormat("%s", fbFriend->getName());
		
		friendId = std::string(fbFriend->getFBID());
		gift = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetGiftFromFriend(friendId.c_str()));

		if (strlen(fbFriend->getPhotoPath()) > 1)
		{
			//CCLOG("Photopath != \"\"\n%s", fbFriend->getPhotoPath());
			photo  = CCString::createWithFormat("%s", fbFriend->getPhotoPath());
		}

		if(fbFriend->getFBID() == DataManager::sharedDataManager()->GetProfileID())
		{
			isMyScore = true;
		}
	}

#endif

	CCTableViewCell *cell = table->cellAtIndex(idx);// table->dequeueCell();
	if (!cell) {
		cell = new CustomTableViewCell();
		cell->autorelease();

		((CustomTableViewCell*)cell)->fbID = friendId;
		
		CCSprite *sprite;
		if (isMyScore)
		{
			sprite = CCSprite::create("tablecell_my.png");
		}
		else
		{
			sprite = CCSprite::create("tablecell.png");
		}
		
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(0, 0));
		sprite->setTag(1);
		cell->addChild(sprite);

		CCSprite *avatar = CCSprite::create(photo->getCString());
		avatar->setScale(0.75f);
		avatar->setPosition(ccp(0.75f * G_FRIEND_AVATAR_SIZE/2 + 2, m_sprCell->getContentSize().height/2));
		avatar->setTag(2);
		cell->addChild(avatar);

// 		CCLabelTTF *lbOrder = CCLabelTTF::create(order->getCString(), "Roboto-Medium.ttf", 42);
// 		lbOrder->setFontFillColor(ccc3(0, 0, 0));
// 		lbOrder->setHorizontalAlignment(kCCTextAlignmentLeft); //cocos2d::CCTextAlignment::
// 		lbOrder->setPosition(ccp(20, m_sprCell->getContentSize().height/2));
// 		lbOrder->setAnchorPoint(ccp(0.0f, 0.5f));
// 		lbOrder->setTag(3);
// 		cell->addChild(lbOrder);

		CCLabelTTF *lbName = CCLabelTTF::create(name->getCString(), "Roboto-Medium.ttf", 42);
		lbName->setFontFillColor(ccc3(0,0,0));
		lbName->setPosition(ccp(0.75f * G_FRIEND_AVATAR_SIZE + 20, m_sprCell->getContentSize().height * 3/4));
		//(ccp(m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));
		lbName->setAnchorPoint(ccp(0.0f, 0.5f));
		lbName->setTag(4);
		cell->addChild(lbName);

		CCLabelTTF *lbScore = CCLabelTTF::create(gift->getCString(), "Roboto-Medium.ttf", 42);
		lbScore->setFontFillColor(ccc3(0, 0, 0));
		lbScore->setPosition(ccp(0.75f * G_FRIEND_AVATAR_SIZE + 20, m_sprCell->getContentSize().height * 3/8)); //0.25 * m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));
		lbScore->setAnchorPoint(ccp(0.0f, 0.5f));
		lbScore->setTag(5);
		cell->addChild(lbScore);
	}
	else
	{
		CCSprite *sprite = (CCSprite*)cell->getChildByTag(1);
		if (isMyScore)
		{
			sprite = CCSprite::create("tablecell_my.png");
		}
		else
		{
			sprite = CCSprite::create("tablecell.png");
		}

		CCSprite *avatar = (CCSprite*)cell->getChildByTag(2);
		avatar = CCSprite::create(photo->getCString());

// 		CCLabelTTF *lbOrder = (CCLabelTTF*)cell->getChildByTag(3);
// 		lbOrder->setString(order->getCString());
// 		lbOrder->setPosition(ccp(20, m_sprCell->getContentSize().height/2));

		CCLabelTTF *lbName = (CCLabelTTF*)cell->getChildByTag(4);
		lbName->setString(name->getCString());
		//lbName->setPosition(ccp(m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));
		//lbName->setPosition(ccp(0.75f * G_FRIEND_AVATAR_SIZE + 20, m_sprCell->getContentSize().height/2));

		CCLabelTTF *lbScore = (CCLabelTTF*)cell->getChildByTag(5);
		lbScore->setString(gift->getCString());
		//lbScore->setPosition(ccp(0.25 * m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));
		//lbScore->setPosition(ccp(0.75f * G_FRIEND_AVATAR_SIZE + 20, m_sprCell->getContentSize().height/2)); //0.25 * m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));
	}

	return cell;
}

unsigned int ReceiveGiftScene::numberOfCellsInTableView(CCTableView *table)
{
	return m_listSize;
}



void ReceiveGiftScene::mailInCallback( CCObject* pSender )
{
	int tag = ((CCMenuItemImage*)pSender)->getTag();
	CCLOG("In tag = %d", tag);
	CCString* s = CCString::createWithFormat("In tag %d", tag);
	//CCMessageBox(s->getCString(), "Mail in");

	int idx = tag - 1000;
	CCTableViewCell* cell = m_tableView->cellAtIndex(idx);
	std::string fbID = ((CustomTableViewCell*)cell)->fbID;
}

void ReceiveGiftScene::mailOutCallback( CCObject* pSender )
{
	int tag = ((CCMenuItemImage*)pSender)->getTag();
	CCLOG("Out tag = %d", tag);
	CCString* s = CCString::createWithFormat("Out tag %d", tag);
	//CCMessageBox(s->getCString(), "Mail out");

	int idx = tag - 2000;
	CCTableViewCell* cell = m_tableView->cellAtIndex(idx);
	std::string fbID = ((CustomTableViewCell*)cell)->fbID;
	
	CCLOG("Out facebookID = %s", fbID.c_str());

	s = CCString::createWithFormat("%s", fbID.c_str());
	CCArray* arrFriends = new CCArray();
	arrFriends->addObject(s);

	CCDictionary *giftDictionary = CCDictionary::create();
	giftDictionary->setObject(CCString::create("1"), "LIFE"); //number of gift, name of gift

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	EziSocialObject::sharedObject()->isFacebookSessionActive();

	EziSocialObject::sharedObject()->sendRequestToFriends(
		EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT,
		"Cho mày 1 mạng nè!", 
		arrFriends,
		giftDictionary, 
		"Phi Công Bút Chì");

#endif
}



//end new delegate


// Facebook //=========================================


void ReceiveGiftScene::fbLogInCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_lbWaiting->setVisible(true);
	bool needPublicPermission = true;
	EziSocialObject::sharedObject()->performLoginUsingFacebook(needPublicPermission); // Pass true if you need publish permission also
#endif
}

void ReceiveGiftScene::fbLogOutCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EziSocialObject::sharedObject()->perfromLogoutFromFacebook();
#endif
}



#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void ReceiveGiftScene::fbSessionCallback(int responseCode, const char *responseMessage)
{
	if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL)
	{
		//check incoming request
		EziSocialObject::sharedObject()->checkIncomingRequest();

		m_fbLogInItem->setVisible(false);
		m_lbInvite->setVisible(false);
		m_fbLogOutItem->setVisible(true);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
		{
			EziSocialObject::sharedObject()->postScore(DataManager::sharedDataManager()->GetHighScore());
		}
#endif

		EziSocialObject::sharedObject()->fetchFBUserDetails(true); //need email = true
	}
	else
	{
		m_lbWaiting->setVisible(false);
		m_tableView->setVisible(false);

		m_fbLogInItem->setVisible(true);
		m_lbInvite->setVisible(true);
		m_fbLogOutItem->setVisible(false);
	}
}

void ReceiveGiftScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
	//CCLOG("Gotten avatar for %s", fbID);
	std::string sid = std::string(fbID);

	if ((strcmp(userPhotoPath, "") != 0))
	{
		if(sid == DataManager::sharedDataManager()->GetProfileID())
		{
			//CCLOG("Gotten avatar for user");

			//save 
			DataManager::sharedDataManager()->SetPhotoPath(userPhotoPath);

			CCSprite* userPhoto = CCSprite::create(userPhotoPath);
			userPhoto->setPosition(ccp(m_userSprite->getContentSize().width/2, m_userSprite->getContentSize().height/2));
			userPhoto->setScale(m_userSprite->getContentSize().width/userPhoto->getContentSize().width);
			m_userSprite->addChild(userPhoto);
		}
		
		if (m_arrHighScores != NULL)
		{
			//CCLOG("Gotten avatar for friends");

			CCObject* it;
			CCARRAY_FOREACH(m_arrHighScores, it)
			{
				EziFacebookFriend* fbFriend = dynamic_cast<EziFacebookFriend*>(it);
				if (NULL != fbFriend)
				{
					//CCLOG("%s => %s", sid.c_str(), userPhotoPath);
					if(fbFriend->getFBID() == sid)
					{
						fbFriend->setPhotoPath(userPhotoPath);
						//CCLOG("Set photo path ok --- ");
					}
				}
			}

			m_tableView->setVisible(true);
			m_tableView->reloadData();
		}
	}
}

void ReceiveGiftScene::fbUserDetailCallback( int responseCode, const char* responseMessage, EziFacebookUser* fbUser )
{
	if (fbUser != NULL)
	{
		EziSocialObject::sharedObject()->setCurrentFacebookUser(fbUser);

		//save data
		EziFacebookUser* currentUser = EziSocialObject::sharedObject()->getCurrentFacebookUser();
		std::string firstname = fbUser->getFullName(); //getFirstName //getFullName
		std::string userName = currentUser->getUserName();
		std::string profileID = currentUser->getProfileID();

		DataManager::sharedDataManager()->SetName(firstname.c_str());
		DataManager::sharedDataManager()->SetProfileID(profileID.c_str());
		DataManager::sharedDataManager()->SetFbUserName(userName.c_str());

		m_lbName->setString(firstname.c_str());

		EziSocialObject::sharedObject()->getProfilePicForID(this, currentUser->getProfileID(), // Profile ID of current user
			G_AVATAR_SIZE, G_AVATAR_SIZE, // Size of the image
			false // force download it from server
		);

		callGetHighScores();
	}
}

void ReceiveGiftScene::callSubmitScore()
{
	EziSocialObject::sharedObject()->postScore(DataManager::sharedDataManager()->GetHighScore());
}

void ReceiveGiftScene::callGetHighScores()
{
	EziSocialObject::sharedObject()->getHighScores();
}

void ReceiveGiftScene::fbHighScoresCallback( int responseCode, const char* responseMessage, cocos2d::CCArray* highScores )
{
	if (m_arrHighScores != NULL)
	{
		m_arrHighScores->release();
		m_arrHighScores = NULL;
	}

	m_arrHighScores = CCArray::createWithArray(highScores);
	m_arrHighScores->retain();

	m_listSize = m_arrHighScores->count();

	CCObject* it;
	CCARRAY_FOREACH(m_arrHighScores, it)
	{
		EziFacebookFriend* fbFriend = dynamic_cast<EziFacebookFriend*>(it);
		if (NULL != fbFriend)
		{
			std::string profileID = fbFriend->getFBID();
			std::string myProfileID = DataManager::sharedDataManager()->GetProfileID();

			if(profileID == myProfileID)
			{
				fbFriend->setScore(DataManager::sharedDataManager()->GetHighScore());
			}
			
			//CCLOG("Get avatar for %s", profileID.c_str());
			//get avatar
			EziSocialObject::sharedObject()->getProfilePicForID(this, profileID.c_str(), // Profile ID of current user
				G_FRIEND_AVATAR_SIZE, G_FRIEND_AVATAR_SIZE, // Size of the image
				false // force download it from server
				);

			//CCLOG("%s => %d", fbFriend->getFBID(), (int)fbFriend->getScore());
		}
	}

	MySortHighScore();

	m_tableView->setVisible(true);
	m_tableView->reloadData();
	m_lbWaiting->setVisible(false);
}

void ReceiveGiftScene::fbSendRequestCallback( int responseCode, const char* responseMessage, cocos2d::CCArray* friendsGotRequests )
{
	// 	FB_REQUEST_SENDING_ERROR - In case if there is any error while sending the request
	// 	FB_REQUEST_SENDING_CANCELLED – In case, user decides not to send the request.
	// 	FB_REQUEST_SENT - If sending request is success
	
	if (EziSocialWrapperNS::RESPONSE_CODE::FB_REQUEST_SENT == responseCode)
	{
		int numFriends = friendsGotRequests->count();

		CCLOG("Request sent successfully to %d friends", numFriends);
		CCMessageBox("Infor", "Request sent successfully");
	}
	else
	{
		CCLOG("Request sent failed");
		CCMessageBox("Infor", "Request sent failed");
	}
}

void ReceiveGiftScene::fbIncomingRequestCallback(int responseCode, const char* responseMessage, int totalIncomingRequests)
{
	int pendingRequest = EziFBIncomingRequestManager::sharedManager()->getPendingRequestCount();
	CCLOG("------------------NewRequests= %d\n-----------------PendingRequests= %d", totalIncomingRequests, pendingRequest);

	CCArray* _fbNewList = EziFBIncomingRequestManager::sharedManager()->getAllRequests();
	int totalNew = _fbNewList->count();

	CCLOG("----------------- TOTALT = %d", totalNew);

	for (int i = 0; i < totalNew; i++)
	{
		EziFBIncomingRequest* request = (EziFBIncomingRequest*)_fbNewList->objectAtIndex(i);
		if (request->getRequestType() == EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT)
		{
			if (request->isConsumed())
			{
				CCLOG(" ------------ CONSUMED GIFT ------------- ");
			} 
			else
			{
				CCLOG(" --------------- NEW GIFT --------------- ");
				
				EziFacebookFriend* fr = request->getSender();
				DataManager::sharedDataManager()->IncreaseGiftFromFriend(fr->getFBID());
				
				request->purgeFromUserDefault();
				request->setConsumed(true);
				EziFBIncomingRequestManager::sharedManager()->consumeItem(request);
				EziFBIncomingRequestManager::sharedManager()->clearCompletedRequestList();
			}
		}
		else if (request->getRequestType() == EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE)
		{
			CCLOG(" ------------ INVITE ------------ ");
			
			//check incoming request again, i dont know why
			EziSocialObject::sharedObject()->checkIncomingRequest();
		}
	}
	

	m_tableView->setVisible(true);
	m_tableView->reloadData();
}

#endif

// Facebook //=========================================
