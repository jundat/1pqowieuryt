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

	m_listSize = 0;

	m_friendList = NULL;
	m_arrHighScores = NULL;

	CCSprite* bg = CCSprite::create("bg_friend.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg);

	CCSprite* table_top = CCSprite::create("table_top.png");
	table_top->setPosition(ccp(400, 1280-202));
	this->addChild(table_top);

	CCSprite* table_border = CCSprite::create("table_border.png");
	table_border->setPosition(ccp(400, 1280-767));
	this->addChild(table_border);

	//facebook avatar
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

	std::string name = DataManager::sharedDataManager()->GetName();
	m_lbName = CCLabelTTF::create(name.c_str(), "Roboto-Medium.ttf", 52);
	m_lbName->setFontFillColor(ccc3(0, 0, 0));
	m_lbName->setPosition(ccp(174, 1280-173));
	m_lbName->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbName);

	CCString* score = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetHighScore());
	m_lbScore = CCLabelTTF::create(score->getCString(), "Roboto-Medium.ttf", 48);
	m_lbScore->setFontFillColor(ccc3(0, 0, 0));
	m_lbScore->setPosition(ccp(174, 1280-239));
	m_lbScore->setAnchorPoint(ccp(0.0f, 0.5f));
	this->addChild(m_lbScore);

	//Xep hang

	CCMenuItem* xephangOn = CCMenuItemImage::create("xephang.png", NULL, NULL);
	CCMenuItem* xephangOff = CCMenuItemImage::create("xephang1.png", NULL, NULL);
	m_xephangToggle = CCMenuItemToggle::createWithTarget(this,  menu_selector(ScoreScene::xephangCallback), xephangOn, xephangOff, NULL);
	m_xephangToggle->setSelectedIndex(0);
	m_xephangToggle->setPosition(ccp(202, 1280-351));

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

	//
	m_fbLogOutItem = CCMenuItemImage::create(
		"disconnect_facebook.png",
		"disconnect_facebook.png",
		this,
		menu_selector(ScoreScene::fbLogOutCallback));
	m_fbLogOutItem->setPosition(ccp(611, 1280-1205));
	this->addChild(m_fbLogOutItem);

	//
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

		m_fbLogInItem->setVisible(true);
		m_lbInvite->setVisible(true);
		m_fbLogOutItem->setVisible(false);
	}
#endif

	CCMenu* pMenu = CCMenu::create(backItem, m_fbLogInItem, m_fbLogOutItem, m_xephangToggle, m_quatangToggle, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	
	//SEPERATE ///////////////////////////////////

	m_isXepHangView = true;
	
	//table view XepHang //////////////////////////////////////

	m_sprCell = CCSprite::create("table_cell_xephang.png");
	m_sprCell->retain();
	CCSize cellsize = m_sprCell->getContentSize();
	CCSize tableSize = CCSizeMake(783, 718); //CCSizeMake(cellsize.width, cellsize.height * 6.0f);

	//vertical
	m_tableXephang = CCTableView::create(this, tableSize);
	m_tableXephang->setDirection(kCCScrollViewDirectionVertical);
	m_tableXephang->setAnchorPoint(CCPointZero);
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
	m_tableQuatang->setAnchorPoint(CCPointZero);
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

	if (m_isLoggedIn == false)
	{
		m_tableXephang->setVisible(false);
		m_tableQuatang->setVisible(false);
		m_lbWaiting->setVisible(false);
	}

	this->schedule(schedule_selector(ScoreScene::scheduleTimer), 1);

	return true;
}

void ScoreScene::menuCallback(CCObject* pSender)
{
	PLAY_BUTTON_EFFECT;

	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
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
	} 
	else ///////////////////////// Qua tang
	{
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
			m_tableQuatang->reloadData();
		} 
		else
		{
			PLAY_OUT_PORP_EFFECT;
		}
	}	
}

CCSize ScoreScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_sprCell->getContentSize();
}

