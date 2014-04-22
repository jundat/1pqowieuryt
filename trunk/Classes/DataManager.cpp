#include "DataManager.h"
#include "Global.h"
#include "MyMacro.h"
#include "jansson/jansson.h"
#include "GameClientObjects.h"


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
void DataManager::SetHighScore(int score)
{
	if(score > this->GetHighScore() ) {
		CCUserDefault::sharedUserDefault()->setIntegerForKey("CURRENT_HIGHSCORE", score);
		CCUserDefault::sharedUserDefault()->flush();
	}
}

void DataManager::SetHighScoreNotCheck(int score)
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("CURRENT_HIGHSCORE", score);
    CCUserDefault::sharedUserDefault()->flush();
}

//Default value = 0
int	DataManager::GetValueFromKey(const char* key)
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(key, 0);
}


//Default value = 0
void DataManager::SetValueFromKey(const char* key, int val)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey(key, val);
	CCUserDefault::sharedUserDefault()->flush();
}


tm* DataManager::GetLastDeadTime()
{
	tm* _tm = new tm();
	_tm->tm_hour = CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_DEAD_TIME_HOUR", -1);
	_tm->tm_min = CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_DEAD_TIME_MIN", -1);
	_tm->tm_sec = CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_DEAD_TIME_SEC", -1);
	_tm->tm_mday = CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_DEAD_TIME_MDAY", -1);
	_tm->tm_mon = CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_DEAD_TIME_MON", -1);
	_tm->tm_year = CCUserDefault::sharedUserDefault()->getIntegerForKey("G_LAST_DEAD_TIME_YEAR", -1);

	if (_tm->tm_hour == -1)
	{
		time_t t = time(NULL);
		_tm = localtime(&t);
	}

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

//default = false
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
		if (add_lastLife > 0)
		{
			lastLife += add_lastLife;
			DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);

			//save next time
			tm* _tm = DataManager::sharedDataManager()->GetLastDeadTime();
			_tm->tm_sec += add_lastLife * G_PLAYER_TIME_TO_REVIVE;

			mktime(_tm); //normalize

			DataManager::sharedDataManager()->SetLastDeadTime(_tm);
		}
	}
}

//default = "NULL"
std::string DataManager::GetFbID()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_PROFILE_ID", std::string("NULL"));
}

void DataManager::SetFbProfileID( const char* profileID )
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_PROFILE_ID", std::string(profileID));
	CCUserDefault::sharedUserDefault()->flush();
}

void DataManager::ClearFbProfileID()
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_PROFILE_ID", std::string("NULL"));
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


std::string DataManager::GetFbFullName()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_FB_FULL_NAME", std::string("NULL"));
}

void DataManager::SetFbFullName( const char* fbusernam )
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_FB_FULL_NAME", std::string(fbusernam));
	CCUserDefault::sharedUserDefault()->flush();
}


std::string DataManager::GetFbEmail()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_FB_EMAIL", std::string("NULL"));
}

void DataManager::SetFbEmail( const char* fbemail )
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_FB_EMAIL", std::string(fbemail));
	CCUserDefault::sharedUserDefault()->flush();
}

//return ""
std::string DataManager::GetPhotoPath()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_PHOTO_PATH", std::string(""));
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
	CCString* sKey = CCString::createWithFormat("LAST_GET_BOOM_FROM_%s", fbId);
	SetTime(sKey->getCString(), time);
}

tm* DataManager::GetTimeBoomFriend( const char* fbId )
{
	CCString* sKey = CCString::createWithFormat("LAST_GET_BOOM_FROM_%s", fbId);
	return GetTime(sKey->getCString());
}

void DataManager::SetTimeBoomFriendNow( const char* fbId )
{
	time_t curTime = time(NULL);
	tm* _tm = localtime(&curTime);

	DataManager::sharedDataManager()->SetTimeBoomFriend(fbId, _tm);
}




void DataManager::SetIsJustGetBoomNowFriend( const char* fbId, bool isTrue )
{
	CCString* sKey = CCString::createWithFormat("IS_JUST_GET_BOOM_NOW_%s", fbId);
	CCUserDefault::sharedUserDefault()->setBoolForKey(sKey->getCString(), isTrue);
	CCUserDefault::sharedUserDefault()->flush();
}

bool DataManager::GetIsJustGetBoomNowFriend( const char* fbId )
{
	CCString* sKey = CCString::createWithFormat("IS_JUST_GET_BOOM_NOW_%s", fbId);
	return CCUserDefault::sharedUserDefault()->getBoolForKey(sKey->getCString(), false);
}



void DataManager::SetTimeLifeToFriend( const char* fbId, tm* time )
{
	CCString* sKey = CCString::createWithFormat("LAST_SEND_LIFE_TO_%s", fbId);
	SetTime(sKey->getCString(), time);
}

tm* DataManager::GetTimeLifeToFriend( const char* fbId )
{
	CCString* sKey = CCString::createWithFormat("LAST_SEND_LIFE_TO_%s", fbId);
	return GetTime(sKey->getCString());
}

void DataManager::SetTimeLifeToFriendNow( const char* fbId )
{
	time_t curTime = time(NULL);
	tm* _tm = localtime(&curTime);

	DataManager::sharedDataManager()->SetTimeLifeToFriend(fbId, _tm);
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
	int oldBoom = DataManager::sharedDataManager()->GetBoom();
	SetBoom(oldBoom + 1);
}

