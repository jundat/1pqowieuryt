#include "GameClientManager.h"

GameClientManager* GameClientManager::s_instance = NULL;

//

void GameClientManager::sendPlayerFbProfile( std::string fbId, std::string fbName, std::string email )
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(G_URL_SEND_PLAYER_FB_PROFILE);
	request->setTag("sendPlayerFbProfile");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onSendPlayerFbProfileCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("{ fbId: \"%s\", fbName: \"%s\", email: \"%s\" }",
		fbId.c_str(),
		fbName.c_str(),
		email.c_str());

	CCLOG("Data: %s", strData->getCString());
	request->setRequestData(strData->getCString(), strlen(strData->getCString()));


	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onSendPlayerFbProfileCompleted( CCHttpClient *sender, CCHttpResponse *response )
{
	if (!response || m_clientDelegate == NULL)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		m_clientDelegate->onSendPlayerFbProfileCompleted(false);
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
		
		CCLOG("Content: %s", str.c_str());
		m_clientDelegate->onSendPlayerFbProfileCompleted(true);
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

//

void GameClientManager::sendFriendList(std::string fbId, CCArray* arrFriends )
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(G_URL_SEND_FRIEND_LIST);
	request->setTag("sendFriendList");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onSendFriendListCompleted));

	// write the post data

	//parse arrFriendList to json
	std::string strFriendList = std::string("");
	int count = arrFriends->count();

	for (int i = 0; i < count; ++i)
	{
		FacebookAccount* fbFriend = (FacebookAccount*)arrFriends->objectAtIndex(i);

		strFriendList.append(fbFriend->toJson());
		if (i != count - 1)
		{
			strFriendList.append(",");
		}
	}	

	CCString* strData = CCString::createWithFormat("{ fbId: \"%s\", list: [%s]}",
		fbId.c_str(),
		strFriendList.c_str());

	CCLOG("Data: %s", strData->getCString());
	request->setRequestData(strData->getCString(), strlen(strData->getCString()));


	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onSendFriendListCompleted( CCHttpClient *sender, CCHttpResponse *response )
{
	if (!response || m_clientDelegate == NULL)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		m_clientDelegate->onSendFriendListCompleted(false);
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		CCLOG("Content: %s", str.c_str());
		m_clientDelegate->onSendFriendListCompleted(true);		
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

//

void GameClientManager::sendDeviceProfile( std::string fbId, std::string deviceId, std::string deviceToken, std::string deviceConfig, std::string devicePhoneNumber )
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(G_URL_SEND_DEVICE_PROFILE);
	request->setTag("sendDeviceProfile");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onSendDeviceProfileCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("{ fbId: \"%s\", deviceId: \"%s\", deviceToken: \"%s\", deviceConfig: \"%s\", devicePhoneNumber: \"%s\" }",
		fbId.c_str(),
		deviceId.c_str(),
		deviceToken.c_str(),
		deviceConfig.c_str(),
		devicePhoneNumber.c_str()
		);

	CCLOG("Data: %s", strData->getCString());
	request->setRequestData(strData->getCString(), strlen(strData->getCString()));


	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onSendDeviceProfileCompleted( CCHttpClient *sender, CCHttpResponse *response )
{
	if (!response || m_clientDelegate == NULL)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		m_clientDelegate->onSendDeviceProfileCompleted(false);
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		CCLOG("Content: %s", str.c_str());
		m_clientDelegate->onSendDeviceProfileCompleted(true);
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

//

void GameClientManager::sendScore( std::string appId, std::string fbId, int score, long time )
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(G_URL_SEND_SCORE);
	request->setTag("sendScore");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onSendScoreCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("{appId: \"%s\", fbId: \"%s\", score: %d, method: \"set\", time: %ld }",
		appId.c_str(),
		fbId.c_str(),
		score,
		time
		);

	CCLOG("Data: %s", strData->getCString());
	request->setRequestData(strData->getCString(), strlen(strData->getCString()));


	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onSendScoreCompleted( CCHttpClient *sender, CCHttpResponse *response )
{
	if (!response || m_clientDelegate == NULL)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		m_clientDelegate->onSendScoreCompleted(false);
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		CCLOG("Content: %s", str.c_str());
		m_clientDelegate->onSendScoreCompleted(true);
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

//
/*
{
	appId: "DBPTK"
	fbId: "123456789"
}
*/
void GameClientManager::getScore( std::string appId, std::string fbId )
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(G_URL_GET_SCORE);
	request->setTag("getScore");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetScoreCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("{appId: \"%s\", fbId: \"%s\", method: \"get\" }",
		appId.c_str(),
		fbId.c_str()
		);

	CCLOG("Data: %s", strData->getCString());
	request->setRequestData(strData->getCString(), strlen(strData->getCString()));


	CCHttpClient::getInstance()->send(request);
	request->release();
}

/*
{
	score: 1234
}
*/
void GameClientManager::_onGetScoreCompleted( CCHttpClient *sender, CCHttpResponse *response )
{
	if (!response || m_clientDelegate == NULL)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		m_clientDelegate->onGetScoreCompleted(false, -1);
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		CCLOG("Content: %s", str.c_str());

		//get score from response
		json_t *root;
		json_error_t error;
		json_t *result;

		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		result = json_object_get(root, "score");
		m_clientDelegate->onGetScoreCompleted(true, (int)json_integer_value(result));
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

//
/*
{
	appId: "DBPTK"
	fbId: "123456789"
}
*/
void GameClientManager::getFriendLeaderboard( std::string appId, std::string fbId )
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(G_URL_GET_FRIEND_LEADERBOARD);
	request->setTag("getFriendLeaderboard");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetFriendLeaderboardCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("{appId: \"%s\", fbId: \"%s\" }",
		appId.c_str(),
		fbId.c_str()
		);

	CCLOG("Data: %s", strData->getCString());
	request->setRequestData(strData->getCString(), strlen(strData->getCString()));


	CCHttpClient::getInstance()->send(request);
	request->release();
}

/*
{
	list:
	[
		{ 
			fbId: "12345", 
			score: 1234
		},
		{ 
			fbId: "97848", 
			score: 1234
		}
		//...
	]
}
*/
void GameClientManager::_onGetFriendLeaderboardCompleted( CCHttpClient *sender, CCHttpResponse *response )
{
	if (!response || m_clientDelegate == NULL)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		m_clientDelegate->onGetFriendLeaderboardCompleted(false, NULL);
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		CCLOG("Content: %s", str.c_str());

		//get score from response
		json_t *root;
		json_error_t error;
		json_t *friendList;

		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		friendList = json_object_get(root, "list");

		//foreach to get all friend, insert to list
		int count = json_array_size(friendList);
		CCArray* arrHighScore = new CCArray();
		arrHighScore->retain();

		for(int i = 0; i < count; i++)
		{
			json_t *fbFriend = json_array_get(friendList, i);

			json_t* fbId;
			json_t* score;

			fbId = json_object_get(fbFriend, "fbId");
			score = json_object_get(fbFriend, "score");

			FacebookAccount* acc = new FacebookAccount(json_string_value(fbId), std::string(""), (int)json_integer_value(score));
			arrHighScore->addObject(acc);
		}

		m_clientDelegate->onGetFriendLeaderboardCompleted(true, arrHighScore);
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

//
/*
{
	appId: "DBPTK"
	fbId: "123456789"
}
*/
void GameClientManager::getWorldLeaderboard( std::string appId, std::string fbId )
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(G_URL_GET_WORLD_LEADERBOARD);
	request->setTag("getWorldLeaderboard");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetWorldLeaderboardCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("{appId: \"%s\", fbId: \"%s\" }",
		appId.c_str(),
		fbId.c_str()
		);

	CCLOG("Data: %s", strData->getCString());
	request->setRequestData(strData->getCString(), strlen(strData->getCString()));


	CCHttpClient::getInstance()->send(request);
	request->release();
}

/*
{
	list:
	[
		{ 
			fbId: "12345", 
			score: 1234
		},
		{ 
			fbId: "97848", 
			score: 1234
		}
		//...
	]
}
*/
void GameClientManager::_onGetWorldLeaderboardCompleted( CCHttpClient *sender, CCHttpResponse *response )
{
	if (!response || m_clientDelegate == NULL)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		m_clientDelegate->onGetWorldLeaderboardCompleted(false, NULL);
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		CCLOG("Content: %s", str.c_str());

		//get score from response
		json_t *root;
		json_error_t error;
		json_t *friendList;

		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		friendList = json_object_get(root, "list");

		//foreach to get all friend, insert to list
		int count = json_array_size(friendList);
		CCArray* arrHighScore = new CCArray();
		arrHighScore->retain();

		for(int i = 0; i < count; i++)
		{
			json_t *fbFriend = json_array_get(friendList, i);

			json_t* fbId;
			json_t* score;

			fbId = json_object_get(fbFriend, "fbId");
			score = json_object_get(fbFriend, "score");

			FacebookAccount* acc = new FacebookAccount(json_string_value(fbId), std::string(""), (int)json_integer_value(score));
			arrHighScore->addObject(acc);
		}

		m_clientDelegate->onGetWorldLeaderboardCompleted(true, arrHighScore);
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}