CCTableViewCell* ScoreScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	bool isMyScore = false;
	CCString *order = CCString::createWithFormat("%d", idx + 1);
	CCString *score = CCString::create("0");
	CCString *name = CCString::create(G_DEFAULT_NAME);
	CCString *photo = CCString::create("fb-profile.png");
	std::string friendId;
	CCString *gift = CCString::create("0");

	tm* lastBoomTime;
	int boomWaitTime = 0;
	CCString* sBoomTimer = CCString::create("00:00:00");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(m_arrHighScores == NULL)
	{
		CCLOG("m_arrHighScores == NULL");
	}
	else
	{
		EziFacebookFriend* fbFriend = dynamic_cast<EziFacebookFriend*>(m_arrHighScores->objectAtIndex(idx));
		score = CCString::createWithFormat("%d", (int)fbFriend->getScore());

		std::string sname = std::string(fbFriend->getName());
		if (sname.length() > 18)
		{
			sname = sname.substr(0, 15);
			sname.append("...");
		}			

		name  = CCString::create(sname);
		friendId = std::string(fbFriend->getFBID());
		gift = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetGiftFromFriend(friendId.c_str()));
		
		//boomTimer
		lastBoomTime = DataManager::sharedDataManager()->GetTimeBoomFriend(friendId.c_str());
		if (lastBoomTime == NULL)
		{
			//2014 - 114 = 1900
			//very last
			lastBoomTime = new tm();
			lastBoomTime->tm_hour = 0;
			lastBoomTime->tm_min = 0;
			lastBoomTime->tm_sec = 0;
			lastBoomTime->tm_mday = 0;
			lastBoomTime->tm_mon = 0;
			lastBoomTime->tm_year = 0;

			DataManager::sharedDataManager()->SetTimeBoomFriend(friendId.c_str(), lastBoomTime);
		}

		time_t lastTime = mktime(lastBoomTime);
		time_t curTime = time(NULL);
		boomWaitTime = 24 * 60 * 60 - (int)difftime(curTime, lastTime);
		boomWaitTime = (boomWaitTime > 0) ? boomWaitTime : 0;


		int hour, min, sec;
		sec = boomWaitTime % 60;
		boomWaitTime -= sec;
		min =  ((int)(boomWaitTime / 60)) % 60;
		hour = ((int)(boomWaitTime / 60)) / 60;

		sBoomTimer = CCString::createWithFormat("%d:%d:%d", hour, min, sec);


		int lastLife = (int)(boomWaitTime / G_PLAYER_TIME_TO_REVIVE);
		lastLife = (lastLife > G_MAX_PLAYER_LIFE) ? G_MAX_PLAYER_LIFE : lastLife;

		if (strlen(fbFriend->getPhotoPath()) > 1)
		{
			photo  = CCString::createWithFormat("%s", fbFriend->getPhotoPath());
		}
		if(fbFriend->getFBID() == DataManager::sharedDataManager()->GetProfileID())
		{
			isMyScore = true;
		}
	}
