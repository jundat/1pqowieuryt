#ifndef __GAME_CLIENT_DELEGATE_H__
#define __GAME_CLIENT_DELEGATE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameClientDelegate
{
public:

	//////////////////////////////////////////////////////////////////////////
	//SEND DATA FUNCTIONS

	virtual void onSendPlayerFbProfileCompleted(
		bool isSuccess
		){}

	virtual void onSendFriendListCompleted(
		bool isSuccess
		){}

	virtual void onSendDeviceProfileCompleted(
		bool isSuccess
		){}

	virtual void onSendScoreCompleted(
		bool isSuccess
		){}


	//////////////////////////////////////////////////////////////////////////
	//GET DATA FUNCTIONS

	virtual void onGetScoreCompleted(
		bool isSuccess,
		int score
		){}

	virtual void onGetFriendLeaderboardCompleted(
		bool isSuccess,
		CCArray* arrHighScore //array of FacebookAccount (our code)
		){}

	virtual void onGetWorldLeaderboardCompleted(
		bool isSuccess,
		CCArray* arrHighScore //array of FacebookAccount (our code)
		){}
};


#endif //__GAME_CLIENT_DELEGATE_H__