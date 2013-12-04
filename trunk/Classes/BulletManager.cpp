#include "BulletManager.h"

BulletManager* BulletManager::_instance = NULL;

BulletManager* BulletManager::sharedBulletManager()
{
	if (_instance == NULL)
	{
		_instance = new BulletManager();
	}

	return _instance;
}

void BulletManager::UpdateBullet( float dt )
{
	CCObject* it = NULL;

	CCARRAY_FOREACH(m_arrPlayerBullets, it)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it);
		bullet->update(dt);
	}

	CCARRAY_FOREACH(m_arrEnemiBullets, it)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it);
		bullet->update(dt);
	}
}

