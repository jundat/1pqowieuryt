#ifndef __GAME_CLIENT_OBJECTS_H__
#define __GAME_CLIENT_OBJECTS_H__

#include "cocos2d.h"

USING_NS_CC;




class FacebookAccount : public CCNode
{
public:
	std::string fbId;
	std::string fbName; //get the fullname
	std::string email;
	int score;

public:
	FacebookAccount(){}

	FacebookAccount(std::string _fbId, std::string _fbName, std::string _email, int _score)
	{
		fbId = _fbId;
		fbName = _fbName;
		email = _email;
		score = _score;
	}

	std::string toJson()
	{
		CCString* s = CCString::createWithFormat("{fbId: \"%s\", fbName: \"%s\", email: \"%s\", score: %d}", fbId.c_str(), fbName.c_str(), email.c_str(), score);
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

	DeviceProfile(){}

	DeviceProfile(std::string _deviceId, std::string _deviceToken, std::string _deviceConfig, std::string _devicePhoneNumber)
	{
		deviceId = _deviceId;
		deviceToken = _deviceToken;
		deviceConfig = _deviceConfig;
		devicePhoneNumber = _devicePhoneNumber;
	}

	std::string toJson()
	{
		CCString* s = CCString::createWithFormat("{deviceId: \"%s\", deviceToken: \"%s\", deviceConfig: \"%s\", devicePhoneNumber: \"%s\" }", 
			deviceId.c_str(), deviceToken.c_str(), deviceConfig.c_str(), devicePhoneNumber.c_str());
		return std::string(s->getCString());
	}
};




#endif // __GAME_CLIENT_OBJECTS_H__
