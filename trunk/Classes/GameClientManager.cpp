#include "GameClientManager.h"
#include "DataManager.h"
#include "Base64.h"
#include "Md5.h"

#define ENCODE_BUFFER_SIZE		2048

GameClientManager* GameClientManager::s_instance = NULL;

string GameClientManager::s_urlProfile = string("");
string GameClientManager::s_urlDevice = string("");
string GameClientManager::s_urlFriend = string("");
string GameClientManager::s_urlScore = string("");

//

void GameClientManager::setUrls( string urlProfile, string urlDevice, string urlFriend, string urlScore )
{
	CCLOG("SET URL");
	GameClientManager::s_urlProfile = string(urlProfile);
	GameClientManager::s_urlDevice = string(urlDevice);
	GameClientManager::s_urlFriend = string(urlFriend);
	GameClientManager::s_urlScore = string(urlScore);
}

std::string GameClientManager::encodeBeforeSend( std::string src )
{
	CCLOG("BEFORE:\n%s", src.c_str());
	
	if (G_IS_ENCODE)
	{
		int len = src.length();
		unsigned char m_Test[ENCODE_BUFFER_SIZE];
		std::copy(src.c_str(), src.c_str() + len, m_Test);
		string result = "data=";
		result.append(Base64::encode(m_Test, len));

		CCLOG("AFTER:\n%s", result.c_str());

		return result;
	}
	else
	{
		string result = "data=";
		result.append(src);

		CCLOG("AFTER:\n%s", result.c_str());

		return result;
	}
}

std::string GameClientManager::decodeBeforeProcess( std::string src )
{
	if (G_IS_ENCODE)
	{
		return Base64::decode(src);
	} 
	else
	{
		return src;
	}
}


std::string GameClientManager::getMD5()
{
	string srcMd5 = "";
	srcMd5.append(G_APP_ID);
	srcMd5.append(KEY); //Key
	srcMd5.append(DataManager::sharedDataManager()->GetFbID());
	srcMd5.append(""); //meId
	
	CCLOG("SRC MD5: %s", srcMd5.c_str());
	
	string md5 = MD5::createMd5(srcMd5);

	CCLOG("DEST MD5: %s", md5.c_str());

	return md5;
}


