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



ScoreScene::ScoreScene()
{
	
}

ScoreScene::~ScoreScene()
{
	//CCHttpClient::getInstance()->destroyInstance();
}

bool ScoreScene::init()
{

	GameClient::sharedGameClient()->submitScore();

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

	//pre process

	if (!CCLayer::init())
	{
		return false;
	}

	m_isLoader = false;

	m_arrName = new CCArray(LIST_SIZE);
	m_arrScore = new CCArray(LIST_SIZE);
	m_arrName->retain();
	m_arrScore->retain();
	
	
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
	CCMenuItemImage *backItem = CCMenuItemImage::create(
		"back.png",
		"back1.png",
		this,
		menu_selector(ScoreScene::menuCallback));
	backItem->setPosition(ccp(400, 1280-1166));

	CCMenu* pMenu = CCMenu::create(/*fbItem,*/ backItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	// table view

	m_sprCell = CCSprite::create("tablecell.png");
	m_sprCell->retain();
	CCSize cellsize = m_sprCell->getContentSize();
	CCSize tableSize = CCSizeMake(cellsize.width, cellsize.height * 5.5f);

	//vertical
	m_tableView = CCTableView::create(this, tableSize);
	m_tableView->setDirection(kCCScrollViewDirectionVertical); // (kCCScrollViewDirectionHorizontal);
	m_tableView->setAnchorPoint(CCPointZero);
	m_tableView->setPosition(ccp(G_DESIGN_WIDTH/2 - tableSize.width/2, G_DESIGN_HEIGHT/2 - tableSize.height/2));
	m_tableView->setDelegate(this);
	m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
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
		CCLOG("Request Failed: Error buffer: %s", response->getErrorBuffer());
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
			//add you at first

			m_arrName->addObject(CCString::create(DataManager::sharedDataManager()->GetUsername()));
			m_arrScore->addObject(CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetHighScore()));
			int sizearr = (int)json_array_size(results);
			for(int i = 0; i < sizearr; i++)
			{
				json_t *user, *username, *name, *score;
				user = json_array_get(results, i);				
				username = json_object_get(user, "username");
				name = json_object_get(user, "name");
				score = json_object_get(user, "score");

				m_arrName->addObject(CCString::create(json_string_value(username)));
				m_arrScore->addObject(CCString::createWithFormat("%d", (int)json_integer_value(score)));

				CCLOG("%s: %d", json_string_value(name), (int)json_integer_value(score));
			}
		}
	}

	m_isLoader = true;
	m_tableView->reloadData();
}

void ScoreScene::keyBackClicked()
{
	menuCallback(NULL);
}

void ScoreScene::fbCallback( CCObject* pSender )
{
// 	m_isLoader = ! m_isLoader;
// 	m_tableView->reloadData();
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
	CCString *order = CCString::createWithFormat("%d", idx);
	if (idx == 0)
	{
		order = CCString::create("My");
	}

	CCString *score;
	CCString *name;

	if (m_isLoader == false)
	{
		score = CCString::create("0");
		name = CCString::create("Name");
	}
	else
	{
		score = (CCString*)m_arrScore->objectAtIndex(idx);
		name = (CCString*)m_arrName->objectAtIndex(idx);
	}

	
	CCTableViewCell *cell = table->dequeueCell();
	if (!cell) {
		cell = new CustomTableViewCell();
		cell->autorelease();

		CCSprite *sprite;
		if (idx == 2)
		{
			sprite = CCSprite::create("tablecell_my.png");
		}
		else
		{
			sprite = CCSprite::create("tablecell.png");
		}

		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(0, 0));
		cell->addChild(sprite);

		CCLabelBMFont *lbOrder = CCLabelBMFont::create(order->getCString(), "Mia_64.fnt");
		lbOrder->setScale(0.6f);
		lbOrder->setAlignment(kCCTextAlignmentLeft); //cocos2d::CCTextAlignment::
		lbOrder->setPosition(ccp(20, m_sprCell->getContentSize().height/2));
		lbOrder->setAnchorPoint(ccp(0.0f, 0.5f));
		lbOrder->setTag(100);
		cell->addChild(lbOrder);

		CCLabelBMFont *lbName = CCLabelBMFont::create(name->getCString(), "Mia_64.fnt");
		lbName->setScale(0.6f);
		lbName->setAlignment(kCCTextAlignmentRight); //cocos2d::CCTextAlignment::
		lbName->setPosition(ccp(m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));
		lbName->setAnchorPoint(ccp(1.0f, 0.5f));
		lbName->setTag(101);
		cell->addChild(lbName);

		CCLabelBMFont *lbScore = CCLabelBMFont::create(score->getCString(), "Mia_64.fnt");
		lbScore->setScale(0.6f);
		lbScore->setAlignment(kCCTextAlignmentCenter); //cocos2d::CCTextAlignment::
		lbScore->setPosition(ccp(m_sprCell->getContentSize().width/2, m_sprCell->getContentSize().height/2));
		lbScore->setAnchorPoint(ccp(0.5f, 0.5f));
		lbScore->setTag(102);
		cell->addChild(lbScore);
	}
	else
	{
		CCLabelBMFont *lbOrder = (CCLabelBMFont*)cell->getChildByTag(100);
		lbOrder->setString(order->getCString());
		if (idx == 0)
		{
			lbOrder->setString("My");
		}
		lbOrder->setPosition(ccp(20, m_sprCell->getContentSize().height/2));

		CCLabelBMFont *lbName = (CCLabelBMFont*)cell->getChildByTag(101);
		lbName->setString(name->getCString());
		lbName->setPosition(ccp(m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));

		CCLabelBMFont *lbScore = (CCLabelBMFont*)cell->getChildByTag(102);
		lbScore->setString(score->getCString());
		lbScore->setPosition(ccp(m_sprCell->getContentSize().width/2, m_sprCell->getContentSize().height/2));
	}


	return cell;
}

unsigned int ScoreScene::numberOfCellsInTableView(CCTableView *table)
{
	return LIST_SIZE;
}




//end new delegate