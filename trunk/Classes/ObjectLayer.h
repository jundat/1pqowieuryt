#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__

#include "cocos2d.h"
#include "Ship.h"
#include "Bullet.h"
#include "EffectLayer.h"
#include "HSJoystick.h"
#include "Global.h"

USING_NS_CC;

class ObjectLayer : public cocos2d::CCLayer
{
public:
	ObjectLayer(){}
	~ObjectLayer(){
		RELEASE(m_arrEnemi);
		RELEASE(m_arrPlayerBullets);
		RELEASE(m_arrEnemiBullets);
	};

    virtual bool init();
	CREATE_FUNC(ObjectLayer);
	virtual void update(float delta);

public:
	void SchedulePlayerFire(float dt);
	void ScheduleGenerateEnemi(float dt);
	void AddBullet(Bullet* bullet);

private:
	void _updateCollision();

private:
	int m_score;
	CCLabelTTF* m_pLabelScore;

	Ship* m_player;
	bool m_IsTouchedPlayer; //check if touched player
	CCPoint m_lastPoint;
	HSJoystick* m_Joystick;
		
	CCArray* m_arrEnemi;
	float m_timeToGenerateEnemi;
	
	CCArray* m_arrPlayerBullets;
	CCArray* m_arrEnemiBullets;

	EffectLayer* m_EffectLayer;

};

#endif // __GAME_OBJECT_MANAGER_H__
