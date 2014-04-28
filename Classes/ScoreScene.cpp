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

	m_lbInviteQuatang = NULL;
    m_isFirstTimeLogIn = false;
    m_waitDialog = NULL;


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

    string sfbid = DataManager::sharedDataManager()->GetFbID();
    if (sfbid.compare("NULL") != 0)
	//if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		syncScore();


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
	
    string sfbid = DataManager::sharedDataManager()->GetFbID();
    if (sfbid.compare("NULL") != 0)
    //if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
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
//Need
//DEBUG
//Remove Item, then remove setdeadtime code here
void ScoreScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	CCLOG("Cell at index: %i", cell->getIdx());

	if (table == m_tableQuatang) //Xep hang
	{
        CustomTableViewCell* customCell = (CustomTableViewCell*) cell;

		int curLife = DataManager::sharedDataManager()->GetLastPlayerLife();
		CCLOG("CURRENT LIFE = %d", curLife);
		
		if (curLife < G_MAX_PLAYER_LIFE)
		{
            
            //
            //remove item
            //
            
            GameClientManager::sharedGameClientManager()->removeItem(DataManager::sharedDataManager()->GetFbID(),
                                                                     customCell->m_gift->m_senderId, customCell->m_gift->m_time, customCell->m_gift->m_itemId);
            
            PLAY_BUTTON_EFFECT;
		} 
		else
		{
			PLAY_OUT_PORP_EFFECT;
            
            CCString* s = CCString::createWithFormat(TXT("score_limit_life"), G_MAX_PLAYER_LIFE);
            CCMessageBox(s->getCString(), TXT("menu_thank_caption"));
		}
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
	else if (table == m_tableQuatang) //Qua tang
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
    
    return 0;
}


//
//get laze free
//
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
        GameClientManager::sharedGameClientManager()->getLazeFree(DataManager::sharedDataManager()->GetFbID(), fbID);
		
        //show wait sprite
        //disable all item to wait
        cell->m_sprWait->setVisible(true);
        cell->m_itGetBoom->setEnabled(false);
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
    //buyItem
    //fbId
    //friendId
    //itemId = "laze"
    //count = 1
    
    
	int tag = ((CCMenuItemImage*)pSender)->getTag();
	int idx = tag - BEGIN_TAG_IT_GET_BOOM_NOW;
	CustomTableViewCell* cell = (CustomTableViewCell*)m_tableXephang->cellAtIndex(idx);
	std::string fbID = ((CustomTableViewCell*)cell)->fbID;

	//check if ok
	int numBoom = DataManager::sharedDataManager()->GetBoom();
	int diamond = DataManager::sharedDataManager()->GetDiamon();

	if (numBoom < G_MAX_BOOM && diamond >= G_DIAMON_TO_GET_BOOM_NOW) //eveything ok
	{
        GameClientManager::sharedGameClientManager()->buyItem(DataManager::sharedDataManager()->GetFbID(), "laze", 1, fbID);
		
        //show wait sprite
        //disable all item to wait
        cell->m_sprWait->setVisible(true);
        cell->m_itGetBoom->setEnabled(false);
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
	
    
    ///
    //
    
    string myfbid = DataManager::sharedDataManager()->GetFbID();
    GameClientManager::sharedGameClientManager()->sendItem(myfbid, fbID, "life", 1);
    
    //show wait sprite
    //disable all item to wait
    cell->m_sprWaitSendLife->setVisible(true);
    cell->m_itSendLife->setEnabled(false);
    
    //
    ///
    /*
	CCString* s = CCString::createWithFormat("%s", fbID.c_str());
	CCArray* arrFriends = new CCArray();
	arrFriends->addObject(s);

	CCDictionary *giftDictionary = CCDictionary::create();
	giftDictionary->setObject(CCString::create("1"), "LIFE");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	
     string sfbid = DataManager::sharedDataManager()->GetFbID();
     if (sfbid.compare("NULL") != 0)
     //EziSocialObject::sharedObject()->isFacebookSessionActive();

	EziSocialObject::sharedObject()->sendRequestToFriends(
		EziSocialWrapperNS::FB_REQUEST::REQUEST_GIFT,
		TXT("score_send_life_msg"), 
		arrFriends,
		giftDictionary, 
		TXT("game_name"));
#endif
     */
    
	//m_sendLifeCell = cell;
}



