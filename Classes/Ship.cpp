#include "Ship.h"
#include "Bullet.h"
#include "ObjectLayer.h"
#include "Global.h"
#include "MyMacro.h"
#include "AudioManager.h"


USING_NS_CC;

#define ARMOR_TIME_ANIMATION 0.1f //(G_PLAYER_ARMOR_TIME / 2)


bool Ship::init()
{
	if (!GameObject::init())
	{
		return false;
	}

	this->setHp(G_PLAYER_HP);
	this->setDamage(G_PLAYER_DAMAGE);
	this->setBulletLevel(G_MIN_PLAYER_BULLET_LEVEL);
	this->m_isArmor = false;

	m_sprite = CCSprite::createWithSpriteFrameName("ship_0.png");
	m_sprite->setPosition(CCPointZero);
	m_sprite->setScale(1.15f);
	this->addChild(m_sprite);

	//animation -------------------
	
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCString* strSpriteName = CCString::create("ship_0.png");
	
	//1 flying //0-1
	
	CCArray* animFramesFlying = CCArray::createWithCapacity(2);
	for(int i = 0; i <= 1; i++)
	{
		strSpriteName = CCString::createWithFormat("ship_%d.png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName( strSpriteName->getCString() );
		animFramesFlying->addObject(frame);
	}

	CCAnimation* animationFlying = CCAnimation::createWithSpriteFrames(animFramesFlying, 0.25f);
	CCAnimate* animateFlying = CCAnimate::create(animationFlying);
	m_acFlying = CCRepeatForever::create(animateFlying);
	m_acFlying->retain();
	
	//2 explosion //2-3-4

	CCArray* animFramesExplosion = CCArray::createWithCapacity(3);
	for(int i = 2; i <= 4; i++)
	{
		strSpriteName = CCString::createWithFormat("ship_%d.png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName( strSpriteName->getCString() );
		animFramesExplosion->addObject(frame);
	}

	animFramesExplosion->addObject(cache->spriteFrameByName("ship_0.png"));

	CCAnimation* animationExplosion = CCAnimation::createWithSpriteFrames(animFramesExplosion, 0.2f);
	CCAnimate* animateExplosion = CCAnimate::create(animationExplosion);

	m_acExplosion = CCSequence::create(
		animateExplosion,
		CCBlink::create(1, 3),
		CCCallFunc::create(this, callfunc_selector(Ship::AfterDeadEffectCallback)),
		NULL);
	m_acExplosion->retain();

	//3 armor // 5-6

// 	CCArray* animFramesArmor = CCArray::createWithCapacity(2);
// 	for(int i = 5; i <= 6; i++)
// 	{
// 		strSpriteName = CCString::createWithFormat("ship_%d.png", i);
// 		CCSpriteFrame* frame = cache->spriteFrameByName( strSpriteName->getCString() );
// 		animFramesArmor->addObject(frame);
// 	}
//
//	CCAnimation* animationArmor = CCAnimation::createWithSpriteFrames(animFramesArmor, ARMOR_TIME_ANIMATION);
//	CCAnimate* animateArmor = CCAnimate::create(animationArmor);
//	CCRepeat* repeatArmor = CCRepeat::create(animateArmor, (int)((G_PLAYER_ARMOR_TIME - 3) / (ARMOR_TIME_ANIMATION * 2)));
//	
//	CCCallFunc* callfArmor = CCCallFunc::create(this, callfunc_selector(Ship::DisableArmor));
//
//	m_acArmor = CCSequence::create(repeatArmor, callfArmor, NULL);
//	m_acArmor->retain();
	

	//animation -------------------

	
	m_sprite->runAction(m_acFlying);


	m_bulletLevel = G_MIN_PLAYER_BULLET_LEVEL;
	m_timeOutBulletLevel = m_bulletLevel * G_TIMEOUT_BULLET_LEVEL;
	m_timeToFire = 0;
	//////////////////////////////////////////////////////////////////////////

	this->scheduleUpdate();
	return true;
}

cocos2d::CCRect Ship::collisionBox()
{
	float tw = 0.21875f; // 1/3
	float th = 0.46f; // 1/2
	CCRect rect = GameObject::boundingBox();
	rect.origin = ccp(this->getPosition().x - tw * rect.size.width/2, this->getPosition().y - th * rect.size.height/2);
	rect.size.width = tw * rect.size.width;
	rect.size.height = th * rect.size.height;
	return rect;
}

cocos2d::CCRect Ship::bigcollisionBox()
{
	float tw = 0.5f;
	float th = 0.5f;
	CCRect rect = GameObject::boundingBox();
	rect.origin = ccp(this->getPosition().x - tw * rect.size.width/2, this->getPosition().y - th * rect.size.height/2);
	rect.size.width = tw * rect.size.width;
	rect.size.height = th * rect.size.height;
	return rect;
}

void Ship::EnableArmor()
{
//	if (false == m_isArmor)
//	{
//		this->m_isArmor = true;
//		
// 		CCDelayTime* delay = CCDelayTime::create(G_PLAYER_ARMOR_TIME - 5);
// 		CCBlink* blink = CCBlink::create(3, 12);
// 		CCCallFunc* callf = CCCallFunc::create(this, callfunc_selector(Ship::DisableArmor));
// 		CCSequence* seq = CCSequence::create(delay, blink, callf, NULL);
// 		
// 		CCRepeatForever* ac = CCRepeatForever::create(m_acArmor);
// 		CCSpawn* allac = CCSpawn::createWithTwoActions(seq, ac);
//
//		m_sprite->runAction(m_acArmor);
//	}
}

void Ship::DisableArmor()
{
//	this->m_isArmor = false;
}

void Ship::Fire()
{
	PLAY_BULLET_EFFECT;

	CCSize s = getContentSize();
	s.width = s.width/2;
	ObjectLayer* parent = (ObjectLayer*)this->getParent();
	Bullet *bullet1, *bullet2, *bullet3;

	m_bulletLevel = (int)(m_timeOutBulletLevel / G_TIMEOUT_BULLET_LEVEL) + 1;
	m_bulletLevel = (m_bulletLevel < G_MAX_PLAYER_BULLET_LEVEL) ? m_bulletLevel : G_MAX_PLAYER_BULLET_LEVEL;
	m_bulletLevel = (m_bulletLevel > G_MIN_PLAYER_BULLET_LEVEL) ? m_bulletLevel : G_MIN_PLAYER_BULLET_LEVEL;

	switch (m_bulletLevel)
	{
	case 1:
		bullet1 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), this->getPosition(), m_bulletLevel);
		parent->AddBullet(bullet1);
		break;

	case 2:
		bullet1 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), ccpAdd(this->getPosition(), ccp(-s.width * 0.3f, 0)), m_bulletLevel);
		parent->AddBullet(bullet1);

		bullet2 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), ccpAdd(this->getPosition(), ccp(s.width * 0.3f, 0)), m_bulletLevel);
		parent->AddBullet(bullet2);
		break;

	case 3:
		bullet1 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), this->getPosition(), m_bulletLevel);
		parent->AddBullet(bullet1);

		bullet2 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), ccpAdd(this->getPosition(), ccp(-s.width/2, 0)), m_bulletLevel);
		parent->AddBullet(bullet2);

		bullet3 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), ccpAdd(this->getPosition(), ccp(s.width/2, 0)), m_bulletLevel);
		parent->AddBullet(bullet3);
		break;

	default:
		bullet1 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), this->getPosition(), m_bulletLevel);
		parent->AddBullet(bullet1);
	}
}

