#include "DataManager.h"

DataManager* DataManager::_instance = 0;


DataManager* DataManager::GetInstance()
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