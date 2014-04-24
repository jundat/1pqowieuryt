#include "MenuScene.h"
#include "ScoreScene.h"
#include "cocos-ext.h"
#include "DataManager.h"
#include "CustomTableViewCell.h"
#include "MyMacro.h"
#include "AudioManager.h"
#include "Global.h"
#include "TextLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


#define 	BEGIN_TAG_LB_GET_BOOM			6000
#define 	BEGIN_TAG_LB_GET_BOOM_TIMER		5000
#define 	BEGIN_TAG_LB_SEND_LIFE			4000
#define 	BEGIN_TAG_LB_SEND_LIFE_TIMER	3000
#define		BEGIN_TAG_IT_SEND_LIFE			2000
#define 	BEGIN_TAG_IT_GET_BOOM_NOW		7000
#define 	BEGIN_TAG_IT_GET_BOOM 			1000


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

	m_lbInviteQuatang = NULL;
    m_isFirstTimeLogIn = false;


	CCSprite* bg = CCSprite::create("bg_stars.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg);
	
	CCSprite* top;
	string lang = DataManager::sharedDataManager()->GetLanguage();
	if (lang.compare("English") == 0)
	{
		top = CCSprite::create("en_score_top.png");
	} 
	else
	{
		top = CCSprite::create("vn_score_top.png");
	}
	top->setPosition(ccp(383, 1280-61));
	this->addChild(top);


	CCSprite* table_top = CCSprite::create("table_top.png");
	table_top->setPosition(ccp(400, 1280-202));
	this->addChild(table_top);

	CCSprite* table_border = CCSprite::create("table_border.png");
	table_border->setPosition(ccp(400, 1280-767));
	this->addChild(table_border);


	//USER Information //////////////////////////////////////////////////////////////////////////


	//avatar
	m_userAvatar = CCSprite::create("fb-profile.png");
	if(DataManager::sharedDataManager()->GetPhotoPath().length() > 0)
	{
		CCSprite* userPhoto = CCSprite::create(DataManager::sharedDataManager()->GetPhotoPath().c_str());
		userPhoto->setPosition(ccp(m_userAvatar->getContentSize().width/2, m_userAvatar->getContentSize().height/2));

		if (m_userAvatar)
		{
			m_userAvatar->addChild(userPhoto);
		}
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
	m_lbName->setColor(ccc3(0, 0, 0));
	m_lbName->setPosition(ccp(174, 1280-173));
	m_lbName->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbName);

	//score
	CCString* str = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetHighScore());
	m_lbScore = CCLabelTTF::create(str->getCString(), "Roboto-Medium.ttf", 48);
	m_lbScore->setColor(ccc3(0, 0, 0));
	m_lbScore->setPosition(ccp(174, 1280-239));
	m_lbScore->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbScore);

	//life
	m_sprLife = CCSprite::create("oil.png");
	m_sprLife->setScale(0.75f);
	m_sprLife->setPosition(ccp(609 + 50, 1280-164));
	this->addChild(m_sprLife);

	str = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetLastPlayerLife());
	m_lbLife = CCLabelTTF::create(str->getCString(), "Roboto-Medium.ttf", 48);
	m_lbLife->setColor(ccc3(0, 0, 0));
	m_lbLife->setPosition(ccp(650 + 50, 1280-164));
	m_lbLife->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbLife);
	
	//boom
	m_sprBoom = CCSprite::create("boomgift.png");
	m_sprBoom->setScale(0.75f);
	m_sprBoom->setPosition(ccp(609 + 50, 1280-245));
	this->addChild(m_sprBoom);

	str = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetBoom());
	m_lbBoom = CCLabelTTF::create(str->getCString(), "Roboto-Medium.ttf", 48);
	m_lbBoom->setColor(ccc3(0, 0, 0));
	m_lbBoom->setPosition(ccp(650 + 50, 1280-245));
	m_lbBoom->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbBoom);

	//Diamond

	m_sprDiamond = CCSprite::create("diamond.png");
	m_sprDiamond->setPosition(ccp(418 + 50, 1280-245));
	this->addChild(m_sprDiamond);

	str = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetDiamon());
	m_lbDiamond = CCLabelTTF::create(str->getCString(), "Roboto-Medium.ttf", 48);
	m_lbDiamond->setColor(ccc3(0, 0, 0));
	m_lbDiamond->setPosition(ccp(454 + 50, 1280-245));
	m_lbDiamond->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbDiamond);


	//Xep hang

	CCMenuItem* xephangOn = CCMenuItemImage::create("xephang1.png", NULL, NULL);
	CCMenuItem* xephangOff = CCMenuItemImage::create("xephang.png", NULL, NULL);
	m_itXephangToggle = CCMenuItemToggle::createWithTarget(this,  menu_selector(ScoreScene::itXephangCallback), xephangOn, xephangOff, NULL);
	m_itXephangToggle->setSelectedIndex(0);
	m_itXephangToggle->setPosition(ccp(202, 1280-351));

	CCLabelTTF* lbXepHang = CCLabelTTF::create(TXT("btn_leaderboard"), "Roboto-Medium.ttf", 48);
	lbXepHang->setPosition(ccp(260, 51));
	lbXepHang->setColor(ccBLACK);
	m_itXephangToggle->addChild(lbXepHang, 10);

	//Qua tang

	CCMenuItem* quatangOn = CCMenuItemImage::create("quatang1.png", NULL, NULL);
	CCMenuItem* quatangOff = CCMenuItemImage::create("quatang.png", NULL, NULL);
	m_itQuatangToggle = CCMenuItemToggle::createWithTarget(this,  menu_selector(ScoreScene::itQuatangCallback), quatangOn, quatangOff, NULL);
	m_itQuatangToggle->setSelectedIndex(1);
	m_itQuatangToggle->setPosition(ccp(600, 1280-351));

	CCLabelTTF* lbQuatang = CCLabelTTF::create(TXT("btn_gift"), "Roboto-Medium.ttf", 48);
	lbQuatang->setPosition(ccp(260, 51));
	lbQuatang->setColor(ccBLACK);
	m_itQuatangToggle->addChild(lbQuatang, 10);

	//
	CCMenuItemImage *backItem = CCMenuItemImage::create(
		"back.png",
		"back1.png",
		this,
		menu_selector(ScoreScene::itMenuCallback));
	backItem->setPosition(ccp(92, 1280-1201));


	//add more friends
	CCMenuItemImage* itAddFriend = CCMenuItemImage::create("add_friend.png", "add_friend_hover.png", this, menu_selector(ScoreScene::itAddFriendCallback));
	itAddFriend->setPosition(ccp(720, 1280-1198)); //382, 1280-1205));
	
	//Facebook button
	

	m_isLoggedIn = true;
	m_itFbLogInItem = CCMenuItemImage::create(
		"connect_facebook.png", 
		"connect_facebook.png", 
		this,  
		menu_selector(ScoreScene::itFbLogInCallback));
	m_itFbLogInItem->setPosition(ccp(400, 1280-805));

	m_lbInvite = CCLabelTTF::create(TXT("score_invite_to_facebook"), 
		"Roboto-Medium.ttf", 
		48, 
		CCSizeMake(600, 0), 
		kCCTextAlignmentCenter, 
		kCCVerticalTextAlignmentCenter);
	m_lbInvite->setColor(ccc3(0, 0, 0));
	m_lbInvite->setPosition(ccp(400, 1280-672)); //320
	this->addChild(m_lbInvite, 1); //samw menu


	m_menu = CCMenu::create(backItem, m_itFbLogInItem, m_itXephangToggle, m_itQuatangToggle, itAddFriend, NULL);
	m_menu->setPosition(CCPointZero);
	this->addChild(m_menu, 1);



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
	m_lbInviteQuatang = CCLabelTTF::create(TXT("score_no_gift"), 
		"Roboto-Medium.ttf", 
		48, 
		CCSizeMake(600, 0), 
		kCCTextAlignmentCenter, 
		kCCVerticalTextAlignmentCenter);

	m_lbInviteQuatang->setColor(ccc3(0, 0, 0));
	m_lbInviteQuatang->setPosition(ccp(400, 1280-672)); //320
	m_lbInviteQuatang->setVisible(false);
	this->addChild(m_lbInviteQuatang, 1);
	

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

	m_sprWaiting = CCSprite::create("loading.png");
	m_sprWaiting->setPosition(ccp(400, 1280-768));
	this->addChild(m_sprWaiting);

	m_sprWaiting->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, -360.0f)));


	m_lbLostConnection = CCLabelTTF::create(TXT("score_server_error"), G_FONT_NORMAL, 48);
	m_lbLostConnection->setColor(ccBLACK);
	m_lbLostConnection->setPosition(ccpSub(m_sprWaiting->getPosition(), ccp(0, 50)));
	this->addChild(m_lbLostConnection, m_sprWaiting->getZOrder());
	m_lbLostConnection->setVisible(false);


	//////////////////////////////////////////////////////////////////////////
	GameClientManager::sharedGameClientManager()->setDelegate(this);


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	EziSocialObject::sharedObject()->setFacebookDelegate(this);

	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		syncScore();

		//check incoming request
		EziSocialObject::sharedObject()->checkIncomingRequest();


		m_isLoggedIn = true;
		m_itFbLogInItem->setVisible(false);
		m_lbInvite->setVisible(false);

		//get avatar
		EziSocialObject::sharedObject()->getProfilePicForID(this, DataManager::sharedDataManager()->GetFbID().c_str(), // Profile ID of current user
			G_AVATAR_SIZE, G_AVATAR_SIZE, // Size of the image
			false // force download it from server
			);
        
        getHighScores();
	}
	else //logged out stated
	{
		m_isLoggedIn = false;

		m_tableXephang->setVisible(false);
		m_tableQuatang->setVisible(false);
		m_sprWaiting->setVisible(false);

		m_itFbLogInItem->setVisible(true);
		m_lbInvite->setVisible(true);
	}
