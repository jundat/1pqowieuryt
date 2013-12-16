#ifndef __GLOBAL_H__
#define __GLOBAL_H__


//////////////////////////////////////////////////////////////////////////
//common

 static int G_DESIGN_WIDTH = 800;
 static int G_DESIGN_HEIGHT = 1280;
 static float G_SCALE_FACTOR = 0.55f;

//////////////////////////////////////////////////////////////////////////
//player & enemy

 static int G_PLAYER_HP = 100;
 static int G_PLAYER_DAMAGE = 1;
 static int G_MIN_PLAYER_BULLET_LEVEL = 1;
 static int G_MAX_PLAYER_BULLET_LEVEL = 3;

 static int G_MAX_PLAYER_BOOM = 3;

 static int G_ITEM_UPGRADE_BULLET = 1;
 static int G_ITEM_ARMOR = 2;
 static int G_ITEM_BOOM = 3;

 static int G_MIN_ENEMY_HP = 1;
 static int G_MIN_ENEMY_DAM = 1;
 static float G_PLAYER_ARMOR_TIME = 10.0f;
 static float G_PLAYER_BULLET_VY = +1.0f;

 static float G_PLAYER_TIME_TO_FIRE = 0.4f;
 static float G_ENEMY_TIME_TO_FIRE = 2.5f;

 static float G_DEFAULT_TIME_TO_GENERATE_ENEMY = 1.0f;

 static float G_ENEMY_TYPE = 3;

 static int G_BULLET_PLAYER_ID = 1;
 static int G_BULLET_ENEMY_ID = 2;

 static float G_MIN_ENEMY_VY = -0.05f;
 static float G_MAX_ENEMY_VY = -0.5f;

 static float G_MIN_ENEMY_BULLET_VY = -0.4f;
 static float G_BULLET_VY_FROM_ENEMY = 1.5f; //bulletVy =  BULLET_VY_FROM_ENEMY * enemyVy

 static float G_DEFAULT_DIFFICULTY = 1.0f;



#endif //__GLOBAL_H__
