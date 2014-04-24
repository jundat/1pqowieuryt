#ifndef __GAME_CLIENT_OBJECTS_H__
#define __GAME_CLIENT_OBJECTS_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;




class FacebookAccount : public CCObject
{
public:
	string m_fbId;
	string m_fbName; //get the fullname
	string m_email;
	int m_score;
	int m_coin;
    
    long m_timeGetLaze;
    long m_timeSendLife;

	string m_photoPath;

public:
	FacebookAccount() 
	{
		m_fbId = string();
		m_fbName = string();
		m_email = string();
		m_score = -1;
		m_coin = 0;
        
        m_timeGetLaze = 0;
        m_timeSendLife = 0;

		m_photoPath = string();
	}

	FacebookAccount(string _fbId, string _fbName, string _email, int _score, int _coin = 0, long timeGetLaze = 0, long timeSendLife = 0)
	{
		m_fbId = _fbId;
		m_fbName = _fbName;
		m_email = _email;
		m_score = _score;
		m_coin = _coin;
        
        m_timeGetLaze = timeGetLaze;
        m_timeSendLife = timeSendLife;

		m_photoPath = string();
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
		pRet->m_fbId = string(m_fbId);
		pRet->m_fbName = string(m_fbName);
		pRet->m_email = string(m_email);
		pRet->m_score = m_score;
		pRet->m_coin = m_coin;
        
        pRet->m_timeGetLaze = m_timeGetLaze;
		pRet->m_timeSendLife = m_timeSendLife;

		pRet->m_photoPath = string(m_photoPath);
		

		CC_SAFE_DELETE(pNewZone);
		return pRet;
	}

	string toJson()
	{
		CCString* s = CCString::createWithFormat("{\"fbId\": \"%s\", \"fbName\": \"%s\", \"email\": \"%s\", \"score\": %d, \"coin\": %d, \"timeGetLaze\": %ld, \"timeSendLife\": %ld }", m_fbId.c_str(), m_fbName.c_str(), m_email.c_str(), m_score, m_coin, m_timeGetLaze, m_timeSendLife);
		return string(s->getCString());
	}
};


//////////////////////////////////////////////////////////////////////////

class DeviceProfile : public CCObject
{
public:
	string m_deviceId;
	string m_deviceToken;
	string m_deviceConfig;
	string m_devicePhoneNumber;

	DeviceProfile(){}

	DeviceProfile(string _deviceId, string _deviceToken, string _deviceConfig, string _devicePhoneNumber)
	{
		m_deviceId = _deviceId;
		m_deviceToken = _deviceToken;
		m_deviceConfig = _deviceConfig;
		m_devicePhoneNumber = _devicePhoneNumber;
	}

	CCObject* copyWithZone(CCZone *pZone)
	{
		CCZone *pNewZone = NULL;
		DeviceProfile *pRet = NULL;
		if(pZone && pZone->m_pCopyObject) //in case of being called at sub class
		{
			pRet = (DeviceProfile*)(pZone->m_pCopyObject);
		}
		else
		{
			pRet = new DeviceProfile();
			pZone = pNewZone = new CCZone(pRet);
		}
		CCObject::copyWithZone(pZone);
		// copy member data
		//pRet->m_nTag = m_nTag;

		pRet->m_deviceId = string(m_deviceId);
		pRet->m_deviceToken = string(m_deviceToken);
		pRet->m_deviceConfig = string(m_deviceConfig);
		pRet->m_devicePhoneNumber = string(m_devicePhoneNumber);

		CC_SAFE_DELETE(pNewZone);
		return pRet;
	}
	
	string toJson()
	{
		CCString* s = CCString::createWithFormat("{\"deviceId\": \"%s\", \"deviceToken\": \"%s\", \"deviceConfig\": \"%s\", \"devicePhoneNumber\": \"%s\" }", 
			m_deviceId.c_str(), m_deviceToken.c_str(), m_deviceConfig.c_str(), m_devicePhoneNumber.c_str());
		return string(s->getCString());
	}
};



class GameItem : public CCObject
{
public:
	string	m_id;
	int		m_number;

public:
	GameItem(){}

	GameItem(string id, int number)
	{
		m_id = string(id);
		m_number = number;
	}


	CCObject* copyWithZone(CCZone *pZone)
	{
		CCZone *pNewZone = NULL;
		GameItem *pRet = NULL;
		if(pZone && pZone->m_pCopyObject) //in case of being called at sub class
		{
			pRet = (GameItem*)(pZone->m_pCopyObject);
		}
		else
		{
			pRet = new GameItem();
			pZone = pNewZone = new CCZone(pRet);
		}
		CCObject::copyWithZone(pZone);
		// copy member data
		//pRet->m_nTag = m_nTag;

		pRet->m_id = string(m_id);
		pRet->m_number = m_number;

		CC_SAFE_DELETE(pNewZone);
		return pRet;
	}

	string toJson()
	{
		CCString* s = CCString::createWithFormat("{\"itemId\": \"%s\", \"itemNumber\": \"%d\"}", m_id.c_str(), m_number);
		return string(s->getCString());
	}
};


#endif // __GAME_CLIENT_OBJECTS_H__