#endif
	
	this->schedule(schedule_selector(ScoreScene::scheduleTimer), 1);
    
    CCLOG("ScoreScene::Init OK");
	return true;
}

void ScoreScene::itMenuCallback(CCObject* pSender)
{
    CCLOG("ScoreScene::itMenuCallback");
	PLAY_BUTTON_EFFECT;

	GameClientManager::sharedGameClientManager()->setDelegate(NULL);

	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void ScoreScene::itAddFriendCallback( CCObject* pSender )
{
    CCLOG("ScoreScene::itAddFriendCallback");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		PLAY_BUTTON_EFFECT;

		CCString* str = CCString::createWithFormat(TXT("score_invite_friend"), 
			DataManager::sharedDataManager()->GetFbFullName().c_str());
		
		//invite friends
		EziSocialObject::sharedObject()->sendRequestToFriends(
			EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE,
			str->getCString(), 
			NULL,
			NULL,
			TXT("game_name"));
	}
#endif
}

void ScoreScene::keyBackClicked()
{
	itMenuCallback(NULL);
}



void ScoreScene::itXephangCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;
	m_isXepHangView = true;
	refreshView();
}

void ScoreScene::itQuatangCallback( CCObject* pSender )
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
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
		m_sprLife->runAction(CCSequence::createWithTwoActions(
			CCScaleTo::create(0.2f, 1.0f),
			CCScaleTo::create(0.2f, 0.75f)
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
		return getTableCellXepHangAtIndex(table, idx);
	} 
	else
	{
		return getTableCellQuatangAtIndex(table, idx);
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
		if (m_isXepHangView == false && m_lbInviteQuatang != NULL)
		{
			if (m_tableQuatangSize <= 0)
			{
				m_lbInviteQuatang->setVisible(true);
			}
			else
			{
				m_lbInviteQuatang->setVisible(false);
			}
		}

		return m_tableQuatangSize;
	}	
}



