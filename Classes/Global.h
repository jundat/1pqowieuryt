#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <string>

//////////////////////////////////////////////////////////////////////////
//CONSTANT
#define G_CCWHITE4				ccc4(255, 255, 255, 255)
#define G_DIM_COLOR				ccc4(0, 0, 0, 128)
#define G_FONT_NORMAL			"Roboto-Medium.ttf"
#define G_FONT_LEADERBOARD		"Villa.ttf"

#define G_NUMBER_FRIEND_TO_GET	200

//GAME CONSTANT
#define G_APP_ID				"DBPTK"
#define G_DEFAULT_NAME			"Việt Nam"
#define G_AVATAR_SIZE			128
#define G_FRIEND_AVATAR_SIZE	110

#define	G_BULLET_PLAYER_ID		1
#define G_BULLET_ENEMY_ID		2

#define G_ITEM_UPGRADE_BULLET	1
#define G_ITEM_ARMOR			2
#define G_ITEM_BOOM				3

#define G_CHECK_INSTALLED		"CHECK_INSTALLED"
#define G_INSTALLED				1
#define G_FPS					1/60.0f
#define G_SMALL_DEVIVE_SIZE		320
#define	G_DEFAULT_VOLUME		0.5f


//////////////////////////////////////////////////////////////////////////
//


//SEND DATA
extern std::string G_URL_PLAYER_FB_PROFILE;
extern std::string G_URL_FRIEND_LIST;
extern std::string G_URL_DEVICE_PROFILE;
extern std::string G_URL_SCORE;
extern std::string G_URL_REVIVE;
extern std::string G_URL_GET_LAZER;

extern std::string G_URL_RATE;


//VARIANT

extern int G_DESIGN_WIDTH;
extern int G_DESIGN_HEIGHT;
extern float G_SCALE_FACTOR;

extern int G_PLAYER_HP;
extern int G_PLAYER_DAMAGE;
extern int G_MIN_PLAYER_BULLET_LEVEL;
extern int G_MAX_PLAYER_BULLET_LEVEL;

extern float G_TIMEOUT_BULLET_LEVEL;

extern float G_ITEM_BULLET_RANDOM_PERCENT;
extern float G_ITEM_ARMOR_RANDOM_PERCENT;
extern float G_ITEM_BOOM_RANDOM_PERCENT;

extern int G_PLAYER_TIME_TO_REVIVE;
extern int G_MAX_PLAYER_LIFE; 

extern int G_MAX_PLAYER_BOOM;

extern int G_MIN_ENEMY_HP;
extern int G_MIN_ENEMY_DAM;
extern float G_PLAYER_LAZER_TIME;
extern float G_PLAYER_ARMOR_TIME;
extern float G_PLAYER_BULLET_VY;

extern float G_PLAYER_TIME_TO_FIRE;
extern float G_ENEMY_TIME_TO_FIRE;

extern float G_DEFAULT_TIME_TO_GENERATE_ENEMY;
extern float G_TIME_TO_GENERATE_ITEM;

extern float G_MIN_ENEMY_VY;
extern float G_MAX_ENEMY_VY;

extern float G_MIN_ENEMY_BULLET_VY;
extern float G_BULLET_VY_FROM_ENEMY; //bulletVy =  BULLET_VY_FROM_ENEMY * enemyVy

extern float G_DEFAULT_DIFFICULTY;

extern float G_VELOCITY_IN_DIFFICULTY;		//velocity = diff / G_VELOCITY_IN_DIFFICULTY
extern float G_HP_IN_DIFFICULTY;			//hp = diff / G_HP_IN_DIFFICULTY
extern float G_DAMAGE_IN_DIFFICULTY;		//damage = diff / G_DAMAGE_IN_DIFFICULTY


extern int G_ENEMY_1_SCORE;		//score for enemy_1
extern int G_ENEMY_2_SCORE;		//score for enemy_2
extern int G_ENEMY_3_SCORE;		//score for enemy_3


extern float G_ENEMY_1_PERCENT;
extern float G_ENEMY_2_PERCENT;
extern float G_ENEMY_3_PERCENT;


extern int G_DEFAULT_BOOM;
extern int G_MAX_BOOM;


extern int G_DEFAULT_DIAMON;
extern int G_DIAMON_PER_LIFE;
extern int G_DIAMON_TO_GET_BOOM_NOW;

//Thời gian hiển thị của Nút REVIVE trong Lose Scene
extern float G_WAIT_TO_REVIVE;

extern int G_POINTS_PER_SECOND;

extern float G_TIME_TO_REFRESH_FRIENDS;

extern bool G_IS_ENCODE;
extern bool G_IS_MONEYTIZE;




#endif //__GLOBAL_H__
