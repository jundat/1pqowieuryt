#ifndef __GAME_CLIENT_MANAGER_H__
#define __GAME_CLIENT_MANAGER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpClient.h"
#include "GameClientDelegate.h"
#include "jansson/jansson.h"
#include <string>
#include "Global.h"
#include "GameClientObjects.h"

USING_NS_CC;
USING_NS_CC_EXT;


class GameClientManager : public CCObject
{

	//////////////////////////////////////////////////////////////////////////
private:
	static GameClientManager* s_instance;

	//object will receive all response
	GameClientDelegate* m_clientDelegate;

private:
	GameClientManager() { m_clientDelegate = NULL; }


	//////////////////////////////////////////////////////////////////////////
public:
	static GameClientManager* sharedGameClientManager() {
		if (s_instance == NULL)
		{
			s_instance = new GameClientManager();
		}

		return s_instance;
	}
	
	std::string encodeBeforeSend(std::string strData)
	{
		return strData;
	}

	void setDelegate(GameClientDelegate* clientDelegate)
	{
		this->m_clientDelegate = clientDelegate;
	}

	GameClientDelegate* getDelegate()
	{
		return m_clientDelegate;
	}


	//////////////////////////////////////////////////////////////////////////
	//main functions
public:

	//////////////////////////////////////////////////////////////////////////
	//								SEND DATA
	//////////////////////////////////////////////////////////////////////////
	
	void sendPlayerFbProfile(
		std::string fbId, 
		std::string fbName,
		std::string email
		);

	void _onSendPlayerFbProfileCompleted(CCHttpClient *sender, CCHttpResponse *response);

	//////////////////////////////////////////////////////////////////////////

	void sendFriendList(
		std::string fbId,
		CCArray* arrFriends //array of FacebookAccount class (our code)
		);

	void _onSendFriendListCompleted(CCHttpClient *sender, CCHttpResponse *response);

	//////////////////////////////////////////////////////////////////////////

	void sendDeviceProfile(
		std::string fbId,
		std::string deviceId,
		std::string deviceToken,
		std::string deviceConfig,
		std::string devicePhoneNumber
		);

	void _onSendDeviceProfileCompleted(CCHttpClient *sender, CCHttpResponse *response);

	//////////////////////////////////////////////////////////////////////////

	void sendScore(
		std::string appId,
		std::string fbId,
		int score,
		long time
		);

	void _onSendScoreCompleted(CCHttpClient *sender, CCHttpResponse *response);


	//////////////////////////////////////////////////////////////////////////
	//								GET DATA
	//////////////////////////////////////////////////////////////////////////

	void getScore(
		std::string appId,
		std::string fbId
		);

	void _onGetScoreCompleted(CCHttpClient *sender, CCHttpResponse *response);

	//////////////////////////////////////////////////////////////////////////

	void getFriendList(
		std::string appId,
		std::string fbId
		);

	void _onGetFriendListCompleted(CCHttpClient *sender, CCHttpResponse *response);

	//////////////////////////////////////////////////////////////////////////
	
	void getPlayerFbProfile(std::string fbId );

	void _onGetPlayerFbProfileCompleted( CCHttpClient *sender, CCHttpResponse *response );

	//////////////////////////////////////////////////////////////////////////

	void getDeviceProfile(std::string fbId );

	void _onGetDeviceProfileCompleted( CCHttpClient *sender, CCHttpResponse *response );



	//more ...
};

#endif //__GAME_CLIENT_MANAGER_H__
