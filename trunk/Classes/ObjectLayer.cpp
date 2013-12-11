#include "ObjectLayer.h"
#include "Global.h"
#include "AudioManager.h"
#include "Enemy.h"
#include "EffectLayer.h"
#include "MainGameScene.h"
#include "CollisionDetection.h"

USING_NS_CC;

//pixel check collision
CCRenderTexture* ObjectLayer::_rt = NULL;


// on "init" you need to initialize your instance
bool ObjectLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	//////////////////////////////////////////////////////////////////////////
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);
	//////////////////////////////////////////////////////////////////////////

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////

	m_arrEnemy = new CCArray();
	m_arrPlayerBullets = new CCArray();
	m_arrEnemyBullets = new CCArray();

	m_arrEnemy->retain();
	m_arrPlayerBullets->retain();
	m_arrEnemyBullets->retain();

	isEndGame = false;
	m_score = 0;
	m_playedTime = 0;

	m_pLabelScore = CCLabelTTF::create("0", "Marker Felt.ttf", 48);
	m_pLabelScore->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - m_pLabelScore->getContentSize().height));
	this->addChild(m_pLabelScore, 10);


	m_player = Ship::create();
	m_player->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.1));
	this->addChild(m_player);

	CCString* s = CCString::createWithFormat("HP: %d", m_player->getHp());
	m_pLabelHp = CCLabelTTF::create(s->getCString(), "Marker Felt.ttf", 48);
	m_pLabelHp->setPosition(ccp(origin.x + m_pLabelHp->getContentSize().width/2,
		origin.y + visibleSize.height - m_pLabelHp->getContentSize().height));
	this->addChild(m_pLabelHp, 10);

	m_timeToGenerateEnemy = G_DEFAULT_TIME_TO_GENERATE_ENEMY;
	this->schedule(schedule_selector(ObjectLayer::ScheduleGenerateEnemy), m_timeToGenerateEnemy);
	
	m_EffectLayer = EffectLayer::create();
	this->addChild(m_EffectLayer, 10);
	//////////////////////////////////////////////////////////////////////////
	this->schedule(schedule_selector(ObjectLayer::ScheduleCheckCollision), CCDirector::sharedDirector()->getAnimationInterval());
	this->scheduleUpdate();

	//pixel check collision
	_rt  = CCRenderTexture::create(visibleSize.width, visibleSize.height);
	_rt->retain();
	_rt->setPosition(ccp(visibleSize.width/2, visibleSize.height/2));
	_rt->setVisible(false);

    return true;
}

#pragma region TOUCH HANDLER

bool ObjectLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	m_lastPoint = pTouch->getLocation();
	return true;
}

void ObjectLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	CCPoint curPoint = pTouch->getLocation();

	float dx = curPoint.x - m_lastPoint.x;
	float dy = curPoint.y - m_lastPoint.y;

	m_player->setPosition(m_player->getPositionX() + dx, m_player->getPositionY() + dy);
	m_lastPoint = curPoint;
}

void ObjectLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
}

#pragma endregion

//schedule generate enemy 
void ObjectLayer::ScheduleGenerateEnemy( float dt )
{
	//ramdom type
	float diff = CCRANDOM_0_1();
	Enemy* enemy = Enemy::create(diff);
	float w = enemy->boundingBox().size.width;

	//random position
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	float x = (int)(CCRANDOM_0_1() * (visibleSize.width - w));
	float y = visibleSize.height + enemy->boundingBox().size.height/2;
	enemy->setPosition(ccp(x + w/2, y));

	m_arrEnemy->addObject(enemy);
	this->addChild(enemy);
}

//Player or enemy call to add bullet when fire
void ObjectLayer::AddBullet(Bullet* bullet)
{
	switch(bullet->getBulletType())
	{
	case G_BULLET_PLAYER_ID:
		m_arrPlayerBullets->addObject(bullet);
		this->addChild(bullet);
		break;

	case G_BULLET_ENEMY_ID:
		m_arrEnemyBullets->addObject(bullet);
		this->addChild(bullet);
		break;
	}
}

void ObjectLayer::update( float delta )
{
	if (m_player->getHp() <= 0 && isEndGame == false)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
		m_player->setVisible(false);
		this->unschedule(schedule_selector(ObjectLayer::ScheduleCheckCollision));
		this->unschedule(schedule_selector(ObjectLayer::ScheduleGenerateEnemy));
		this->unscheduleUpdate();
		this->setTouchEnabled(false);
		MainGameScene* parent = (MainGameScene*) this->getParent();
		parent->showEndGame(m_score);
		
		isEndGame = true;
	}

	//////////////////////////////////////////////////////////////////////////
	m_playedTime += delta;

	CCLOG("HP: \t%d", m_player->getHp());
	CCLOG("Score: \t%d", m_score);
	CCLOG("Time: \t%f", m_playedTime);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCObject* it = NULL;

	//Player's bullets
	CCARRAY_FOREACH(m_arrPlayerBullets, it)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it);
		if (NULL != bullet)
		{
			//out of screen
			if (bullet->getPositionY() > visibleSize.height)
			{
				this->removeChild(bullet);
				m_arrPlayerBullets->removeObject(bullet);
			}
		}
	}

	//Enemy's bullets
	CCARRAY_FOREACH(m_arrEnemyBullets, it)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it);
		if (NULL != bullet)
		{
			//out of screen
			if (bullet->getPositionY() < 0)
			{
				this->removeChild(bullet);
				m_arrEnemyBullets->removeObject(bullet);
			}
		}
	}

	//Enemy
	CCARRAY_FOREACH(m_arrEnemy, it)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(it);
	
		if (NULL != enemy)
		{
			//dead
			if (enemy->getHp() <= 0)
			{
				this->removeChild(enemy);
				m_arrEnemy->removeObject(enemy);

				//score
				m_score += enemy->getDamage();
				CCString* sscore = CCString::createWithFormat("%d", m_score);
				m_pLabelScore->setString(sscore->getCString());
			}

			//out of screen
			if (enemy->getPositionY() < - enemy->boundingBox().size.height)
			{
				this->removeChild(enemy);
				m_arrEnemy->removeObject(enemy);
			}
		}
	}
}

