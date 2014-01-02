#ifndef __GAME_CLIENT_H__
#define __GAME_CLIENT_H__

#include "Global.h"
#include "ParseClient.h"
#include "cocos2d.h"

USING_NS_CC;


#define G_SIGN_IN		"signIn"
#define G_LOG_IN		"logIn"
#define G_SUBMIT_SCORE	"submitScore"



class GameClient
{
public:
	~GameClient(void);
	static GameClient* sharedGameClient() {
		if (s_instance == NULL)
		{
			s_instance = new GameClient();
		}

		return s_instance;
	}

	void startGame();

	void signIn();
	void logIn();
	void submitScore();

private:
	void signInCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);
	void logInCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);
	void submitScoreCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);


private:
	GameClient(void);
	static GameClient* s_instance;
	
};


#endif //__GAME_CLIENT_H__