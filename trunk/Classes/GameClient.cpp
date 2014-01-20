#include "GameClient.h"
#include "DataManager.h"


GameClient* GameClient::s_instance = NULL;


GameClient::GameClient(void)
{
}


GameClient::~GameClient(void)
{
}


void GameClient::startGame()
{
	std::string username = DataManager::sharedDataManager()->GetUsername();
	if (username.compare("NULL") == 0) //signIn
	{
		signIn();
	}
	else //logIn
	{
		logIn();
	}
}

void GameClient::signIn()
{
	ParseClient* pc = ParseClient::sharedParseClient();

	float rd = CCRANDOM_0_1() * 1000;
	CCString* pass = CCString::createWithFormat("password_%f", rd);
	DataManager::sharedDataManager()->SetPassword(pass->getCString()); //////////////////////////////////////////////////////////////////////////
	CCString* json = CCString::createWithFormat("{\"name\":\"PhiCong\",\"password\":\"%s\"}", pass->getCString());
	pc->callCloudFunction("signIn", json->getCString(), httpresponse_selector(GameClient::signInCompleted), "signIn");
}

void GameClient::logIn()
{
	ParseClient* pc = ParseClient::sharedParseClient();

	std::string username = DataManager::sharedDataManager()->GetUsername();
	std::string password = DataManager::sharedDataManager()->GetPassword();
	CCString* json = CCString::createWithFormat("{\"username\":\"%s\",\"password\":\"%s\"}", username.c_str(), password.c_str());
	CCLOG("Call logIn: %s, %s", username.c_str(), password.c_str());
	pc->callCloudFunction("logIn", json->getCString(), httpresponse_selector(GameClient::logInCompleted), "logIn");
}

void GameClient::submitScore()
{
	//use MasterKey
	ParseClient* pc = ParseClient::sharedParseClient();
	int score = DataManager::sharedDataManager()->GetHighScore();
	CCLOG("Call submitScore: %d", score);
	std::string username = DataManager::sharedDataManager()->GetUsername();
	std::string password = DataManager::sharedDataManager()->GetPassword();
	CCString* json = CCString::createWithFormat("{\"username\":\"%s\",\"score\":%d}", username.c_str(), score);

	pc->callCloudFunction("submitScore", json->getCString(), httpresponse_selector(GameClient::submitScoreCompleted), "submitScore");
}

void GameClient::getLeaderboard()
{
	//use MasterKey
	ParseClient* pc = ParseClient::sharedParseClient();
	CCLOG("Call getLeaderboard");

	pc->callCloudFunction("getLeaderboard", "{}", httpresponse_selector(GameClient::getLeaderboardCompleted), "getLeaderboard");
}

void GameClient::signInCompleted( cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response )
{
	if (!response) return;

	//Show info
	CCLOG("-----------------BEGIN-------------------");
	CCLOG("SignIn completed");
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLog("SignIn Failed: %s", response->getErrorBuffer());
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		int ind1 = str.find_first_of("|");
		int ind2 = str.find_first_of("|", ind1 + 1);
		
		std::string username = str.substr(ind1 + 1, ind2 - ind1 - 1);
		CCLOG("username: %s", username.c_str());
		DataManager::sharedDataManager()->SetUsername(username.c_str()); //////////////////////////////////////////////////////////////////////////
	}
	
	CCLOG("-----------------END-------------------");
}

void GameClient::logInCompleted( cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response )
{
	if (!response) return;

	//Show info
	CCLOG("-----------------BEGIN-------------------");
	CCLOG("LogIn completed");
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request Failed: %s", response->getErrorBuffer());
		signIn();
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		int ind1 = str.find_first_of("|");
		int ind2 = str.find_first_of("|", ind1 + 1);

		std::string sscore = str.substr(ind1 + 1, ind2 - ind1 - 1);
		CCString* s = CCString::create(sscore);
		int score = s->intValue();
		CCLOG("score: %d", score);

		if (score != DataManager::sharedDataManager()->GetHighScore())
		{
			CCLOG("Score not match..., submit again");
			submitScore();
		}
	}

	CCLOG("-----------------END-------------------");
}

void GameClient::submitScoreCompleted( cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response )
{
	if (!response) return;

	//Show info
	CCLOG("-----------------BEGIN-------------------");
	CCLOG("SubmitScore completed");
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLog("Request Failed: %s", response->getErrorBuffer());
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		CCLOG("%s", str.c_str());
	}

	CCLOG("-----------------END-------------------");
}

void GameClient::getLeaderboardCompleted( cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response )
{
	if (!response) return;

	//Show info
	CCLOG("-----------------BEGIN-------------------");
	CCLOG("GetLeaderboard completed");
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLog("GetLeaderboard Failed: %s", response->getErrorBuffer());
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
		
		CCLOG("Leaderboard: %s", str.c_str());

		int ind1 = str.find_first_of("|");
		int ind2 = str.find_first_of("|", ind1 + 1);

		std::string username = str.substr(ind1 + 1, ind2 - ind1 - 1);
		CCLOG("username: %s", username.c_str());
		DataManager::sharedDataManager()->SetUsername(username.c_str()); //////////////////////////////////////////////////////////////////////////
	}

	CCLOG("-----------------END-------------------");
}

