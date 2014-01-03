
#include "Global.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//player & enemy

//ID
int G_BULLET_PLAYER_ID = 1;
int G_BULLET_ENEMY_ID = 2;

float G_ENEMY_TYPE = 3;

int G_ITEM_UPGRADE_BULLET = 1;
int G_ITEM_ARMOR = 2;
int G_ITEM_BOOM = 3;

//////////////////////////////////////////////////////////////////////////

//Variant
int G_DESIGN_WIDTH = 800;
int G_DESIGN_HEIGHT = 1280;
float G_SCALE_FACTOR = 0.55f;

int G_PLAYER_HP = 100;
int G_PLAYER_DAMAGE = 1;
int G_MIN_PLAYER_BULLET_LEVEL = 1;
int G_MAX_PLAYER_BULLET_LEVEL = 3;

float G_TIMEOUT_BULLET_LEVEL = 3.0f;

float G_ITEM_BULLET_RANDOM_PERCENT = 0.1f;
float G_ITEM_ARMOR_RANDOM_PERCENT = 0.2f;
float G_ITEM_BOOM_RANDOM_PERCENT = 0.3f;

float G_PLAYER_TIME_TO_REVIVE = 30*60;
int G_MAX_PLAYER_LIFE = 5;
std::string G_LAST_DEAD_TIME;

int G_MAX_PLAYER_BOOM = 3;

int G_MIN_ENEMY_HP = 1;
int G_MIN_ENEMY_DAM = 1;
float G_PLAYER_ARMOR_TIME = 10.0f;
float G_PLAYER_BULLET_VY = 1.0f;

float G_PLAYER_TIME_TO_FIRE = 0.4f;
float G_ENEMY_TIME_TO_FIRE = 2.5f;

float G_DEFAULT_TIME_TO_GENERATE_ENEMY = 1.0f;

float G_MIN_ENEMY_VY = -0.05f;
float G_MAX_ENEMY_VY = -0.5f;

float G_MIN_ENEMY_BULLET_VY = -0.4f;
float G_BULLET_VY_FROM_ENEMY = 1.5f; //bulletVy =  BULLET_VY_FROM_ENEMY * enemyVy

float G_DEFAULT_DIFFICULTY = 1.0f;

float G_VELOCITY_IN_DIFFICULTY = 2000;
float G_HP_IN_DIFFICULTY = 200;
float G_DAMAGE_IN_DIFFICULTY = 200;


int G_SCORE_1 = 100;		//score for enemy_1
int G_SCORE_2 = 300;		//score for enemy_2
int G_SCORE_3 = 1000;		//score for enemy_3