#endif

	if (table == m_tableXephang)
	{
		CCTableViewCell *cell = table->cellAtIndex(idx);// table->dequeueCell();
		if (!cell) {
			cell = new CustomTableViewCell();
			cell->autorelease();

			((CustomTableViewCell*)cell)->fbID = friendId;
			((CustomTableViewCell*)cell)->lastBoomTime = lastBoomTime;

			CCSprite *sprite = CCSprite::create("table_cell_xephang.png");
			sprite->setAnchorPoint(CCPointZero);
			sprite->setPosition(ccp(0, 0));
			sprite->setTag(1);
			cell->addChild(sprite);

			CCSprite *avatar = CCSprite::create(photo->getCString());
			avatar->setScale(0.78125f);
			avatar->setPosition(ccp(140, m_sprCell->getContentSize().height/2));
			avatar->setTag(2);
			cell->addChild(avatar);
			
			CCLabelTTF *lbOrder;
			if (idx < 3)
			{
				lbOrder = CCLabelTTF::create(order->getCString(), "Villa.ttf", 42);
			} 
			else
			{
				lbOrder = CCLabelTTF::create(order->getCString(), "Roboto-Medium.ttf", 42);
			}
			
			lbOrder->setFontFillColor(ccc3(0, 0, 0));
			lbOrder->setHorizontalAlignment(kCCTextAlignmentLeft); //cocos2d::CCTextAlignment::
			lbOrder->setPosition(ccp(30, m_sprCell->getContentSize().height/2));
			lbOrder->setTag(3);
			lbOrder->setAnchorPoint(ccp(0.0f, 0.5f));
			cell->addChild(lbOrder);

			//Jundat TPham..
			//21
			
			CCLabelTTF *lbName = CCLabelTTF::create(name->getCString(), "Roboto-Medium.ttf", 42);
			lbName->setFontFillColor(ccc3(0,0,0));
			lbName->setPosition(ccp(210, m_sprCell->getContentSize().height * 3/4));
			lbName->setAnchorPoint(ccp(0.0f, 0.5f));
			lbName->setTag(4);
			cell->addChild(lbName);

			CCLabelTTF *lbScore = CCLabelTTF::create(score->getCString(), "Roboto-Medium.ttf", 42);
			lbScore->setFontFillColor(ccc3(0, 0, 0));
			lbScore->setPosition(ccp(210, m_sprCell->getContentSize().height * 3/8)); //0.25 * m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));
			lbScore->setAnchorPoint(ccp(0.0f, 0.5f));
			lbScore->setTag(5);
			cell->addChild(lbScore);

			if (isMyScore == false)
			{
				CCMenuItemImage* itBoom = CCMenuItemImage::create("boomgift.png", "boomgift1.png", this, menu_selector(ScoreScene::getBoomCallback));
				itBoom->setPosition(ccp(600, m_sprCell->getContentSize().height/2 + 10));
				itBoom->setTag(1000 + idx);
				((CustomTableViewCell*)cell)->itBoom = itBoom;

				CCMenuItemImage* itSendLife = CCMenuItemImage::create("oil.png", "oil.png", this, menu_selector(ScoreScene::sendLifeCallback));
				itSendLife->setRotation(180);
				itSendLife->setPosition(ccp(725, m_sprCell->getContentSize().height/2 + 15));
				itSendLife->setTag(2000 + idx);

				//Gửi
				CCLabelTTF* lbSendBoom = CCLabelTTF::create("Gửi", "Roboto-Medium.ttf", 28);
				lbSendBoom->setFontFillColor(ccc3(0, 0, 0));
				lbSendBoom->setAnchorPoint(ccp(0.5f, 0.75f));
				lbSendBoom->setPosition(ccp(725, m_sprCell->getContentSize().height/4));
				lbSendBoom->setTag(4000 + idx);
				cell->addChild(lbSendBoom);


				CCMenu* cell_menu = CCMenu::create(itBoom, itSendLife, NULL);
				cell_menu->setPosition(CCPointZero);
				cell->addChild(cell_menu);

				//610
				CCLabelTTF* lbTimer = CCLabelTTF::create(sBoomTimer->getCString(), "Roboto-Medium.ttf", 28); //32
				lbTimer->setFontFillColor(ccc3(0, 0, 0));
				lbTimer->setAnchorPoint(ccp(0.5f, 0.75f));
				lbTimer->setPosition(ccp(600, m_sprCell->getContentSize().height/4));
				lbTimer->setTag(3000 + idx);
				cell->addChild(lbTimer);
				((CustomTableViewCell*)cell)->lbTimer = lbTimer;

				//Nhận
				CCLabelTTF* lbGetBoom = CCLabelTTF::create("Nhận", "Roboto-Medium.ttf", 28);
				lbGetBoom->setFontFillColor(ccc3(0, 0, 0));
				lbGetBoom->setAnchorPoint(ccp(0.5f, 0.75f));
				lbGetBoom->setPosition(ccp(600, m_sprCell->getContentSize().height/4));
				lbGetBoom->setTag(4000 + idx);
				cell->addChild(lbGetBoom);
				((CustomTableViewCell*)cell)->lbGetBoom = lbGetBoom;

				if (boomWaitTime <= 0)
				{
					//FULL TIME
					itBoom->setEnabled(true);
					lbTimer->setVisible(false);
					lbGetBoom->setVisible(true);
				}
				else
				{
					itBoom->selected();

					itBoom->setEnabled(false);
					lbTimer->setVisible(true);
					lbGetBoom->setVisible(false);
				}
			}
		}
		else
		{
			CCSprite *avatar = (CCSprite*)cell->getChildByTag(2);
			avatar = CCSprite::create(photo->getCString());
			
			CCLabelTTF *lbOrder = (CCLabelTTF*)cell->getChildByTag(3);
			lbOrder->setString(order->getCString());

			CCLabelTTF *lbName = (CCLabelTTF*)cell->getChildByTag(4);
			lbName->setString(name->getCString());

			CCLabelTTF *lbScore = (CCLabelTTF*)cell->getChildByTag(5);
			lbScore->setString(score->getCString());
		}
		return cell;
	} 

	//////////////////////////////////////////////////////////////////////////
	
	else //////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////	
	
	{
		CCTableViewCell *cell = table->cellAtIndex(idx);// table->dequeueCell();
		if (!cell) {
			cell = new CustomTableViewCell();
			cell->autorelease();

			((CustomTableViewCell*)cell)->fbID = friendId;

			CCSprite *sprite = CCSprite::create("table_cell_quatang.png");
			sprite->setAnchorPoint(CCPointZero);
			sprite->setPosition(ccp(0, 0));
			sprite->setTag(1);
			cell->addChild(sprite);

			CCSprite *avatar = CCSprite::create(photo->getCString());
			avatar->setScale(0.78125);
			avatar->setPosition(ccp(50 + 30, m_sprCell->getContentSize().height/2));
			avatar->setTag(2);
			cell->addChild(avatar);

			CCLabelTTF *lbName = CCLabelTTF::create(name->getCString(), "Roboto-Medium.ttf", 42);
			lbName->setFontFillColor(ccc3(0,0,0));
			lbName->setPosition(ccp(0.75f * G_FRIEND_AVATAR_SIZE + 50, m_sprCell->getContentSize().height/2));
			lbName->setAnchorPoint(ccp(0.0f, 0.5f));
			lbName->setTag(4);
			cell->addChild(lbName);

			CCSprite* sprLife = CCSprite::create("oil.png");
			sprLife->setPosition(ccp(620, m_sprCell->getContentSize().height/2));
			cell->addChild(sprLife);

			CCLabelTTF *lbGiftNumber = CCLabelTTF::create(gift->getCString(), "Roboto-Medium.ttf", 48);
			lbGiftNumber->setFontFillColor(ccc3(0, 0, 0));
			lbGiftNumber->setPosition(ccp(670, m_sprCell->getContentSize().height/2)); //0.25 * m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));
			lbGiftNumber->setAnchorPoint(ccp(0.0f, 0.5f));
			lbGiftNumber->setHorizontalAlignment(kCCTextAlignmentLeft);
			lbGiftNumber->setTag(5);
			cell->addChild(lbGiftNumber);

		}
		else
		{
			CCSprite *avatar = (CCSprite*)cell->getChildByTag(2);
			avatar = CCSprite::create(photo->getCString());

			CCLabelTTF *lbName = (CCLabelTTF*)cell->getChildByTag(4);
			lbName->setString(name->getCString());

			CCLabelTTF *lbGiftNumber = (CCLabelTTF*)cell->getChildByTag(5);
			lbGiftNumber->setString(gift->getCString());
		}
		return cell;
	}
}

