#include "DataManager.h"
#include "Global.h"

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
int	DataManager::GetCurrenHighScore()
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey("CURRENT_HIGHSCORE");
}


//Default value = 0
void DataManager::SetCurrentHighScore(int score)
{
	if(score > this->GetCurrenHighScore() ) {
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

int DataManager::GetLastPlayerLife()
{
	//check if first time run
	int flagFirstTime = CCUserDefault::sharedUserDefault()->getIntegerForKey("FLAG_FIRST_TIME", 0);
	if (flagFirstTime != 0) //not the first time
	{
		CCLOG("NOT First time", "Info");
		return CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_PLAYER_LIFE");
	} 
	else //==0, first time
	{
		CCLOG("------------------ First time ------------------", "Info");
		CCUserDefault::sharedUserDefault()->setIntegerForKey("FLAG_FIRST_TIME", 1);
		SetLastPlayerLife(G_MAX_PLAYER_LIFE);
		return G_MAX_PLAYER_LIFE;
	}
}

void DataManager::SetLastPlayerLife( int lastLife )
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("G_LAST_PLAYER_LIFE", lastLife);
	CCUserDefault::sharedUserDefault()->flush();
}

void DataManager::SetLastDeadTimeNow()
{
	time_t curTime = time(NULL);
	tm* _tm = localtime(&curTime);
	DataManager::sharedDataManager()->SetLastDeadTime(_tm);
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

std::string DataManager::GetUsername()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_USERNAME", "NULL");
}

void DataManager::SetUsername(const char* username)
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_USERNAME", std::string(username));
	CCUserDefault::sharedUserDefault()->flush();
}

std::string DataManager::GetPassword()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_PASSWORD", "NULL");
}

void DataManager::SetPassword(const char* pass )
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_PASSWORD", std::string(pass));
	CCUserDefault::sharedUserDefault()->flush();
}
