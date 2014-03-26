#include "BreakLeaderboardDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "TextLoader.h"

USING_NS_CC;


#define BreakLeaderboardDialog_AVATAR_Z		2
#define BreakLeaderboardDialog_PLANE_Z		1
#define BreakLeaderboardDialog_NAME_SIZE	11


bool BreakLeaderboardDialog::init()
{
	if ( !CCLayerColor::initWithColor(G_DIM_COLOR) )
	{
		return false;
	}

	CCPoint pExit = ccp(233, 1280-813);
	CCPoint pRevive = ccp(561, 1280-813);
	CCPoint pVuotMat = ccp(610, 1280-610);
	float BUTTON_SCALE = 0.6722f;
	
	CCScale9Sprite* dialog = CCScale9Sprite::create("dialog.png");
	dialog->setPosition(ccp(400, 640));
	dialog->setContentSize(CCSizeMake(680, 480));
	this->addChild(dialog);

	//////////////////////////////////////////////////////////////////////////
	//loser
	m_nodeLoser = CCSprite::create();
	this->addChild(m_nodeLoser, BreakLeaderboardDialog_AVATAR_Z);

	m_loserAvatar = CCSprite::create("fb-profile.png");
	m_loserAvatar->setPosition(ccp(610, 1280-505));
	m_nodeLoser->addChild(m_loserAvatar);

	string _tempName = string(m_loser->m_fbName);
	_tempName = MY_LIMIT_STR(_tempName, BreakLeaderboardDialog_NAME_SIZE, "");

	CCLabelTTF* lbLoserName = CCLabelTTF::create(_tempName.c_str(), G_FONT_NORMAL, 42);
	lbLoserName->setColor(ccBLACK);
	//lbLoserName->setAnchorPoint(ccp(1.0f, 0.5f));
	//lbLoserName->setPosition(ccp(686, 1280-638));
	lbLoserName->setPosition(ccp(610, 1280-608));
	m_nodeLoser->addChild(lbLoserName);

	CCString* strTemp = CCString::createWithFormat("%d", m_loser->m_score);
	CCLabelTTF* lbLoserScore = CCLabelTTF::create(strTemp->getCString(), G_FONT_NORMAL, 42);
	lbLoserScore->setColor(ccBLACK);
// 	lbLoserScore->setAnchorPoint(ccp(1.0f, 0.5f));
// 	lbLoserScore->setPosition(ccp(686, 1280-680));
	lbLoserScore->setPosition(ccp(610, 1280-650));
	m_nodeLoser->addChild(lbLoserScore);


	//////////////////////////////////////////////////////////////////////////
	//user
	m_nodeUser = CCSprite::create();
	this->addChild(m_nodeUser, BreakLeaderboardDialog_AVATAR_Z);

	CCSprite* sprAvatarUser = CCSprite::create("fb-profile.png");
	string userPhotopath = DataManager::sharedDataManager()->GetPhotoPath();
	
	if(userPhotopath.length() > 0) 
	{
		CCSprite* spr = CCSprite::create(userPhotopath.c_str());
		spr->setPosition(ccp(sprAvatarUser->getContentSize().width/2, sprAvatarUser->getContentSize().height/2));
		sprAvatarUser->addChild(spr);
	}
	sprAvatarUser->setPosition(ccp(188, 1280-505));
	m_nodeUser->addChild(sprAvatarUser);

	_tempName = DataManager::sharedDataManager()->GetName();
	_tempName = MY_LIMIT_STR(_tempName, BreakLeaderboardDialog_NAME_SIZE, "");

	CCLabelTTF* lbUserName = CCLabelTTF::create(_tempName.c_str(), G_FONT_NORMAL, 42);
	lbUserName->setColor(ccBLACK);
// 	lbUserName->setAnchorPoint(ccp(0.0f, 0.5f));
// 	lbUserName->setPosition(ccp(112, 1280-638));
	lbUserName->setPosition(ccp(188, 1280-608));
	m_nodeUser->addChild(lbUserName);

	strTemp = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetHighScore());
	CCLabelTTF* lbUserScore = CCLabelTTF::create(strTemp->getCString(), G_FONT_NORMAL, 42);
	lbUserScore->setColor(ccBLACK);
