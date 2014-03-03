
#include "Global.h"


//////////////////////////////////////////////////////////////////////////


//SEND DATA
std::string G_URL_PLAYER_FB_PROFILE;
std::string G_URL_FRIEND_LIST;
std::string G_URL_DEVICE_PROFILE;
std::string G_URL_SCORE;




//////////////////////////////////////////////////////////////////////////
//VARIANT

int G_DESIGN_WIDTH = 800;
int G_DESIGN_HEIGHT = 1280;
float G_SCALE_FACTOR = 0.55f;

int G_PLAYER_HP = 0;
int G_PLAYER_DAMAGE = 0;
int G_MIN_PLAYER_BULLET_LEVEL = 0;
int G_MAX_PLAYER_BULLET_LEVEL = 0;

float G_TIMEOUT_BULLET_LEVEL = 0;

float G_ITEM_BULLET_RANDOM_PERCENT = 0.0f;
float G_ITEM_ARMOR_RANDOM_PERCENT = 0.0f;
float G_ITEM_BOOM_RANDOM_PERCENT = 0.0f;

int G_PLAYER_TIME_TO_REVIVE = 0;
int G_MAX_PLAYER_LIFE = 0;
std::string G_LAST_DEAD_TIME;

int G_MAX_PLAYER_BOOM = 3;

int G_MIN_ENEMY_HP = 0;
int G_MIN_ENEMY_DAM = 0;
float G_PLAYER_LAZER_TIME = 0.0f;
float G_PLAYER_ARMOR_TIME = 0.0f;
float G_PLAYER_BULLET_VY = 0.0f;

float G_PLAYER_TIME_TO_FIRE = 0.4f;
float G_ENEMY_TIME_TO_FIRE = 0.0f;

float G_DEFAULT_TIME_TO_GENERATE_ENEMY = 1.0f;
float G_TIME_TO_GENERATE_ITEM = 0;

float G_MIN_ENEMY_VY = -0.05f;
float G_MAX_ENEMY_VY = -0.5f;

float G_MIN_ENEMY_BULLET_VY = -0.4f;
float G_BULLET_VY_FROM_ENEMY = 1.5f; //bulletVy =  BULLET_VY_FROM_ENEMY * enemyVy

float G_DEFAULT_DIFFICULTY = 1.0f;

float G_VELOCITY_IN_DIFFICULTY = 2000;
float G_HP_IN_DIFFICULTY = 200;
float G_DAMAGE_IN_DIFFICULTY = 200;


int G_ENEMY_1_SCORE = 1000;		//score for enemy_1
int G_ENEMY_2_SCORE = 6000;		//score for enemy_2
int G_ENEMY_3_SCORE = 30000;		//score for enemy_3

float G_ENEMY_1_PERCENT = 0.8f;
float G_ENEMY_2_PERCENT = 0.15f;
float G_ENEMY_3_PERCENT = 0.05f;


int G_DEFAULT_BOOM;
int G_MAX_BOOM;


int G_DEFAULT_DIAMON;
int G_DIAMON_PER_LIFE;


float G_WAIT_TO_REVIVE;

int G_POINTS_PER_SECOND;