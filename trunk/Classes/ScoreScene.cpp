#include "MenuScene.h"
#include "ScoreScene.h"
#include "cocos-ext.h"
#include "DataManager.h"
#include "CustomTableViewCell.h"
#include "MyMacro.h"
#include "AudioManager.h"
#include "Global.h"


USING_NS_CC;
USING_NS_CC_EXT;

bool ScoreScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	this->setKeypadEnabled(true);

	//////////////////////////////////////////////////////////////////////////

	m_tableXepHangSize = 0;
	m_tableQuatangSize = 0;

	m_friendList = NULL;
	m_arrHighScores = NULL;
	m_arrRequests = NULL;
	m_friendCell = NULL;

	CCSprite* bg = CCSprite::create("bg_friend.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg);

	CCSprite* table_top = CCSprite::create("table_top.png");
	table_top->setPosition(ccp(400, 1280-202));
	this->addChild(table_top);

	CCSprite* table_border = CCSprite::create("table_border.png");
	table_border->setPosition(ccp(400, 1280-767));
	this->addChild(table_border);


	//USER Information //////////////////////////////////////////////////////////////////////////


	//avatar
	if(DataManager::sharedDataManager()->GetPhotoPath() != "NULL")
	{
		m_userAvatar = CCSprite::create(DataManager::sharedDataManager()->GetPhotoPath().c_str());
	}
	else
	{
		m_userAvatar = CCSprite::create("fb-profile.png");
	}

	m_userAvatar->setPosition(ccp(93, 1280-202));
	this->addChild(m_userAvatar);

	//name
	std::string name = DataManager::sharedDataManager()->GetName();
	if (name.length() > 18) {
		name = name.substr(0, 16);
		name.append("..");
	}
	m_lbName = CCLabelTTF::create(name.c_str(), "Roboto-Medium.ttf", 52);
	m_lbName->setFontFillColor(ccc3(0, 0, 0));
	m_lbName->setPosition(ccp(174, 1280-173));
	m_lbName->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbName);

	//score
	CCString* str = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetHighScore());
	m_lbScore = CCLabelTTF::create(str->getCString(), "Roboto-Medium.ttf", 48);
	m_lbScore->setFontFillColor(ccc3(0, 0, 0));
	m_lbScore->setPosition(ccp(174, 1280-239));
	m_lbScore->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbScore);

	//life
	m_sprLife = CCSprite::create("oil.png");
	m_sprLife->setScale(0.75f);
	m_sprLife->setPosition(ccp(609, 1280-164));
	this->addChild(m_sprLife);

	str = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetLastPlayerLife());
	m_lbLife = CCLabelTTF::create(str->getCString(), "Roboto-Medium.ttf", 48);
	m_lbLife->setFontFillColor(ccc3(0, 0, 0));
	m_lbLife->setPosition(ccp(650, 1280-164));
	m_lbLife->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbLife);
	
	//boom
	m_sprBoom = CCSprite::create("boomgift.png");
	m_sprBoom->setScale(0.75f);
	m_sprBoom->setPosition(ccp(609, 1280-245));
	this->addChild(m_sprBoom);

	str = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetBoom());
	m_lbBoom = CCLabelTTF::create(str->getCString(), "Roboto-Medium.ttf", 48);
	m_lbBoom->setFontFillColor(ccc3(0, 0, 0));
	m_lbBoom->setPosition(ccp(650, 1280-245));
	m_lbBoom->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbBoom);


	//Xep hang

	CCMenuItem* xephangOn = CCMenuItemImage::create("xephang.png", NULL, NULL);
	CCMenuItem* xephangOff = CCMenuItemImage::create("xephang1.png", NULL, NULL);
	m_xephangToggle = CCMenuItemToggle::createWithTarget(this,  menu_selector(ScoreScene::xephangCallback), xephangOn, xephangOff, NULL);
	m_xephangToggle->setSelectedIndex(0);
	m_xephangToggle->setPosition(ccp(202, 1280-351));

	//Qua tang

	CCMenuItem* quatangOn = CCMenuItemImage::create("quatang.png", NULL, NULL);
	CCMenuItem* quatangOff = CCMenuItemImage::create("quatang1.png", NULL, NULL);
	m_quatangToggle = CCMenuItemToggle::createWithTarget(this,  menu_selector(ScoreScene::quatangCallback), quatangOn, quatangOff, NULL);
	m_quatangToggle->setSelectedIndex(1);
	m_quatangToggle->setPosition(ccp(600, 1280-351));
	

	//
	CCMenuItemImage *backItem = CCMenuItemImage::create(
		"back.png",
		"back1.png",
		this,
		menu_selector(ScoreScene::menuCallback));
	backItem->setPosition(ccp(92, 1280-1201));


	//add more friends
	CCMenuItemImage* itAddFriend = CCMenuItemImage::create("add_friend.png", "add_friend_hover.png", this, menu_selector(ScoreScene::addFriendCallback));
	itAddFriend->setPosition(ccp(255, 1280-1198)); //382, 1280-1205));
	

	//
	m_fbLogOutItem = CCMenuItemImage::create(
		"disconnect_facebook.png",
		"disconnect_facebook.png",
		this,
		menu_selector(ScoreScene::fbLogOutCallback));
	m_fbLogOutItem->setPosition(ccp(611, 1280-1205));
	this->addChild(m_fbLogOutItem);

	//Facebook button
	

	m_isLoggedIn = true;
	m_fbLogInItem = CCMenuItemImage::create(
		"connect_facebook.png", 
		"connect_facebook.png", 
		this,  
		menu_selector(ScoreScene::fbLogInCallback));
	m_fbLogInItem->setPosition(ccp(400, 1280-805));

	m_lbInvite = CCLabelTTF::create("Liên kết Facebook\nthêm niềm vui", "Roboto-Medium.ttf", 48);
	m_lbInvite->setFontFillColor(ccc3(0, 0, 0));
	m_lbInvite->setPosition(ccp(400, 1280-672)); //320
	this->addChild(m_lbInvite, 1); //samw menu


	CCMenu* pMenu = CCMenu::create(backItem, m_fbLogInItem, m_fbLogOutItem, m_xephangToggle, m_quatangToggle, itAddFriend, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);



	//Table //////////////////////////////////////////////////////////////////////////


	m_isXepHangView = true;

	//table view XepHang //////////////////////////////////////

	m_sprCell = CCSprite::create("table_cell_xephang.png");
	m_sprCell->retain();
	CCSize cellsize = m_sprCell->getContentSize();
	CCSize tableSize = CCSizeMake(783, 718);

	//vertical
	m_tableXephang = CCTableView::create(this, tableSize);
	m_tableXephang->setDirection(kCCScrollViewDirectionVertical);
	m_tableXephang->setPosition(ccp(400 - tableSize.width/2, 1280-768 - tableSize.height/2));
	m_tableXephang->setDelegate(this);
	m_tableXephang->setVerticalFillOrder(kCCTableViewFillTopDown);
	this->addChild(m_tableXephang);

	m_tableXephang->setVisible(true);



	// table view QuaTang //////////////////////////////////////////////////////////////////////////



	m_sprCell = CCSprite::create("table_cell_quatang.png");
	m_sprCell->retain();
	cellsize = m_sprCell->getContentSize();
	tableSize = CCSizeMake(783, 718);

	//vertical
	m_tableQuatang = CCTableView::create(this, tableSize);
	m_tableQuatang->setDirection(kCCScrollViewDirectionVertical);
	m_tableQuatang->setPosition(ccp(400 - tableSize.width/2, 1280-768 - tableSize.height/2));
	m_tableQuatang->setDelegate(this);
	m_tableQuatang->setVerticalFillOrder(kCCTableViewFillTopDown);
	this->addChild(m_tableQuatang);

	m_tableQuatang->setVisible(false);

	//////////////////////////////////////////////////////////////////////////

	m_lbWaiting = CCLabelTTF::create("... Đang chờ ...", "Roboto-Medium.ttf", 64);
	m_lbWaiting->setColor(ccc3(0, 0, 0));
	m_lbWaiting->setHorizontalAlignment(kCCTextAlignmentCenter);
	m_lbWaiting->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	m_lbWaiting->setPosition(ccp(400, 640));

	this->addChild(m_lbWaiting);




	//////////////////////////////////////////////////////////////////////////
	


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EziSocialObject::sharedObject()->setFacebookDelegate(this);

	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		callSubmitScore();

		//check incoming request
		EziSocialObject::sharedObject()->checkIncomingRequest();


		m_isLoggedIn = true;
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
		m_isLoggedIn = false;

		m_tableXephang->setVisible(false);
		m_tableQuatang->setVisible(false);
		m_lbWaiting->setVisible(false);

		m_fbLogInItem->setVisible(true);
		m_lbInvite->setVisible(true);
		m_fbLogOutItem->setVisible(false);
	}