// 	lbUserScore->setAnchorPoint(ccp(0.0f, 0.5f));
// 	lbUserScore->setPosition(ccp(112, 1280-680));
	lbUserScore->setPosition(ccp(188, 1280-650));
	m_nodeUser->addChild(lbUserScore);


	//////////////////////////////////////////////////////////////////////////
	//Action
	CCPoint userpos = sprAvatarUser->getPosition();
	CCPoint loserpos = m_loserAvatar->getPosition();

	CCSprite* sprplane = CCSprite::create("oil.png");
	sprplane->setRotation(90);
	sprplane->setPosition(userpos);
	this->addChild(sprplane, BreakLeaderboardDialog_PLANE_Z);

	sprplane->runAction(CCSequence::create(
		CCDelayTime::create(0.5f),
		CCEaseIn::create(CCMoveTo::create(0.3f, loserpos), 2),
		CCCallFuncN::create(this, callfuncN_selector(BreakLeaderboardDialog::afterMovePlane)),
		NULL));



	//////////////////////////////////////////////////////////////////////////

	MY_CREATE_MENU_BUTTON(m_itExit, "button.png", "button_down.png", TXT("btn_exit"), 
		"Roboto-Medium.ttf", 48, ccBLACK, pExit, this, BreakLeaderboardDialog::menuCallBack);


	MY_CREATE_MENU_BUTTON(m_itRevive, "button.png", "button_down.png", TXT("btn_share"), 
		"Roboto-Medium.ttf", 48, ccBLACK, pRevive, this, BreakLeaderboardDialog::shareCallBack);
	
	CCMenu* menu = CCMenu::create(m_itExit, m_itRevive, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	PLAY_GET_BOMB_EFFECT;
    return true;
}

void BreakLeaderboardDialog::afterMovePlane( CCNode* pSender )
{
	PLAY_ENEMY3_DOWN_EFFECT;

	this->removeChild(pSender);

	m_sprBreak = CCSprite::create("break.png");
	m_sprBreak->setAnchorPoint(ccp(1.0f, 0.5f));
	m_sprBreak->setPosition(ccp(538, 1280-532));
	this->addChild(m_sprBreak);
	m_sprBreak->runAction(CCSequence::createWithTwoActions(
		CCDelayTime::create(0.25f),
		CCFadeOut::create(0.1f)
		));

	m_nodeLoser->runAction(CCSequence::create(
		CCDelayTime::create(0.5f),
		CCEaseElasticOut::create(CCMoveBy::create(0.7f, ccp(0, -80))),
		CCDelayTime::create(0.5f),
		CCCallFunc::create(this, callfunc_selector( BreakLeaderboardDialog::afterMoveLoser)),
		NULL)
		);
}

void BreakLeaderboardDialog::afterMoveLoser()
{
	CCSprite* sprVuotMat;
	string lang = DataManager::sharedDataManager()->GetLanguage();
	if (lang.compare("English") == 0)
	{
		sprVuotMat = CCSprite::create("en_beat.png");
	} 
	else
	{
		sprVuotMat = CCSprite::create("vn_beat.png");
	}

	sprVuotMat->setPosition(ccp(400, 700));
	sprVuotMat->setScale(6.0f);
	sprVuotMat->runAction(CCScaleTo::create(0.15f, 2.0f));
	this->addChild(sprVuotMat, BreakLeaderboardDialog_AVATAR_Z);
}

void BreakLeaderboardDialog::menuCallBack( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;
	
	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void BreakLeaderboardDialog::shareCallBack( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;
	postMessageToLoser(m_loser->m_fbName, m_loser->m_score);
}

void BreakLeaderboardDialog::postMessageToLoser( string loserName, int yourScore )
{
	CCLOG("postMessageToLoser");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		CCString* strMessage = CCString::createWithFormat(TXT("feed_msg"), yourScore, loserName.c_str());

		//let try to post message
		//postMessageOnWall
		//autoPostMessageOnWall

		EziSocialObject::sharedObject()->postMessageOnWall(
			TXT("feed_heading"),		//heading => Điện Biên Phủ Trên Không
			TXT("feed_caption"),		//caption
			strMessage->getCString(),	//message => Status
			TXT("feed_description"),	//descripton
			TXT("feed_badge_icon"),		//badgeIconURL
			TXT("feed_deep_link"));		//deepLinkURL
	}
#endif
}

void BreakLeaderboardDialog::fbMessageCallback(int responseCode, const char* responseMessage)
{
	CCLOG("Message Sent");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if(responseCode == EziSocialWrapperNS::RESPONSE_CODE::FB_NORMAL_MESSAGE_PUBLISHED)
	{
		CCLOG("Message published successfully! -> Exit");

		CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
	else
	{
		CCMessageBox(TXT("feed_error"), TXT("feed_error_caption"));

		CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
#endif
}

void BreakLeaderboardDialog::onEnterTransitionDidFinish()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	EziSocialObject::sharedObject()->setFacebookDelegate(this);

	EziSocialObject::sharedObject()->getProfilePicForID(this, m_loser->m_fbId.c_str(), G_AVATAR_SIZE, G_AVATAR_SIZE, false);
#endif
}

void BreakLeaderboardDialog::onExitTransitionDidStart()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	EziSocialObject::sharedObject()->setFacebookDelegate(NULL);
#endif
}

void BreakLeaderboardDialog::fbUserPhotoCallback( const char *userPhotoPath, const char* fbID )
{
	if ((strcmp(userPhotoPath, "") != 0))
	{
		CCSprite* userPhoto = CCSprite::create(userPhotoPath);
		userPhoto->setPosition(ccp(m_loserAvatar->getContentSize().width/2, m_loserAvatar->getContentSize().height/2));

		if (m_loserAvatar)
		{
			m_loserAvatar->addChild(userPhoto);
		}
		else
		{
			CCLOG("ERROR: m_loserAvatar == NULL");
		}
	}
}
