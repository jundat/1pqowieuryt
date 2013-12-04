#ifndef __BULLET_MANAGER_H__
#define __BULLET_MANAGER_H__

#include "cocos2d.h"
#include "Bullet.h"

USING_NS_CC;

class BulletManager : public CCNode
{
public:
	BulletManager(){
		m_arrPlayerBullets = new CCArray();
		m_arrEnemiBullets = new CCArray();
	};

	~BulletManager(){
		if (m_arrPlayerBullets)
		{
			m_arrPlayerBullets->release();
		}


		if (m_arrEnemiBullets)
		{
			m_arrEnemiBullets->release();
		}
	};

	static BulletManager* _instance;
	static BulletManager* sharedBulletManager();

public:
	void UpdateBullet(float dt);
	//bool CheckPlayer(Ship* ship);

public:
	void AddPlayerBullet(Bullet* bullet);
	void AddEnemiBullet(Bullet* bullet);

private:
	CCArray* m_arrPlayerBullets;
	CCArray* m_arrEnemiBullets;
};

#endif //__BULLET_MANAGER_H__