void ScoreScene::onGetLazeFreeCompleted(bool isSuccess, std::string friendId)
{
    CustomTableViewCell *cell;

    for (int i = 0; i < m_tableXepHangSize; i++) {
        cell = (CustomTableViewCell*) m_tableXephang->cellAtIndex(i);
        if (cell->fbID.compare(friendId) == 0) {
            break;
        }
    }
    
    if (isSuccess) {
        PLAY_GET_BOMB_EFFECT;
		DataManager::sharedDataManager()->IncreaseBoom();
        
		//show clock
		cell->m_lbGetBoom->setVisible(false);
		cell->m_lbGetBoomTimer->setVisible(true);
		cell->m_lastTimeGetBoom = static_cast<long int>(time(NULL));
        
        
		refreshUserDetail();
        
		//animation
		m_sprBoom->runAction(CCSequence::createWithTwoActions(
              CCScaleTo::create(0.2f, 1.0f),
              CCScaleTo::create(0.2f, 0.75f)
              ));
    } else {
        CCMessageBox(TXT("get_laze_free_error"), TXT("error_caption"));
    }
    
    cell->m_itGetBoom->setEnabled(true);
    cell->m_itGetBoom->setVisible(true);
    cell->m_sprWait->setVisible(false);
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
				cell->m_lbGetBoom != NULL &&
				cell->m_itGetBoomNow != NULL)
			{
                //senconds
                long currentTime = static_cast<long int>(time(NULL));
                long waitTime = 24 * 60 * 60 - (currentTime - cell->m_lastTimeGetBoom);

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
                
                MY_TIME getLazeWaitTime = CONVERT_SECOND_TO_TIME(waitTime);
				CCString* str = CCString::createWithFormat("%d:%d:%d", getLazeWaitTime.hour, getLazeWaitTime.min, getLazeWaitTime.sec);

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

                //senconds
                long currentTime = static_cast<long int>(time(NULL));
                long waitTime = 24 * 60 * 60 - (currentTime - cell->m_lastTimeSendLife);

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

                MY_TIME sendLifeWaitTime = CONVERT_SECOND_TO_TIME(waitTime);
				CCString* str = CCString::createWithFormat("%d:%d:%d", sendLifeWaitTime.hour, sendLifeWaitTime.min, sendLifeWaitTime.sec);
                
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
		DataManager::sharedDataManager()->GetFbID(), 
		DataManager::sharedDataManager()->GetHighScore());
}

void ScoreScene::syncScore()
{
    CCLOG("ScoreScene::syncScore");
    this->showWaitDialog(TXT("wait_connect_server"));
    
	//get score from server
	GameClientManager::sharedGameClientManager()->getScore(DataManager::sharedDataManager()->GetFbID());
}

void ScoreScene::getHighScores()
{
    CCLOG("ScoreScene::getHighScores");
    
    this->showWaitDialog(TXT("wait_connect_server"));
    
	GameClientManager::sharedGameClientManager()->getFriendList( DataManager::sharedDataManager()->GetFbID());
}


