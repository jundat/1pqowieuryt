#ifndef __GAME_CLIENT_DELEGATE_H__
#define __GAME_CLIENT_DELEGATE_H__

#include "cocos2d.h"
#include "GameClientObjects.h"

USING_NS_CC;

class GameClientDelegate
{
public:

	virtual void onSendPlayerFbProfileCompleted( bool isSuccess ){}
	virtual void onGetPlayerFbProfileCompleted(bool isSuccess, FacebookAccount* acc){}

	virtual void onSendFriendListCompleted( bool isSuccess ){}
	//array of FacebookAccount (our code)
	virtual void onGetFriendListCompleted( bool isSuccess, CCArray* arrFriends){}

	virtual void onSendDeviceProfileCompleted( bool isSuccess ){}
	virtual void onGetDeviceProfileCompleted(bool isSuccess, DeviceProfile* deviceProfile){}

	virtual void onSendScoreCompleted( bool isSuccess, int newScore ){}
	virtual void onGetScoreCompleted( bool isSuccess, int score, std::string time ){}

	virtual void onRequestReviveCompleted( bool isSuccess, int newDiamond ){}
	virtual void onRequestGetLazerCompleted(bool isSuccess, int newDiamond){}

 	virtual void onGetAllItemsCompleted(bool isSuccess, int laze, int coin){}
    
    virtual void onBuyItemCompleted(bool isSuccess, int newCoin, std::string itemType, int itemCount, std::string uniqueTag){}
    
    virtual void onGetLazeFreeCompleted(bool isSuccess, std::string friendId){}
    
    virtual void onGetLifeCompleted(bool isSuccess, int life, long lastTimeClient_Second){}
    
    virtual void onUseLifeCompleted(bool isSuccess, int newLife, long lastTime_client){}
    
    virtual void onUseItemCompleted(bool isSuccess, string itemId, int newCount){}
    
    virtual void onAddItemCompleted(bool isSuccess, string itemId, int newCount){}
    
    
    
    virtual void onSendItemCompleted(bool isSuccess, string friendId, string itemId, int count){}
    
    virtual void onGetInboxCompleted(bool isSuccess, CCArray* arrFriends){}
    
    virtual void onRemoveItemCompleted(bool isSuccess, string senderId, long long time){}
    
    
};


#endif //__GAME_CLIENT_DELEGATE_H__