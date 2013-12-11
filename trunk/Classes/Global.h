#ifndef __GLOBAL_H__
#define __GLOBAL_H__


//////////////////////////////////////////////////////////////////////////
//common

const static int G_DESIGN_WIDTH = 800;
const static int G_DESIGN_HEIGHT = 1280;
const static float G_SCALE_FACTOR = 0.55f;

//////////////////////////////////////////////////////////////////////////
//player & enemy

const static int G_PLAYER_HP = 100;
const static int G_PLAYER_DAMAGE = 1;

const static int G_ENEMY_HP = 1;
const static int G_ENEMY_DAM = 2;

const static float G_ENEMY_BULLET_VY = -0.5f;
const static float G_PLAYER_BULLET_VY = +0.7f;

const static float G_PLAYER_TIME_TO_FIRE = 0.4f;
const static float G_ENEMY_TIME_TO_FIRE = 1.0f;

const static float G_DEFAULT_TIME_TO_GENERATE_ENEMY = 1.0f;

const static float G_ENEMY_TYPE = 2;
const static float G_ENEMY_VY = -0.2f;

const static int G_BULLET_PLAYER_ID = 1;
const static int G_BULLET_ENEMY_ID = 2;

const static float G_BULLET_VY_FROM_ENEMY = 1.5f; //bulletVy =  BULLET_VY_FROM_ENEMY * enemyVy


#endif //__GLOBAL_H__