#endif
	
	this->schedule(schedule_selector(ScoreScene::scheduleTimer), 1);

	return true;
}

void ScoreScene::menuCallback(CCObject* pSender)
{
	PLAY_BUTTON_EFFECT;

	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void ScoreScene::addFriendCallback( CCObject* pSender )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		CCString* str = CCString::createWithFormat("%s đang tả xung hữu đột tiêu diệt máy bay địch trong Điện Biên Phủ trên không. Hãy cùng tham chiến nào.", 
			DataManager::sharedDataManager()->GetFbFullName().c_str());
		
		//invite friends
		EziSocialObject::sharedObject()->sendRequestToFriends(
			EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE,
			str->getCString(), 
			NULL,
			NULL,
			"Điện Biên Phủ Trên Không");
	}
#endif
}

void ScoreScene::keyBackClicked()
{
	menuCallback(NULL);
}



void ScoreScene::xephangCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;
	m_isXepHangView = true;
	refreshView();
}

void ScoreScene::quatangCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;
	m_isXepHangView = false;
	refreshView();
}



//new delegate

void ScoreScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	CCLOG("Cell at index: %i", cell->getIdx());

	if (table == m_tableXephang) //Xep hang
	{
		//nothing
	} 
	else ///////////////////////// Qua tang
	{
		CustomTableViewCell* customCell = (CustomTableViewCell*) cell;

		int curLife = DataManager::sharedDataManager()->GetLastPlayerLife();
		CCLOG("CURRENT LIFE = %d", curLife);
		
		if (curLife < G_MAX_PLAYER_LIFE)
		{
			PLAY_GET_BOMB_EFFECT;

			curLife++;

			DataManager::sharedDataManager()->SetLastPlayerLife(curLife);
			CCLOG("ADD TO CURRENT LIFE = %d", curLife);

			//save next time
			tm* _tm = DataManager::sharedDataManager()->GetLastDeadTime();
			_tm->tm_sec += G_PLAYER_TIME_TO_REVIVE;
			mktime(_tm); //normalize
			DataManager::sharedDataManager()->SetLastDeadTime(_tm);

			//delete request
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			CCLOG("REMOVE REQUEST");

			m_arrRequests->removeObject(customCell->m_request);
			m_tableQuatangSize = m_arrRequests->count();

			//delete request in pool
			customCell->m_request->purgeFromUserDefault();
			customCell->m_request->setConsumed(true);
			EziFBIncomingRequestManager::sharedManager()->consumeItem(customCell->m_request);
			EziFBIncomingRequestManager::sharedManager()->clearCompletedRequestList();
#endif

			//refresh UI
			m_tableQuatang->reloadData();

			refreshUserDetail();
		} 
		else
		{
			PLAY_OUT_PORP_EFFECT;
		}

		//animation
		m_sprLife->setScale(0.75f);
		m_sprLife->runAction(CCSequence::createWithTwoActions(
			CCScaleBy::create(0.2f, 1.5f / 1.0f),
			CCScaleBy::create(0.2f, 1.0f / 1.5f)
			));
	}	
}

