#include "Bullet.h"
#include "ObjectLayer.h"
#include "Enemy.h"
#include "Global.h"
#include "LevelLoader.h"

USING_NS_CC;

int Enemy::S_NUMBER_BIG = 0;
int Enemy::S_NUMBER_MED = 0;



Enemy::Enemy(int _type, int _smallType, int _hp, float _vy) : GameObject()
{
	this->setEnemyType(_type);
	this->setEnemySmallType(_smallType);
	this->setHp(_hp);
	this->setVy(_vy);
	
	switch(m_type)
	{
	case 1:
		break;
	case 2:
		S_NUMBER_MED++;
		break;
	case 3:
		S_NUMBER_BIG++;
		break;
	}
}

Enemy::~Enemy()
{
	m_acFlying->release();
	m_acExplosion->release();
	m_acPreExplosion->release();
}

bool Enemy::init()
{
	if (!GameObject::init())
	{
		return false;
	}
	
	//////////////////////////////////////////////////////////////////////////

	m_damage = 0;
	m_vx = 0;
	m_originHp = m_hp;

	CCString* s = CCString::createWithFormat("enemy_%d_0.png", m_type);
	m_sprite = CCSprite::createWithSpriteFrameName(s->getCString());
	m_sprite->setScale(1.15f);

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
		m_sprite->setScale(1.15f + 0.20f);
		NUM_FRAME_EXPLOSION = 9;
		TIME_ANIMATION = 0.08f;
		break;
	}

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
		CCCallFunc::create(this, callfunc_selector(Enemy::RemoveFromObjectLayer)),
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

cocos2d::CCRect Enemy::collisionBox()
{
	if (m_type == 3)
	{
		float tw = 1.0f; //0.25f; // 1/4
		float th = 0.6f; // 1/2
		CCRect rect = GameObject::boundingBox();
		rect.origin = ccp(this->getPosition().x - tw * rect.size.width/2, this->getPosition().y - th * rect.size.height/2);
		rect.size.width = tw * rect.size.width;
		rect.size.height = th * rect.size.height;
		return rect;
	} 
	else
	{
		return GameObject::boundingBox();
	}
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
    if (m_hp <= 0) return;
    
	m_hp -= damage;

	if (m_hp > 0)
	{
        if(m_acPreExplosion->getTarget() == NULL || (m_acPreExplosion->getTarget() != NULL && m_acPreExplosion->isDone()))
        {
            //m_sprite->stopAction(m_acPreExplosion);
            //CCDirector::sharedDirector()->getActionManager()->removeAction(m_acPreExplosion);
            m_sprite->runAction(m_acPreExplosion);
        }
	}
	else
	{
		m_vy = 0;
        
        if (!m_acFlying->isDone())
        {
            m_sprite->stopAction(m_acFlying);
        }
        
        if (!m_acPreExplosion->isDone())
        {
            m_sprite->stopAction(m_acPreExplosion);
        }
        
        //CCDirector::sharedDirector()->getActionManager()->removeAction(m_acFlying);
		//m_sprite->stopAction(m_acPreExplosion);
        //CCDirector::sharedDirector()->getActionManager()->removeAction(m_acPreExplosion);
        
		m_sprite->runAction(m_acExplosion);
	}
}

void Enemy::RemoveFromObjectLayer()
{
    m_sprite->stopAllActions();
    CCDirector::sharedDirector()->getActionManager()->removeAction(m_acPreExplosion);
    CCDirector::sharedDirector()->getActionManager()->removeAction(m_acFlying);
    CCDirector::sharedDirector()->getActionManager()->removeAction(m_acPreExplosion);

	ObjectLayer* parent = (ObjectLayer*) this->getParent();
	parent->RemoveEnemy(this);
}
