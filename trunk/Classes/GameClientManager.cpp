#include "GameClientManager.h"
#include "DataManager.h"
#include "Base64.h"
#include "Md5.h"

#define ENCODE_BUFFER_SIZE		8192//4096

GameClientManager* GameClientManager::s_instance = NULL;

string GameClientManager::s_urlProfile = string("");
string GameClientManager::s_urlDevice = string("");
string GameClientManager::s_urlFriend = string("");
string GameClientManager::s_urlScore = string("");

//

void GameClientManager::setUrls( string urlProfile, string urlDevice, string urlFriend, string urlScore )
{
	GameClientManager::s_urlProfile = string(urlProfile);
	GameClientManager::s_urlDevice = string(urlDevice);
	GameClientManager::s_urlFriend = string(urlFriend);
	GameClientManager::s_urlScore = string(urlScore);
}

std::string GameClientManager::encodeBeforeSend( std::string src )
{
	//CCLOG("BEFORE:\n%s", src.c_str());
    
	if (G_IS_ENCODE)
	{
		int len = src.length();
		unsigned char m_Test[ENCODE_BUFFER_SIZE];
		std::copy(src.c_str(), src.c_str() + len, m_Test);
		string result = "data=";
		result.append(Base64::encode(m_Test, len));
		//CCLOG("AFTER:\n%s", result.c_str());

		return result;
	}
	else
	{
		string result = "data=";
		result.append(src);

		//CCLOG("AFTER:\n%s", result.c_str());

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
    static string md5 = "";
    
    if (md5.empty()) {
        
        string srcMd5 = "";
        srcMd5.append(G_APP_ID);
        srcMd5.append(KEY); //Key
        srcMd5.append(DataManager::sharedDataManager()->GetFbID());
        srcMd5.append(""); //meId
        
        CCLOG("SRC MD5: %s", srcMd5.c_str());
        
        md5 = MD5::createMd5(srcMd5);
        
        //CCLOG("DEST MD5: %s", md5.c_str());
    }
    
    CCLOG("MD5: %s", md5.c_str());
    
	return md5;
}


void GameClientManager::sendPlayerFbProfile( std::string fbId, std::string fbName, std::string email)
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
	if (!response)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onSendPlayerFbProfileCompleted(false);
		}
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
		
		str = decodeBeforeProcess(str);

		CCLOG("Content: %s", str.c_str());
		if (m_clientDelegate)
		{
			m_clientDelegate->onSendPlayerFbProfileCompleted(true);
		}
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
	if (!response)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onGetPlayerFbProfileCompleted(false, NULL);
		}
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
        json_t *coin;


		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		fbId = json_object_get(root, "fbId");
		fbName = json_object_get(root, "fbName");
		email = json_object_get(root, "email");
		coin = json_object_get(root, "coin");

        //(string _fbId, string _fbName, string _email, int _score, int _coin, long timeGetLaze, long timeSendLife)
        
		FacebookAccount* acc = new FacebookAccount(
           json_string_value(fbId),
           json_string_value(fbName),
           std::string(json_string_value(email)),
           -1,
           (int)atof(json_string_value(coin)),
           -1,
           -1);

		if (m_clientDelegate)
		{
			m_clientDelegate->onGetPlayerFbProfileCompleted(true, acc);
		}		
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
    CCLOG("1");
	CCString* strData = CCString::createWithFormat(
		"{ method: \"set\", data: { fbId: \"%s\", list: [%s] }, sign: \"%s\", appId: \"%s\" }",
		fbId.c_str(),
		strFriendList.c_str(),
		getMD5().c_str(),
		G_APP_ID);
    
    CCLOG("2");
	CCLOG("%s", strData->getCString());
	//////////////////////////////////////////////////////////////////////////
    CCLOG("3");
    static int c = 1;
    c++;
    CCLOG("COUNT: %d", c);
	std::string s = encodeBeforeSend(strData->getCString());
    CCLOG("4");
	request->setRequestData(s.c_str(), strlen(s.c_str()));
    CCLOG("5");

	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onSendFriendListCompleted( CCHttpClient *sender, CCHttpResponse *response )
{
	if (!response)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onSendFriendListCompleted(false);
		}
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		str = decodeBeforeProcess(str);

		CCLOG("Content: %s", str.c_str());
		if (m_clientDelegate)
		{
			m_clientDelegate->onSendFriendListCompleted(true);	
		}			
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

