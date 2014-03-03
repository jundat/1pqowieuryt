#include "GameClientManager.h"

GameClientManager* GameClientManager::s_instance = NULL;

//

void GameClientManager::sendPlayerFbProfile( std::string fbId, std::string fbName, std::string email )
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(G_URL_PLAYER_FB_PROFILE.c_str());
	request->setTag("sendPlayerFbProfile");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onSendPlayerFbProfileCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("data={ fbId: \"%s\", fbName: \"%s\", email: \"%s\" }&method=set",
		fbId.c_str(),
		fbName.c_str(),
		email.c_str());

	CCLOG("Data: %s", strData->getCString());
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));


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

	request->setUrl(G_URL_FRIEND_LIST.c_str());
	request->setTag("sendFriendList");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onSendFriendListCompleted));

	// write the post data

	//parse arrFriendList to json
	std::string strFriendList = std::string("");
	int count = arrFriends->count();

	for (int i = 0; i < count; ++i)
	{
		FacebookAccount* fbFriend = (FacebookAccount*)arrFriends->objectAtIndex(i);

		strFriendList.append("\"");
		strFriendList.append(fbFriend->fbId);
		strFriendList.append("\"");
		if (i != count - 1)
		{
			strFriendList.append(",");
		}
	}	

	CCString* strData = CCString::createWithFormat("data={ fbId: \"%s\", list: [%s]}&method=set",
		fbId.c_str(),
		strFriendList.c_str());


	CCLOG("Data: %s", strData->getCString());
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));


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

	request->setUrl(G_URL_DEVICE_PROFILE.c_str());
	request->setTag("sendDeviceProfile");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onSendDeviceProfileCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("data={ fbId: \"%s\", deviceId: \"%s\", deviceToken: \"%s\", deviceConfig: \"%s\", devicePhoneNumber: \"%s\" }&method=set",
		fbId.c_str(),
		deviceId.c_str(),
		deviceToken.c_str(),
		deviceConfig.c_str(),
		devicePhoneNumber.c_str()
		);


	CCLOG("Data: %s", strData->getCString());
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));


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

	request->setUrl(G_URL_SCORE.c_str());
	request->setTag("sendScore");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onSendScoreCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("data={appId: \"%s\", fbId: \"%s\", score: %d, time: %ld }&method=set",
		appId.c_str(),
		fbId.c_str(),
		score,
		time
		);


	CCLOG("Data: %s", strData->getCString());
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));


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

void GameClientManager::getScore( std::string appId, std::string fbId )
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(G_URL_SCORE.c_str());
	request->setTag("getScore");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetScoreCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("data={appId: \"%s\", fbId: \"%s\" }&method=get",
		appId.c_str(),
		fbId.c_str()
		);


	CCLOG("Data: %s", strData->getCString());
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));


	CCHttpClient::getInstance()->send(request);
	request->release();
}

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

void GameClientManager::getFriendList( std::string appId, std::string fbId )
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(G_URL_FRIEND_LIST.c_str());
	request->setTag("getFriendList");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetFriendListCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("data={appId: \"%s\", fbId: \"%s\" }&method=get",
		appId.c_str(),
		fbId.c_str()
		);


	CCLOG("Data: %s", strData->getCString());
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));


	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onGetFriendListCompleted( CCHttpClient *sender, CCHttpResponse *response )
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
		m_clientDelegate->onGetFriendListCompleted(false, NULL);
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
		CCArray* arrFriends = new CCArray();
		arrFriends->retain();

		for(int i = 0; i < count; i++)
		{
			json_t *fbFriend = json_array_get(friendList, i);

			json_t* fbId;
			json_t* fbName;
			json_t* score;


			fbId = json_object_get(fbFriend, "fbId");
			fbName = json_object_get(fbFriend, "fbName");
			score = json_object_get(fbFriend, "score");

			FacebookAccount* acc = new FacebookAccount(json_string_value(fbId), json_string_value(fbName), (int)json_integer_value(score));
			arrFriends->addObject(acc);
		}

		m_clientDelegate->onGetFriendListCompleted(true, arrFriends);
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

//

void GameClientManager::getPlayerFbProfile(std::string fbId )
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(G_URL_PLAYER_FB_PROFILE.c_str());
	request->setTag("getPlayerFbProfile");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetPlayerFbProfileCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("data={ fbId: \"%s\" }&method=get",
		fbId.c_str()
		);


	CCLOG("Data: %s", strData->getCString());
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));


	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onGetPlayerFbProfileCompleted( CCHttpClient *sender, CCHttpResponse *response )
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
		m_clientDelegate->onGetPlayerFbProfileCompleted(false, NULL);
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		CCLOG("Content: %s", str.c_str());

		//get score from response
// 		json_t *root;
// 		json_error_t error;
// 		json_t *friendList;
// 
// 		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
// 		friendList = json_object_get(root, "list");
// 
// 		//foreach to get all friend, insert to list
// 		int count = json_array_size(friendList);
// 		CCArray* arrFriends = new CCArray();
// 		arrFriends->retain();
// 
// 		for(int i = 0; i < count; i++)
// 		{
// 			json_t *fbFriend = json_array_get(friendList, i);
// 
// 			json_t* fbId;
// 			json_t* fbName;
// 			json_t* score;
// 
// 
// 			fbId = json_object_get(fbFriend, "fbId");
// 			fbName = json_object_get(fbFriend, "fbName");
// 			score = json_object_get(fbFriend, "score");
// 
// 			FacebookAccount* acc = new FacebookAccount(json_string_value(fbId), json_string_value(fbName), (int)json_integer_value(score));
// 			arrFriends->addObject(acc);
// 		}
// 
// 		m_clientDelegate->onGetPlayerFbProfileCompleted(true, arrFriends);

	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}



void GameClientManager::getDeviceProfile(std::string fbId )
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(G_URL_DEVICE_PROFILE.c_str());
	request->setTag("getDeviceProfile");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetDeviceProfileCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat("data={ fbId: \"%s\" }&method=get",
		fbId.c_str()
		);


	CCLOG("Data: %s", strData->getCString());
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));


	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onGetDeviceProfileCompleted( CCHttpClient *sender, CCHttpResponse *response )
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
		m_clientDelegate->onGetDeviceProfileCompleted(false, NULL);
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		CCLOG("Content: %s", str.c_str());

		//get score from response
		// 		json_t *root;
		// 		json_error_t error;
		// 		json_t *friendList;
		// 
		// 		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		// 		friendList = json_object_get(root, "list");
		// 
		// 		//foreach to get all friend, insert to list
		// 		int count = json_array_size(friendList);
		// 		CCArray* arrFriends = new CCArray();
		// 		arrFriends->retain();
		// 
		// 		for(int i = 0; i < count; i++)
		// 		{
		// 			json_t *fbFriend = json_array_get(friendList, i);
		// 
		// 			json_t* fbId;
		// 			json_t* fbName;
		// 			json_t* score;
		// 
		// 
		// 			fbId = json_object_get(fbFriend, "fbId");
		// 			fbName = json_object_get(fbFriend, "fbName");
		// 			score = json_object_get(fbFriend, "score");
		// 
		// 			FacebookAccount* acc = new FacebookAccount(json_string_value(fbId), json_string_value(fbName), (int)json_integer_value(score));
		// 			arrFriends->addObject(acc);
		// 		}
		// 
		// 		m_clientDelegate->onGetPlayerFbProfileCompleted(true, arrFriends);

	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}




