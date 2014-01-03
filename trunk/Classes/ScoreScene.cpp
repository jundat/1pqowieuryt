#include "MenuScene.h"
#include "ScoreScene.h"
#include "ParseClient.h"
#include "cocos-ext.h"
#include "HttpClient.h"
#include "GameClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* ScoreScene::scene()
{
    CCScene *scene = CCScene::create();
    ScoreScene *layer = ScoreScene::create();
    scene->addChild(layer);
    return scene;
}

bool ScoreScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	//////////////////////////////////////////////////////////////////////////

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite* bg = CCSprite::create("bg_menu.png");
	bg->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(bg, 0);

	CCSprite* score_top = CCSprite::create("setting_top.png");
	score_top->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(score_top, 0);

	//

	m_lbTitle = CCLabelTTF::create("Connecting", "Marker Felt", 64);
	//m_lbTitle->setScale(0.5f);
	m_lbTitle->setPosition(ccp(visibleSize.width/2, visibleSize.height/2));
	this->addChild(m_lbTitle);

	m_lbMsg = CCLabelTTF::create("...", "Marker Felt", 64);
	//m_lbMsg->setScale(0.5f);
	m_lbMsg->setPosition(ccp(visibleSize.width/2, visibleSize.height/2 - 90));
	this->addChild(m_lbMsg);

	//

	CCMenuItemImage *backItem = CCMenuItemImage::create(
		"back_button.png",
		"back_button_press.png",
		this,
		menu_selector(ScoreScene::menuCallback));

	backItem->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + backItem->getContentSize().height - 3));
	
	CCMenu* pMenu = CCMenu::create(backItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	GameClient::sharedGameClient()->getLeaderboard();

    return true;
}

void ScoreScene::menuCallback(CCObject* pSender)
{
	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void ScoreScene::onHttpRequestCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response)
{
	if (!response)
	{
		return;
	}

	//Show info
	CCLOG("-----------------BEGIN-------------------");
	CCLOG("Request: [%s] completed", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLog("Request Failed: Error buffer: %s", response->getErrorBuffer());
	}
	else
	{
	}

	std::vector<char> *buffer = response->getResponseData();
	std::string str(buffer->begin(), buffer->end());
	CCLOG("Content: %s", str.c_str());
	
	CCLOG("-----------------END-------------------");
}
