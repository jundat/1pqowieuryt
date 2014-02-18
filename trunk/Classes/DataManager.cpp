#include "DataManager.h"
#include "Global.h"
#include "MyMacro.h"


DataManager* DataManager::_instance = 0;


DataManager* DataManager::sharedDataManager()
{
	if(DataManager::_instance == 0)
	{
		_instance = new DataManager();
	}

	return _instance;
}


DataManager::DataManager()
{
}

//Default value = 0
int DataManager::GetHighScore()
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey("CURRENT_HIGHSCORE");
}


//Default value = 0
void DataManager::SetCurrentHighScore(int score)
{
	if(score > this->GetHighScore() ) {
		CCUserDefault::sharedUserDefault()->setIntegerForKey("CURRENT_HIGHSCORE", score);
		CCUserDefault::sharedUserDefault()->flush();
	}
}


//Default value = 0
int	DataManager::GetValueFromKey(char* key)
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(key);
}


//Default value = 0
void DataManager::SetValueFromKey(char* key, int val)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey(key, val);
	CCUserDefault::sharedUserDefault()->flush();
}


tm* DataManager::GetLastDeadTime()
{
	tm* _tm = new tm();
	_tm->tm_hour = CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_DEAD_TIME_HOUR");
	_tm->tm_min = CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_DEAD_TIME_MIN");
	_tm->tm_sec = CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_DEAD_TIME_SEC");
	_tm->tm_mday = CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_DEAD_TIME_MDAY");
	_tm->tm_mon = CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_DEAD_TIME_MON");
	_tm->tm_year = CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_DEAD_TIME_YEAR");

	return _tm;	
}


void DataManager::SetLastDeadTime( tm* time )
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("G_LAST_DEAD_TIME_HOUR", time->tm_hour);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("G_LAST_DEAD_TIME_MIN", time->tm_min);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("G_LAST_DEAD_TIME_SEC", time->tm_sec);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("G_LAST_DEAD_TIME_MDAY", time->tm_mday);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("G_LAST_DEAD_TIME_MON", time->tm_mon);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("G_LAST_DEAD_TIME_YEAR", time->tm_year);
	CCUserDefault::sharedUserDefault()->flush();
}

void DataManager::SetLastDeadTimeNow()
{
	time_t curTime = time(NULL);
	tm* _tm = localtime(&curTime);
	DataManager::sharedDataManager()->SetLastDeadTime(_tm);
}

int DataManager::GetLastPlayerLife()
{
	//check if first time run
	int flagFirstTime = CCUserDefault::sharedUserDefault()->getIntegerForKey("FLAG_FIRST_TIME", 0);
	if (flagFirstTime != 0) //not the first time
	{
		return CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_PLAYER_LIFE");
	} 
	else //==0, first time
	{
		CCUserDefault::sharedUserDefault()->setIntegerForKey("FLAG_FIRST_TIME", 1);
		SetLastPlayerLife(G_MAX_PLAYER_LIFE);
		return G_MAX_PLAYER_LIFE;
	}
}

void DataManager::SetLastPlayerLife( int lastLife )
{
	lastLife = (lastLife < 0) ? 0 : lastLife;
	lastLife = (lastLife > G_MAX_PLAYER_LIFE) ? G_MAX_PLAYER_LIFE : lastLife;

	CCUserDefault::sharedUserDefault()->setIntegerForKey("G_LAST_PLAYER_LIFE", lastLife);
	CCUserDefault::sharedUserDefault()->flush();
}

bool DataManager::GetIsJustRevived()
{
	return CCUserDefault::sharedUserDefault()->getBoolForKey("G_IS_JUST_REVIVED", false);
}

void DataManager::SetIsJustRevived(bool isJustRevived)
{
	CCUserDefault::sharedUserDefault()->setBoolForKey("G_IS_JUST_REVIVED", isJustRevived);
	CCUserDefault::sharedUserDefault()->flush();
}

std::string DataManager::GetName()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_NAME", std::string(G_DEFAULT_NAME));
}

void DataManager::SetName( const char* name )
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_NAME", std::string(name));
	CCUserDefault::sharedUserDefault()->flush();
}

std::string DataManager::GetUsername()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_USERNAME", std::string("NULL"));
}

void DataManager::SetUsername(const char* username)
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_USERNAME", std::string(username));
	CCUserDefault::sharedUserDefault()->flush();
}

std::string DataManager::GetPassword()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_PASSWORD", std::string("NULL"));
}

void DataManager::SetPassword(const char* pass )
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_PASSWORD", std::string(pass));
	CCUserDefault::sharedUserDefault()->flush();
}