void GameClientManager::getFriendList(std::string fbId )
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
	if (!response)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onGetFriendListCompleted(false, NULL);
		}
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
        json_t *serverTime;
		json_t *friendList;

		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
        serverTime = json_object_get(root, "time");
		friendList = json_object_get(root, "list");

		//foreach to get all friend, insert to list
		int count = json_array_size(friendList);
		CCArray* arrFriends = CCArray::create();
        
        
        long _serverTime = ((long long)atoll(json_string_value(serverTime)) / 1000);
        long _clientTime = static_cast<long int> (time(NULL));

		for(int i = 0; i < count; i++)
		{
			json_t *fbFriend = json_array_get(friendList, i);

			json_t* fbId;
			json_t* fbName;
			json_t* timeGetLaze;
            json_t* timeSendLife;
			json_t* score;


			fbId = json_object_get(fbFriend, "id");
			fbName = json_object_get(fbFriend, "fbName");
			timeGetLaze = json_object_get(fbFriend, "timeLaze");
            timeSendLife = json_object_get(fbFriend, "timeSendLife");
			score = json_object_get(fbFriend, "score");
            
            long _timeGetLaze = _clientTime - ( _serverTime - ((long long)atoll(json_string_value(timeGetLaze)) / 1000));
            long _timeSendLife = _clientTime - ( _serverTime - ((long long)atoll(json_string_value(timeSendLife)) / 1000));
            
            CCLOG("Time_Get_Laze: %ld", _timeGetLaze);
            CCLOG("Time_Send_Life: %ld", _timeSendLife);

            //(string _fbId, string _fbName, string _email, int _score, int _coin, long timeGetLaze, long timeSendLife)
            
			FacebookAccount* acc = new FacebookAccount(json_string_value(fbId),
                                                       json_string_value(fbName),
                                                       std::string(""),
                                                       (int)atoi(json_string_value(score)),
                                                       -1,
                                                       _timeGetLaze,
                                                       _timeSendLife
                                                       );
			arrFriends->addObject(acc);
		}

		if (m_clientDelegate)
		{
			m_clientDelegate->onGetFriendListCompleted(true, arrFriends);
		}
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
	if (!response)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onSendDeviceProfileCompleted(false);
		}
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		str = decodeBeforeProcess(str);

		CCLOG("Content: %s", str.c_str());
		if (m_clientDelegate)
		{
			m_clientDelegate->onSendDeviceProfileCompleted(true);
		}
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
	if (!response)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onGetDeviceProfileCompleted(false, NULL);
		}
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

		if (m_clientDelegate)
		{
			m_clientDelegate->onGetDeviceProfileCompleted(true, acc);
		}
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

//

void GameClientManager::sendScore(std::string fbId, int score )
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
	if (!response)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onSendScoreCompleted(false, -1);
		}
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

		if (success == true)
		{
			if (m_clientDelegate)
			{
				m_clientDelegate->onSendScoreCompleted(success, DataManager::sharedDataManager()->GetHighScore());
			}
		} 
		else //false, get new score from server
		{
			score = json_object_get(root, "score");
			if (m_clientDelegate)
			{
				m_clientDelegate->onSendScoreCompleted(success, (int)atof(json_string_value(score)));
			}
		}
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

