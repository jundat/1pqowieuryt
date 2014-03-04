#ifndef __GAME_CLIENT_OBJECTS_H__
#define __GAME_CLIENT_OBJECTS_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;




class FacebookAccount : public CCObject
{
public:
	string fbId;
	string fbName; //get the fullname
	string email;
	int score;

	string photoPath;

public:
	FacebookAccount() 
	{
		fbId = string();
		fbName = string();
		email = string();
		score = -1;

		photoPath = string();
	}

	FacebookAccount(string _fbId, string _fbName, string _email, int _score)
	{
		fbId = _fbId;
		fbName = _fbName;
		email = _email;
		score = _score;

		photoPath = string();
	}

	CCObject* copyWithZone(CCZone *pZone)
	{
		CCZone *pNewZone = NULL;
		FacebookAccount *pRet = NULL;
		if(pZone && pZone->m_pCopyObject) //in case of being called at sub class
		{
			pRet = (FacebookAccount*)(pZone->m_pCopyObject);
		}
		else
		{
			pRet = new FacebookAccount();
			pZone = pNewZone = new CCZone(pRet);
		}
		CCObject::copyWithZone(pZone);
		// copy member data
		//pRet->m_nTag = m_nTag;
		pRet->fbId = string(fbId);
		pRet->fbName = string(fbName);
		pRet->email = string(email);
		pRet->score = score;

		pRet->photoPath = string(photoPath);
		

		CC_SAFE_DELETE(pNewZone);
		return pRet;
	}

	string toJson()
	{
		CCString* s = CCString::createWithFormat("{fbId: \"%s\", fbName: \"%s\", email: \"%s\", score: %d}", fbId.c_str(), fbName.c_str(), email.c_str(), score);
		return string(s->getCString());
	}
};


//////////////////////////////////////////////////////////////////////////

class DeviceProfile : public CCObject
{
public:
	string deviceId;
	string deviceToken;
	string deviceConfig;
	string devicePhoneNumber;

	DeviceProfile(){}

	DeviceProfile(string _deviceId, string _deviceToken, string _deviceConfig, string _devicePhoneNumber)
	{
		deviceId = _deviceId;
		deviceToken = _deviceToken;
		deviceConfig = _deviceConfig;
		devicePhoneNumber = _devicePhoneNumber;
	}

	string toJson()
	{
		CCString* s = CCString::createWithFormat("{deviceId: \"%s\", deviceToken: \"%s\", deviceConfig: \"%s\", devicePhoneNumber: \"%s\" }", 
			deviceId.c_str(), deviceToken.c_str(), deviceConfig.c_str(), devicePhoneNumber.c_str());
		return string(s->getCString());
	}
};




#endif // __GAME_CLIENT_OBJECTS_H__
