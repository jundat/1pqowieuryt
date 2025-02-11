﻿#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__

#include "cocos2d.h"
#include "Global.h"
#include "MyMacro.h"
#include "Ship.h"
#include "Bullet.h"
#include "EffectLayer.h"
#include "Item.h"
#include "Enemy.h"
#include "EnemyFactory.h"

USING_NS_CC;


#define CROSS_NUMBER  3
#define BULLET_COOL_DOWN    true



class ObjectLayer : public cocos2d::CCLayer
{
public:
	ObjectLayer(){}
	~ObjectLayer(){
		MY_RELEASE(m_arrEnemies);
		MY_RELEASE(m_arrPlayerBullets);
		MY_RELEASE(m_arrItems);
	};

    virtual bool init();
	CREATE_FUNC(ObjectLayer);
	virtual void update(float delta);

public:
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

    
	void AddBullet(Bullet* bullet);
	void AddItem(Item* item);
	void AddEmemy(Enemy* enemy);
	void RemoveEnemy(Enemy* enemy);
	void IncreaseBoom();
	void ActiveBoom(CCObject* pSender);
	void Revive();
	void Restart();
	void Pause();
	void Resume();
	void resetCrosses();
	void IncreaseCross();
	void Vibrate(int _time = 300);

public:
	void AfterDeadEffectCallback();

private:
	void GenerateEnemy(float dt);
	void ScheduleGenerateItem(float dt);
	void ScheduleCheckCollision(float dt);
    
    void addScore(CCObject* pSender);

public:
	CC_SYNTHESIZE(float, m_playedTime, PlayedTime);
	CC_SYNTHESIZE(int, m_killedEnemies, KilledEnemies);
	CC_SYNTHESIZE(int, m_score, Score);
	CC_SYNTHESIZE(float, m_difficulty, Difficulty);
	CC_SYNTHESIZE(bool, m_isEndGame, IsEndGame);
	CC_SYNTHESIZE(bool, m_isPauseGame, IsPauseGame);
	CC_SYNTHESIZE(bool, m_isActiveBoom, IsActiveBoom);
	CC_SYNTHESIZE(Ship*, m_player, Player);
	CC_SYNTHESIZE(int, m_crossCount, CrossCount);


	void ScheduleStopLazer(float dt);
	CCLabelTTF* m_labelScore;
	CCLabelTTF* m_labelBoom;
	CCMenuItemImage* m_itemBoom;
	CCSprite* m_sprLazer;
	CCSprite* m_arrSprCrosses[CROSS_NUMBER];

	
	CCPoint m_lastPoint;
	
	CCArray* m_arrEnemies;
	CCArray* m_arrPlayerBullets;
	CCArray* m_arrItems;

	EffectLayer* m_EffectLayer;

	EnemyFactory* m_enemyFactory;
    
    //cool down

#ifdef BULLET_COOL_DOWN
    CCSprite* m_sprCooldown;
    CCSprite* m_sprCooldownBlank;
#endif
};

#endif // __GAME_OBJECT_MANAGER_H__
