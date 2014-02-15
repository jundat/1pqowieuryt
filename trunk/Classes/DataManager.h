
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

	//facebook

	std::string GetPhotoPath();
	void SetPhotoPath(const char* path);

	std::string GetProfileID();
	void SetProfileID(const char* profileID);

	std::string GetFbUserName();
	void SetFbUserName(const char* fbusernam);

	//facebook

	std::string	GetName();
	void	SetName(const char* name);
	std::string	GetUsername();
	void	SetUsername(const char* username);
	std::string	GetPassword();
	void	SetPassword(const char* pass);

	int	GetHighScore();
	void	SetCurrentHighScore(int currentScore);

	tm*		GetLastDeadTime();
	void	SetLastDeadTime(tm* time); //int hour, int min, int sec, int mday, int mon, int year);
	void	SetLastDeadTimeNow();

	int		GetLastPlayerLife();
	void	SetLastPlayerLife(int lastLife);
	void	RefreshPlayerLife();

	bool	GetIsJustRevived();
	void	SetIsJustRevived(bool isJustRevived);

	int		GetValueFromKey(char* key);
	void	SetValueFromKey(char* key, int val);

	int		GetGiftFromFriend(const char* fbID);
	void	IncreaseGiftFromFriend(const char* fbID);
	void	DecreaseGiftFromFriend(const char* fbID);
	
	
	~DataManager(void);
};
#endif