//check collision every frame
void ObjectLayer::ScheduleCheckCollision(float dt)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCObject* it1 = NULL;

	//////////////////////////////////////////////////////////////////////////
	//player's bullet -----VS------ Enemy
	CCARRAY_FOREACH(m_arrPlayerBullets, it1)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it1);
		if (NULL != bullet)
		{
			CCRect bulletRect = bullet->boundingBox();

			CCObject* it2 = NULL;

			//foreach all enemy
			CCARRAY_FOREACH(m_arrEnemy, it2)
			{
				Enemy* enemy = dynamic_cast<Enemy*>(it2);
				if (NULL != enemy)
				{
					CCRect enemyRect = enemy->boundingBox();
					
					if (enemyRect.intersectsRect(bulletRect))
					{
						this->removeChild(bullet);
						m_arrPlayerBullets->removeObject(bullet);

						//sound
						AudioManager::sharedAudioManager()->PlayEffect("explosion.wav");
						m_EffectLayer->AddExploisionEff(enemy->getEnemyType(), enemy->getPosition());
						enemy->HitBullet(bullet->getDamage());
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Enemy's bullet -----VS------ Player
	CCRect playerRect = m_player->boundingBox();
	CCARRAY_FOREACH(m_arrEnemyBullets, it1)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it1);
		if (NULL != bullet)
		{
			CCRect bulletRect = bullet->boundingBox();

			if (playerRect.intersectsRect(bulletRect))
			{
				//pixel check collision	

				CCSprite* sprBullet = CCSprite::createWithTexture(bullet->getSprite()->getTexture());
				CCSprite* sprPlayer = CCSprite::createWithTexture(m_player->getSprite()->getTexture());
				sprBullet->setPosition(bullet->getPosition());
				sprPlayer->setPosition(m_player->getPosition());
				if (CollisionDetection::GetInstance()->areTheSpritesColliding(sprBullet, sprPlayer, true, _rt)) 
				{
					this->removeChild(bullet);
					m_arrEnemyBullets->removeObject(bullet);

					//sound
					AudioManager::sharedAudioManager()->PlayEffect("explosion.wav");
					m_EffectLayer->AddExploisionEff(G_ENEMY_TYPE, m_player->getPosition());
					m_player->HitBullet(bullet->getDamage());

					//
					CCString* s = CCString::createWithFormat("HP: %d", (m_player->getHp() > 0) ? m_player->getHp() : 0);
					m_pLabelHp->setString(s->getCString());
				}				
			}
		}
	}
}

void ObjectLayer::ContinueGame()
{
	//keep:
	//	score
	//	difficulty
	//	player's Position
	//reset:
	//	player's HP


	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);
	isEndGame = false;
	//reset
	m_player->setVisible(true);
	m_player->Restart();
	CCString* s = CCString::createWithFormat("HP: %d", (m_player->getHp() > 0) ? m_player->getHp() : 0);
	m_pLabelHp->setString(s->getCString());
	this->schedule(schedule_selector(ObjectLayer::ScheduleGenerateEnemy), m_timeToGenerateEnemy);
	this->schedule(schedule_selector(ObjectLayer::ScheduleCheckCollision), CCDirector::sharedDirector()->getAnimationInterval());
	this->scheduleUpdate();
}

void ObjectLayer::RestartGame()
{
	//reset:
	//	difficulty (time, score)
	//	player's HP
	//	player's Position
	//	score
	
	m_playedTime = 0;
	m_score = 0;

	m_arrEnemy->removeAllObjects();
	m_arrEnemyBullets->removeAllObjects();
	m_arrPlayerBullets->removeAllObjects();

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);
	isEndGame = false;
	m_player->setVisible(true);
	m_player->Restart();
	CCString* s = CCString::createWithFormat("HP: %d", (m_player->getHp() > 0) ? m_player->getHp() : 0);
	m_pLabelHp->setString(s->getCString());
	m_player->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.1));
	this->schedule(schedule_selector(ObjectLayer::ScheduleGenerateEnemy), m_timeToGenerateEnemy);
	this->schedule(schedule_selector(ObjectLayer::ScheduleCheckCollision), CCDirector::sharedDirector()->getAnimationInterval());
	this->scheduleUpdate();
}