CCSize ScoreScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_sprCell->getContentSize();
}

CCTableViewCell* ScoreScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	if (table == m_tableXephang)
	{
		return tableCellXepHangAtIndex(table, idx);
	} 
	else
	{
		return tableCellQuatangAtIndex(table, idx);
	}
}

unsigned int ScoreScene::numberOfCellsInTableView(CCTableView *table)
{
	if (table == m_tableXephang)
	{
		return m_tableXepHangSize;
	} 
	else //Qua tang
	{
		return m_tableQuatangSize;
	}	
}



void ScoreScene::getBoomCallback( CCObject* pSender )
{
	int tag = ((CCMenuItemImage*)pSender)->getTag();
	int idx = tag - 1000;
	CustomTableViewCell* cell = (CustomTableViewCell*)m_tableXephang->cellAtIndex(idx);
	std::string fbID = ((CustomTableViewCell*)cell)->fbID;

	//check if ok
	int numBoom = DataManager::sharedDataManager()->GetBoom();

	if (numBoom < G_MAX_BOOM)
	{
		PLAY_GET_BOMB_EFFECT;
		DataManager::sharedDataManager()->IncreaseBoom();
		DataManager::sharedDataManager()->SetTimeBoomFriendNow(fbID.c_str());

		//show clock
		cell->m_lbGetBoom->setVisible(false);
		cell->m_lbGetBoomTimer->setVisible(true);
		cell->m_lastTimeGetBoom = DataManager::sharedDataManager()->GetTimeBoomFriend(fbID.c_str());

		refreshUserDetail();

		//animation
		m_sprBoom->runAction(CCSequence::createWithTwoActions(
			CCScaleBy::create(0.2f, 1.5f / 1.0f),
			CCScaleBy::create(0.2f, 1.0f / 1.5f)
			));

// 		CCSprite* spr = CCSprite::create("boomgift.png");
// 		spr->setPosition(cell->m_itGetBoom->getPosition() + cell->getPosition() + m_tableXephang->getPosition());
// 		this->addChild(spr);
// 		spr->runAction(CCSpawn::createWithTwoActions(
// 			CCMoveTo::create(0.5f, m_sprBoom->getPosition()),
// 			CCScaleTo::create(0.5f, m_sprBoom->getScale())
// 			)); //no need to remove
	}
	else
	{
		PLAY_OUT_PORP_EFFECT;
	}
}

void ScoreScene::sendLifeCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	int tag = ((CCMenuItemImage*)pSender)->getTag();
	int idx = tag - 2000;
	CustomTableViewCell* cell = (CustomTableViewCell*)m_tableXephang->cellAtIndex(idx);
	std::string fbID = cell->fbID;
	
	CCString* s = CCString::createWithFormat("%s", fbID.c_str());
	CCArray* arrFriends = new CCArray();
	arrFriends->addObject(s);

	CCDictionary *giftDictionary = CCDictionary::create();
	giftDictionary->setObject(CCString::create("1"), "LIFE");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EziSocialObject::sharedObject()->isFacebookSessionActive();

	EziSocialObject::sharedObject()->sendRequestToFriends(
		EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT,
		"Cho bạn 1 mạng nè!", 
		arrFriends,
		giftDictionary, 
		"Điện Biên Phủ Trên Không");
#endif

	m_friendCell = cell;
}



void ScoreScene::refreshView()
{
	if (m_isXepHangView)
	{
		m_xephangToggle->setSelectedIndex(0);
		m_quatangToggle->setSelectedIndex(1);

		m_tableXephang->setVisible(true);
		m_tableQuatang->setVisible(false);
	} 
	else
	{
		m_xephangToggle->setSelectedIndex(1);
		m_quatangToggle->setSelectedIndex(0);

		m_tableXephang->setVisible(false);
		m_tableQuatang->setVisible(true);
	}

	if (m_isLoggedIn == false)
	{
		m_tableXephang->setVisible(false);
		m_tableQuatang->setVisible(false);
	}
}

void ScoreScene::refreshUserDetail()
{
	CCString* str = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetLastPlayerLife());
	m_lbLife->setString(str->getCString());

	str = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetBoom());
	m_lbBoom->setString(str->getCString());
}

