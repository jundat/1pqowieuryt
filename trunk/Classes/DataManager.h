
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

	std::string GetFbID();
	void SetFbProfileID(const char* profileID);

	std::string GetFbUserName();
	void SetFbUserName(const char* fbusernam);

	std::string GetFbFullName();
	void SetFbFullName(const char* fbusernam);

	std::string GetFbEmail();
	void SetFbEmail(const char* fbemail);

	//facebook

	//device

	std::string	GetRegistrationId();
	void	SetRegistrationId(const char* regid);

	//device

	std::string	GetName();
	void	SetName(const char* name);
	std::string	GetUsername();
	void	SetUsername(const char* username);
	std::string	GetPassword();
	void	SetPassword(const char* pass);

	int		GetHighScore();
	void	SetHighScore(int currentScore);

	tm*		GetLastDeadTime();
	void	SetLastDeadTime(tm* time); //int hour, int min, int sec, int mday, int mon, int year);
	void	SetLastDeadTimeNow();

	int		GetLastPlayerLife();
	void	SetLastPlayerLife(int lastLife);
	void	RefreshPlayerLife();

	bool	GetIsJustRevived();
	void	SetIsJustRevived(bool isJustRevived);

	int		GetValueFromKey(const char* key);
	void	SetValueFromKey(const char* key, int val);

	int		GetGiftFromFriend(const char* fbID);
	void	IncreaseGiftFromFriend(const char* fbID);
	void	DecreaseGiftFromFriend(const char* fbID);

	tm*		GetTime(const char* key);
	void	SetTime(const char* key, tm* time);

	tm*		GetTimeBoomFriend(const char* fbId);
	void	SetTimeBoomFriend(const char* fbId, tm* time);
	void	SetTimeBoomFriendNow(const char* fbId);

	bool	GetIsJustGetBoomNowFriend(const char* fbId); ////////////////////////////////// new ////////////////////////////////////////
	void	SetIsJustGetBoomNowFriend(const char* fbId, bool isTrue);

	tm*		GetTimeLifeToFriend(const char* fbId);
	void	SetTimeLifeToFriend(const char* fbId, tm* time);
	void	SetTimeLifeToFriendNow(const char* fbId);


	int		GetBoom();
	void	SetBoom(int boom);
	void	IncreaseBoom();
	void	DecreaseBoom();

	int		GetDiamon();
	void	SetDiamon(int diamon);
	
	tm*		GetTimeRefreshFriend();
	void	SetTimeRefreshFriend(tm* time);
	void	SetTimeRefreshFriendNow();


	CCArray*	GetHigherFriends();
	void		SetHigherFriends(CCArray* arrFriends);

	bool	GetFirstTimeViewTutorial();
	void	SetFirstTimeViewTutorial(bool isFirstTime);

	std::string		GetLanguage();
	void			SetLanguage(const char* language);

	bool	GetFirstTimeChooseLanguage();
	void	SetFirstTimeChooseLanguage(bool isFirstTime);

	bool	GetIsMoneytize();
	void	SetIsMoneytize(bool isMoneytize);
	
	~DataManager(void);
};
#endif
