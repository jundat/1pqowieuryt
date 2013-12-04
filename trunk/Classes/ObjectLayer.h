#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__

#include "cocos2d.h"
#include "Ship.h"
USING_NS_CC;

class ObjectLayer : public cocos2d::CCLayer
{
public:
	ObjectLayer(){};
	~ObjectLayer(){};
    virtual bool init();

public:
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

public:
	Ship* GetPlayer(){ return m_player; }

private:
	Ship* m_player;
	bool m_IsTouchedPlayer; //check if touched player
};

#endif // __GAME_OBJECT_MANAGER_H__
