#include "Bullet.h"
#include "ObjectLayer.h"
#include "Enemy.h"
#include "Global.h"
#include "LevelLoader.h"

USING_NS_CC;


int Enemy::S_HP1 = G_MIN_ENEMY_HP;
int Enemy::S_HP2 = G_MIN_ENEMY_HP;
int Enemy::S_HP3 = G_MIN_ENEMY_HP;
float Enemy::S_VELOCITY1 = G_MIN_ENEMY_VY;
float Enemy::S_VELOCITY2 = G_MIN_ENEMY_VY;
float Enemy::S_VELOCITY3 = G_MIN_ENEMY_VY;
float Enemy::S_GENERATE_TIME = G_DEFAULT_TIME_TO_GENERATE_ENEMY;


int Enemy::S_NUM_ENEMY_1_1 = 0;
int Enemy::S_NUM_ENEMY_1_2 = 0;
int Enemy::S_NUM_ENEMY_1_3 = 0;
int Enemy::S_NUM_ENEMY_1_4 = 0;
int Enemy::S_NUM_ENEMY_1_5 = 0;
int Enemy::S_NUM_ENEMY_1_6 = 0;

int Enemy::S_NUM_ENEMY_2_1 = 0;
int Enemy::S_NUM_ENEMY_2_2 = 0;
int Enemy::S_NUM_ENEMY_2_3 = 0;
int Enemy::S_NUM_ENEMY_2_4 = 0;
int Enemy::S_NUM_ENEMY_2_5 = 0;

int Enemy::S_NUM_ENEMY_3_1 = 0;
int Enemy::S_NUM_ENEMY_3_2 = 0;
int Enemy::S_NUM_ENEMY_3_3 = 0;
int Enemy::S_NUM_ENEMY_3_4 = 0;

//////////////////////////////////////////////////////////////////////////

int Enemy::S_COUNT_ENEMY_1_1 = 0;
int Enemy::S_COUNT_ENEMY_1_2 = 0;
int Enemy::S_COUNT_ENEMY_1_3 = 0;
int Enemy::S_COUNT_ENEMY_1_4 = 0;
int Enemy::S_COUNT_ENEMY_1_5 = 0;
int Enemy::S_COUNT_ENEMY_1_6 = 0;

int Enemy::S_COUNT_ENEMY_2_1 = 0;
int Enemy::S_COUNT_ENEMY_2_2 = 0;
int Enemy::S_COUNT_ENEMY_2_3 = 0;
int Enemy::S_COUNT_ENEMY_2_4 = 0;
int Enemy::S_COUNT_ENEMY_2_5 = 0;

int Enemy::S_COUNT_ENEMY_3_1 = 0;
int Enemy::S_COUNT_ENEMY_3_2 = 0;
int Enemy::S_COUNT_ENEMY_3_3 = 0;
int Enemy::S_COUNT_ENEMY_3_4 = 0;

//////////////////////////////////////////////////////////////////////////


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

	//////////////////////////////////////////////////////////////////////////
	LevelData* ld = LevelLoader::shareLevelLoader()->GetValueLowerThan((int)m_difficulty);

	if (ld != NULL)
	{
		S_HP1 = ld->m_hp1;
		S_HP2 = ld->m_hp2;
		S_HP3 = ld->m_hp3;
		S_VELOCITY1 = ld->m_velocity1;
		S_VELOCITY2 = ld->m_velocity2;
		S_VELOCITY3 = ld->m_velocity3;
		S_GENERATE_TIME = ld->m_genTime;

		S_NUM_ENEMY_1_1 = ld->m_small1;
		S_NUM_ENEMY_1_2 = ld->m_small2;
		S_NUM_ENEMY_1_3 = ld->m_small3;
		S_NUM_ENEMY_1_4 = ld->m_small4;
		S_NUM_ENEMY_1_5 = ld->m_small5;
		S_NUM_ENEMY_1_6 = ld->m_small6;

		S_NUM_ENEMY_2_1 = ld->m_med1;
		S_NUM_ENEMY_2_2 = ld->m_med2;
		S_NUM_ENEMY_2_3 = ld->m_med3;
		S_NUM_ENEMY_2_4 = ld->m_med4;
		S_NUM_ENEMY_2_5 = ld->m_med5;

		S_NUM_ENEMY_3_1 = ld->m_lar1;
		S_NUM_ENEMY_3_2 = ld->m_lar2;
		S_NUM_ENEMY_3_3 = ld->m_lar3;
		S_NUM_ENEMY_3_4 = ld->m_lar4;
	}

