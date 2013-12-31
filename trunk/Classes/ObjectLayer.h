#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__

#include "cocos2d.h"
#include "Global.h"
#include "MyMacro.h"
#include "Ship.h"
#include "Bullet.h"
#include "EffectLayer.h"
#include "Item.h"

USING_NS_CC;

class ObjectLayer : public cocos2d::CCLayer
{
public:
	ObjectLayer(){}
	~ObjectLayer(){
		RELEASE(m_arrEnemies);
		RELEASE(m_arrPlayerBullets);
		RELEASE(m_arrEnemyBullets);
		RELEASE(m_arrItems);
		
		if (_rt != NULL)
		{
			_rt->release();
		}
	};

    virtual bool init();
	CREATE_FUNC(ObjectLayer);
	virtual void update(float delta);

public:
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void ScheduleGenerateEnemy(float dt);
	void AddBullet(Bullet* bullet);
	void AddItem(Item* item);
	void IncreaseBoom();
	void ActiveBoom(CCObject* pSender);
	void ContinueGame();
	void RestartGame();
	void Pause();
	void Resume();

public:
	void AfterDeadEffectCallback();

private:
	void ScheduleCheckCollision(float dt);

private:
	CC_SYNTHESIZE(float, m_playedTime, PlayedTime);
	CC_SYNTHESIZE(int, m_score, Score);
	CC_SYNTHESIZE(float, m_difficulty, Difficulty);
	CC_SYNTHESIZE(bool, m_isEndGame, IsEndGame);
	CC_SYNTHESIZE(Ship*, m_player, Player);
	CC_SYNTHESIZE(int, m_numberBoom, NumberBoom);
	CC_SYNTHESIZE(float, m_genTimeCounter, GenTimeCounter);

	CCLabelBMFont* m_labelScore;
	CCLabelBMFont* m_labelBoom;
	CCMenuItemImage* m_itemBoom;
	
	CCPoint m_lastPoint;
	
	CCArray* m_arrEnemies;
	
	CCArray* m_arrPlayerBullets;
	CCArray* m_arrEnemyBullets;
	
	CCArray* m_arrItems;

	EffectLayer* m_EffectLayer;

	//pixel check collision
	static CCRenderTexture* _rt;
};

#endif // __GAME_OBJECT_MANAGER_H__