void ScoreScene::scheduleTimer( float dt )
{
	//CCLOG("Schedule timer");
	int numCell = numberOfCellsInTableView(m_tableXephang);

	for (int i = 0; i < numCell; ++i)
	{
		CustomTableViewCell* cell = (CustomTableViewCell*) tableCellAtIndex(m_tableXephang, i);

		//Get boom //////////////////////////////////////////////////////////////////////////

		if (cell != NULL)
		{
			//CCLOG("Schedule timer ------ Get boom");
			//Get boom //////////////////////////////////////////////////////////////////////////
			
			if (cell->m_itGetBoom != NULL &&
				cell->m_lbGetBoomTimer != NULL &&
				cell->m_lbGetBoom != NULL)
			{
				time_t lastTime = mktime(cell->m_lastTimeGetBoom);
				time_t curTime = time(NULL);
				int waitTime = 24 * 60 * 60 - (int)difftime(curTime, lastTime);

				if (waitTime <= 0)
				{
					//FULL TIME
					cell->m_itGetBoom->setEnabled(true);
					cell->m_lbGetBoomTimer->setVisible(false);
					cell->m_lbGetBoom->setVisible(true);
				}
				else
				{
					cell->m_itGetBoom->setEnabled(false);
					cell->m_lbGetBoomTimer->setVisible(true);
					cell->m_lbGetBoom->setVisible(false);
				}

				waitTime = (waitTime > 0) ? waitTime : 0;
			
				int hour, min, sec;
				sec = waitTime % 60;
				waitTime -= sec;
				min =  ((int)(waitTime / 60)) % 60;
				hour = ((int)(waitTime / 60)) / 60;
				CCString* str = CCString::createWithFormat("%d:%d:%d", hour, min, sec);

				if (cell->m_lbGetBoomTimer != NULL)
				{
					cell->m_lbGetBoomTimer->setString(str->getCString());
				}
			}

			//Send life //////////////////////////////////////////////////////////////////////////
			
			if (cell->m_itSendLife != NULL &&
				cell->m_lbSendLifeTimer != NULL &&
				cell->m_lbSendLife != NULL)
			{
				//CCLOG("Schedule timer ------ Send life");

				time_t lastTime = mktime(cell->m_lastTimeSendLife);
				time_t curTime = time(NULL);
				int waitTime = 24 * 60 * 60 - (int)difftime(curTime, lastTime);

				if (waitTime <= 0)
				{
					//FULL TIME
					cell->m_itSendLife->setEnabled(true);
					cell->m_lbSendLifeTimer->setVisible(false);
					cell->m_lbSendLife->setVisible(true);
				}
				else
				{
					cell->m_itSendLife->setEnabled(false);
					cell->m_lbSendLifeTimer->setVisible(true);
					cell->m_lbSendLife->setVisible(false);
				}

				waitTime = (waitTime > 0) ? waitTime : 0;


				int hour, min, sec;
				sec = waitTime % 60;
				waitTime -= sec;
				min =  ((int)(waitTime / 60)) % 60;
				hour = ((int)(waitTime / 60)) / 60;
				CCString* str = CCString::createWithFormat("%d:%d:%d", hour, min, sec);

				if (cell->m_lbSendLifeTimer != NULL)
				{
					cell->m_lbSendLifeTimer->setString(str->getCString());
				}
			}
		}
	}
}



//end new delegate


// Facebook //=========================================


void ScoreScene::fbLogInCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_lbWaiting->setVisible(true);
	bool needPublicPermission = true;
	EziSocialObject::sharedObject()->performLoginUsingFacebook(needPublicPermission); // Pass true if you need publish permission also
#endif
}

void ScoreScene::fbLogOutCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EziSocialObject::sharedObject()->perfromLogoutFromFacebook();
#endif
}



void ScoreScene::fbSessionCallback(int responseCode, const char *responseMessage)
{
	CCLOG("fbSessionCallback");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL)
	{
		CCLOG("fbSessionCallback: SUCCESSFUL");
		m_isLoggedIn = true;

		m_fbLogInItem->setVisible(false);
		m_lbInvite->setVisible(false);
		m_fbLogOutItem->setVisible(true);

		refreshView();

		callSubmitScore();
		
		//Auto fetchFBUserDetails, do not call it again
		//It make exception
		//CCLOG("call: fetchFBUserDetails");
		//EziSocialObject::sharedObject()->fetchFBUserDetails(true); //need email = true
	}
	else
	{
		CCLOG("fbSessionCallback: FAILED");
		m_isLoggedIn = false;

		m_lbWaiting->setVisible(false);
		m_tableXephang->setVisible(false);
		m_tableQuatang->setVisible(false);

		m_fbLogInItem->setVisible(true);
		m_lbInvite->setVisible(true);
		m_fbLogOutItem->setVisible(false);
	}
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // fbUserDetailCallback

void ScoreScene::fbUserDetailCallback( int responseCode, const char* responseMessage, EziFacebookUser* fbUser )
{
	CCLOG("fbUserDetailCallback");
	if (fbUser != NULL)
	{
		CCLOG("fbUserDetailCallback: user != NULL");
		EziSocialObject::sharedObject()->setCurrentFacebookUser(fbUser);

		//save data
		std::string firstname = fbUser->getFirstName(); //getFirstName //getFullName
		std::string userName = fbUser->getUserName();
		std::string profileID = fbUser->getProfileID();
		std::string fullName = fbUser->getFullName();

		DataManager::sharedDataManager()->SetName(firstname.c_str());
		DataManager::sharedDataManager()->SetProfileID(profileID.c_str());
		DataManager::sharedDataManager()->SetFbUserName(userName.c_str());
		DataManager::sharedDataManager()->SetFbFullName(fullName.c_str());

		m_lbName->setString(firstname.c_str());

		CCLOG("call: getProfilePicForID");
		EziSocialObject::sharedObject()->getProfilePicForID(this, fbUser->getProfileID(), // Profile ID of current user
			G_AVATAR_SIZE, G_AVATAR_SIZE, // Size of the image
			false // force download it from server
		);

		callGetHighScores();

		//check incoming request
		EziSocialObject::sharedObject()->checkIncomingRequest();
	}
}
#endif

void ScoreScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
	CCLOG("fbUserPhotoCallback");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//CCLOG("Gotten avatar for %s", fbID);
	std::string sid = std::string(fbID);

	if ((strcmp(userPhotoPath, "") != 0))
	{
		CCLOG("fbUserPhotoCallback: userPhotoPath != NULL");
		if(sid == DataManager::sharedDataManager()->GetProfileID())
		{
			CCLOG("fbUserPhotoCallback: this user");
			//CCLOG("Gotten avatar for user");

			//save 
			DataManager::sharedDataManager()->SetPhotoPath(userPhotoPath);

			CCSprite* userPhoto = CCSprite::create(userPhotoPath);
			userPhoto->setPosition(ccp(m_userAvatar->getContentSize().width/2, m_userAvatar->getContentSize().height/2));
			userPhoto->setScale(m_userAvatar->getContentSize().width/userPhoto->getContentSize().width);
			
			if (m_userAvatar)
			{
				m_userAvatar->addChild(userPhoto);
			}
			else
			{
				CCLOG("ERROR: m_userAvatar == NULL");
			}
		}
		
		if (m_arrHighScores != NULL)
		{
			CCLOG("fbUserPhotoCallback: friends");

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

			refreshView();

			m_tableXephang->reloadData();
			m_tableQuatang->reloadData();
		}
	}
#endif
}

void ScoreScene::callSubmitScore()
{
	CCLOG("callSubmitScore");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		EziSocialObject::sharedObject()->postScore(DataManager::sharedDataManager()->GetHighScore());
	}
#endif
}

void ScoreScene::callGetHighScores()
{
	CCLOG("callGetHighScores");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		EziSocialObject::sharedObject()->getHighScores();
	}
#endif
}

void ScoreScene::fbHighScoresCallback( int responseCode, const char* responseMessage, cocos2d::CCArray* highScores )
{
	CCLOG("fbHighScoresCallback");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (m_arrHighScores != NULL)
	{
		m_arrHighScores->release();
		m_arrHighScores = NULL;
	}

	m_arrHighScores = CCArray::createWithArray(highScores);
	m_arrHighScores->retain();

	m_tableXepHangSize = m_arrHighScores->count();

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
			
			//get avatar
			EziSocialObject::sharedObject()->getProfilePicForID(this, profileID.c_str(), // Profile ID of current user
				G_FRIEND_AVATAR_SIZE, G_FRIEND_AVATAR_SIZE, // Size of the image
				false // force download it from server
				);
		}
	}

	MySortHighScore();

	m_lbWaiting->setVisible(false);

	m_tableXephang->reloadData();
	m_tableQuatang->reloadData();
#endif
}

void ScoreScene::fbSendRequestCallback( int responseCode, const char* responseMessage, cocos2d::CCArray* friendsGotRequests )
{
	CCLOG("fbSendRequestCallback");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (EziSocialWrapperNS::RESPONSE_CODE::FB_REQUEST_SENT == responseCode)
	{
		CCLOG("fbSendRequestCallback: FB_REQUEST_SENT");
		
		if (m_friendCell != NULL) //send life
		{
			CCLOG("fbSendRequestCallback: FB_REQUEST_SENT: m_friendCell != NULL");
			PLAY_GET_BOMB_EFFECT;

			//reset timer
			DataManager::sharedDataManager()->SetTimeLifeToFriendNow(m_friendCell->fbID.c_str());

			//show clock
			m_friendCell->m_lbSendLife->setVisible(false);
			m_friendCell->m_lbSendLifeTimer->setVisible(true);
			m_friendCell->m_lastTimeSendLife = DataManager::sharedDataManager()->GetTimeLifeToFriend(m_friendCell->fbID.c_str());

			m_friendCell = NULL;
		}		
	}
	else
	{
		CCLOG("fbSendRequestCallback: FB_REQUEST_SENT: m_friendCell == NULL");
		PLAY_OUT_PORP_EFFECT;
		CCLOG("Request sent failed");
	}
#endif
}

void ScoreScene::fbIncomingRequestCallback(int responseCode, const char* responseMessage, int totalIncomingRequests)
{
	CCLOG("fbIncomingRequestCallback");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	int pendingRequest = EziFBIncomingRequestManager::sharedManager()->getPendingRequestCount();
	CCLOG("------------------NewRequests= %d\n-----------------PendingRequests= %d", totalIncomingRequests, pendingRequest);

	if (pendingRequest > 0)
	{
		if (m_arrRequests != NULL)
		{
			m_arrRequests->release();
			m_arrRequests = NULL;
		}
		m_arrRequests = new CCArray();
		m_arrRequests->retain();

		CCDictionary* dictPendingRequests = EziFBIncomingRequestManager::sharedManager()->getPendingRequests();

		//foreach Dictionary to put into m_arrRequests

		m_tableQuatangSize = 0;
		CCDictElement* pElement = NULL;
		CCDICT_FOREACH(dictPendingRequests, pElement)
		{
			EziFBIncomingRequest* request = (EziFBIncomingRequest*)pElement->getObject();
			if (request->getRequestType() == EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT)
			{
				if (request->isConsumed())
				{
					CCLOG(" ------------ CONSUMED GIFT ------------- ");
				} 
				else
				{
					CCLOG(" --------------- NEW GIFT --------------- ");
					m_arrRequests->addObject(request);
				}
			}
			else if (request->getRequestType() == EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE)
			{
				if (request->isConsumed())
				{
					CCLOG(" ------------ CONSUMED INVITE ------------- ");
				}
				else
				{
					CCLOG(" --------------- NEW INVITE --------------- ");
				}
			}
		}
		m_tableQuatangSize = m_arrRequests->count();
		
		CCLOG("CALL: m_tableQuatang->reloadData()");
		m_tableQuatang->reloadData();
	}
#endif
}