void GameClientManager::sendPlayerFbProfile( std::string fbId, std::string fbName, std::string email, string appId )
{
	CCAssert(s_urlProfile.length() > 0, "Not set s_urlProfile yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(s_urlProfile.c_str());
	request->setTag("sendPlayerFbProfile");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onSendPlayerFbProfileCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat(
"{ method: \"set\", data: { fbId: \"%s\", fbName: \"%s\", email: \"%s\", appId: \"%s\" }, sign: \"%s\", appId: \"%s\" }",
		fbId.c_str(),
		fbName.c_str(),
		email.c_str(),
		G_APP_ID,
		getMD5().c_str(),
		G_APP_ID);

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
		
		str = decodeBeforeProcess(str);

		CCLOG("Content: %s", str.c_str());
		m_clientDelegate->onSendPlayerFbProfileCompleted(true);
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

void GameClientManager::getPlayerFbProfile(std::string fbId )
{
	CCAssert(s_urlProfile.length() > 0, "Not set s_urlProfile yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(s_urlProfile.c_str());
	request->setTag("getPlayerFbProfile");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetPlayerFbProfileCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat(
		"{ method: \"get\", data: { fbId: \"%s\" }, sign: \"%s\", appId: \"%s\" }",
		fbId.c_str(),
		getMD5().c_str(),
		G_APP_ID);

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

		str = decodeBeforeProcess(str);

		CCLOG("Content: %s", str.c_str());

		//get score from response
		json_t *root;
		json_error_t error;
		json_t *fbId;
		json_t *fbName;
		json_t *email;
		//json_t *score;


		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		fbId = json_object_get(root, "fbId");
		fbName = json_object_get(root, "fbName");
		email = json_object_get(root, "email");
		//score = json_object_get(root, "score");

		FacebookAccount* acc = new FacebookAccount(json_string_value(fbId), json_string_value(fbName), std::string(json_string_value(email)), -1);

		m_clientDelegate->onGetPlayerFbProfileCompleted(true, acc);

	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

//

void GameClientManager::sendFriendList(std::string fbId, CCArray* arrFriends )
{
	CCAssert(s_urlFriend.length() > 0, "Not set s_urlFriend yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(s_urlFriend.c_str());
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
		strFriendList.append(fbFriend->m_fbId);
		strFriendList.append("\"");
		if (i != count - 1)
		{
			strFriendList.append(",");
		}
	}

	//////////////////////////////////////////////////////////////////////////
	
	// write the post data
	CCString* strData = CCString::createWithFormat(
		"{ method: \"set\", data: { fbId: \"%s\", list: [%s] }, sign: \"%s\", appId: \"%s\" }",
		fbId.c_str(),
		strFriendList.c_str(),
		getMD5().c_str(),
		G_APP_ID);

	//////////////////////////////////////////////////////////////////////////

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

		str = decodeBeforeProcess(str);

		CCLOG("Content: %s", str.c_str());
		m_clientDelegate->onSendFriendListCompleted(true);		
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

void GameClientManager::getFriendList( std::string appId, std::string fbId )
{
	CCAssert(s_urlFriend.length() > 0, "Not set s_urlFriend yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(s_urlFriend.c_str());
	request->setTag("getFriendList");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetFriendListCompleted));

	//////////////////////////////////////////////////////////////////////////
	
	// write the post data
	CCString* strData = CCString::createWithFormat(
		"{ method: \"get\", data: { fbId: \"%s\", appId: \"%s\" }, sign: \"%s\", appId: \"%s\" }",
		fbId.c_str(),
		G_APP_ID,
		getMD5().c_str(),
		G_APP_ID);

	//////////////////////////////////////////////////////////////////////////

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

		str = decodeBeforeProcess(str);

		CCLOG("Content: %s", str.c_str());

		//get score from response
		json_t *root;
		json_error_t error;
		json_t *friendList;

		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		friendList = json_object_get(root, "list");

		//foreach to get all friend, insert to list
		int count = json_array_size(friendList);
		CCArray* arrFriends = CCArray::create();
		//arrFriends->retain();

		for(int i = 0; i < count; i++)
		{
			json_t *fbFriend = json_array_get(friendList, i);

			json_t* fbId;
			json_t* fbName;
			//json_t* email;
			json_t* score;


			fbId = json_object_get(fbFriend, "id");
			fbName = json_object_get(fbFriend, "fbName");
			//email = json_object_get(fbFriend, "email");
			score = json_object_get(fbFriend, "score");

			FacebookAccount* acc = new FacebookAccount(json_string_value(fbId), json_string_value(fbName), std::string(), (int)atof(json_string_value(score)));
			arrFriends->addObject(acc);
		}

		//GameClientManager::SortFriendScore(arrFriends);
		m_clientDelegate->onGetFriendListCompleted(true, arrFriends);
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

//

void GameClientManager::sendDeviceProfile( std::string fbId, std::string deviceId, std::string deviceToken, std::string deviceConfig, std::string devicePhoneNumber )
{
	CCAssert(s_urlDevice.length() > 0, "Not set s_urlDevice yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(s_urlDevice.c_str());
	request->setTag("sendDeviceProfile");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onSendDeviceProfileCompleted));

	//////////////////////////////////////////////////////////////////////////

	// write the post data
	CCString* strData = CCString::createWithFormat(
"{ method: \"set\", data: { fbId: \"%s\", deviceId: \"%s\", deviceToken: \"%s\", deviceConfig: \"%s\", devicePhoneNumber: \"%s\" }, sign: \"%s\", appId: \"%s\" }",
		fbId.c_str(),
		deviceId.c_str(),
		deviceToken.c_str(),
		deviceConfig.c_str(),
		devicePhoneNumber.c_str(),
		getMD5().c_str(),
		G_APP_ID);

	//////////////////////////////////////////////////////////////////////////

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

		str = decodeBeforeProcess(str);

		CCLOG("Content: %s", str.c_str());
		m_clientDelegate->onSendDeviceProfileCompleted(true);
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

void GameClientManager::getDeviceProfile(std::string fbId, std::string deviceId )
{
	CCAssert(s_urlDevice.length() > 0, "Not set s_urlDevice yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(s_urlDevice.c_str());
	request->setTag("getDeviceProfile");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetDeviceProfileCompleted));

	// write the post data
	CCString* strData = CCString::createWithFormat(
		"{ method: \"get\", data: { fbId: \"%s\", deviceId: \"%s\" }, sign: \"%s\", appId: \"%s\" }",
		fbId.c_str(),
		deviceId.c_str(),
		getMD5().c_str(),
		G_APP_ID);


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

		str = decodeBeforeProcess(str);

		CCLOG("Content: %s", str.c_str());

		//get score from response
		json_t *root;
		json_error_t error;
		json_t *fbId;
		json_t *deviceId;
		json_t *deviceToken;
		json_t *devicePhoneNumber;
		json_t *deviceConfig;

		root = json_loads(str.c_str(), strlen(str.c_str()), &error);

		fbId = json_object_get(root, "fbId");
		deviceId = json_object_get(root, "deviceId");
		deviceToken = json_object_get(root, "deviceToken");
		deviceConfig = json_object_get(root, "deviceConfig");
		devicePhoneNumber = json_object_get(root, "devicePhoneNumber");

		DeviceProfile* acc = new DeviceProfile(
			json_string_value(deviceId), 
			json_string_value(deviceToken), 
			json_string_value(deviceConfig),
			json_string_value(devicePhoneNumber));

		m_clientDelegate->onGetDeviceProfileCompleted(true, acc);

	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

//

void GameClientManager::sendScore( std::string appId, std::string fbId, int score )
{
	CCLOG("URL: %s", s_urlScore.c_str());
	CCAssert(s_urlScore.length() > 0, "Not set s_urlScore yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(s_urlScore.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setTag("sendScore");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onSendScoreCompleted));


	// write the post data
	CCString* strData = CCString::createWithFormat(
		"{ method: \"set\", data: { appId: \"%s\", fbId: \"%s\", score: \"%d\" }, sign: \"%s\", appId: \"%s\" }",
		G_APP_ID,
		fbId.c_str(),
		score,
		getMD5().c_str(),
		G_APP_ID);


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
		m_clientDelegate->onSendScoreCompleted(false, -1);
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		str = decodeBeforeProcess(str);

		CCLOG("Content: %s", str.c_str());

		//get score from response
		json_t *root;
		json_error_t error;
		json_t *isSuccess;
		json_t *score;

		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		isSuccess = json_object_get(root, "isSuccess");
		bool success = CCString::create(json_string_value(isSuccess))->boolValue();

		if (success)
		{
			m_clientDelegate->onSendScoreCompleted(success, DataManager::sharedDataManager()->GetHighScore());
		} 
		else
		{
			score = json_object_get(root, "score");
			m_clientDelegate->onSendScoreCompleted(success, (int)atof(json_string_value(score)));
		}
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

void GameClientManager::getScore( std::string appId, std::string fbId )
{
	CCAssert(s_urlScore.length() > 0, "Not set s_urlScore yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setUrl(s_urlScore.c_str());
	request->setTag("getScore");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetScoreCompleted));
	
	// write the post data
	CCString* strData = CCString::createWithFormat(
		"{ method: \"get\", data: { appId: \"%s\", fbId: \"%s\" }, sign: \"%s\", appId: \"%s\" }",
		G_APP_ID,
		fbId.c_str(),
		getMD5().c_str(),
		G_APP_ID);


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
		m_clientDelegate->onGetScoreCompleted(false, -1, std::string());
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		str = decodeBeforeProcess(str);

		CCLOG("Content: %s", str.c_str());

		//get score from response
		json_t *root;
		json_error_t error;
		json_t *score;
		json_t *time;

		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		score = json_object_get(root, "score");
		time = json_object_get(root, "time");

		m_clientDelegate->onGetScoreCompleted(true, (int)atof(json_string_value(score)), json_string_value(time));
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}