//get highscore list completed
void ScoreScene::onGetFriendListCompleted(bool isSuccess, CCArray* arrFriends)
{
	CCLOG("onGetFriendListCompleted");
	if (isSuccess == false || arrFriends == NULL)
	{
		CCLOG("FAILED TO GET FRIEND LIST");
		return;
	}

    //
    //SYNC TIME SERVER-CLIENT -----------
    //
    
	if (m_arrHighScores != NULL)
	{
		m_arrHighScores->release();
		m_arrHighScores = NULL;
	}

	m_arrHighScores = CCArray::createWithArray(arrFriends);
	m_arrHighScores->retain();

	int score = DataManager::sharedDataManager()->GetHighScore();
	//add current user to list
    
    //(string _fbId, string _fbName, string _email, int _score, int _coin, long timeGetLaze, long timeSendLife)
    
	FacebookAccount* curUser = new FacebookAccount(
		DataManager::sharedDataManager()->GetFbID(),
		DataManager::sharedDataManager()->GetFbFullName(),
		DataManager::sharedDataManager()->GetFbEmail(),
		score, -1, -1, -1);
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
            
            
            //DEBUG
            CCLOG("TIME LAZE: %s, %ld", profileID.c_str(), fbFriend->m_timeGetLaze);
            


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
    
    CCLOG("Close dialog : onGetFriendListCompleted");
    this->closeWaitDialog();
    
    
    //check inbox
    this->getInbox();

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
        
        CCLOG("Close dialog : onGetScoreCompleted");
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
					if(fbFriend->m_fbId == sid)
					{
						fbFriend->m_photoPath = userPhotoPath;
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
	}
	else
	{
		CCLOG("fbSendRequestCallback: FB_REQUEST_SENT: m_friendCell == NULL");
		PLAY_OUT_PORP_EFFECT;
		CCLOG("Request sent failed");
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
    long _lastTimeGetBoom;
	int _getBoomWaitTime = 0;
	CCString* strGetBoomTimer = CCString::create("00:00:00");

	//send life
	long _lastTimeSendLife;
	int _sendLifeWaitTime = 0;
	CCString* strSendLifeTimer = CCString::create("00:00:00");

    FacebookAccount* fbFriend = NULL;
    
	if(m_arrHighScores == NULL)
	{
		CCLOG("m_arrHighScores == NULL");
	}
	else
	{
		fbFriend = (FacebookAccount*) (m_arrHighScores->objectAtIndex(idx));
		
		strScore = CCString::createWithFormat("%d", fbFriend->m_score);

		std::string sname = std::string(fbFriend->m_fbName);
		if (sname.length() > 18) {
			sname = sname.substr(0, 15);
			sname.append("...");
		}			
		strName  = CCString::create(sname);

		strFriendId = std::string(fbFriend->m_fbId);

		//get boom
        _lastTimeGetBoom = fbFriend->m_timeGetLaze;
		      
        _getBoomWaitTime = 24 * 60 * 60 - (static_cast<long int> (time(NULL)) - _lastTimeGetBoom);
        _getBoomWaitTime = (_getBoomWaitTime > 0) ? _getBoomWaitTime : 0;

        MY_TIME getLazeWaitTime = CONVERT_SECOND_TO_TIME(_getBoomWaitTime);
		strGetBoomTimer = CCString::createWithFormat("%d:%d:%d", getLazeWaitTime.hour, getLazeWaitTime.min, getLazeWaitTime.sec);



        //
		//send life //////////////////////////////////////
		//
        
        _lastTimeSendLife = fbFriend->m_timeSendLife;
		      
		_sendLifeWaitTime = 24 * 60 * 60  - (static_cast<long int> (time(NULL)) - _lastTimeSendLife);
		_sendLifeWaitTime = (_sendLifeWaitTime > 0) ? _sendLifeWaitTime : 0;
        
        MY_TIME sendLifeWaitTime = CONVERT_SECOND_TO_TIME(_sendLifeWaitTime);
		strSendLifeTimer = CCString::createWithFormat("%d:%d:%d", sendLifeWaitTime.hour, sendLifeWaitTime.min, sendLifeWaitTime.sec);
		
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
        ((CustomTableViewCell*)cell)->m_fbFriend = fbFriend;


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

            //
            //waiting... get laze
            //
            CCSprite* sprWait = CCSprite::create("wait.png");
            sprWait->setPosition( ccpAdd( itGetBoom->getPosition(), ccp(-7, -7)));
            sprWait->setScale(0.65f);
            sprWait->setVisible(false);
            sprWait->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, -360.0f)));
            cell->addChild(sprWait);
            ((CustomTableViewCell*)cell)->m_sprWait = sprWait;
            

            //
            //waiting... send life
            //
            CCSprite* sprWaitSendLife = CCSprite::create("wait.png");
            sprWaitSendLife->setPosition( ccpAdd( itSendLife->getPosition(), ccp(-2, -2)));
            sprWaitSendLife->setScale(0.65f);
            sprWaitSendLife->setVisible(false);
            sprWaitSendLife->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, -360.0f)));
            cell->addChild(sprWaitSendLife);
            ((CustomTableViewCell*)cell)->m_sprWaitSendLife = sprWaitSendLife;
            
            

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
				lbSendLifeTimer->setVisible(false);
				itSendLife->setEnabled(true);
				lbSendLife->setVisible(true);
			}
			else
			{
				itSendLife->setEnabled(false);
				lbSendLife->setVisible(false);
				lbSendLifeTimer->setVisible(true);
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
    
    Gift* gift = (Gift*) m_arrRequests->objectAtIndex(idx);
	
    if (NULL != gift)
	{
		strFriendId = std::string(gift->m_senderId);

		std::string sname;
        string photopath;
        
        this->getFriendInfo(strFriendId, &sname, &photopath);
        
		if (sname.length() > 24) {
			sname = sname.substr(0, 22);
			sname.append("..");
		}
        
		strName  = CCString::create(sname);
        strPhoto = CCString::create(photopath);
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

        ((CustomTableViewCell*)(cell))->fbID = string(gift->m_senderId);
		((CustomTableViewCell*)(cell))->m_gift = gift;
        ((CustomTableViewCell*)(cell))->m_idx = idx;

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
		lbName->setPosition(ccp(0.75f * G_FRIEND_AVATAR_SIZE + 60, 0.75f * m_sprCell->getContentSize().height));
		lbName->setAnchorPoint(ccp(0.0f, 0.5f));
		lbName->setTag(4);
		cell->addChild(lbName);

		//icon life 
		CCSprite* iconLife = CCSprite::create("oil.png");
		iconLife->setPosition(ccp(700, m_sprCell->getContentSize().height/2 + 15));
		cell->addChild(iconLife);

		//lable nhận
		CCLabelTTF* lbGetBoom = CCLabelTTF::create(TXT("score_get"), "Roboto-Medium.ttf", 28);
		lbGetBoom->setColor(ccc3(0, 0, 0));
		lbGetBoom->setAnchorPoint(ccp(0.5f, 0.75f));
		lbGetBoom->setPosition(ccp(700, m_sprCell->getContentSize().height/4));
		cell->addChild(lbGetBoom);
        
        //label time
        char buffer[25];
        struct tm* tm_info;
        time_t tt = static_cast<time_t>(gift->m_time / 1000);
        tm_info = localtime(&tt);
        strftime(buffer, 25, "%r - %F", tm_info);
        CCString *s = CCString::create(buffer);
        
        
        CCLabelTTF *lbTime = CCLabelTTF::create(s->getCString(), "Roboto-Medium.ttf", 32);
		lbTime->setColor(ccc3(50,50,50));
		lbTime->setPosition(ccp(0.75f * G_FRIEND_AVATAR_SIZE + 60, 0.35f * m_sprCell->getContentSize().height));
		lbTime->setAnchorPoint(ccp(0.0f, 0.5f));
		lbTime->setTag(5);
		cell->addChild(lbTime);
        

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



void ScoreScene::onBuyItemCompleted(bool isSuccess, int newCoin, std::string itemType, int itemCount, std::string uniqueTag)
{
    CCLOG("onBuyItemCompleted: %s, %d, %s", itemType.c_str(), itemCount, uniqueTag.c_str());
    
    CustomTableViewCell *cell;
    
    for (int i = 0; i < m_tableXepHangSize; i++) {
        cell = (CustomTableViewCell*) m_tableXephang->cellAtIndex(i);
        if (cell->fbID.compare(uniqueTag) == 0) {
            CCLOG("FIND OUT CELL");
            break;
        }
    }
    
    if (isSuccess) {
        PLAY_GET_BOMB_EFFECT;
		
        DataManager::sharedDataManager()->SetDiamon(newCoin);
        DataManager::sharedDataManager()->SetBoom(itemCount + DataManager::sharedDataManager()->GetBoom());
        
        CCLOG("NEW LAZE: %d", DataManager::sharedDataManager()->GetBoom());
        
		//animation
		m_sprBoom->runAction(CCSequence::createWithTwoActions(
              CCScaleTo::create(0.2f, 1.0f),
              CCScaleTo::create(0.2f, 0.75f)
              ));
        
        m_sprDiamond->runAction(CCSequence::createWithTwoActions(
             CCScaleTo::create(0.2f, 1.25f),
             CCScaleTo::create(0.2f, 1.0f)
             ));
        
		refreshUserDetail();
        
        DataManager::sharedDataManager()->SetIsJustGetBoomNowFriend(uniqueTag.c_str(), true);
        cell->m_itGetBoomNow->setVisible(false);
        cell->m_itGetBoom->setVisible(true);
        cell->m_itGetBoom->setEnabled(false);
        
    } else {
        CCMessageBox(TXT("get_laze_free_error"), TXT("error_caption"));
        
        cell->m_itGetBoomNow->setVisible(true);
        cell->m_itGetBoom->setVisible(false);
        cell->m_itGetBoom->setEnabled(true);
    }
    
    cell->m_sprWait->setVisible(false);
}


void ScoreScene::getInbox()
{
    GameClientManager::sharedGameClientManager()->getInbox(DataManager::sharedDataManager()->GetFbID());
}


void ScoreScene::onGetInboxCompleted(bool isSuccess, CCArray* arrFriends)
{
    CCLOG("ScoreScene::onGetInboxCompleted");
    
    if (arrFriends == NULL) {
        return;
    }
    
    int count = arrFriends->count();
    if (count > 0) {
        
        if (m_arrRequests != NULL)
		{
			m_arrRequests->release();
			m_arrRequests = NULL;
		}
		m_arrRequests = CCArray::createWithArray(arrFriends);
		m_arrRequests->retain();
        
        //senderid, itemid, count, time
        //need photopath
        
        
		CCLOG("CALL: m_tableQuatang->reloadData()");
        
		m_tableQuatangSize = count;
		m_tableQuatang->reloadData();
        
    } else {
        CCLOG("NOT GIFT");
    }
}



void ScoreScene::getFriendInfo(string fbId, string* outName, string* outPhotopath)
{
    //CCLOG("ScoreScene::getFriendInfo: %s", fbId.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    CCObject* it;
    CCARRAY_FOREACH(m_arrHighScores, it)
    {
        FacebookAccount* fbFriend = (FacebookAccount*) (it);
        if (NULL != fbFriend)
        {
            if(fbFriend->m_fbId.compare(fbId) == 0)
            {
                *outName = string(fbFriend->m_fbName);
                *outPhotopath = string(fbFriend->m_photoPath);
                
                break;
            }
        }
    }
    
#endif
    
    //CCLOG("ScoreScene::getFriendInfo :: END");
}


void ScoreScene::onSendItemCompleted(bool isSuccess, string friendId, string itemId, int count)
{
    CCLOG("ScoreScene::onSendItemCompleted ~~~");

    PLAY_GET_BOMB_EFFECT;
    
    CustomTableViewCell *cell;
    
    for (int i = 0; i < m_tableXepHangSize; i++) {
        cell = (CustomTableViewCell*) m_tableXephang->cellAtIndex(i);
        if (cell->fbID.compare(friendId) == 0) {
            CCLOG("FIND OUT CELL");
            break;
        }
    }
    
    if (isSuccess) {
        
        CCLOG("Success");
        //show clock
        cell->m_itSendLife->setEnabled(false);
        cell->m_lbSendLife->setVisible(false);
        cell->m_lbSendLifeTimer->setVisible(true);
        
        cell->m_lastTimeSendLife = static_cast<long int>(time(NULL));
        cell->m_fbFriend->m_timeSendLife = static_cast<long int>(time(NULL));
        
    } else {
        CCLOG("Failed");
        
        cell->m_itSendLife->setEnabled(true);
        cell->m_lbSendLife->setVisible(true);
        cell->m_lbSendLifeTimer->setVisible(false);
    }
    
    cell->m_sprWaitSendLife->setVisible(false);
}


void ScoreScene::onRemoveItemCompleted(bool isSuccess, string senderId, long long time)
{
    CCLOG("ScoreScene::onRemoveItemCompleted");
    CCLOG("%s, %lld", senderId.c_str(), time);
    
    CustomTableViewCell *cell = NULL;
    int cellIndex = -1;
    
    for (int i = 0; i < m_tableQuatangSize; i++) {
        cell = (CustomTableViewCell*) m_tableQuatang->cellAtIndex(i);
        cellIndex = i;
        //CCLOG("Cell: %s, %lld", cell->m_gift->m_senderId.c_str(), cell->m_gift->m_time);
        
        if (cell != NULL) {
            if (cell->m_gift->m_senderId.compare(senderId) == 0 && cell->m_gift->m_time == time) {
                CCLOG("FIND OUT CELL: %d", i);
                break;
            }
        } else {
            cell = NULL;
            cellIndex = -1;
        }
    }
    
    
    if (isSuccess == true && cell != NULL && cellIndex != -1) {
        
        PLAY_GET_BOMB_EFFECT;
        
        int curLife = DataManager::sharedDataManager()->GetLastPlayerLife();
        curLife += 1;
        DataManager::sharedDataManager()->SetLastPlayerLife(curLife);
        CCLOG("CURRENT LIFE = %d", curLife);
        
        //save next time
        //long _tm = DataManager::sharedDataManager()->GetLastDeadTime();
        //_tm += G_PLAYER_TIME_TO_REVIVE;
        //DataManager::sharedDataManager()->SetLastDeadTime(_tm);
        
        //delete request
        
        CCLOG("REMOVE GIFT");
        
        int old = m_arrRequests->count();
        
        m_arrRequests->removeObjectAtIndex(cell->m_idx);
        m_tableQuatangSize = m_arrRequests->count();
        
        CCLOG("OLD: %d, NEW: %d", old, m_tableQuatangSize);
        
        //refresh UI
        m_tableQuatang->reloadData();
        //m_tableQuatang->removeCellAtIndex(cellIndex);
        
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