// 	m_type = 1;
//	m_smallType = 1;
// 	m_hp = S_HP1;
// 	m_vy = S_VELOCITY1;
// 	S_NUM_ENEMY_1_1++;

	float rd = CCRANDOM_0_1();
	if (rd <= G_ENEMY_1_PERCENT)
	{
		m_type = 1;
		m_hp = S_HP1;
		m_vy = S_VELOCITY1;
	}
	else if (rd <= G_ENEMY_1_PERCENT + G_ENEMY_2_PERCENT)
	{
		m_type = 2;
		m_hp = S_HP2;
		m_vy = S_VELOCITY2;
	}
	else
	{
		m_type = 3;
		m_hp = S_HP3;
		m_vy = S_VELOCITY3;
	}

	//float dv = CCRANDOM_0_1() * 0.2f - 0.1f;
	//m_vy += dv;

	//////////////////////////////////////////////////////////////////////////

	m_damage = 0;
	m_vx = 0;
	m_originHp = m_hp;

	CCString* s = CCString::createWithFormat("enemy_%d_0.png", m_type);
	m_sprite = CCSprite::createWithSpriteFrameName(s->getCString());

	m_sprite->setPosition(CCPointZero);
	this->addChild(m_sprite);

	//////////////////////////////////////////////////////////////////////////


	float TIME_ANIMATION;
	int NUM_FRAME_EXPLOSION = 0;
	switch (m_type)
	{
	case 1:
		NUM_FRAME_EXPLOSION = 4;
		TIME_ANIMATION = 0.15f;
		break;
	case 2:
		NUM_FRAME_EXPLOSION = 6;
		TIME_ANIMATION = 0.15f;
		break;
	case 3:
		NUM_FRAME_EXPLOSION = 9;
		TIME_ANIMATION = 0.08f;
		break;
	}

	//////////////////////////////////////////////////////////////////////////

	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCString* strSpriteName = CCString::createWithFormat("enemy_%d_0.png", m_type);

	//explosion ================================

	CCArray* animFrames = CCArray::createWithCapacity(NUM_FRAME_EXPLOSION);
	for(int i = 0; i < NUM_FRAME_EXPLOSION; i++)
	{
		strSpriteName = CCString::createWithFormat("enemy_%d_%d.png", m_type, i);
		CCSpriteFrame* frame = cache->spriteFrameByName( strSpriteName->getCString() );
		animFrames->addObject(frame);
	}

	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, TIME_ANIMATION);

	m_acExplosion = CCSequence::create(
		CCAnimate::create(animation),
		CCCallFunc::create(this, callfunc_selector(Enemy::removeFromParent)),
		NULL);
	m_acExplosion->retain();

	//pre explosion =========================

	CCArray* animFramesPre;

	switch(m_type) 
	{
	case 1:
		{
			animFramesPre = CCArray::createWithCapacity(2);
			animFramesPre->addObject(cache->spriteFrameByName("enemy_1_0.png"));
			animFramesPre->addObject(cache->spriteFrameByName("enemy_1_1.png"));
			animFramesPre->addObject(cache->spriteFrameByName("enemy_1_0.png"));
		}
		break;
	case 2:
		{
			animFramesPre = CCArray::createWithCapacity(2);
			animFramesPre->addObject(cache->spriteFrameByName("enemy_2_0.png"));
			animFramesPre->addObject(cache->spriteFrameByName("enemy_2_1.png"));
			animFramesPre->addObject(cache->spriteFrameByName("enemy_2_0.png"));
		}
		break;
	case 3:
		{
			animFramesPre = CCArray::createWithCapacity(3);
			animFramesPre->addObject(cache->spriteFrameByName("enemy_3_1.png"));
			animFramesPre->addObject(cache->spriteFrameByName("enemy_3_2.png"));
			animFramesPre->addObject(cache->spriteFrameByName("enemy_3_3.png"));
			animFramesPre->addObject(cache->spriteFrameByName("enemy_3_0.png"));
		}
		break;
	}
	
	CCAnimation* animationPre = CCAnimation::createWithSpriteFrames(animFramesPre, 0.15f);

	m_acPreExplosion = CCSequence::create(
		CCAnimate::create(animationPre),
		NULL);
	m_acPreExplosion->retain();

	// flying -===========================================

	CCArray* animFramesFlying;

	switch(m_type) 
	{
	case 1:
		{
			animFramesFlying = CCArray::createWithCapacity(1);
			animFramesFlying->addObject(cache->spriteFrameByName("enemy_1_0.png"));
		}
		break;
	case 2:
		{
			animFramesFlying = CCArray::createWithCapacity(1);
			animFramesFlying->addObject(cache->spriteFrameByName("enemy_2_0.png"));
		}
		break;
	case 3:
		{
			animFramesFlying = CCArray::createWithCapacity(2);
			animFramesFlying->addObject(cache->spriteFrameByName("enemy_3_0.png"));
			animFramesFlying->addObject(cache->spriteFrameByName("enemy_3_1.png"));
		}
		break;
	}

	CCAnimation* animationFlying = CCAnimation::createWithSpriteFrames(animFramesFlying, 0.2f);

	m_acFlying = CCRepeatForever::create(CCAnimate::create(animationFlying));
	m_acFlying->retain();

	//-------------------------

	m_sprite->runAction(m_acFlying);

	//////////////////////////////////////////////////////////////////////////

	if (G_ENEMY_TIME_TO_FIRE > 0)
	{
		this->schedule(schedule_selector(Enemy::ScheduleFire), G_ENEMY_TIME_TO_FIRE);
	}

	//////////////////////////////////////////////////////////////////////////
	this->scheduleUpdate();
	return true;
}

void Enemy::update( float delta )
{
	this->setPositionX(m_vx * delta * 1000 + this->getPositionX());
	this->setPositionY(m_vy * delta * 1000 + this->getPositionY());
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

void Enemy::HitBullet(int damage)
{
	m_hp -= damage;

	if (m_hp > 0)
	{
		m_sprite->runAction(m_acPreExplosion);
	}
	else
	{
		m_vy = 0;
		m_sprite->stopAction(m_acFlying);
		m_sprite->stopAction(m_acPreExplosion);
		m_sprite->runAction(m_acExplosion);
	}
}
