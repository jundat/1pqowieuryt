
#ifndef __DATA_MANAGER__
#define __DATA_MANAGER__

#include "cocos2d.h"
USING_NS_CC;


class DataManager
{
private:
	DataManager();
	static DataManager *_instance;

public:
	static DataManager* GetInstance();

	int		GetCurrenHighScore();
	void	SetCurrentHighScore(int currentLevel);

	int		GetDeadTime();
	void	SetDeadTime(int currentLevel);

	int		GetValueFromKey(char* key);
	void	SetValueFromKey(char* key, int val);
	
	~DataManager(void);
};
#endif