void ScoreScene::itGetBoomCallback( CCObject* pSender )
{
	int tag = ((CCMenuItemImage*)pSender)->getTag();
	int idx = tag - BEGIN_TAG_IT_GET_BOOM;
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
			CCScaleTo::create(0.2f, 1.0f),
			CCScaleTo::create(0.2f, 0.75f)
			));
	}
	else
	{
		PLAY_OUT_PORP_EFFECT;
		
		m_sprBoom->runAction(CCSequence::createWithTwoActions(
			CCScaleTo::create(0.2f, 1.0f),
			CCScaleTo::create(0.2f, 0.75f)
			));

		CCMessageBox(CCString::createWithFormat(TXT("score_limit_lazer"), G_MAX_BOOM)->getCString(), TXT("score_popup_caption"));
	}
}

void ScoreScene::itGetBoomNowCallback( CCObject* pSender )
{
	int tag = ((CCMenuItemImage*)pSender)->getTag();
	int idx = tag - BEGIN_TAG_IT_GET_BOOM_NOW;
	CustomTableViewCell* cell = (CustomTableViewCell*)m_tableXephang->cellAtIndex(idx);
	std::string fbID = ((CustomTableViewCell*)cell)->fbID;

	//check if ok
	int numBoom = DataManager::sharedDataManager()->GetBoom();
	int diamond = DataManager::sharedDataManager()->GetDiamon();

	if (numBoom < G_MAX_BOOM && diamond >= G_DIAMON_TO_GET_BOOM_NOW) //eveything ok
	{
		PLAY_GET_BOMB_EFFECT;

		DataManager::sharedDataManager()->SetDiamon(diamond - G_DIAMON_TO_GET_BOOM_NOW);
		DataManager::sharedDataManager()->IncreaseBoom();
		DataManager::sharedDataManager()->SetIsJustGetBoomNowFriend(fbID.c_str(), true);

		m_sprDiamond->runAction(CCSequence::createWithTwoActions(
			CCScaleTo::create(0.2f, 1.25f),
			CCScaleTo::create(0.2f, 1.0f)
			));

		m_sprBoom->runAction(CCSequence::createWithTwoActions(
			CCScaleTo::create(0.2f, 1.0f),
			CCScaleTo::create(0.2f, 0.75f)
			));

		refreshUserDetail();

		//disable
		cell->m_itGetBoomNow->setVisible(false);
	}
	else
	{
		PLAY_OUT_PORP_EFFECT;

		if (numBoom >= G_MAX_BOOM)
		{
			m_sprBoom->runAction(CCSequence::createWithTwoActions(
				CCScaleTo::create(0.2f, 1.0f),
				CCScaleTo::create(0.2f, 0.75f)
			));

			CCMessageBox(CCString::createWithFormat(TXT("score_limit_lazer"), G_MAX_BOOM)->getCString(), TXT("score_popup_caption"));
		} 
		else if (diamond < G_DIAMON_TO_GET_BOOM_NOW)
		{
			//animation in diamond number
			//pham tan long

			m_sprDiamond->runAction(CCSequence::createWithTwoActions(
				CCScaleTo::create(0.2f, 1.25f),
				CCScaleTo::create(0.2f, 1.0f)
				));

			CCMessageBox(TXT("score_lack_diamond"), TXT("score_popup_caption"));
		}
	}
}

