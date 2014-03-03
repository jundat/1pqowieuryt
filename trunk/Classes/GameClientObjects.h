#ifndef __GAME_CLIENT_OBJECTS_H__
#define __GAME_CLIENT_OBJECTS_H__

#include "cocos2d.h"

USING_NS_CC;




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

class DeviceProfile : public CCNode
{
public:
	std::string deviceId;
	std::string deviceToken;
	std::string deviceConfig;
	std::string devicePhoneNumber;
};




#endif // __GAME_CLIENT_OBJECTS_H__
