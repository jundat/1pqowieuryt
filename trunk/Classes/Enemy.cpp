#include "Bullet.h"
#include "ObjectLayer.h"
#include "Enemy.h"
#include "Global.h"

USING_NS_CC;


void Enemy::DifficultySplit(float difficulty, float& vy, int& hp, int& dm)
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
	vy = G_MIN_ENEMY_VY - difficulty / G_VELOCITY_IN_DIFFICULTY; //min -> min + 0.45

// 	float delta = (CCRANDOM_0_1() * 0.1f) - 0.05f;
// 	vy += delta;

	//min = 1
	//max = 5
	//1000 / 4 = 250 (~200)
	hp = G_MIN_ENEMY_HP + difficulty / G_HP_IN_DIFFICULTY;

	//min = 1
	//max = 5
	//1000 / 4 = 250 (~200)
	dm = G_MIN_ENEMY_DAM + difficulty / G_DAMAGE_IN_DIFFICULTY;

	CCLOG("Vy: %f\tHp: %d\tDam: %d", vy, hp, dm);
}

Enemy::Enemy(float difficulty, int MOVE_TYPE) : GameObject()
{
	this->setDifficulty(difficulty);
	this->setMoveType(MOVE_TYPE);
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

	int n = 1;//(int)(CCRANDOM_0_1() * 3) + 1;
	CCString* s = CCString::createWithFormat("enemy_%d.png", n);
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
	
	switch (m_moveType)
	{
	case G_MOVE_STRAINGH:
		{
			float d = 5 + visibleSize.height + m_sprite->getContentSize().height;
			float t = abs(d / m_vy / 1000.0);
			CCAction* move = CCMoveBy::create(t, ccp(0, -d));
			this->runAction(move);
		}
		break;

	case G_MOVE_CIRCLE_LEFT:
		{
			float h = visibleSize.height;
			float w = visibleSize.width;
			ccBezierConfig conf = {
				ccp(-w, -h),
				ccp(0, -h/2),
				ccp(0, -h)
			};

			CCAction* bezier = CCBezierBy::create(abs(h/m_vy/1000.0), conf);
			this->runAction(bezier);
		}
		break;

	case G_MOVE_CIRCLE_RIGHT:
		{
			float h = visibleSize.height;
			float w = visibleSize.width;
			ccBezierConfig conf = {
				ccp(w, -h),
				ccp(0, -h/2),
				ccp(0, -h)
			};

			CCAction* bezier = CCBezierBy::create(abs(h/m_vy/1000.0), conf);
			this->runAction(bezier);
		}
		break;
	}


	//////////////////////////////////////////////////////////////////////////
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