void DataManager::DecreaseBoom()
{
	int oldBoom = DataManager::sharedDataManager()->GetBoom();
	SetBoom(oldBoom - 1);
}




int DataManager::GetDiamon()
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey("DIAMON", G_DEFAULT_DIAMON);
}

void DataManager::SetDiamon( int diamon )
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("DIAMON", diamon);
	CCUserDefault::sharedUserDefault()->flush();
}



void DataManager::SetTimeRefreshFriend( tm* time )
{
	SetTime("LAST_REFRESH_FRIEND", time);
}

tm* DataManager::GetTimeRefreshFriend()
{
	return GetTime("LAST_REFRESH_FRIEND");
}

void DataManager::SetTimeRefreshFriendNow()
{
	time_t curTime = time(NULL);
	tm* _tm = localtime(&curTime);

	DataManager::sharedDataManager()->SetTimeRefreshFriend(_tm);
}



CCArray* DataManager::GetHigherFriends()
{
	string str = CCUserDefault::sharedUserDefault()->getStringForKey("HIGHER_FRIENDS", string("{list:[]}")); //empty list
	CCLOG("GET: %s", str.c_str());

	//get score from response
	json_t *root;
	json_error_t error;
	json_t *friendList;

	root = json_loads(str.c_str(), strlen(str.c_str()), &error);
	friendList = json_object_get(root, "list");

	//foreach to get all friend, insert to list
	int count = json_array_size(friendList);
	CCLOG("COUNT: %d", count);
	CCArray* arrFriends = CCArray::create();

	for(int i = 0; i < count; i++)
	{
		json_t *fbFriend = json_array_get(friendList, i);

		json_t* fbId;
		json_t* fbName;
		json_t* email;
		json_t* score;


		fbId = json_object_get(fbFriend, "fbId");
		fbName = json_object_get(fbFriend, "fbName");
		email = json_object_get(fbFriend, "email");
		score = json_object_get(fbFriend, "score");

		CCLOG("%s : %s", json_string_value(fbName), json_string_value(score));

		FacebookAccount* acc = new FacebookAccount(
			json_string_value(fbId),
			json_string_value(fbName), 
			std::string(json_string_value(email)), 
			(int)json_number_value(score));
		arrFriends->addObject(acc);
	}

	return arrFriends;
}

void DataManager::SetHigherFriends( CCArray* arrFriends )
{
    if (arrFriends == NULL) { //clear all
        CCUserDefault::sharedUserDefault()->setStringForKey("HIGHER_FRIENDS", "{list: []}");
        CCUserDefault::sharedUserDefault()->flush();
    } else {
        //parse arrFriendList to json
        std::string strFriendList = std::string("");
        int count = arrFriends->count();

        for (int i = 0; i < count; ++i)
        {
            FacebookAccount* fbFriend = (FacebookAccount*)arrFriends->objectAtIndex(i);
            strFriendList.append(fbFriend->toJson());

            if (i != count - 1)
            {
                strFriendList.append(",");
            }
        }

        CCString* strData = CCString::createWithFormat("{\"list\": [%s]}", strFriendList.c_str());
        CCLOG("SAVE: %s", strData->getCString());

        CCUserDefault::sharedUserDefault()->setStringForKey("HIGHER_FRIENDS", strData->getCString());
        CCUserDefault::sharedUserDefault()->flush();
    }
}



std::string DataManager::GetRegistrationId()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("G_REGISTRATION_ID", std::string(""));
}

void DataManager::SetRegistrationId( const char* regid )
{
	CCUserDefault::sharedUserDefault()->setStringForKey("G_REGISTRATION_ID", std::string(regid));
	CCUserDefault::sharedUserDefault()->flush();
}



//default = true
bool DataManager::GetFirstTimeViewTutorial()
{
	return CCUserDefault::sharedUserDefault()->getBoolForKey("G_IS_FIRST_TIME_VIEW_TUTORIAL", true);
}

void DataManager::SetFirstTimeViewTutorial(bool isFirstTime)
{
	CCUserDefault::sharedUserDefault()->setBoolForKey("G_IS_FIRST_TIME_VIEW_TUTORIAL", isFirstTime);
	CCUserDefault::sharedUserDefault()->flush();
}



std::string DataManager::GetLanguage()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("CURRENT_LANGUAGE", std::string("Vietnamese"));
}

void DataManager::SetLanguage( const char* language )
{
	CCUserDefault::sharedUserDefault()->setStringForKey("CURRENT_LANGUAGE", std::string(language));
	CCUserDefault::sharedUserDefault()->flush();
}


//default = true
bool DataManager::GetFirstTimeChooseLanguage()
{
	return CCUserDefault::sharedUserDefault()->getBoolForKey("IS_FIRST_TIME_CHOOSE_LANGUAGE", true);
}

void DataManager::SetFirstTimeChooseLanguage(bool isFirstTime)
{
	CCUserDefault::sharedUserDefault()->setBoolForKey("IS_FIRST_TIME_CHOOSE_LANGUAGE", isFirstTime);
	CCUserDefault::sharedUserDefault()->flush();
}


//default = G_IS_MONEYTIZE
bool DataManager::GetIsMoneytize()
{
	return CCUserDefault::sharedUserDefault()->getBoolForKey("IS_MONEYTIZE", G_IS_MONEYTIZE);
}

void DataManager::SetIsMoneytize(bool ismoneytize)
{
	CCUserDefault::sharedUserDefault()->setBoolForKey("IS_MONEYTIZE", ismoneytize);
	CCUserDefault::sharedUserDefault()->flush();
}

