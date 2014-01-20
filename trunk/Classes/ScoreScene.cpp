#include "MenuScene.h"
#include "ScoreScene.h"
#include "ParseClient.h"
#include "cocos-ext.h"
#include "HttpClient.h"
#include "GameClient.h"
#include "DataManager.h"
#include "CustomTableViewCell.h"


USING_NS_CC;
USING_NS_CC_EXT;



ScoreScene::ScoreScene() : m_lbMsg(NULL), m_lbTitle(NULL)
{
	
}

ScoreScene::~ScoreScene()
{
	//CCHttpClient::getInstance()->destroyInstance();
}

bool ScoreScene::init()
{
	//pre process

	if (!CCLayer::init())
	{
		return false;
	}
	
	m_arrName = new CCArray(LIST_SIZE);
	m_arrScore = new CCArray(LIST_SIZE);
	m_arrName->retain();
	m_arrScore->retain();


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
	isclicked = false;

	CCMenuItemImage *fbItem = CCMenuItemImage::create(
		"back.png",
		"back1.png",
		this,
		menu_selector(ScoreScene::fbCallback));
	fbItem->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));


	//CCTableViewDataSource* data = CCTableViewDataSource::
	//CCTableView* tbView = CCTableView::create()

	CCMenuItemImage *backItem = CCMenuItemImage::create(
		"back.png",
		"back1.png",
		this,
		menu_selector(ScoreScene::menuCallback));
	backItem->setPosition(ccp(400, 1280-1166));

	CCMenu* pMenu = CCMenu::create(fbItem, backItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	// table view

	m_sprCell = CCSprite::create("tablecell.png");
	m_sprCell->retain();
	CCSize cellsize = m_sprCell->getContentSize();
	CCSize tableSize = CCSizeMake(cellsize.width, cellsize.height * 6.0f);

	//vertical
	m_tableView = CCTableView::create(this, tableSize);
	m_tableView->setDirection(kCCScrollViewDirectionVertical); // (kCCScrollViewDirectionHorizontal);
	m_tableView->setAnchorPoint(CCPointZero);
	m_tableView->setPosition(ccp(G_DESIGN_WIDTH/2 - tableSize.width/2, G_DESIGN_HEIGHT/2 - tableSize.height/2));
	m_tableView->setDelegate(this);
	m_tableView->setVerticalFillOrder(extension::CCTableViewVerticalFillOrder::kCCTableViewFillTopDown);
	this->addChild(m_tableView);
	m_tableView->reloadData();
	
	return true;
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
		processData(str);
	}

	
	CCLOG("-----------------END-------------------");
}

void ScoreScene::processData( std::string str )
{
	//m_arrName
	//m_arrScore

	json_t *root;
	json_error_t error;
	json_t *results;

	root = json_loads(str.c_str(), strlen(str.c_str()), &error);

	if(!root)
	{
		CCLOG("Error: on line %d: %s", error.line, error.text);
	}
	else
	{
		results = json_object_get(root, "result");
		if(!json_is_array(results))
		{
			CCLOG("Error: Result is not a array");
		}
		else
		{
			for(int i = 0; i < json_array_size(results); i++)
			{
				json_t *user, *username, *name, *score;
				user = json_array_get(results, i);				
				username = json_object_get(user, "username");
				name = json_object_get(user, "name");
				score = json_object_get(user, "score");

				m_arrName->addObject(CCString::create(json_string_value(name)));
				m_arrScore->addObject(CCString::createWithFormat("%d", json_integer_value(score)));

				CCLOG("%s: %d", json_string_value(name), json_integer_value(score));
			}
		}
	}
}

void ScoreScene::keyBackClicked()
{
	menuCallback(NULL);
}

void ScoreScene::fbCallback( CCObject* pSender )
{
	isclicked = ! isclicked;
	m_tableView->reloadData();
}

//new delegate


void ScoreScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize ScoreScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_sprCell->getContentSize();
}

CCTableViewCell* ScoreScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCString *string;
	if (isclicked)
	{
		string = CCString::createWithFormat("%d", idx + 1);
	}
	else
	{
		string = CCString::createWithFormat("--%d--", idx + 1);
	}
	
	CCTableViewCell *cell = table->dequeueCell();
	if (!cell) {
		cell = new CustomTableViewCell();
		cell->autorelease();
		CCSprite *sprite = CCSprite::create("tablecell.png");
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(0, 0));
		cell->addChild(sprite);

		CCLabelBMFont *label = CCLabelBMFont::create(string->getCString(), "Mia_64.fnt");
		label->setScale(0.6f);
		label->setAlignment(kCCTextAlignmentLeft); //cocos2d::CCTextAlignment::
		label->setPosition(ccp(10, m_sprCell->getContentSize().height/2));
		label->setAnchorPoint(ccp(0.0f, 0.5f));
		label->setTag(123);
		cell->addChild(label);
	}
	else
	{
		CCLabelBMFont *label = (CCLabelBMFont*)cell->getChildByTag(123);
		label->setString(string->getCString());
	}


	return cell;
}

unsigned int ScoreScene::numberOfCellsInTableView(CCTableView *table)
{
	return LIST_SIZE;
}




//end new delegate