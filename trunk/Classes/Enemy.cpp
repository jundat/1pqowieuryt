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

	
	//0.05 -> 0.5
	//delta = 0.45 in 480s
	//1000 / 0.45 = 2222 (~2000)
	vy = G_MIN_ENEMY_VY - difficulty/2000; //min -> min + 0.45

	//min = 1
	//max = 5
	//1000 / 4 = 250 (~200)
	hp = G_MIN_ENEMY_HP + difficulty/200;

	//min = 1
	//max = 5
	//1000 / 4 = 250 (~200)
	dm = G_MIN_ENEMY_DAM + difficulty/200;

	CCLOG("Vy: %f\tHp: %d\tDam: %d", vy, hp, dm);
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

	m_EffectLayer = EffectLayer::create();
	this->addChild(m_EffectLayer, 100);

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

	if (m_hp > 0)
	{
		//small effect explosion
		m_EffectLayer->AddExploisionEff(2, CCPointZero);
	}
	else
	{
		CCSize s = getContentSize();
		CCPoint p1 = CCPointZero;
		CCPoint p2 = ccp(-s.width/2, s.height/2);
		CCPoint p3 = ccp(s.width/2, s.height/2);

		//big effect explosion
		float t1 = m_EffectLayer->AddExploisionEff(3, p1);
		float t2 = m_EffectLayer->AddExploisionEff(3, p2);
		float t3 = m_EffectLayer->AddExploisionEff(3, p3);
		t1 = (t1 > t2) ? t1 : t2;
		t1 = (t1 > t3) ? t1 : t3;

		CCSequence* sequence = CCSequence::create(
			CCDelayTime::create(t1),
			CCCallFunc::create(this, callfunc_selector(Enemy::removeFromParent)),
			NULL
			);
		this->runAction(sequence);
	}
}
