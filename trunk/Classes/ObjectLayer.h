#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__

#include "cocos2d.h"
#include "Ship.h"
#include "Bullet.h"
#include "EffectLayer.h"

USING_NS_CC;

class ObjectLayer : public cocos2d::CCLayer
{
public:
	ObjectLayer(){
		m_arrEnemi = new CCArray();
		m_arrPlayerBullets = new CCArray();
		m_arrEnemiBullets = new CCArray();
	};
	~ObjectLayer(){};
    virtual bool init();
	virtual void update(float delta);

public:
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

public:
	void SchedulePlayerFire(float dt);
	void ScheduleGenerateEnemi(float dt);
	void AddBullet(Bullet* bullet);

private:
	void _UpdateCollision();

private:
	int m_score;
	CCLabelTTF* m_pLabelScore;

	Ship* m_player;
	bool m_IsTouchedPlayer; //check if touched player
	CCPoint m_lastPoint;
	
	CCArray* m_arrEnemi;
	float m_timeToGenerateEnemi;

	CCArray* m_arrPlayerBullets;
	CCArray* m_arrEnemiBullets;

	EffectLayer* m_EffectLayer;

};

#endif // __GAME_OBJECT_MANAGER_H__