void GameClientManager::getScore( std::string fbId )
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
	if (!response)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onGetScoreCompleted(false,
                                                  DataManager::sharedDataManager()->GetHighScore(), "0");
		}
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());

		str = decodeBeforeProcess(str);

		CCLOG("Content: %s", str.c_str());
        
        if (str.length() == 0) {
            CCLOG("Request failed: %s", response->getErrorBuffer());
            if (m_clientDelegate)
            {
                m_clientDelegate->onGetScoreCompleted(
                                                      false,
                                                      DataManager::sharedDataManager()->GetHighScore(), "0");
            }
        } else {

            //get score from response
            json_t *root;
            json_error_t error;
            json_t *score;
            json_t *time;

            root = json_loads(str.c_str(), strlen(str.c_str()), &error);
            score = json_object_get(root, "score");
            time = json_object_get(root, "time");

            if (m_clientDelegate)
            {
                m_clientDelegate->onGetScoreCompleted(true, (int)atof(json_string_value(score)), json_string_value(time));
            }
        }
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}


//request to check diamond and charge with diamond

void GameClientManager::requestRevive( string fbId )
{
	string sUrl = string(G_URL_REVIVE);
	CCLOG("URL: %s", sUrl.c_str());
	CCAssert(sUrl.length() > 0, "Not set G_URL_REVIVE yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(sUrl.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setTag("requestRevive");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onRequestReviveCompleted));


	// write the post data
	CCString* strData = CCString::createWithFormat(
		"{ method: \"set\", data: { appId: \"%s\", fbId: \"%s\" }, sign: \"%s\", appId: \"%s\" }",
		G_APP_ID,
		fbId.c_str(),
		getMD5().c_str(),
		G_APP_ID);


	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));

	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onRequestReviveCompleted( CCHttpClient *sender, CCHttpResponse *response )
{
	if (!response)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onRequestReviveCompleted(false, -1);
		}
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
		json_t *newDiamond;

		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		isSuccess = json_object_get(root, "isSuccess");
		newDiamond = json_object_get(root, "newDiamond");
        bool success = CCString::create(json_string_value(isSuccess))->boolValue();
        
		if (m_clientDelegate)
		{
			m_clientDelegate->onRequestReviveCompleted(success, (int)atof(json_string_value(newDiamond)));
		}
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}

//

void GameClientManager::requestGetLazer( string fbId, string friendId)
{
	string sUrl = string(G_URL_GET_LAZER);
	CCLOG("URL: %s", sUrl.c_str());
	CCAssert(sUrl.length() > 0, "Not set G_URL_GET_LAZER yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(sUrl.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);

	request->setTag("getLazer");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onRequestGetLazerCompleted));


	// write the post data
	CCString* strData = CCString::createWithFormat(
		"{ method: \"set\", data: { appId: \"%s\", fbId: \"%s\", friendId: \"%s\" }, sign: \"%s\", appId: \"%s\" }",
		G_APP_ID,
		fbId.c_str(),
		friendId.c_str(),
		getMD5().c_str(),
		G_APP_ID);


	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));

	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onRequestGetLazerCompleted( CCHttpClient *sender, CCHttpResponse *response )
{
	if (!response)
	{
		return;
	}

	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onRequestGetLazerCompleted(false, -1);
		}
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
		json_t *newDiamond;

		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		isSuccess = json_object_get(root, "isSuccess");
		newDiamond = json_object_get(root, "newDiamond");
        bool success = CCString::create(json_string_value(isSuccess))->boolValue();
        
		if (m_clientDelegate)
		{
			m_clientDelegate->onRequestGetLazerCompleted(success, (int)atof(json_string_value(newDiamond)));
		}
	}

	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}


//

void GameClientManager::getAllItem( std::string fbId)
{
    CCAssert(G_URL_ITEM.length() > 0, "Not set G_URL_ITEM yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setRequestType(CCHttpRequest::kHttpPost);
    
	request->setUrl(G_URL_ITEM.c_str());
	request->setTag("getAllItem");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetAllItemCompleted));
	
    CCString* strData = CCString::createWithFormat(
           "{ method: \"get\", data: { appId: \"%s\", fbId: \"%s\", typeArr:[\"%s\", \"%s\"] }, sign: \"%s\", appId: \"%s\" }",
           G_APP_ID,
           fbId.c_str(),
           G_ITEM_LAZE,
           G_ITEM_COIN,
           getMD5().c_str(),
           G_APP_ID);
    
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));
    
    
	CCHttpClient::getInstance()->send(request);
	request->release();
}