void Ship::update( float delta )
{
	float x = getPositionX();
	float y = getPositionY();
	float w_2 = boundingBox().size.width/2;
	float h_2 = boundingBox().size.height/2;

	LIMIT_VALUE(x, w_2, G_DESIGN_WIDTH - w_2);
	LIMIT_VALUE(y, h_2, G_DESIGN_HEIGHT - h_2);
	this->setPosition(x, y);

	//armor
	m_timeOutBulletLevel -= delta;
	
	//fire
	m_timeToFire += delta;
	if (m_timeToFire >= G_PLAYER_TIME_TO_FIRE)
	{
		m_timeToFire = 0;
		this->Fire();
	}
}

void Ship::HitBullet( int damage )
{
	if (m_isArmor == false || damage >= 1000)
	{
		m_hp -= damage;
		m_hp  = (m_hp > 0) ? m_hp : 0;

		m_sprite->stopAction(m_acFlying);
		m_sprite->runAction(m_acExplosion);
	}
}

void Ship::Dead()
{
	this->setVisible(false);
	this->unscheduleUpdate();
}

void Ship::Restart()
{
	m_bulletLevel = G_MIN_PLAYER_BULLET_LEVEL;
	m_timeOutBulletLevel = m_bulletLevel * G_TIMEOUT_BULLET_LEVEL;	
	m_sprite->runAction(m_acFlying);
	this->DisableArmor();
	this->setHp(G_PLAYER_HP);
	this->setVisible(true);
	this->scheduleUpdate();
}

void Ship::UpgradeBullet()
{
	m_bulletLevel++;

	if (m_bulletLevel <= G_MAX_PLAYER_BULLET_LEVEL) 
	{
		//CCSprite* sprLevelUp = CCSprite::create("levelup.png");
		//this->addChild(sprLevelUp);

		//CCMoveBy* move = CCMoveBy::create(0.7f, ccp(0, sprLevelUp->getContentSize().height));
		//CCFadeOut* fade = CCFadeOut::create(0.7f);
		//CCSpawn* spaw = CCSpawn::create(move, fade, NULL);
		//sprLevelUp->runAction(spaw);
	}
	else
	{
		m_bulletLevel = G_MAX_PLAYER_BULLET_LEVEL;
	}

	m_timeOutBulletLevel = m_bulletLevel * G_TIMEOUT_BULLET_LEVEL;
}

void Ship::AfterDeadEffectCallback()
{
	ObjectLayer* parent = (ObjectLayer*)this->getParent();
	parent->AfterDeadEffectCallback();
}
