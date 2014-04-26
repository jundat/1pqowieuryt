#ifndef __GAME_CLIENT_MANAGER_H__
#define __GAME_CLIENT_MANAGER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpClient.h"
#include "GameClientDelegate.h"
#include "jansson/jansson.h"
#include <string>
#include <algorithm>
#include "Global.h"
#include "GameClientObjects.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define KEY		"cGk@proJectDBPtkkEy-askdjhsakdhajakshdwi"

class GameClientManager : public CCObject
{

	//////////////////////////////////////////////////////////////////////////
private:
	static GameClientManager* s_instance;
	static string s_urlProfile;
	static string s_urlDevice;
	static string s_urlFriend;
	static string s_urlScore;

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
	
	void setUrls(string urlProfile, string urlDevice, string urlFriend, string urlScore);
	std::string encodeBeforeSend(std::string src);
	std::string decodeBeforeProcess(std::string src);
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

	string getMD5();

	void sendPlayerFbProfile(std::string fbId, std::string fbName, std::string email);
	void _onSendPlayerFbProfileCompleted(CCHttpClient *sender, CCHttpResponse *response);

	void getPlayerFbProfile(std::string fbId );
	void _onGetPlayerFbProfileCompleted( CCHttpClient *sender, CCHttpResponse *response);

	//////////////////////////////////////////////////////////////////////////

	//array of FacebookAccount class (our code)
	void sendFriendList( std::string fbId, CCArray* arrFriends );
	void _onSendFriendListCompleted(CCHttpClient *sender, CCHttpResponse *response);

	void getFriendList(std::string fbId );
	void _onGetFriendListCompleted(CCHttpClient *sender, CCHttpResponse *response);

	//////////////////////////////////////////////////////////////////////////

	void sendDeviceProfile( std::string fbId, std::string deviceId, std::string deviceToken, std::string deviceConfig, std::string devicePhoneNumber);
	void _onSendDeviceProfileCompleted(CCHttpClient *sender, CCHttpResponse *response);

	void getDeviceProfile(std::string fbId, std::string deviceId );
	void _onGetDeviceProfileCompleted( CCHttpClient *sender, CCHttpResponse *response );

	//////////////////////////////////////////////////////////////////////////

	void sendScore(std::string fbId, int score );
	void _onSendScoreCompleted(CCHttpClient *sender, CCHttpResponse *response);

	void getScore(std::string fbId );
	void _onGetScoreCompleted(CCHttpClient *sender, CCHttpResponse *response);

	//////////////////////////////////////////////////////////////////////////

	void requestRevive(string fbId);
	void _onRequestReviveCompleted(CCHttpClient *sender, CCHttpResponse *response);
	
	//////////////////////////////////////////////////////////////////////////

	void requestGetLazer(string fbId, string friendId);
	void _onRequestGetLazerCompleted(CCHttpClient *sender, CCHttpResponse *response);

	//////////////////////////////////////////////////////////////////////////

    void getAllItem(std::string fbId);
    void _onGetAllItemCompleted(CCHttpClient *sender, CCHttpResponse *response);
    
    
    //////////////////////////////////////////////////////////////////////////
    
    void buyItem(std::string fbId, std::string itemName, int count, std::string uniqueTag);
    void _onBuyItemCompleted(CCHttpClient *sender, CCHttpResponse *response);
    
    
    void getLazeFree(std::string fbId, std::string friendId);
    void _onGetLazeFreeCompleted(CCHttpClient *sender, CCHttpResponse *response);
    
    
    //////////////////////////////////////////////////////////////////////////
    
    void getLife(string fbId);
    void _onGetLifeCompleted(CCHttpClient *sender, CCHttpResponse *response);
    
    
    //////////////////////////////////////////////////////////////////////////
    
    void useLife(std::string fbId);
    void _onUseLifeCompleted(CCHttpClient *sender, CCHttpResponse *response);
    
    //////////////////////
    
    void useItem(std::string fbId, string itemId);
    void _onUseItemCompleted(CCHttpClient *sender, CCHttpResponse *response);
    
    void addItem(std::string fbId, string itemId);
    void _onAddItemCompleted(CCHttpClient *sender, CCHttpResponse *response);
    
    
    
    //////////////////////////////////////////////////////////////////////////
    
    void sendItem(std::string fbId, std::string friendId, string itemId, int count);
    void _onSendItemCompleted(CCHttpClient *sender, CCHttpResponse *response);
    
    
    void getInbox(string fbId);
    void _onGetInboxCompleted(CCHttpClient *sender, CCHttpResponse *response);
    
    
    void removeItem(std::string fbId, std::string friendId, long long time);
    void _onRemoveItemCompleted(CCHttpClient *sender, CCHttpResponse *response);
    
    
    //////////////////////////////////////////////////////////////////////////
    
    
    
    
    
    
    
    
    //-------------------------------------------------------------------------//
    
	//compare score in facebook account
	static int CompareFriendScore(const CCObject* p1, const CCObject* p2)
	{
		FacebookAccount* acc1 = (FacebookAccount*) p1;
		FacebookAccount* acc2 = (FacebookAccount*) p2;

		return (acc1->m_score > acc2->m_score);
	}

	//array of FacebookAccount
	static void SortFriendScore(CCArray* arrFriends)
	{
		std::sort(arrFriends->data->arr, 
			arrFriends->data->arr + arrFriends->data->num, CompareFriendScore);
	}


	//more ...
};

#endif //__GAME_CLIENT_MANAGER_H__