void ScoreScene::itSendLifeCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	int tag = ((CCMenuItemImage*)pSender)->getTag();
	int idx = tag - BEGIN_TAG_IT_SEND_LIFE;
	CustomTableViewCell* cell = (CustomTableViewCell*)m_tableXephang->cellAtIndex(idx);
	std::string fbID = cell->fbID;
	
	CCString* s = CCString::createWithFormat("%s", fbID.c_str());
	CCArray* arrFriends = new CCArray();
	arrFriends->addObject(s);

	CCDictionary *giftDictionary = CCDictionary::create();
	giftDictionary->setObject(CCString::create("1"), "LIFE");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	EziSocialObject::sharedObject()->isFacebookSessionActive();

	EziSocialObject::sharedObject()->sendRequestToFriends(
		EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT,
		TXT("score_send_life_msg"), 
		arrFriends,
		giftDictionary, 
		TXT("game_name"));
#endif

	m_friendCell = cell;
}



void ScoreScene::refreshView()
{
    CCLOG("ScoreScene::refreshView");
	if (m_isXepHangView) //left
	{
		m_itXephangToggle->setSelectedIndex(0);
		m_itQuatangToggle->setSelectedIndex(1);

		m_tableXephang->setVisible(true);
		m_tableQuatang->setVisible(false);

		m_lbInviteQuatang->setVisible(false);
	} 
	else //right
	{
		m_itXephangToggle->setSelectedIndex(1);
		m_itQuatangToggle->setSelectedIndex(0);

		m_tableXephang->setVisible(false);
		m_tableQuatang->setVisible(true);

		if (m_tableQuatangSize <= 0 && m_lbLostConnection->isVisible() == false)
		{
			m_lbInviteQuatang->setVisible(true);
		}
		else
		{
			m_lbInviteQuatang->setVisible(false);
		}
	}

	if (m_isLoggedIn == false)
	{
		m_tableXephang->setVisible(false);
		m_tableQuatang->setVisible(false);

		if (m_lbInviteQuatang != NULL) m_lbInviteQuatang->setVisible(false);		
	}
}

void ScoreScene::refreshUserDetail()
{
    CCLOG("ScoreScene::refreshUserDetail");
	CCString* str = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetLastPlayerLife());
	m_lbLife->setString(str->getCString());

	str = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetBoom());
	m_lbBoom->setString(str->getCString());

	str = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetDiamon());
	m_lbDiamond->setString(str->getCString());

}

