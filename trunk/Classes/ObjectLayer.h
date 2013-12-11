#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__

#include "cocos2d.h"
#include "Ship.h"
#include "Bullet.h"
#include "EffectLayer.h"
#include "Global.h"
#include "MyMacro.h"

USING_NS_CC;

class ObjectLayer : public cocos2d::CCLayer
{
public:
	ObjectLayer(){}
	~ObjectLayer(){
		RELEASE(m_arrEnemy);
		RELEASE(m_arrPlayerBullets);
		RELEASE(m_arrEnemyBullets);
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
	void ContinueGame();
	void RestartGame();

private:
	void ScheduleCheckCollision(float dt);

private:
	CC_SYNTHESIZE(float, m_playedTime, PlayedTime);

	bool isEndGame;
	int m_score;
	CCLabelTTF* m_pLabelScore;
	CCLabelTTF* m_pLabelHp;

	Ship* m_player;
	CCPoint m_lastPoint;
		
	CCArray* m_arrEnemy;
	float m_timeToGenerateEnemy;
	
	CCArray* m_arrPlayerBullets;
	CCArray* m_arrEnemyBullets;

	EffectLayer* m_EffectLayer;

	//pixel check collision
	static CCRenderTexture* _rt;
};

#endif // __GAME_OBJECT_MANAGER_H__
