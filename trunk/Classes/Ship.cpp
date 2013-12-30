#include "Ship.h"
#include "Bullet.h"
#include "ObjectLayer.h"
#include "Global.h"
#include "MyMacro.h"

USING_NS_CC;


Ship::~Ship()
{

}

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

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_sprite = CCSprite::create("ship.png");
	m_sprite->setPosition(CCPointZero);
	this->addChild(m_sprite);

	m_sprArmor = CCSprite::create("armor.png");
	this->addChild(m_sprArmor, -1);
	m_sprArmor->setVisible(false);

	m_EffectLayer = EffectLayer::create();
	this->addChild(m_EffectLayer, 100);

	m_timeToFire = 0;
	//////////////////////////////////////////////////////////////////////////
	
	this->scheduleUpdate();
	return true;
}

void Ship::EnableArmor()
{
	if (false == m_isArmor)
	{
		this->m_isArmor = true;

		m_sprArmor->setVisible(true);

		CCDelayTime* delay = CCDelayTime::create(G_PLAYER_ARMOR_TIME - 5);
		CCBlink* blink = CCBlink::create(3, 12);
		CCCallFunc* callf = CCCallFunc::create(this, callfunc_selector(Ship::DisableArmor));
		CCSequence* seq = CCSequence::create(delay, blink, callf, NULL);
		m_sprArmor->runAction(seq);
	}
}

void Ship::DisableArmor()
{
	this->m_isArmor = false;
	m_sprArmor->setVisible(false);
}

void Ship::Fire()
{
	CCSize s = getContentSize();
	ObjectLayer* parent = (ObjectLayer*)this->getParent();
	Bullet *bullet1, *bullet2, *bullet3;

	switch (m_bulletLevel)
	{
	case 1:
		bullet1 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), this->getPosition());
		parent->AddBullet(bullet1);
		break;

	case 2:
		bullet1 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), ccpAdd(this->getPosition(), ccp(-s.width/2, 0)));
		parent->AddBullet(bullet1);

		bullet2 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), ccpAdd(this->getPosition(), ccp(s.width/2, 0)));
		parent->AddBullet(bullet2);
		break;

	case 3:
		bullet1 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), this->getPosition());
		parent->AddBullet(bullet1);

		bullet2 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), ccpAdd(this->getPosition(), ccp(-s.width/2, 0)));
		parent->AddBullet(bullet2);

		bullet3 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), ccpAdd(this->getPosition(), ccp(s.width/2, 0)));
		parent->AddBullet(bullet3);
		break;

	default:
		bullet1 = Bullet::create(G_BULLET_PLAYER_ID, G_PLAYER_BULLET_VY, this->getDamage(), this->getPosition());
		parent->AddBullet(bullet1);
	}
}

void Ship::update( float delta )
{
	//limit on screen
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	float x = getPositionX();
	float y = getPositionY();
	float w_2 = boundingBox().size.width/2;
	float h_2 = boundingBox().size.height/2;

	LIMIT_VALUE(x, origin.x + w_2, origin.x + visibleSize.width - w_2);
	LIMIT_VALUE(y, origin.y + h_2, origin.y + visibleSize.height - h_2);
	this->setPosition(x, y);

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
			m_EffectLayer->AddExploisionEff(3, p1);
			m_EffectLayer->AddExploisionEff(3, p2);
			m_EffectLayer->AddExploisionEff(3, p3);
		}
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
		CCSprite* sprLevelUp = CCSprite::create("levelup.png");
		this->addChild(sprLevelUp);

		CCMoveBy* move = CCMoveBy::create(0.7f, ccp(0, sprLevelUp->getContentSize().height));
		CCFadeOut* fade = CCFadeOut::create(0.7f);
		CCSpawn* spaw = CCSpawn::create(move, fade, NULL);
		sprLevelUp->runAction(spaw);

		//timeout
		CCDelayTime* delay = CCDelayTime::create(G_TIMEOUT_BULLET_LEVEL);
		CCCallFunc* callf = CCCallFunc::create(this, callfunc_selector(Ship::DowngradeBullet));
		CCSequence* seq = CCSequence::create(delay, callf, NULL);
		this->runAction(seq);
	}
	else
	{
		m_bulletLevel = G_MAX_PLAYER_BULLET_LEVEL;
	}
}

void Ship::DowngradeBullet()
{
	if (m_bulletLevel >= G_MIN_PLAYER_BULLET_LEVEL)
	{
		this->Fire();
		this->Fire();
		this->Fire();

		m_bulletLevel--;

		this->Fire();
		this->Fire();
		this->Fire();
		
		if (m_bulletLevel > G_MIN_PLAYER_BULLET_LEVEL)
		{
			CCDelayTime* delay = CCDelayTime::create(G_TIMEOUT_BULLET_LEVEL);
			CCCallFunc* callf = CCCallFunc::create(this, callfunc_selector(Ship::DowngradeBullet));
			CCSequence* seq = CCSequence::create(delay, callf, NULL);
			this->runAction(seq);
		}
	}
}