unsigned int ScoreScene::numberOfCellsInTableView(CCTableView *table)
{
	return m_listSize;
}



void ScoreScene::getBoomCallback( CCObject* pSender )
{
	int tag = ((CCMenuItemImage*)pSender)->getTag();
	CCLOG("In tag = %d", tag);
	CCString* s = CCString::createWithFormat("In tag %d", tag);

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
		cell->lbGetBoom->setVisible(false);
		cell->lbTimer->setVisible(true);
		cell->itBoom->setEnabled(false);
		cell->lastBoomTime = DataManager::sharedDataManager()->GetTimeBoomFriend(fbID.c_str());
	}
	else
	{
		PLAY_OUT_PORP_EFFECT;
	}
}

void ScoreScene::sendLifeCallback( CCObject* pSender )
{
	int curLife = DataManager::sharedDataManager()->GetLastPlayerLife();

	if (curLife <= 0)
	{
		PLAY_OUT_PORP_EFFECT;
		return;
	}
	
	PLAY_BUTTON_EFFECT;

	int tag = ((CCMenuItemImage*)pSender)->getTag();
	CCLOG("Out tag = %d", tag);
	CCString* s = CCString::createWithFormat("Out tag %d", tag);

	int idx = tag - 2000;
	CCTableViewCell* cell = m_tableXephang->cellAtIndex(idx);
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

// #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// 		//check incoming request
// 		EziSocialObject::sharedObject()->checkIncomingRequest();
// #endif

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

void ScoreScene::scheduleTimer( float dt )
{
	int numCell = numberOfCellsInTableView(m_tableXephang);

	for (int i = 0; i < numCell; ++i)
	{
		CustomTableViewCell* cell = (CustomTableViewCell*) tableCellAtIndex(m_tableXephang, i);

		if (cell != NULL && cell->lbTimer != NULL && cell->lbGetBoom != NULL)
		{
			time_t lastTime = mktime(cell->lastBoomTime);
			time_t curTime = time(NULL);
			int boomWaitTime = 24 * 60 * 60 - (int)difftime(curTime, lastTime);

			if (boomWaitTime <= 0)
			{
				//FULL TIME
				cell->itBoom->setEnabled(true);
				cell->lbTimer->setVisible(false);
				cell->lbGetBoom->setVisible(true);
			}
			else
			{
				cell->itBoom->setEnabled(false);
				cell->lbTimer->setVisible(true);
				cell->lbGetBoom->setVisible(false);
			}

			boomWaitTime = (boomWaitTime > 0) ? boomWaitTime : 0;
			
			int hour, min, sec;
			sec = boomWaitTime % 60;
			boomWaitTime -= sec;
			min =  ((int)(boomWaitTime / 60)) % 60;
			hour = ((int)(boomWaitTime / 60)) / 60;
			CCString* sBoomTimer = CCString::createWithFormat("%d:%d:%d", hour, min, sec);

			if (cell->lbTimer != NULL)
			{
				cell->lbTimer->setString(sBoomTimer->getCString());
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL)
	{
		m_isLoggedIn = true;

		m_fbLogInItem->setVisible(false);
		m_lbInvite->setVisible(false);
		m_fbLogOutItem->setVisible(true);
		refreshView();

		if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
		{
			EziSocialObject::sharedObject()->postScore(DataManager::sharedDataManager()->GetHighScore());
		}
		EziSocialObject::sharedObject()->fetchFBUserDetails(true); //need email = true
	}
	else
	{
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

void ScoreScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
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
			userPhoto->setPosition(ccp(m_userAvatar->getContentSize().width/2, m_userAvatar->getContentSize().height/2));
			userPhoto->setScale(m_userAvatar->getContentSize().width/userPhoto->getContentSize().width);
			m_userAvatar->addChild(userPhoto);
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

			//m_tableXephang->setVisible(true);
			//m_tableQuatang->setVisible(true);

			refreshView();

			m_tableXephang->reloadData();
			m_tableXephang->reloadData();

			//check incoming request
			EziSocialObject::sharedObject()->checkIncomingRequest();
		}
	}
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) // fbUserDetailCallback

void ScoreScene::fbUserDetailCallback( int responseCode, const char* responseMessage, EziFacebookUser* fbUser )
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


		//check incoming request
		//EziSocialObject::sharedObject()->checkIncomingRequest();

		callGetHighScores();
	}
}
#endif

