#include "Bullet.h"
#include "ObjectLayer.h"
#include "Enemy.h"
#include "Global.h"
#include "LevelLoader.h"

USING_NS_CC;


int Enemy::S_HP1 = G_MIN_ENEMY_HP;
int Enemy::S_HP2 = G_MIN_ENEMY_HP;
int Enemy::S_HP3 = G_MIN_ENEMY_HP;
float Enemy::S_VELOCITY = G_MIN_ENEMY_VY;
float Enemy::S_GENERATE_TIME = G_DEFAULT_TIME_TO_GENERATE_ENEMY;


void Enemy::DifficultySplit(float difficulty)
{
	LevelData* ld = LevelLoader::shareLevelLoader()->GetValueLowerThan((int)difficulty);

	if (ld != NULL)
	{
		S_HP1 = ld->m_hp1;
		S_HP2 = ld->m_hp2;
		S_HP3 = ld->m_hp3;
		S_VELOCITY = ld->m_velocity;
		S_GENERATE_TIME = ld->m_genTime;
	}
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
	DifficultySplit(m_difficulty);

	m_vy = S_VELOCITY;
	float dv = CCRANDOM_0_1() * 0.2f - 0.1f;
	m_vy += dv;

	m_damage = 0;

	this->setVx(0);

	m_type = (int)(CCRANDOM_0_1() * 10);
	if (m_type <= 6)
	{
		m_type = 1;
		m_hp = S_HP1;
	}
	else if (m_type <= 8)
	{
		m_type = 2;
		m_hp = S_HP2;
	}
	else
	{
		m_type = 3;
		m_hp = S_HP3;
	}

	m_originHp = m_hp;

	CCString* s = CCString::createWithFormat("enemy_%d.png", m_type);
	m_sprite = CCSprite::create(s->getCString());

	m_sprite->setPosition(CCPointZero);
	this->addChild(m_sprite);

	m_EffectLayer = EffectLayer::create();
	this->addChild(m_EffectLayer, 100);

	if (G_ENEMY_TIME_TO_FIRE > 0)
	{
		this->schedule(schedule_selector(Enemy::ScheduleFire), G_ENEMY_TIME_TO_FIRE);
	}
	//////////////////////////////////////////////////////////////////////////
	
	//float d = 5 + visibleSize.height + m_sprite->getContentSize().height;
	//float t = abs(d / m_vy / 1000.0);
	//CCAction* move = CCMoveBy::create(t, ccp(0, -d));
	//this->runAction(move);

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
		//small effect explosion
		//m_EffectLayer->AddExploisionEff(2, CCPointZero);
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