void GameClientManager::_onGetAllItemCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
	{
		return;
	}
    
	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());
    
	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onGetAllItemsCompleted(
                 false,
                 DataManager::sharedDataManager()->GetBoom(),
                 DataManager::sharedDataManager()->GetDiamon());
		}
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
        
		str = decodeBeforeProcess(str);
        
		CCLOG("Content: %s", str.c_str());
        
        if (str.length() == 0) {
            CCLOG("Request failed: %s", response->getErrorBuffer());
            if (m_clientDelegate) {
                m_clientDelegate->onGetAllItemsCompleted(
                     false,
                     DataManager::sharedDataManager()->GetBoom(),
                     DataManager::sharedDataManager()->GetDiamon());
            }
        } else {
        
            //get score from response
            json_t *root;
            json_error_t error;
            json_t *isSuccess;
            json_t *laze;
            json_t *coin;
            
            root = json_loads(str.c_str(), strlen(str.c_str()), &error);
            isSuccess = json_object_get(root, "isSuccess");
            bool success = CCString::create(json_string_value(isSuccess))->boolValue();
            
            if (success == true) {
                
                laze = json_object_get(root, G_ITEM_LAZE);
                coin = json_object_get(root, G_ITEM_COIN);
                
                if (m_clientDelegate)
                {
                    m_clientDelegate->onGetAllItemsCompleted(
                             true,
                             (int)atof(json_string_value(laze)),
                             (int)atof(json_string_value(coin)));
                }
            } else {
                if (m_clientDelegate)
                {
                    m_clientDelegate->onGetAllItemsCompleted(
                             false,
                             DataManager::sharedDataManager()->GetBoom(),
                             DataManager::sharedDataManager()->GetDiamon());
                }
            }
        }
	}
    
	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}


//


void GameClientManager::buyItem( std::string fbId, std::string itemName,  int count, std::string uniqueTag)
{
    string sUrl = string(G_URL_ITEM);
	CCLOG("URL: %s", sUrl.c_str());
	CCAssert(sUrl.length() > 0, "Not set G_URL_ITEM yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(sUrl.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);
    
	request->setTag(uniqueTag.c_str());
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onBuyItemCompleted));
    
    
	// write the post data
	CCString* strData = CCString::createWithFormat(
       "{ method: \"buy\", data: { appId: \"%s\", fbId: \"%s\", type: \"%s\", count: \"%d\" }, sign: \"%s\", appId: \"%s\" }",
       G_APP_ID,
       fbId.c_str(),
       itemName.c_str(),
       count,
       getMD5().c_str(),
       G_APP_ID);
    
    
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));
    
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onBuyItemCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
	{
		return;
	}
    
	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());
    
	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onBuyItemCompleted(false,
                                                 DataManager::sharedDataManager()->GetDiamon(),
                                                 "",
                                                 0,
                                                 response->getHttpRequest()->getTag());
		}
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
		json_t *newCoin;
        json_t *type;
        json_t *count;
        
        
        
		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		isSuccess = json_object_get(root, "isSuccess");
		bool success = CCString::create(json_string_value(isSuccess))->boolValue();
        
        if (success) {
            newCoin = json_object_get(root, "coin");
            type    = json_object_get(root, "type");
            count    = json_object_get(root, "count");
            
            if (m_clientDelegate)
            {
                m_clientDelegate->onBuyItemCompleted(success, (int)atof(json_string_value(newCoin)), json_string_value(type), (int)atof(json_string_value(count)), response->getHttpRequest()->getTag());
            }
        } else {
            if (m_clientDelegate)
            {
                //m_clientDelegate->onBuyItemCompleted(false, -1, "", -1, response->getHttpRequest()->getTag());
                m_clientDelegate->onBuyItemCompleted(false,
                                                     DataManager::sharedDataManager()->GetDiamon(),
                                                     "",
                                                     0,
                                                     response->getHttpRequest()->getTag());
            }
        }
	}
    
	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}


//


