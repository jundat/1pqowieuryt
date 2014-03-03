#ifndef __GAME_CLIENT_MANAGER_H__
#define __GAME_CLIENT_MANAGER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpClient.h"
#include "GameClientDelegate.h"
#include "jansson/jansson.h"
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;



//SEND DATA
#define G_URL_SEND_PLAYER_FB_PROFILE	"192.168.0.118"
#define G_URL_SEND_FRIEND_LIST			""
#define G_URL_SEND_DEVICE_PROFILE		""
#define G_URL_SEND_SCORE				"192.168.0.118:8025/score"


//GET DATA
#define G_URL_GET_SCORE					""
#define G_URL_GET_FRIEND_LEADERBOARD	""
#define G_URL_GET_WORLD_LEADERBOARD		""




class FacebookAccount : public CCNode
{
public:
	std::string fbId;
	std::string fbName; //get the fullname
	int score;

public:
	FacebookAccount(std::string _fbId, std::string _fbName, int _score)
	{
		fbId = _fbId;
		fbName = _fbName;
		score = _score;
	}

	std::string toJson()
	{
		CCString* s = CCString::createWithFormat("{fbId: \"%s\", fbName: \"%s\", score: %d}", fbId.c_str(), fbName.c_str(), score);
		return std::string(s->getCString());
	}
};


//////////////////////////////////////////////////////////////////////////


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

	void getFriendLeaderboard(
		std::string appId,
		std::string fbId
		);

	void _onGetFriendLeaderboardCompleted(CCHttpClient *sender, CCHttpResponse *response);

	//////////////////////////////////////////////////////////////////////////

	void getWorldLeaderboard(
		std::string appId,
		std::string fbId
		);

	void _onGetWorldLeaderboardCompleted(CCHttpClient *sender, CCHttpResponse *response);

	//////////////////////////////////////////////////////////////////////////
	//more ...
};

#endif //__GAME_CLIENT_MANAGER_H__
