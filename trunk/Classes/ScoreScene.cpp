#include "MenuScene.h"
#include "ScoreScene.h"
#include "ParseClient.h"
#include "cocos-ext.h"
#include "HttpClient.h"
#include "GameClient.h"
#include "DataManager.h"

USING_NS_CC;
USING_NS_CC_EXT;



ScoreScene::ScoreScene() : m_lbMsg(NULL), m_lbTitle(NULL)
{
	//pre process

	CCLOG("Call getLeaderboard");
	CCString* url = ParseClient::sharedParseClient()->getUrl("getLeaderboard");

	CCHttpRequest* request = new CCHttpRequest();
	request->setHeaders(ParseClient::sharedParseClient()->m_header);
	request->setUrl(url->getCString());
	request->setRequestType(CCHttpRequest::kHttpPost);
	request->setResponseCallback(this, httpresponse_selector(ScoreScene::onGetLeaderboardCompleted));
	request->setTag("getLeaderboard");
	CCString* json = CCString::createWithFormat("{}");
	request->setRequestData(json->getCString(), strlen(json->getCString()));
	CCHttpClient::getInstance()->send(request);
	request->release();

	// pres ////////////////////////////////////

	this->setKeypadEnabled(true);

	//////////////////////////////////////////////////////////////////////////

	CCSprite* bg = CCSprite::create("bg_stars.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg, 0);

	CCSprite* score_top = CCSprite::create("setting_top.png");
	score_top->setPosition(ccp(G_DESIGN_WIDTH/2, - 110 + G_DESIGN_HEIGHT));
	this->addChild(score_top, 0);

	//

	CCLabelTTF* m_lbTitle = CCLabelTTF::create("Điểm cao", "Marker Felt.ttf", 64);
	m_lbTitle->setFontFillColor(ccc3(56, 56, 56));
	m_lbTitle->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(m_lbTitle);

	CCString* s = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetHighScore());
	m_lbMsg = CCLabelTTF::create(s->getCString(), "Marker Felt.ttf", 64);
	m_lbMsg->setFontFillColor(ccc3(56, 56, 56));
	m_lbMsg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 - 90));
	this->addChild(m_lbMsg);

	//


	CCMenuItemImage *fbItem = CCMenuItemImage::create(
		"back.png",
		"back1.png",
		this,
		menu_selector(ScoreScene::fbCallback));
	fbItem->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));




	CCMenuItemImage *backItem = CCMenuItemImage::create(
		"back.png",
		"back1.png",
		this,
		menu_selector(ScoreScene::menuCallback));
	backItem->setPosition(ccp(G_DESIGN_WIDTH/2, backItem->getContentSize().height - 3));

	CCMenu* pMenu = CCMenu::create(fbItem, backItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);
}

ScoreScene::~ScoreScene()
{
	//CCHttpClient::getInstance()->destroyInstance();
}

void ScoreScene::menuCallback(CCObject* pSender)
{
	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void ScoreScene::onGetLeaderboardCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response)
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
		m_lbMsg->setString(response->getErrorBuffer());
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
		CCLOG("Content: %s", str.c_str());
		m_lbMsg->setString(str.c_str());
	}

	
	CCLOG("-----------------END-------------------");
}

void ScoreScene::keyBackClicked()
{
	menuCallback(NULL);
}

void ScoreScene::fbCallback( CCObject* pSender )
{
	
}