void GameClientManager::getLazeFree(std::string fbId, std::string friendId)
{
    string sUrl = string(G_URL_FRIEND_LIST);
	CCLOG("URL: %s", sUrl.c_str());
	CCAssert(sUrl.length() > 0, "Not set G_URL_FRIEND_LIST yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(sUrl.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);
    
	request->setTag(friendId.c_str());
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetLazeFreeCompleted));
    
    
	// write the post data
	CCString* strData = CCString::createWithFormat(
           "{ method: \"getGift\", data: { appId: \"%s\", fbId: \"%s\", \"friendId\": \"%s\" }, sign: \"%s\", appId: \"%s\" }",
           G_APP_ID,
           fbId.c_str(),
           friendId.c_str(),
           getMD5().c_str(),
           G_APP_ID);
    
    
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));
    
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onGetLazeFreeCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
	{
		return;
	}
    
	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());
    
	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onGetLazeFreeCompleted(false, response->getHttpRequest()->getTag());
		}
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
        //json_t *friendId;
        
        
		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		isSuccess = json_object_get(root, "isSuccess");
		bool success = CCString::create(json_string_value(isSuccess))->boolValue();
        //friendId = json_object_get(root, "friendId");
        
        if (m_clientDelegate)
        {
            m_clientDelegate->onGetLazeFreeCompleted(success, response->getHttpRequest()->getTag());
        }
    }
    
	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}


//


void GameClientManager::getLife(string fbId)
{
    string sUrl = string(G_URL_ITEM);
	CCLOG("URL: %s", sUrl.c_str());
	CCAssert(sUrl.length() > 0, "Not set G_URL_ITEM yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(sUrl.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);
    
	request->setTag("Get_life");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetLifeCompleted));
    
    
	// write the post data
	CCString* strData = CCString::createWithFormat(
           "{ method: \"getLife\", data: { appId: \"%s\", fbId: \"%s\"}, sign: \"%s\", appId: \"%s\" }",
           G_APP_ID,
           fbId.c_str(),
           getMD5().c_str(),
           G_APP_ID);
    
    
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));
    
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onGetLifeCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
	{
		return;
	}
    
	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());
    
	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onGetLifeCompleted(false,
                                                 DataManager::sharedDataManager()->GetLastPlayerLife(),
                                                 0);
		}
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
        
		str = decodeBeforeProcess(str);
        
		CCLOG("Content: %s", str.c_str());
        
        if (str.length() == 0) {
            CCLOG("Request failed: %s", response->getErrorBuffer());
            if (m_clientDelegate)
            {
                m_clientDelegate->onGetLifeCompleted(false,
                                                     DataManager::sharedDataManager()->GetLastPlayerLife(),
                                                     0);
            }
            
        } else {
        
            //get score from response
            json_t *root;
            json_error_t error;
            //json_t *isSuccess;
            json_t *newLife;
            json_t *serverTime;
            json_t *lastTime;
            
            root = json_loads(str.c_str(), strlen(str.c_str()), &error);
            //isSuccess = json_object_get(root, "isSuccess");
            
            bool success = true;//(bool) CCString::create(json_string_value(isSuccess))->boolValue();
            
            if (success ) {
                newLife = json_object_get(root, "life");
                serverTime = json_object_get(root, "time");
                lastTime = json_object_get(root, "lastTime");
                
                
                long _serverTime = ((long long)atoll(json_string_value(serverTime)) / 1000);
                long _clientTime = static_cast<long int> (time(NULL));
                
                
                long _lastTime = _clientTime - ( _serverTime - ((long long)atoll(json_string_value(lastTime)) / 1000));
                
                CCLOG("~~~LAST TIME GET LIFE Client: %ld", _lastTime);
                
                
                if (m_clientDelegate)
                {
                    m_clientDelegate->onGetLifeCompleted(true, (int)atoi(json_string_value(newLife)), _lastTime);
                }
            } else {
                CCLOG("Request failed: %s", response->getErrorBuffer());
                if (m_clientDelegate)
                {
                    m_clientDelegate->onGetLifeCompleted(
                         false,
                         DataManager::sharedDataManager()->GetLastPlayerLife(),
                         0);
                }
            }
        }
        
    }
    
	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}


