#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__

#include "cocos2d.h"
#include "Ship.h"
#include "Bullet.h"

USING_NS_CC;

class ObjectLayer : public cocos2d::CCLayer
{
public:
	ObjectLayer(){
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
	void PlayerFireCallback(float dt);
	void AddBullet(Bullet* bullet);

private:
	Ship* m_player;
	bool m_IsTouchedPlayer; //check if touched player
	
	CCArray* m_arrPlayerBullets;
	CCArray* m_arrEnemiBullets;
};

#endif // __GAME_OBJECT_MANAGER_H__
