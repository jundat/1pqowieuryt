#include "BreakLeaderboardDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "AudioManager.h"
#include "DataManager.h"

USING_NS_CC;

bool BreakLeaderboardDialog::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCPoint pExit = ccp(233, 1280-813);
	CCPoint pRevive = ccp(561, 1280-813);
	float BUTTON_SCALE = 0.6722f;
	
	CCScale9Sprite* dialog = CCScale9Sprite::create("dialog.png");
	dialog->setPosition(ccp(400, 640));
	dialog->setContentSize(CCSizeMake(680, 480));
	this->addChild(dialog);

	//////////////////////////////////////////////////////////////////////////
	//loser
	CCSprite* nodeLoser = CCSprite::create();
	this->addChild(nodeLoser);

	m_loserAvatar = CCSprite::create("fb-profile.png");
	m_loserAvatar->setPosition(ccp(148, 1280-495));
	nodeLoser->addChild(m_loserAvatar);

	CCLabelTTF* lbLoserName = CCLabelTTF::create(m_loser->m_fbName.c_str(), G_FONT_NORMAL, 48);
	lbLoserName->setFontFillColor(ccBLACK);
	lbLoserName->setAnchorPoint(ccp(0.0f, 0.5f));
	lbLoserName->setPosition(ccp(246, 1280-468));
	nodeLoser->addChild(lbLoserName);

	CCString* strTemp = CCString::createWithFormat("%d", m_loser->m_score);
	CCLabelTTF* lbLoserScore = CCLabelTTF::create(strTemp->getCString(), G_FONT_NORMAL, 48);
	lbLoserScore->setFontFillColor(ccBLACK);
	lbLoserScore->setAnchorPoint(ccp(0.0f, 0.5f));
	lbLoserScore->setPosition(ccp(246, 1280-522));
	nodeLoser->addChild(lbLoserScore);


	//////////////////////////////////////////////////////////////////////////
	//user
	CCSprite* nodeUser = CCSprite::create();
	this->addChild(nodeUser);

	CCSprite* sprAvatarUser = CCSprite::create("fb-profile.png");
	string userPhotopath = DataManager::sharedDataManager()->GetPhotoPath();
	
	if(userPhotopath.length() > 0) 
	{
		CCSprite* spr = CCSprite::create(userPhotopath.c_str());
		spr->setPosition(ccp(sprAvatarUser->getContentSize().width/2, sprAvatarUser->getContentSize().height/2));
		sprAvatarUser->addChild(spr);
	}
	sprAvatarUser->setPosition(ccp(148, 1280-649));
	nodeUser->addChild(sprAvatarUser);

	CCLabelTTF* lbUserName = CCLabelTTF::create(DataManager::sharedDataManager()->GetName().c_str(), G_FONT_NORMAL, 48);
	lbUserName->setFontFillColor(ccBLACK);
	lbUserName->setAnchorPoint(ccp(0.0f, 0.5f));
	lbUserName->setPosition(ccp(246, 1280-626));
	nodeUser->addChild(lbUserName);

	strTemp = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetHighScore());
	CCLabelTTF* lbUserScore = CCLabelTTF::create(strTemp->getCString(), G_FONT_NORMAL, 48);
	lbUserScore->setFontFillColor(ccBLACK);
	lbUserScore->setAnchorPoint(ccp(0.0f, 0.5f));
	lbUserScore->setPosition(ccp(246, 1280-680));
	nodeUser->addChild(lbUserScore);


	//////////////////////////////////////////////////////////////////////////
	//Action
	CCPoint userpos = sprAvatarUser->getPosition();
	CCPoint loserpos = m_loserAvatar->getPosition();
	
	nodeUser->runAction(CCSequence::createWithTwoActions(
		CCDelayTime::create(0.25f), 
		CCMoveBy::create(0.5f, ccpSub(loserpos, userpos))));
	nodeLoser->runAction(CCSequence::create(
		CCDelayTime::create(0.25f),
		CCMoveBy::create(0.5f, ccpSub(userpos, loserpos)),
		CCCallFunc::create(this, callfunc_selector( BreakLeaderboardDialog::onBreakLeaderboardFinish)),
		NULL)
		);

	//////////////////////////////////////////////////////////////////////////

	CCMenuItemImage* m_itExit = CCMenuItemImage::create(
		"exit_button.png",
		"exit_button_press.png",
		this,
		menu_selector(BreakLeaderboardDialog::menuCallBack));
	m_itExit->setScale(BUTTON_SCALE);
	m_itExit->setPosition(pExit);


	CCMenuItemImage* m_itRevive = CCMenuItemImage::create(
		"share.png",
		"shareDown.png",
		this,
		menu_selector(BreakLeaderboardDialog::shareCallBack));
	m_itRevive->setScale(BUTTON_SCALE);
	m_itRevive->setPosition(pRevive);


	CCMenu* menu = CCMenu::create(m_itExit, m_itRevive, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	PLAY_GET_BOMB_EFFECT;
    return true;
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
		CCString* strMessage = CCString::createWithFormat("Hey, i just got %d points in The Croods, following me is %s! Kaka :v", yourScore, loserName.c_str());

		//let try to post message
		//postMessageOnWall
		//autoPostMessageOnWall

		EziSocialObject::sharedObject()->postMessageOnWall(
			"The Croods",					//heading => Điện Biên Phủ Trên Không
			"Let got it!",					//caption
			strMessage->getCString(),		//message => Status
			"From the creators of Angry Birds and the creative minds at DreamWorks Animation: a FREE new game based on the motion picture phenomenon!",	//descripton
			"http://www.reelmama.com/wp-content/uploads/2013/04/Grug-from-THE-CROODS.jpg", //badgeIconURL
			"https://play.google.com/store/apps/details?id=com.rovio.croods");	//deepLinkURL
	}
#endif
}

void BreakLeaderboardDialog::onBreakLeaderboardFinish()
{

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
		CCMessageBox("Không thể chia sẻ", "Lỗi");

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