//


void GameClientManager::useLife( std::string fbId)
{
    string sUrl = string(G_URL_ITEM);
	CCLOG("URL: %s", sUrl.c_str());
	CCAssert(sUrl.length() > 0, "Not set G_URL_ITEM yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(sUrl.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);
    
	request->setTag("Use_life");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onUseLifeCompleted));
    
    
	// write the post data
	CCString* strData = CCString::createWithFormat(
           "{ method: \"useLife\", data: { appId: \"%s\", fbId: \"%s\"}, sign: \"%s\", appId: \"%s\" }",
           G_APP_ID,
           fbId.c_str(),
           getMD5().c_str(),
           G_APP_ID);
    
    
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));
    
	CCHttpClient::getInstance()->send(request);
	request->release();
}


void GameClientManager::_onUseLifeCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
	{
		return;
	}
    
	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());
    
	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onUseLifeCompleted(false,
                                                 DataManager::sharedDataManager()->GetLastPlayerLife() - 1,
                                                 0);
		}
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
        json_t *newLife;
        json_t *serverTime;
        json_t *lastTime;
        
		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		isSuccess = json_object_get(root, "isSuccess");
		bool success = CCString::create(json_string_value(isSuccess))->boolValue();
        newLife = json_object_get(root, "life");
        serverTime = json_object_get(root, "time");
        lastTime = json_object_get(root, "lastTime");
        
        
        long _serverTime = ((long long)atoll(json_string_value(serverTime)) / 1000);
        long _clientTime = static_cast<long int> (time(NULL));
        

        long _lastTime = _clientTime - ( _serverTime - ((long long)atoll(json_string_value(lastTime)) / 1000));
        
        CCLOG("~~~LAST TIME GET LIFE Client: %ld", _lastTime);
       
        
        if (m_clientDelegate)
        {
            m_clientDelegate->onUseLifeCompleted(success, (int)atoi(json_string_value(newLife)), _lastTime);
        }
    }
    
	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}


//


void GameClientManager::useItem(std::string fbId, string itemId)
{
    string sUrl = string(G_URL_ITEM);
	CCLOG("URL: %s", sUrl.c_str());
	CCAssert(sUrl.length() > 0, "Not set G_URL_ITEM yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(sUrl.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);
    
	request->setTag(itemId.c_str());
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onUseItemCompleted));
    
    
	// write the post data
	CCString* strData = CCString::createWithFormat(
       "{ method: \"use\", data: { appId: \"%s\", fbId: \"%s\", type: \"%s\"}, sign: \"%s\", appId: \"%s\" }",
       G_APP_ID,
       fbId.c_str(), itemId.c_str(),
       getMD5().c_str(),
       G_APP_ID);
    
    
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));
    
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onUseItemCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
	{
		return;
	}
    
	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());
    
	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onUseItemCompleted(false,
                                                 response->getHttpRequest()->getTag(),
                                                 1);
		}
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
        json_t *type;
        json_t *count;
        
		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
        isSuccess = json_object_get(root, "isSuccess");
        type = json_object_get(root, "type");
        count = json_object_get(root, "count");
        
        if (m_clientDelegate)
        {
            m_clientDelegate->onUseItemCompleted(isSuccess,
                                                 json_string_value(type),
                                                 (int)atoi(json_string_value(count)));
        }
    }
    
	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}


//