CCTableViewCell* ScoreScene::tableCellXepHangAtIndex( CCTableView *table, unsigned int idx )
{
	bool isMyScore = false;
	CCString *strOrder = CCString::createWithFormat("%d", idx + 1);
	CCString *strScore = CCString::create("0");
	CCString *strName = CCString::create(G_DEFAULT_NAME);
	CCString *strPhoto = CCString::create("fb-profile.png");
	std::string strFriendId;

	//get boom
	tm* _lastTimeGetBoom;
	int _getBoomWaitTime = 0;
	CCString* strGetBoomTimer = CCString::create("00:00:00");

	//send life
	tm* _lastTimeSendLife;
	int _sendLifeWaitTime = 0;
	CCString* strSendLifeTimer = CCString::create("00:00:00");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(m_arrHighScores == NULL)
	{
		CCLOG("m_arrHighScores == NULL");
	}
	else
	{
		EziFacebookFriend* fbFriend = dynamic_cast<EziFacebookFriend*>(m_arrHighScores->objectAtIndex(idx));

		strScore = CCString::createWithFormat("%d", (int)fbFriend->getScore());

		std::string sname = std::string(fbFriend->getName());
		if (sname.length() > 18) {
			sname = sname.substr(0, 15);
			sname.append("...");
		}			
		strName  = CCString::create(sname);

		strFriendId = std::string(fbFriend->getFBID());

		//get boom
		_lastTimeGetBoom = DataManager::sharedDataManager()->GetTimeBoomFriend(strFriendId.c_str());
		if (_lastTimeGetBoom == NULL)
		{
			//2014 - 114 = 1900
			//very last
			_lastTimeGetBoom = new tm();
			_lastTimeGetBoom->tm_hour = 0;
			_lastTimeGetBoom->tm_min = 0;
			_lastTimeGetBoom->tm_sec = 0;
			_lastTimeGetBoom->tm_mday = 0;
			_lastTimeGetBoom->tm_mon = 0;
			_lastTimeGetBoom->tm_year = 0;

			DataManager::sharedDataManager()->SetTimeBoomFriend(strFriendId.c_str(), _lastTimeGetBoom);
		}
		time_t lastTime = mktime(_lastTimeGetBoom);
		time_t curTime = time(NULL);
		_getBoomWaitTime = 24 * 60 * 60 - (int)difftime(curTime, lastTime);
		_getBoomWaitTime = (_getBoomWaitTime > 0) ? _getBoomWaitTime : 0;

		int hour, min, sec;
		sec = _getBoomWaitTime % 60;
		_getBoomWaitTime -= sec;
		min =  ((int)(_getBoomWaitTime / 60)) % 60;
		hour = ((int)(_getBoomWaitTime / 60)) / 60;
		strGetBoomTimer = CCString::createWithFormat("%d:%d:%d", hour, min, sec);



		//send life
		_lastTimeSendLife = DataManager::sharedDataManager()->GetTimeLifeToFriend(strFriendId.c_str());
		if (_lastTimeSendLife == NULL)
		{
			//2014 - 114 = 1900
			//very last
			_lastTimeSendLife = new tm();
			_lastTimeSendLife->tm_hour = 0;
			_lastTimeSendLife->tm_min = 0;
			_lastTimeSendLife->tm_sec = 0;
			_lastTimeSendLife->tm_mday = 0;
			_lastTimeSendLife->tm_mon = 0;
			_lastTimeSendLife->tm_year = 0;

			DataManager::sharedDataManager()->SetTimeLifeToFriend(strFriendId.c_str(), _lastTimeSendLife);
		}
		time_t lastTimeSendLife = mktime(_lastTimeSendLife);
		time_t curTimeSendLife = time(NULL);
		_sendLifeWaitTime = 24 * 60 * 60 - (int)difftime(curTimeSendLife, lastTimeSendLife);
		_sendLifeWaitTime = (_sendLifeWaitTime > 0) ? _sendLifeWaitTime : 0;

		//int hour, min, sec;
		sec = _sendLifeWaitTime % 60;
		_sendLifeWaitTime -= sec;
		min =  ((int)(_sendLifeWaitTime / 60)) % 60;
		hour = ((int)(_sendLifeWaitTime / 60)) / 60;
		strSendLifeTimer = CCString::createWithFormat("%d:%d:%d", hour, min, sec);
		
		//////////////////////////////////////////////////////////////////////////


		if (strlen(fbFriend->getPhotoPath()) > 1)
		{
			strPhoto  = CCString::createWithFormat("%s", fbFriend->getPhotoPath());
		}
		if(fbFriend->getFBID() == DataManager::sharedDataManager()->GetProfileID())
		{
			isMyScore = true;
		}
	}
#endif

	CCTableViewCell *cell = table->cellAtIndex(idx);
	if (!cell) 
	{
		cell = new CustomTableViewCell();
		cell->autorelease();

		((CustomTableViewCell*)cell)->fbID = strFriendId;
		((CustomTableViewCell*)cell)->m_lastTimeGetBoom = _lastTimeGetBoom;
		((CustomTableViewCell*)cell)->m_lastTimeSendLife = _lastTimeSendLife;

		CCSprite *sprite = CCSprite::create("table_cell_xephang.png");
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(0, 0));
		sprite->setTag(1);
		cell->addChild(sprite);

		CCSprite *avatar = CCSprite::create(strPhoto->getCString());
		avatar->setScale(0.78125f);
		avatar->setPosition(ccp(140, m_sprCell->getContentSize().height/2));
		avatar->setTag(2);
		cell->addChild(avatar);

		CCLabelTTF *lbOrder;
		if (idx < 3)
		{
			lbOrder = CCLabelTTF::create(strOrder->getCString(), "Villa.ttf", 42);
		} 
		else
		{
			lbOrder = CCLabelTTF::create(strOrder->getCString(), "Roboto-Medium.ttf", 42);
		}

		lbOrder->setFontFillColor(ccc3(0, 0, 0));
		lbOrder->setHorizontalAlignment(kCCTextAlignmentLeft); //cocos2d::CCTextAlignment::
		lbOrder->setPosition(ccp(30, m_sprCell->getContentSize().height/2));
		lbOrder->setTag(3);
		lbOrder->setAnchorPoint(ccp(0.0f, 0.5f));
		cell->addChild(lbOrder);

		//Jundat TPham..
		//21

		CCLabelTTF *lbName = CCLabelTTF::create(strName->getCString(), "Roboto-Medium.ttf", 42);
		lbName->setFontFillColor(ccc3(0,0,0));
		lbName->setPosition(ccp(210, m_sprCell->getContentSize().height * 3/4));
		lbName->setAnchorPoint(ccp(0.0f, 0.5f));
		lbName->setTag(4);
		cell->addChild(lbName);

		CCLabelTTF *lbScore = CCLabelTTF::create(strScore->getCString(), "Roboto-Medium.ttf", 42);
		lbScore->setFontFillColor(ccc3(0, 0, 0));
		lbScore->setPosition(ccp(210, m_sprCell->getContentSize().height * 3/8)); //0.25 * m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));
		lbScore->setAnchorPoint(ccp(0.0f, 0.5f));
		lbScore->setTag(5);
		cell->addChild(lbScore);

		if (isMyScore == false)
		{
			CCMenuItemImage* itGetBoom = CCMenuItemImage::create("boomgift.png", "boomgift1.png", "boomgift1.png", this, menu_selector(ScoreScene::getBoomCallback));
			itGetBoom->setPosition(ccp(600, m_sprCell->getContentSize().height/2 + 10));
			itGetBoom->setTag(1000 + idx);
			((CustomTableViewCell*)cell)->m_itGetBoom = itGetBoom;

			CCMenuItemImage* itSendLife = CCMenuItemImage::create("oil.png", "oil_blur.png", "oil_blur.png", this, menu_selector(ScoreScene::sendLifeCallback));
			itSendLife->setPosition(ccp(725, m_sprCell->getContentSize().height/2 + 15));
			itSendLife->setTag(2000 + idx);
			((CustomTableViewCell*)cell)->m_itSendLife = itSendLife;

			CCMenu* cell_menu = CCMenu::create(itGetBoom, itSendLife, NULL);
			cell_menu->setPosition(CCPointZero);
			cell->addChild(cell_menu);


			//////////////////////////////////////////////////////////////////////////


			//Send life
			CCLabelTTF* lbSendLifeTimer = CCLabelTTF::create(strSendLifeTimer->getCString(), "Roboto-Medium.ttf", 28); //32
			lbSendLifeTimer->setFontFillColor(ccc3(0, 0, 0));
			lbSendLifeTimer->setAnchorPoint(ccp(0.5f, 0.75f));
			lbSendLifeTimer->setPosition(ccp(725, m_sprCell->getContentSize().height/4));
			lbSendLifeTimer->setTag(3000 + idx);
			cell->addChild(lbSendLifeTimer);
			((CustomTableViewCell*)cell)->m_lbSendLifeTimer = lbSendLifeTimer;

			CCLabelTTF* lbSendLife = CCLabelTTF::create("Gửi", "Roboto-Medium.ttf", 28);
			lbSendLife->setFontFillColor(ccc3(0, 0, 0));
			lbSendLife->setAnchorPoint(ccp(0.5f, 0.75f));
			lbSendLife->setPosition(ccp(725, m_sprCell->getContentSize().height/4));
			lbSendLife->setTag(4000 + idx);
			cell->addChild(lbSendLife);
			((CustomTableViewCell*)cell)->m_lbSendLife = lbSendLife;


			//////////////


			//Get boom
			CCLabelTTF* lbGetBoomTimer = CCLabelTTF::create(strGetBoomTimer->getCString(), "Roboto-Medium.ttf", 28); //32
			lbGetBoomTimer->setFontFillColor(ccc3(0, 0, 0));
			lbGetBoomTimer->setAnchorPoint(ccp(0.5f, 0.75f));
			lbGetBoomTimer->setPosition(ccp(600, m_sprCell->getContentSize().height/4));
			lbGetBoomTimer->setTag(3000 + idx);
			cell->addChild(lbGetBoomTimer);
			((CustomTableViewCell*)cell)->m_lbGetBoomTimer = lbGetBoomTimer;

			CCLabelTTF* lbGetBoom = CCLabelTTF::create("Nhận", "Roboto-Medium.ttf", 28);
			lbGetBoom->setFontFillColor(ccc3(0, 0, 0));
			lbGetBoom->setAnchorPoint(ccp(0.5f, 0.75f));
			lbGetBoom->setPosition(ccp(600, m_sprCell->getContentSize().height/4));
			lbGetBoom->setTag(4000 + idx);
			cell->addChild(lbGetBoom);
			((CustomTableViewCell*)cell)->m_lbGetBoom = lbGetBoom;


			//////////////////////////////////////////////////////////////////////////

			//Get boom
			if (_getBoomWaitTime <= 0)
			{
				//FULL TIME
				itGetBoom->setEnabled(true);
				lbGetBoomTimer->setVisible(false);
				lbGetBoom->setVisible(true);
			}
			else
			{
				itGetBoom->selected();

				itGetBoom->setEnabled(false);
				lbGetBoomTimer->setVisible(true);
				lbGetBoom->setVisible(false);
			}

			//////////////////

			//Send life
			if (_sendLifeWaitTime <= 0)
			{
				//FULL TIME
				itSendLife->setEnabled(true);
				lbSendLifeTimer->setVisible(false);
				lbSendLife->setVisible(true);
			}
			else
			{
				itSendLife->selected();

				itSendLife->setEnabled(false);
				lbSendLifeTimer->setVisible(true);
				lbSendLife->setVisible(false);
			}
		}
	}
	else
	{
		CCSprite *avatar = (CCSprite*)cell->getChildByTag(2);
		avatar = CCSprite::create(strPhoto->getCString());

		CCLabelTTF *lbOrder = (CCLabelTTF*)cell->getChildByTag(3);
		lbOrder->setString(strOrder->getCString());

		CCLabelTTF *lbName = (CCLabelTTF*)cell->getChildByTag(4);
		lbName->setString(strName->getCString());

		CCLabelTTF *lbScore = (CCLabelTTF*)cell->getChildByTag(5);
		lbScore->setString(strScore->getCString());
	}

	return cell;
}