void DataManager::RefreshPlayerLife()
{
	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
	if (lastLife < G_MAX_PLAYER_LIFE)
	{
		tm* lasttm = DataManager::sharedDataManager()->GetLastDeadTime();
		time_t lastTime = mktime(lasttm);
		time_t curTime = time(NULL);
		double seconds = difftime(curTime, lastTime);

		int add_lastLife = (int)((int)seconds / (int)G_PLAYER_TIME_TO_REVIVE);
		lastLife += add_lastLife;

		DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);
	}
}

std::string DataManager::GetProfileID()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_PROFILE_ID", std::string("NULL"));
}

void DataManager::SetProfileID( const char* profileID )
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_PROFILE_ID", std::string(profileID));
	CCUserDefault::sharedUserDefault()->flush();
}

std::string DataManager::GetFbUserName()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_FB_USER_NAME", std::string("NULL"));
}

void DataManager::SetFbUserName( const char* fbusernam )
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_FB_USER_NAME", std::string(fbusernam));
	CCUserDefault::sharedUserDefault()->flush();
}

std::string DataManager::GetPhotoPath()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_PHOTO_PATH", std::string("NULL"));
}

void DataManager::SetPhotoPath( const char* path )
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_PHOTO_PATH", std::string(path));
	CCUserDefault::sharedUserDefault()->flush();
}

int DataManager::GetGiftFromFriend(const char* fbID )
{
	CCString* s = CCString::createWithFormat("GIFT_FROM_%s", fbID);
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(s->getCString(), 0);
}

void DataManager::IncreaseGiftFromFriend(const char* fbID)
{
	CCString* s = CCString::createWithFormat("GIFT_FROM_%s", fbID);

	CCUserDefault::sharedUserDefault()->setIntegerForKey(
		s->getCString(),
		DataManager::sharedDataManager()->GetGiftFromFriend(fbID) + 1
	);

	CCUserDefault::sharedUserDefault()->flush();
}

void DataManager::DecreaseGiftFromFriend( const char* fbID )
{
	CCString* s = CCString::createWithFormat("GIFT_FROM_%s", fbID);
	int gift = DataManager::sharedDataManager()->GetGiftFromFriend(fbID) - 1;
	gift = (gift < 0) ? 0: gift;

	CCUserDefault::sharedUserDefault()->setIntegerForKey(
		s->getCString(), 
		gift
	);

	CCUserDefault::sharedUserDefault()->flush();
}

void DataManager::SetTime( const char* key, tm* time )
{
	CCString* sValue = CCString::createWithFormat("%d:%d:%d_%d-%d-%d", 
		time->tm_hour,
		time->tm_min,
		time->tm_sec,
		time->tm_mday,
		time->tm_mon,
		time->tm_year
	);

	CCUserDefault::sharedUserDefault()->setStringForKey(key, sValue->getCString());
	CCUserDefault::sharedUserDefault()->flush();
}

tm* DataManager::GetTime( const char* key )
{
	std::string sValue = std::string(CCUserDefault::sharedUserDefault()->getStringForKey(key));
	if (sValue.length() <= 0)
	{
		return NULL;
	} 
	else
	{
		int hour, min, sec, mday, mon, year;
		sscanf(sValue.c_str(), "%d:%d:%d_%d-%d-%d", &hour, &min, &sec, &mday, &mon, &year);

		tm* _tm = new tm();
		_tm->tm_hour = hour;
		_tm->tm_min = min;
		_tm->tm_sec = sec;
		_tm->tm_mday = mday;
		_tm->tm_mon = mon;
		_tm->tm_year = year;

		return _tm;	
	}
}

void DataManager::SetTimeBoomFriend( const char* fbId, tm* time )
{
	CCString* sKey = CCString::createWithFormat("LAST_BOOM_FROM_%s", fbId);
	SetTime(sKey->getCString(), time);
}

tm* DataManager::GetTimeBoomFriend( const char* fbId )
{
	CCString* sKey = CCString::createWithFormat("LAST_BOOM_FROM_%s", fbId);
	return GetTime(sKey->getCString());
}

void DataManager::SetTimeBoomFriendNow( const char* fbId )
{
	time_t curTime = time(NULL);
	tm* _tm = localtime(&curTime);

	DataManager::sharedDataManager()->SetTimeBoomFriend(fbId, _tm);
}

int DataManager::GetBoom()
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey("BOOM", G_DEFAULT_BOOM);
}

void DataManager::SetBoom( int boom )
{
	LIMIT_VALUE(boom, 0, G_MAX_BOOM);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("BOOM", boom);
	CCUserDefault::sharedUserDefault()->flush();
}

void DataManager::IncreaseBoom()
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("BOOM", 
		DataManager::sharedDataManager()->GetBoom() + 1);

	CCUserDefault::sharedUserDefault()->flush();
}

void DataManager::DecreaseBoom()
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("BOOM", 
		DataManager::sharedDataManager()->GetBoom() - 1);

	CCUserDefault::sharedUserDefault()->flush();
}