void GameClientManager::addItem(std::string fbId, string itemId)
{
    string sUrl = string(G_URL_ITEM);
	CCLOG("URL: %s", sUrl.c_str());
	CCAssert(sUrl.length() > 0, "Not set G_URL_ITEM yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(sUrl.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);
    
	request->setTag(itemId.c_str());
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onAddItemCompleted));
    
    
	// write the post data
	CCString* strData = CCString::createWithFormat(
           "{ method: \"add\", data: { appId: \"%s\", fbId: \"%s\", type: \"%s\"}, sign: \"%s\", appId: \"%s\" }",
           G_APP_ID,
           fbId.c_str(), itemId.c_str(),
           getMD5().c_str(),
           G_APP_ID);
    
    
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));
    
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onAddItemCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
	{
		return;
	}
    
	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());
    
	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onAddItemCompleted(false, response->getHttpRequest()->getTag(), -1);
		}
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
        json_t *type;
        json_t *count;
        
		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
        isSuccess = json_object_get(root, "isSuccess");
        type = json_object_get(root, "type");
        count = json_object_get(root, "count");
        
        if (m_clientDelegate)
        {
            m_clientDelegate->onAddItemCompleted(isSuccess,
                                                 json_string_value(type),
                                                 (int)atoi(json_string_value(count)));
        }
    }
    
	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}




//Send, receive items
//


void GameClientManager::sendItem(std::string fbId, std::string friendId, string itemId, int count)
{
    string sUrl = string(G_URL_GIFT_SEND_ITEM);
	CCLOG("URL: %s", sUrl.c_str());
	CCAssert(sUrl.length() > 0, "Not set G_URL_GIFT_SEND_ITEM yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(sUrl.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);
    
    CCString *stag = CCString::createWithFormat("{ \"friendId\": \"%s\", \"itemId\": \"%s\", \"count\": %d}", friendId.c_str(), itemId.c_str(), count);
	request->setTag(stag->getCString());
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onSendItemCompleted));
    
    
	// write the post data
	CCString* strData = CCString::createWithFormat(
           "{ method: \"get\", data: { appId: \"%s\", fbId: \"%s\", receiverId: \"%s\", itemId: \"%s\", count: \"%d\", metadata: \"\"}, sign: \"%s\", appId: \"%s\" }",
           G_APP_ID, fbId.c_str(), friendId.c_str(), itemId.c_str(), count, getMD5().c_str(), G_APP_ID);
    
    
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));
    
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onSendItemCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
	{
		return;
	}
    
	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());
    
    string friendId;
    string itemId;
    int count;

    const char* stag = response->getHttpRequest()->getTag();
    if (true)
    {
        json_t *root;
        json_error_t error;
        
        root = json_loads(stag, strlen(stag), &error);
        friendId = string( json_string_value( json_object_get(root, "friendId") ) );
        itemId = string( json_string_value( json_object_get(root, "itemId") ) );
        count = (int)json_integer_value( json_object_get(root, "count") );
    }
    
    
	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onSendItemCompleted(false, friendId, itemId, count);
		}
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
        
		str = decodeBeforeProcess(str);
        
		CCLOG("Content: %s", str.c_str());
        
		json_t *root;
		json_error_t error;
		json_t *isSuccess;
       
        
		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		isSuccess = json_object_get(root, "isSuccess");
		bool success = CCString::create(json_string_value(isSuccess))->boolValue();
        
        if (m_clientDelegate)
        {
            m_clientDelegate->onSendItemCompleted(success, friendId, itemId, count);
        }
    }
    
	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}



