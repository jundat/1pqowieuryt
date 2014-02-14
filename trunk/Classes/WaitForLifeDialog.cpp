#include "WaitForLifeDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "DataManager.h"
#include <time.h>

USING_NS_CC;

bool WaitForLifeDialog::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCPoint pexit = ccp(250, G_DESIGN_HEIGHT - 760);
	CCPoint pask = ccp(G_DESIGN_WIDTH-250, G_DESIGN_HEIGHT - 760);
	float textScale = 0.6f;

	CCSprite* bg = CCSprite::create("dialog.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg, -2);

	CCMenuItemImage* exitButton = CCMenuItemImage::create(
		"exit_button.png",
		"exit_button_press.png",
		this,
		menu_selector(WaitForLifeDialog::exitCallback));
	exitButton->setScale(textScale);
	exitButton->setPosition(pexit);


	CCMenuItemImage* askButton = CCMenuItemImage::create(
		"ask_button.png",
		"ask_button_press.png",
		this,
		menu_selector(WaitForLifeDialog::askFriendCallback));
	askButton->setScale(textScale);
	askButton->setPosition(pask);

	CCMenu* menu = CCMenu::create(exitButton, askButton, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	CCLabelTTF* labelTitle = CCLabelTTF::create("Không đủ số mạng\nHãy chờ thêm", "Roboto-Medium.ttf", 48);
	labelTitle->setFontFillColor(ccc3(56, 56, 56));
	labelTitle->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 120));
	this->addChild(labelTitle);

	int mins = (int)m_waitTime/60;
	int seconds = (int)m_waitTime%60;

	CCString* s = CCString::createWithFormat("%d:%d", mins, seconds);
	m_lbTime = CCLabelTTF::create(s->getCString(), "Roboto-Medium", 48);
	m_lbTime->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	m_lbTime->setColor(ccc3(56, 56, 56));
	this->addChild(m_lbTime);

	this->schedule(schedule_selector(WaitForLifeDialog::ScheduleTick), 1);

    return true;
}

void WaitForLifeDialog::exitCallback( CCObject* pSender )
{
	MenuScene* parent = (MenuScene*)this->getParent();
	parent->setTouchEnabled(true);
	parent->onCloseDialog();
	this->removeFromParent();
}

void WaitForLifeDialog::askFriendCallback( CCObject* pSender )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

// 	EziSocialObject::sharedObject()->sendRequestToFriends(
// 		EziSocialWrapperNS::FB_REQUEST::TYPE, //EziSocialWrapperNS::FB_REQUEST::TYPErequestType,
// 		"Giúp mình với!", //const char* message,
// 		NULL, //cocos2d::CCArray *selectedFriendIDs,
// 		NULL, //cocos2d::CCDictionary *dataDictionary, 
// 		"Phi Công Bút Chì" //const char* customTitle
// 		);

	//invite friends
	EziSocialObject::sharedObject()->sendRequestToFriends(
		EziSocialWrapperNS::FB_REQUEST::REQUEST_INVITE,
		"Cứu với, đang hết mạng nè!", 
		NULL,
		NULL, 
		"Phi Công Bút Chì");

	//send challenge
// 	EziSocialObject::sharedObject()->sendRequestToFriends(
// 		EziSocialWrapperNS::FB_REQUEST::REQUEST_CHALLENGE,
// 		"I have score 900 points in EziSocialDemo. Can you beat me?",
// 		NULL,
// 		NULL, 
// 		"");

	//send gift
	

#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void WaitForLifeDialog::fbMessageCallback(int responseCode, const char* responseMessage)
{
	if(responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_NORMAL_MESSAGE_PUBLISHED)
	{
		CCLOG("Message published successfully!");
		DataManager::sharedDataManager()->SetLastPlayerLife(5);

		//stop timer
		this->unschedule(schedule_selector(WaitForLifeDialog::ScheduleTick));

		MenuScene* parent = (MenuScene*)this->getParent();
		parent->setTouchEnabled(true);
		parent->onCloseDialog();
		this->removeFromParent();
	}
	else
	{
		CCLOG("Message published failed!");
	}
}


void WaitForLifeDialog::fbSendRequestCallback( int responseCode, const char* responseMessage, cocos2d::CCArray* friendsGotRequests )
{
// 	FB_REQUEST_SENDING_ERROR - In case if there is any error while sending the request
// 	FB_REQUEST_SENDING_CANCELLED – In case, user decides not to send the request.
// 	FB_REQUEST_SENT - If sending request is success

	if (EziSocialWrapperNS::RESPONSE_CODE::FB_REQUEST_SENT == responseCode)
	{
		int numFriends = friendsGotRequests->count();

		CCLOG("Request sent successfully to %d friends", numFriends);
		CCMessageBox("Infor", "Request sent successfully");

		numFriends = (numFriends > G_MAX_PLAYER_LIFE) ? G_MAX_PLAYER_LIFE : numFriends;
		DataManager::sharedDataManager()->SetLastPlayerLife(numFriends);
		
		//stop timer
		this->unschedule(schedule_selector(WaitForLifeDialog::ScheduleTick));

		MenuScene* parent = (MenuScene*)this->getParent();
		parent->setTouchEnabled(true);
		parent->onCloseDialog();
		this->removeFromParent();
	}
	else
	{
		CCMessageBox("Infor", "Request sent failed");
	}
}


#endif

void WaitForLifeDialog::ScheduleTick( float dt )
{

	tm* lasttm = DataManager::sharedDataManager()->GetLastDeadTime();
	time_t lastTime = mktime(lasttm);
	time_t curTime = time(NULL);
	double _secondsElapsed = difftime(curTime, lastTime);

	m_waitTime = (float)(G_PLAYER_TIME_TO_REVIVE - _secondsElapsed);

	//m_waitTime -= dt;

	if (m_waitTime < 0)
	{
		this->unschedule(schedule_selector(WaitForLifeDialog::ScheduleTick));
		MenuScene* parent = (MenuScene*)this->getParent();
		parent->onCompletedWaiting();
	}
	else
	{
		int mins = (int)m_waitTime/60;
		int seconds = (int)m_waitTime % 60;

		CCString* s = CCString::createWithFormat("%d:%d", mins, seconds);
		m_lbTime->setString(s->getCString());
	}
}



