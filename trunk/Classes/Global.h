#ifndef __GLOBAL_H__
#define __GLOBAL_H__


#include <string>
#include "ConfigLoader.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////
//MACRO

#pragma region MACRO



//get value in ConfigLoader
#define CONF_INT(key)		ConfigLoader::shareConfigLoader()->GetValue(#key)->intValue()
#define CONF_FLOAT(key)		ConfigLoader::shareConfigLoader()->GetValue(#key)->floatValue()
#define CONF_STR(key)		ConfigLoader::shareConfigLoader()->GetValue(#key)->getCString()
#define CONF_CCSTR(key)		ConfigLoader::shareConfigLoader()->GetValue(#key)


//create a config variant
#define CREATE_CONF_INT(key)	static int key = CONF_INT(key)
#define CREATE_CONF_FLOAT(key)	static float key = CONF_FLOAT(key)
#define CREATE_CONF_STR(key)	static std::string key = CONF_STR(key)

#pragma endregion

//////////////////////////////////////////////////////////////////////////
//CONSTANT

#pragma region CONSTANT

#define G_DEFAULT_NAME			"Việt Nam"
#define G_AVATAR_SIZE			128
#define G_FRIEND_AVATAR_SIZE	128


#define	G_BULLET_PLAYER_ID		0
#define	G_BULLET_ENEMY_ID		1


#define	G_ENEMY_TYPE			3


#define	G_ITEM_UPGRADE_BULLET	0
#define	G_ITEM_ARMOR			1
#define	G_ITEM_BOOM				2


#define	G_MOVE_STRAINGH			0
#define	G_MOVE_CIRCLE_LEFT		1
#define	G_MOVE_CIRCLE_RIGHT		2


#pragma endregion

//////////////////////////////////////////////////////////////////////////
//VARIANT

#pragma region VARIANT


CREATE_CONF_INT(G_DESIGN_WIDTH);
CREATE_CONF_INT(G_DESIGN_HEIGHT);
CREATE_CONF_FLOAT(G_SCALE_FACTOR);

CREATE_CONF_INT(G_PLAYER_HP);
CREATE_CONF_INT(G_PLAYER_DAMAGE);
CREATE_CONF_INT(G_MIN_PLAYER_BULLET_LEVEL);
CREATE_CONF_INT(G_MAX_PLAYER_BULLET_LEVEL);

CREATE_CONF_FLOAT(G_TIMEOUT_BULLET_LEVEL);

CREATE_CONF_FLOAT(G_ITEM_BULLET_RANDOM_PERCENT);
CREATE_CONF_FLOAT(G_ITEM_ARMOR_RANDOM_PERCENT);
CREATE_CONF_FLOAT(G_ITEM_BOOM_RANDOM_PERCENT);

CREATE_CONF_INT(G_PLAYER_TIME_TO_REVIVE);
CREATE_CONF_INT(G_MAX_PLAYER_LIFE);


CREATE_CONF_INT(G_MAX_PLAYER_BOOM);

CREATE_CONF_INT(G_MIN_ENEMY_HP);
CREATE_CONF_INT(G_MIN_ENEMY_DAM);
CREATE_CONF_FLOAT(G_PLAYER_LAZER_TIME);
CREATE_CONF_FLOAT(G_PLAYER_ARMOR_TIME);
CREATE_CONF_FLOAT(G_PLAYER_BULLET_VY);

CREATE_CONF_FLOAT(G_PLAYER_TIME_TO_FIRE);
CREATE_CONF_FLOAT(G_ENEMY_TIME_TO_FIRE);

CREATE_CONF_FLOAT(G_DEFAULT_TIME_TO_GENERATE_ENEMY);
CREATE_CONF_FLOAT(G_TIME_TO_GENERATE_ITEM);

CREATE_CONF_FLOAT(G_MIN_ENEMY_VY);
CREATE_CONF_FLOAT(G_MAX_ENEMY_VY);

CREATE_CONF_FLOAT(G_MIN_ENEMY_BULLET_VY);
CREATE_CONF_FLOAT(G_BULLET_VY_FROM_ENEMY); //bulletVy =  BULLET_VY_FROM_ENEMY * enemyVy

CREATE_CONF_FLOAT(G_DEFAULT_DIFFICULTY);

CREATE_CONF_FLOAT(G_VELOCITY_IN_DIFFICULTY);
CREATE_CONF_FLOAT(G_HP_IN_DIFFICULTY);
CREATE_CONF_FLOAT(G_DAMAGE_IN_DIFFICULTY);


CREATE_CONF_INT(G_ENEMY_1_SCORE);		//score for enemy_1
CREATE_CONF_INT(G_ENEMY_2_SCORE);		//score for enemy_2
CREATE_CONF_INT(G_ENEMY_3_SCORE);		//score for enemy_3

CREATE_CONF_FLOAT(G_ENEMY_1_PERCENT);
CREATE_CONF_FLOAT(G_ENEMY_2_PERCENT);
CREATE_CONF_FLOAT(G_ENEMY_3_PERCENT);


CREATE_CONF_INT(G_DEFAULT_BOOM);
CREATE_CONF_INT(G_MAX_BOOM);


CREATE_CONF_INT(G_DEFAULT_DIAMON);
CREATE_CONF_INT(G_DIAMON_PER_LIFE);


CREATE_CONF_FLOAT(G_WAIT_TO_REVIVE);


#pragma endregion

#endif //__GLOBAL_H__