void GameClientManager::getInbox(string fbId)
{
    string sUrl = string(G_URL_GIFT_GET_INBOX);
	CCLOG("URL: %s", sUrl.c_str());
	CCAssert(sUrl.length() > 0, "Not set G_URL_GIFT_GET_INBOX yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(sUrl.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);
    
	request->setTag("GEt_inbox");
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onGetInboxCompleted));
    
    
	// write the post data
	CCString* strData = CCString::createWithFormat(
           "{ method: \"get\", data: { appId: \"%s\", fbId: \"%s\"}, sign: \"%s\", appId: \"%s\" }",
           G_APP_ID, fbId.c_str(), getMD5().c_str(), G_APP_ID);
    
    
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));
    
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onGetInboxCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
	{
		return;
	}
    
	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onGetInboxCompleted(false, NULL);
		}
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
        
		str = decodeBeforeProcess(str);
        
		CCLOG("Content: %s", str.c_str());
        
		json_t *root;
		json_error_t error;
		json_t *isSuccess;
        json_t *friendList;
        
        
		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		isSuccess = json_object_get(root, "isSuccess");
		bool success = CCString::create(json_string_value(isSuccess))->boolValue();
        friendList = json_object_get(root, "list");
        
        //foreach to get all friend, insert to list
		int count = json_array_size(friendList);
		CCArray* arrFriends = CCArray::create();
        
		for(int i = 0; i < count; i++)
		{
			json_t *fbFriend = json_array_get(friendList, i);
            
			
            json_t *senderId;
            json_t *itemId;
            json_t *count;
            json_t *time;
            
            
			senderId = json_object_get(fbFriend, "senderId");
			itemId = json_object_get(fbFriend, "itemId");
			count = json_object_get(fbFriend, "count");
			time = json_object_get(fbFriend, "time");
            
            long long _time =  (long long) atoll(json_string_value(time));
            
			Gift* acc = new Gift(json_string_value(senderId),
                                 json_string_value(itemId),
                                 (int)atoi(json_string_value(count)),
                                 _time);
            
			arrFriends->addObject(acc);
            
            CCLOG("%s", acc->toJson().c_str());
		}
        
        if (m_clientDelegate)
        {
            m_clientDelegate->onGetInboxCompleted(success, arrFriends);
        }
    }
    
	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}



void GameClientManager::removeItem(std::string fbId, std::string senderId, long long time, string itemId)
{
    string sUrl = string(G_URL_GIFT_REMOVE_ITEM);
	CCLOG("URL: %s", sUrl.c_str());
	CCAssert(sUrl.length() > 0, "Not set G_URL_GIFT_REMOVE_ITEM yet");
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl(sUrl.c_str());
	request->setRequestType(CCHttpRequest::kHttpPost);
    
    CCString *stag = CCString::createWithFormat("{ \"senderId\": \"%s\", \"time\": \"%lld\" }", senderId.c_str(), time);
	request->setTag(stag->getCString());
	request->setResponseCallback(this, httpresponse_selector(GameClientManager::_onRemoveItemCompleted));
    
    
	// write the post data
	CCString* strData = CCString::createWithFormat(
           "{ method: \"set\", data: { appId: \"%s\", fbId: \"%s\", senderId: \"%s\", time: \"%lld\", itemId: \"%s\" }, sign: \"%s\", appId: \"%s\" }",
           G_APP_ID, fbId.c_str(), senderId.c_str(), time, itemId.c_str(), getMD5().c_str(), G_APP_ID);
    
    
	std::string s = encodeBeforeSend(strData->getCString());
	request->setRequestData(s.c_str(), strlen(s.c_str()));
    
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameClientManager::_onRemoveItemCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
	{
		return;
	}
    
	//Show info
	CCLOG("------- BEGIN %s -------", response->getHttpRequest()->getTag());
	CCLOG("Status: [%i]", response->getResponseCode());
    
    string senderId;
    long long time;
    
    const char* stag = response->getHttpRequest()->getTag();
    if (true)
    {
        json_t *root;
        json_error_t error;
        
        root = json_loads(stag, strlen(stag), &error);
        senderId = string( json_string_value( json_object_get(root, "senderId") ) );
        time = (long long)atoll( json_string_value( json_object_get(root, "time") ) );
    }
    
    
	if (!response->isSucceed())
	{
		CCLOG("Request failed: %s", response->getErrorBuffer());
		if (m_clientDelegate)
		{
			m_clientDelegate->onRemoveItemCompleted(false, senderId, time);
		}
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
        
		str = decodeBeforeProcess(str);
        
		CCLOG("Content: %s", str.c_str());
        
		json_t *root;
		json_error_t error;
		json_t *isSuccess;
        
        
		root = json_loads(str.c_str(), strlen(str.c_str()), &error);
		isSuccess = json_object_get(root, "isSuccess");
		bool success = CCString::create(json_string_value(isSuccess))->boolValue();
        
        if (m_clientDelegate)
        {
            m_clientDelegate->onRemoveItemCompleted(success, senderId, time);
        }
    }
    
	CCLOG("------- END %s -------", response->getHttpRequest()->getTag());
}