void ScoreScene::scheduleTimer( float dt )
{
	CCLOG("Schedule timer");
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
				cell->m_lbGetBoom != NULL &&
				cell->m_itGetBoomNow != NULL)
			{
				time_t lastTime = mktime(cell->m_lastTimeGetBoom);
				time_t curTime = time(NULL);
				int waitTime = 24 * 60 * 60 - (int)difftime(curTime, lastTime);

				if (waitTime <= 0) //free
				{
					//FULL TIME
					cell->m_itGetBoom->setEnabled(true);
					cell->m_itGetBoomNow->setVisible(false);
					cell->m_lbGetBoomTimer->setVisible(false);
					cell->m_lbGetBoom->setVisible(true);

					DataManager::sharedDataManager()->SetIsJustGetBoomNowFriend(cell->fbID.c_str(), false);
				}
				else //not free
				{
					cell->m_lbGetBoomTimer->setVisible(true);
					cell->m_lbGetBoom->setVisible(false);

					cell->m_itGetBoom->setEnabled(false);
					
					bool isJustGetBoomNow = DataManager::sharedDataManager()->GetIsJustGetBoomNowFriend(cell->fbID.c_str());
					if (isJustGetBoomNow)
					{
						cell->m_itGetBoomNow->setVisible(false);
					} 
					else
					{
						cell->m_itGetBoomNow->setVisible(true);
					}
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

				if (waitTime <= 0) //free
				{
					//FULL TIME
					cell->m_itSendLife->setEnabled(true);
					cell->m_lbSendLifeTimer->setVisible(false);
					cell->m_lbSendLife->setVisible(true);
				}
				else //not free
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

//MY FUNCTION

void ScoreScene::itFbLogInCallback( CCObject* pSender )
{
    CCLOG("ScoreScene::itFbLogInCallback");
	PLAY_BUTTON_EFFECT;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	m_sprWaiting->setVisible(true);
	bool needPublicPermission = true;
	EziSocialObject::sharedObject()->performLoginUsingFacebook(needPublicPermission); // Pass true if you need publish permission also
#endif
    
    //show wait dialog to ignore all touch
    this->showWaitDialog(TXT("wait_connect_server"));
}

void ScoreScene::itFbLogOutCallback( CCObject* pSender )
{
    CCLOG("ScoreScene::itFbLogOutCallback");
	PLAY_BUTTON_EFFECT;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	EziSocialObject::sharedObject()->perfromLogoutFromFacebook();
#endif
}



void ScoreScene::submitScore()
{
	CCLOG("callSubmitScore");
	GameClientManager::sharedGameClientManager()->sendScore(
		string(G_APP_ID), 
		DataManager::sharedDataManager()->GetFbID(), 
		DataManager::sharedDataManager()->GetHighScore());
}

void ScoreScene::syncScore()
{
    CCLOG("ScoreScene::syncScore");
    this->showWaitDialog(TXT("wait_connect_server"));
    
	//get score from server
	GameClientManager::sharedGameClientManager()->getScore(string(G_APP_ID), DataManager::sharedDataManager()->GetFbID());
}

void ScoreScene::getHighScores()
{
    CCLOG("ScoreScene::getHighScores");
    
    this->showWaitDialog(TXT("wait_connect_server"));
    
	GameClientManager::sharedGameClientManager()->getFriendList(string(G_APP_ID), DataManager::sharedDataManager()->GetFbID());
}


//get highscore list completed
void ScoreScene::onGetFriendListCompleted(bool isSuccess, CCArray* arrFriends)
{
	CCLOG("onGetFriendListCompleted");
	if (isSuccess == false) 
	{
		CCLOG("FAILED");
		return;
	}

	if (m_arrHighScores != NULL)
	{
		m_arrHighScores->release();
		m_arrHighScores = NULL;
	}

	m_arrHighScores = CCArray::createWithArray(arrFriends);
	m_arrHighScores->retain();

	int score = DataManager::sharedDataManager()->GetHighScore();
	//add current user to list
	FacebookAccount* curUser = new FacebookAccount(
		DataManager::sharedDataManager()->GetFbID(),
		DataManager::sharedDataManager()->GetFbFullName(),
		DataManager::sharedDataManager()->GetFbEmail(),
		score);
	m_arrHighScores->addObject(curUser);

	m_tableXepHangSize = m_arrHighScores->count();
	std::string myProfileID = DataManager::sharedDataManager()->GetFbID();

	GameClientManager::SortFriendScore(m_arrHighScores);

	//////////////////////////////////////////////////////////////////////////
	//save higher friend than you
	CCArray* arrHigherFriends = CCArray::create();

	for (int i = 0; i < m_tableXepHangSize; ++i)
	{
		FacebookAccount* acc = dynamic_cast<FacebookAccount*>( m_arrHighScores->objectAtIndex(i));
		if(acc->m_score > score)
		{
			arrHigherFriends->addObject(acc);
		}
	}

	DataManager::sharedDataManager()->SetHigherFriends(arrHigherFriends);

	//////////////////////////////////////////////////////////////////////////


	for (int i = 0; i < m_tableXepHangSize; ++i)
	{
		FacebookAccount* fbFriend = (FacebookAccount*) (m_arrHighScores->objectAtIndex(i));
		if (NULL != fbFriend)
		{
			std::string profileID = fbFriend->m_fbId;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			EziSocialObject::sharedObject()->getProfilePicForID(this, profileID.c_str(), // Profile ID of current user
				G_AVATAR_SIZE, G_AVATAR_SIZE, // Size of the image
				false // force download it from server
				);
#endif
		}
	}

	m_sprWaiting->setVisible(false);
	m_tableXephang->reloadData();
	m_tableQuatang->reloadData();
    
    this->closeWaitDialog();
}

void ScoreScene::onGetScoreCompleted( bool isSuccess, int score, std::string time )
{
	CCLOG("onGetScoreCompleted");

	if (isSuccess)
	{
		int deviceScore = DataManager::sharedDataManager()->GetHighScore();
        score = (score <= 0) ? 0 : score;
        
        if (m_isFirstTimeLogIn == true) {
            CCLOG("FIRST TIME LOG IN");
        } else {
            CCLOG("NO---FIRST TIME LOG IN");
        }
        
        if (m_isFirstTimeLogIn == true || deviceScore <= score) {
            CCLOG("SERVER -> LOCAL");
            
            //USE ANOTHER FUNCTION TO SET SCORE
            DataManager::sharedDataManager()->SetHighScoreNotCheck(score);
            
			//score
			CCString* str = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetHighScore());
			m_lbScore->setString(str->getCString());
			
			PLAY_GET_BOMB_EFFECT;
            
			//animation
			m_lbScore->runAction(CCSequence::createWithTwoActions(
                                                                  CCScaleTo::create(0.2f, 1.25f),
                                                                  CCScaleTo::create(0.2f, 1.0f)
                                                                  ));
        } else {
            CCLOG("LOCAL -> SERVER");
            submitScore();
        }
	} 
	else
	{
		CCLOG("FAILED TO GET SCORE");

		this->m_sprWaiting->setVisible(false);
		m_lbLostConnection->setVisible(true);
		m_lbInviteQuatang->setVisible(false);
        
        this->closeWaitDialog();
	}	
}




//END MY FUNCTION




void ScoreScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
	CCLOG("fbUserPhotoCallback");

	std::string sid = std::string(fbID);

	if ((strcmp(userPhotoPath, "") != 0))
	{
		CCLOG("fbUserPhotoCallback: userPhotoPath != NULL");
		if(sid == DataManager::sharedDataManager()->GetFbID()) //////////////////////////////////////////////////
		{
			CCLOG("fbUserPhotoCallback: this user");
			//CCLOG("Gotten avatar for user");

			//save 
			DataManager::sharedDataManager()->SetPhotoPath(userPhotoPath);

			CCSprite* userPhoto = CCSprite::create(userPhotoPath);
			userPhoto->setPosition(ccp(m_userAvatar->getContentSize().width/2, m_userAvatar->getContentSize().height/2));
			
			if (m_userAvatar)
			{
				m_userAvatar->addChild(userPhoto);
			}
			else
			{
				CCLOG("ERROR: m_userAvatar == NULL");
			}
		}
		
		if (m_arrHighScores != NULL) ////////////////////////////////////////////////////////////////
		{
			CCLOG("fbUserPhotoCallback: friends");

			CCObject* it;
			CCARRAY_FOREACH(m_arrHighScores, it)
			{
				FacebookAccount* fbFriend = (FacebookAccount*) (it);
				if (NULL != fbFriend)
				{
					//CCLOG("%s => %s", sid.c_str(), userPhotoPath);
					if(fbFriend->m_fbId == sid)
					{
						fbFriend->m_photoPath = userPhotoPath;
						//CCLOG("Set photo path ok --- ");
					}
				}
			}

			refreshView();
			m_tableXephang->reloadData();
			m_tableQuatang->reloadData();
		}
	}
}

