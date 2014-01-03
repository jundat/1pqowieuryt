
#ifndef __DATA_MANAGER__
#define __DATA_MANAGER__

#include "cocos2d.h"
#include <time.h>
USING_NS_CC;


class DataManager
{
private:
	DataManager();
	static DataManager *_instance;

public:
	static DataManager* sharedDataManager();

	std::string	GetUsername();
	void	SetUsername(const char* username);
	std::string	GetPassword();
	void	SetPassword(const char* pass);

	long	GetCurrenHighScore();
	void	SetCurrentHighScore(long currentScore);

	tm*		GetLastDeadTime();
	void	SetLastDeadTime(tm* time); //int hour, int min, int sec, int mday, int mon, int year);
	void	SetLastDeadTimeNow();

	int		GetLastPlayerLife();
	void	SetLastPlayerLife(int lastLife);

	bool	GetIsJustRevived();
	void	SetIsJustRevived(bool isJustRevived);

	int		GetValueFromKey(char* key);
	void	SetValueFromKey(char* key, int val);

	
	
	~DataManager(void);
};
#endif