CCTableViewCell* ScoreScene::tableCellQuatangAtIndex( CCTableView *table, unsigned int idx )
{
	CCString* strName = CCString::create(G_DEFAULT_NAME);
	CCString* strPhoto = CCString::create("fb-profile.png");
	std::string strFriendId;
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EziFBIncomingRequest* request = (EziFBIncomingRequest*)m_arrRequests->objectAtIndex(idx);
	if (NULL != request)
	{
		EziFacebookFriend* fbFriend = request->getSender();
		strFriendId = std::string(fbFriend->getFBID());

		std::string sname = std::string(fbFriend->getName());
		if (sname.length() > 18) {
			sname = sname.substr(0, 15);
			sname.append("...");
		}			
		strName  = CCString::create(sname);

		CCLOG("1");
		
		//check if photopath is ok
		if (strlen(fbFriend->getPhotoPath()) > 1)
		{
			CCLOG("GET RIGHT PHOTOPATH");
			strPhoto = CCString::createWithFormat("%s", fbFriend->getPhotoPath());
		} 
		else
		{
			CCLOG("FOREACH TO GET PHOTOPATH");
			//foreach m_arrHighScores to get photopath
			CCObject* it = NULL;
			CCARRAY_FOREACH(m_arrHighScores, it)
			{
				EziFacebookFriend* fr = (EziFacebookFriend*)(it);

				if (NULL != fr && fr->getFBID() == strFriendId)
				{
					if (strlen(fr->getPhotoPath()) > 1)
					{
						CCLOG("FOREACH TO GET PHOTOPATH: --- OK");
						strPhoto = CCString::createWithFormat("%s", fr->getPhotoPath());
						fbFriend->setPhotoPath(fr->getPhotoPath());
						break;
					}
				}
			}
		}		

		CCLOG("2");
	}
	else
	{
		CCLOG("NULL REQUEST");
		return NULL;
	}

	CCTableViewCell *cell = table->cellAtIndex(idx);
	if (!cell) 
	{
		cell = new CustomTableViewCell();
		cell->autorelease();

		((CustomTableViewCell*)(cell))->m_request = request;


		CCLOG("3");
		CCSprite *sprite = CCSprite::create("table_cell_quatang.png");
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(0, 0));
		sprite->setTag(1);
		cell->addChild(sprite);

		CCLOG("4");
		CCSprite *avatar = CCSprite::create(strPhoto->getCString());
		avatar->setScale(0.78125);
		avatar->setPosition(ccp(50 + 30, m_sprCell->getContentSize().height/2));
		avatar->setTag(2);
		cell->addChild(avatar);

		CCLOG("5");
		CCLabelTTF *lbName = CCLabelTTF::create(strName->getCString(), "Roboto-Medium.ttf", 42);
		lbName->setFontFillColor(ccc3(0,0,0));
		lbName->setPosition(ccp(0.75f * G_FRIEND_AVATAR_SIZE + 50, m_sprCell->getContentSize().height/2));
		lbName->setAnchorPoint(ccp(0.0f, 0.5f));
		lbName->setTag(4);
		cell->addChild(lbName);

		//icon life 
		CCSprite* iconLife = CCSprite::create("oil.png");
		iconLife->setPosition(ccp(600, m_sprCell->getContentSize().height/2 + 15));
		cell->addChild(iconLife);

		//lable nhận
		CCLabelTTF* lbGetBoom = CCLabelTTF::create("Nhận", "Roboto-Medium.ttf", 28);
		lbGetBoom->setFontFillColor(ccc3(0, 0, 0));
		lbGetBoom->setAnchorPoint(ccp(0.5f, 0.75f));
		lbGetBoom->setPosition(ccp(600, m_sprCell->getContentSize().height/4));
		cell->addChild(lbGetBoom);
	}
	else
	{
		CCLOG("6");
		CCSprite *avatar = (CCSprite*)cell->getChildByTag(2);
		avatar = CCSprite::create(strPhoto->getCString());

		CCLOG("7");
		CCLabelTTF *lbName = (CCLabelTTF*)cell->getChildByTag(4);
		lbName->setString(strName->getCString());
	}

	return cell;
#else
	return NULL;
#endif
}




// Facebook //=========================================