void ScoreScene::fbSendRequestCallback( int responseCode, const char* responseMessage, cocos2d::CCArray* friendsGotRequests )
{
	CCLOG("fbSendRequestCallback");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (EziSocialWrapperNS::RESPONSE_CODE::FB_REQUEST_SENT == responseCode)
	{
		CCLOG("fbSendRequestCallback: FB_REQUEST_SENT");
		
		PLAY_GET_BOMB_EFFECT;

		if (m_friendCell != NULL) //send life
		{
			CCLOG("fbSendRequestCallback: FB_REQUEST_SENT: m_friendCell != NULL");

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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
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

CCTableViewCell* ScoreScene::getTableCellXepHangAtIndex( CCTableView *table, unsigned int idx )
{
	//CCLOG("getTableCellXepHangAtIndex: %d", idx);
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

	if(m_arrHighScores == NULL)
	{
		CCLOG("m_arrHighScores == NULL");
	}
	else
	{
		FacebookAccount* fbFriend = (FacebookAccount*) (m_arrHighScores->objectAtIndex(idx));
		
		strScore = CCString::createWithFormat("%d", fbFriend->m_score);

		std::string sname = std::string(fbFriend->m_fbName);
		if (sname.length() > 18) {
			sname = sname.substr(0, 15);
			sname.append("...");
		}			
		strName  = CCString::create(sname);

		strFriendId = std::string(fbFriend->m_fbId);

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


		if (fbFriend->m_photoPath.length() > 1)
		{
			strPhoto  = CCString::create(fbFriend->m_photoPath);
		}

		if(fbFriend->m_fbId == DataManager::sharedDataManager()->GetFbID())
		{
			isMyScore = true;
		}
	}

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

		CCSprite *defaultAvatar = CCSprite::create("fb-profile.png");
		defaultAvatar->setScale((float)G_FRIEND_AVATAR_SIZE/(float)G_AVATAR_SIZE);
		defaultAvatar->setPosition(ccp(140, m_sprCell->getContentSize().height/2));
		defaultAvatar->setTag(2);
		cell->addChild(defaultAvatar);

		CCSprite *avatar = CCSprite::create(strPhoto->getCString());
		avatar->setPosition(ccp(defaultAvatar->getContentSize().width/2, defaultAvatar->getContentSize().height/2));
		avatar->setTag(2);
		defaultAvatar->addChild(avatar);
		
		CCLabelTTF *lbOrder;
		if (idx < 3)
		{
			lbOrder = CCLabelTTF::create(strOrder->getCString(), "Villa.ttf", 42);
		} 
		else
		{
			lbOrder = CCLabelTTF::create(strOrder->getCString(), "Roboto-Medium.ttf", 42);
		}

		lbOrder->setColor(ccc3(0, 0, 0));
		lbOrder->setHorizontalAlignment(kCCTextAlignmentLeft); //cocos2d::CCTextAlignment::
		lbOrder->setPosition(ccp(30, m_sprCell->getContentSize().height/2));
		lbOrder->setTag(3);
		lbOrder->setAnchorPoint(ccp(0.0f, 0.5f));
		cell->addChild(lbOrder);

		//Jundat TPham..
		//21

		CCLabelTTF *lbName = CCLabelTTF::create(strName->getCString(), "Roboto-Medium.ttf", 42);
		lbName->setColor(ccc3(0,0,0));
		lbName->setPosition(ccp(210, m_sprCell->getContentSize().height * 3/4));
		lbName->setAnchorPoint(ccp(0.0f, 0.5f));
		lbName->setTag(4);
		cell->addChild(lbName);

		CCLabelTTF *lbScore = CCLabelTTF::create(strScore->getCString(), "Roboto-Medium.ttf", 42);
		lbScore->setColor(ccc3(0, 0, 0));
		lbScore->setPosition(ccp(210, m_sprCell->getContentSize().height * 3/8));
		lbScore->setAnchorPoint(ccp(0.0f, 0.5f));
		lbScore->setTag(5);
		cell->addChild(lbScore);

		if (isMyScore == false)
		{
			CCMenuItemImage* itGetBoom = CCMenuItemImage::create("boom_free.png", "boom_disable.png", "boom_disable.png", this, menu_selector(ScoreScene::itGetBoomCallback));
			itGetBoom->setPosition(ccp(605, m_sprCell->getContentSize().height/2 + 20));
			itGetBoom->setTag(BEGIN_TAG_IT_GET_BOOM + idx); //1000
			((CustomTableViewCell*)cell)->m_itGetBoom = itGetBoom;

			CCMenuItemImage *itGetBoomNow = CCMenuItemImage::create("boom_not_free.png", "boom_disable.png", "boom_disable.png", this, menu_selector(ScoreScene::itGetBoomNowCallback));
			itGetBoomNow->setPosition(ccp(605, m_sprCell->getContentSize().height/2 + 20));
			itGetBoomNow->setTag(BEGIN_TAG_IT_GET_BOOM_NOW + idx); //7000			
			((CustomTableViewCell*)cell)->m_itGetBoomNow = itGetBoomNow;

			CCMenuItemImage* itSendLife = CCMenuItemImage::create("oil.png", "oil_blur.png", "oil_blur.png", this, menu_selector(ScoreScene::itSendLifeCallback));
			itSendLife->setPosition(ccp(725, m_sprCell->getContentSize().height/2 + 15));
			itSendLife->setTag(BEGIN_TAG_IT_SEND_LIFE + idx); //2000
			((CustomTableViewCell*)cell)->m_itSendLife = itSendLife;

			CCMenu* cell_menu = CCMenu::create(itGetBoom, itSendLife, itGetBoomNow, NULL);
			cell_menu->setPosition(CCPointZero);
			cell->addChild(cell_menu);


			//////////////////////////////////////////////////////////////////////////


			//Send life
			CCLabelTTF* lbSendLifeTimer = CCLabelTTF::create(strSendLifeTimer->getCString(), "Roboto-Medium.ttf", 28); //32
			lbSendLifeTimer->setColor(ccc3(0, 0, 0));
			lbSendLifeTimer->setAnchorPoint(ccp(0.5f, 0.75f));
			lbSendLifeTimer->setPosition(ccp(725, m_sprCell->getContentSize().height/4));
			lbSendLifeTimer->setTag(BEGIN_TAG_LB_SEND_LIFE_TIMER + idx); //3000
			cell->addChild(lbSendLifeTimer);
			((CustomTableViewCell*)cell)->m_lbSendLifeTimer = lbSendLifeTimer;

			CCLabelTTF* lbSendLife = CCLabelTTF::create(TXT("score_send_life"), "Roboto-Medium.ttf", 28);
			lbSendLife->setColor(ccc3(0, 0, 0));
			lbSendLife->setAnchorPoint(ccp(0.5f, 0.75f));
			lbSendLife->setPosition(ccp(725, m_sprCell->getContentSize().height/4));
			lbSendLife->setTag(BEGIN_TAG_LB_SEND_LIFE + idx); //4000
			cell->addChild(lbSendLife);
			((CustomTableViewCell*)cell)->m_lbSendLife = lbSendLife;


			//////////////


			//Get boom
			CCLabelTTF* lbGetBoomTimer = CCLabelTTF::create(strGetBoomTimer->getCString(), "Roboto-Medium.ttf", 28); //32
			lbGetBoomTimer->setColor(ccc3(0, 0, 0));
			lbGetBoomTimer->setAnchorPoint(ccp(0.5f, 0.75f));
			lbGetBoomTimer->setPosition(ccp(600, m_sprCell->getContentSize().height/4));
			lbGetBoomTimer->setTag(BEGIN_TAG_LB_GET_BOOM_TIMER + idx); //5000
			cell->addChild(lbGetBoomTimer);
			((CustomTableViewCell*)cell)->m_lbGetBoomTimer = lbGetBoomTimer;

			CCLabelTTF* lbGetBoom = CCLabelTTF::create(TXT("score_get"), "Roboto-Medium.ttf", 28);
			lbGetBoom->setColor(ccc3(0, 0, 0));
			lbGetBoom->setAnchorPoint(ccp(0.5f, 0.75f));
			lbGetBoom->setPosition(ccp(600, m_sprCell->getContentSize().height/4 - 5));
			lbGetBoom->setTag(BEGIN_TAG_LB_GET_BOOM + idx); //6000
			cell->addChild(lbGetBoom);
			((CustomTableViewCell*)cell)->m_lbGetBoom = lbGetBoom;


			//////////////////////////////////////////////////////////////////////////

			bool isJustGetBoomNow = DataManager::sharedDataManager()->GetIsJustGetBoomNowFriend(strFriendId.c_str());

			//Get boom
			if (_getBoomWaitTime <= 0) //free
			{
				//FULL TIME
				lbGetBoomTimer->setVisible(false);
				lbGetBoom->setVisible(true);
				itGetBoom->setEnabled(true);
				itGetBoomNow->setVisible(false);
			}
			else //not free
			{
				lbGetBoomTimer->setVisible(true);
				lbGetBoom->setVisible(false);
				itGetBoom->setEnabled(false);

				if (isJustGetBoomNow)
				{
					itGetBoomNow->setVisible(false);
				}
				else
				{
					itGetBoomNow->setVisible(true);
				}				
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
		CCSprite *defaultAvatar = (CCSprite*)cell->getChildByTag(2);
		CCSprite *avatar = (CCSprite*)defaultAvatar->getChildByTag(2);
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

CCTableViewCell* ScoreScene::getTableCellQuatangAtIndex( CCTableView *table, unsigned int idx )
{
	//CCLOG("getTableCellQuatangAtIndex: %d", idx);
	CCString* strName = CCString::create(G_DEFAULT_NAME);
	CCString* strPhoto = CCString::create("fb-profile.png");
	std::string strFriendId;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
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
				FacebookAccount* fr = (FacebookAccount*) (it);

				if (NULL != fr && fr->m_fbId == strFriendId)
				{
					if (fr->m_photoPath.length() > 1)
					{
						CCLOG("FOREACH TO GET PHOTOPATH: --- OK");
						strPhoto = CCString::create(fr->m_photoPath);
						fbFriend->setPhotoPath(fr->m_photoPath.c_str());
						break;
					}
				}
			}
		}
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


		CCSprite *sprite = CCSprite::create("table_cell_quatang.png");
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(0, 0));
		sprite->setTag(1);
		cell->addChild(sprite);


		CCSprite *defaultAvatar = CCSprite::create("fb-profile.png");
		defaultAvatar->setPosition(ccp(75, m_sprCell->getContentSize().height/2));
		defaultAvatar->setScale((float)G_FRIEND_AVATAR_SIZE/(float)G_AVATAR_SIZE);
		defaultAvatar->setTag(2);
		cell->addChild(defaultAvatar);

		CCSprite *avatar = CCSprite::create(strPhoto->getCString());
		avatar->setPosition(ccp(defaultAvatar->getContentSize().width/2, defaultAvatar->getContentSize().height/2));
		avatar->setTag(2);
		defaultAvatar->addChild(avatar);


		CCLabelTTF *lbName = CCLabelTTF::create(strName->getCString(), "Roboto-Medium.ttf", 42);
		lbName->setColor(ccc3(0,0,0));
		lbName->setPosition(ccp(0.75f * G_FRIEND_AVATAR_SIZE + 60, m_sprCell->getContentSize().height/2));
		lbName->setAnchorPoint(ccp(0.0f, 0.5f));
		lbName->setTag(4);
		cell->addChild(lbName);

		//icon life 
		CCSprite* iconLife = CCSprite::create("oil.png");
		iconLife->setPosition(ccp(600, m_sprCell->getContentSize().height/2 + 15));
		cell->addChild(iconLife);

		//lable nhận
		CCLabelTTF* lbGetBoom = CCLabelTTF::create(TXT("score_get"), "Roboto-Medium.ttf", 28);
		lbGetBoom->setColor(ccc3(0, 0, 0));
		lbGetBoom->setAnchorPoint(ccp(0.5f, 0.75f));
		lbGetBoom->setPosition(ccp(600, m_sprCell->getContentSize().height/4));
		cell->addChild(lbGetBoom);
	}
	else
	{
		CCSprite *defaultAvatar = (CCSprite*)cell->getChildByTag(2);
		CCSprite *avatar = (CCSprite*)defaultAvatar->getChildByTag(2);
		avatar = CCSprite::create(strPhoto->getCString());

		CCLabelTTF *lbName = (CCLabelTTF*)cell->getChildByTag(4);
		lbName->setString(strName->getCString());
	}

	return cell;
#else
	return NULL;
#endif
}


// Facebook //=========================================


void ScoreScene::showWaitDialog(string title)
{
    CCLOG("ScoreScene::showWaitDialog");
    m_menu->setEnabled(false);
    this->setTouchEnabled(false);
    this->setKeypadEnabled(false);
    
    if (m_waitDialog != NULL) {
        m_waitDialog->m_refCount++;
        
    } else {
        m_waitDialog = WaitDialog::create();
        m_waitDialog->m_refCount = 1;
        m_waitDialog->setTitle(title);
        
        this->addChild(m_waitDialog, 100); // =1
    }
}

void ScoreScene::closeWaitDialog()
{
    CCLOG("ScoreScene::closeWaitDialog");
    if (m_waitDialog != NULL) {
        m_waitDialog->m_refCount--;
        
        if (m_waitDialog->m_refCount <= 1) {
            this->removeChild(m_waitDialog);
            m_waitDialog = NULL;
            
            m_menu->setEnabled(true);
            this->setTouchEnabled(true);
            this->setKeypadEnabled(true);
        }
    }
}