void ScoreScene::callSubmitScore()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EziSocialObject::sharedObject()->postScore(DataManager::sharedDataManager()->GetHighScore());
#endif
}

void ScoreScene::callGetHighScores()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	EziSocialObject::sharedObject()->getHighScores();
#endif
}

void ScoreScene::fbHighScoresCallback( int responseCode, const char* responseMessage, cocos2d::CCArray* highScores )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (EziSocialWrapperNS::RESPONSE_CODE::FB_REQUEST_SENT == responseCode)
	{
		int numFriends = friendsGotRequests->count();
		CCLOG("Request sent successfully to %d friends", numFriends);

		int curLife = DataManager::sharedDataManager()->GetLastPlayerLife();
		CCLOG("LAST_LIFE_FIRST = %d", DataManager::sharedDataManager()->GetLastPlayerLife());
		DataManager::sharedDataManager()->SetLastPlayerLife(curLife - 1);
		CCLOG("LAST_LIFE = %d", DataManager::sharedDataManager()->GetLastPlayerLife());
	}
	else
	{
		CCLOG("Request sent failed");
	}
#endif
}

void ScoreScene::fbIncomingRequestCallback(int responseCode, const char* responseMessage, int totalIncomingRequests)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
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
			//EziSocialObject::sharedObject()->checkIncomingRequest();
		}
	}

	m_tableQuatang->reloadData();

#endif
}

// Facebook //=========================================
