#include "Bullet.h"
#include "ObjectLayer.h"
#include "Enemy.h"
#include "Global.h"

USING_NS_CC;


static void DifficultySplit(float difficulty, float& vy, int& hp, int& dm)
{
	//vy = A * diff
	//hp = B * diff
	//dm = C * diff
	//
	//random A, B, C
// 	float A = CCRANDOM_0_1();
// 	float B = CCRANDOM_0_1();
// 	float C = CCRANDOM_0_1();
// 
// 	vy = A * (difficulty/(A+B+C));
// 	hp = B * (difficulty/(A+B+C));
// 	dm = C * (difficulty/(A+B+C));
	static float currentVy = G_MIN_ENEMY_VY;
	vy = currentVy;
	hp = G_ENEMY_HP;
	dm = G_ENEMY_DAM;
}


Enemy::Enemy(float difficulty) : GameObject()
{
	this->setDifficulty(difficulty);
}

bool Enemy::init()
{
	if (!GameObject::init())
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////

	DifficultySplit(m_difficulty, m_vy, m_hp, m_damage);
	this->setVx(0);
	this->setEnemyType(G_ENEMY_TYPE);
	m_sprite = CCSprite::create("enemy_1.png");
	m_sprite->setPosition(CCPointZero);
	this->addChild(m_sprite);

	this->schedule(schedule_selector(Enemy::ScheduleFire), G_ENEMY_TIME_TO_FIRE);
	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();
	return true;
}

void Enemy::Fire()
{
	float bulletvy = G_BULLET_VY_FROM_ENEMY * this->getVy();
	bulletvy = (bulletvy < G_MIN_ENEMY_BULLET_VY) ? bulletvy : G_MIN_ENEMY_BULLET_VY;

	Bullet* bullet = Bullet::create(G_BULLET_ENEMY_ID, bulletvy, this->getDamage(), this->getPosition());
	ObjectLayer* parent = (ObjectLayer*)this->getParent();
	parent->AddBullet(bullet);
}

void Enemy::ScheduleFire( float dt )
{
	this->Fire();
}

void Enemy::update( float delta )
{
	this->setPositionX(m_vx * delta * 1000 + this->getPositionX());
	this->setPositionY(m_vy * delta * 1000 + this->getPositionY());
}

void Enemy::HitBullet(int damage)
{
	m_hp -= damage;
}
