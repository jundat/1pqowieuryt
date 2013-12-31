#include "AppDelegate.h"
#include "Global.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "DataManager.h"
#include "ParseClient.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() 
{

}

AppDelegate::~AppDelegate() 
{
}


void AppDelegate::initgame()
{
	ParseClient* pc = ParseClient::sharedParseClient();

	std::string username = DataManager::sharedDataManager()->GetUsername();
	if (username.compare("NULL") == 0) //signIn
	{
		float rd = CCRANDOM_0_1() * 1000;
		CCString* pass = CCString::createWithFormat("password_%f", rd);
		DataManager::sharedDataManager()->SetPassword(pass->getCString()); //////////////////////////////////////////////////////////////////////////
		CCString* json = CCString::createWithFormat("{\"name\":\"PLAYER\",\"password\":\"%s\"}", pass->getCString());
		
		pc->callCloudFunction("signIn", json->getCString(), httpresponse_selector(AppDelegate::onHttpRequestCompleted), "signIn");
	}
	else //logIn
	{
		std::string username = DataManager::sharedDataManager()->GetUsername();
		std::string password = DataManager::sharedDataManager()->GetPassword();
		CCString* json = CCString::createWithFormat("{\"username\":\"%s\",\"password\":\"%s\"}", username.c_str(), password.c_str());
		CCLOG("Call logIn: %s, %s", username.c_str(), password.c_str());
		pc->callCloudFunction("logIn", json->getCString(), httpresponse_selector(AppDelegate::onHttpRequestCompleted), "logIn");
	}
}


void AppDelegate::onHttpRequestCompleted( cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response )
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
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
		CCLOG("Content: %s", str.c_str());
		processMessage(str);
	}

	CCLOG("-----------------END-------------------");
}


void AppDelegate::processMessage( std::string str )
{
	CCLOG("-----------PROCESS MESSAGE-------------");
	
	int ind1 = str.find_first_of("|");
	int ind2 = str.find_first_of("|", ind1 + 1);
	std::string smallStr = str.substr(ind1 + 1, ind2 - ind1 - 1);
	CCLOG(smallStr.c_str());

	if (smallStr.compare("signIn") == 0) //signIn
	{
		CCLOG("THIS IS SIGN IN");
		ind1 = ind2;
		ind2 = str.find_first_of("|", ind2 + 1);
		std::string username = str.substr(ind1 + 1, ind2 - ind1 - 1);
		CCLOG("USERNAME: %s", username.c_str());
		DataManager::sharedDataManager()->SetUsername(username.c_str()); //////////////////////////////////////////////////////////////////////////
	} 
	else if (smallStr.compare("logIn") == 0) //logIn
	{
		CCLOG("THIS IS LOG IN");
		ind1 = ind2;
		ind2 = str.find_first_of("|", ind2 + 1);
		std::string sscore = str.substr(ind1 + 1, ind2 - ind1 - 1);
		CCString* s = CCString::create(sscore);
		int score = s->intValue();
		CCLOG("SCORE: %d", score);

		if (score != DataManager::sharedDataManager()->GetCurrenHighScore())
		{
			submitScore();
		}
	}
	else if (smallStr.compare("submitScore") == 0) //submitScore
	{
		CCLOG("THIS IS SUBMIT SCORE");
		CCLOG(str.c_str());
	}
}


void AppDelegate::submitScore()
{
	/************************************************************************/
	/* 
	Còn 1 vài lỗi khi update score, cần fix theo cách sau:
	C1: sửa lệnh POST bằng PUT
	C2: tạo 1 table riêng để quản lý score, khi đó thì vẫn dùng lệnh POST
	*/
	/************************************************************************/

	ParseClient* pc = ParseClient::sharedParseClient();
	pc->setRequestType(CCHttpRequest::kHttpPut);
	int score = DataManager::sharedDataManager()->GetCurrenHighScore();
	CCLOG("Call submitScore: %d", score);
	std::string username = DataManager::sharedDataManager()->GetUsername();
	std::string password = DataManager::sharedDataManager()->GetPassword();
	CCString* json = CCString::createWithFormat("{\"username\":\"%s\",\"score\":%d}", username.c_str(), password.c_str(), score);

	pc->callCloudFunction("submitScore", json->getCString(), httpresponse_selector(AppDelegate::onHttpRequestCompleted), "submitScore");
}


bool AppDelegate::applicationDidFinishLaunching() {
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

#ifdef WIN32
	pEGLView->setFrameSize(G_DESIGN_WIDTH, G_DESIGN_HEIGHT);
	pEGLView->setFrameZoomFactor(G_SCALE_FACTOR);
#endif

    pDirector->setOpenGLView(pEGLView);
    pDirector->setAnimationInterval(1.0 / 60);
	pEGLView->setDesignResolutionSize(800, 1280, kResolutionExactFit);

    CCScene *pScene = MenuScene::scene(); //  MainGameScene::scene(); //
    pDirector->runWithScene(pScene);

	initgame();

    return true;
}


void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}


void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
