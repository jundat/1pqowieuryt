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

	m_arrEnemies = new CCArray();
	m_arrPlayerBullets = new CCArray();
	m_arrEnemyBullets = new CCArray();
	m_arrItems = new CCArray();

	m_arrEnemies->retain();
	m_arrPlayerBullets->retain();
	m_arrEnemyBullets->retain();
	m_arrItems->retain();

	m_isEndGame = false;
	m_score = 0;
	m_playedTime = 0;
	m_difficulty = 0;
	m_numberBoom = 0;

	m_labelScore = CCLabelBMFont::create("0", "Mia_64.fnt");
	m_labelScore->setScale(48.0f/64);
	m_labelScore->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - m_labelScore->getContentSize().height));
	this->addChild(m_labelScore, 10);

	m_player = Ship::create();
	m_player->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.1));
	this->addChild(m_player);

	CCString* s = CCString::createWithFormat("HP: %d", m_player->getHp());
	m_labelHp = CCLabelBMFont::create(s->getCString(), "Mia_64.fnt");
	m_labelHp->setScale(48.0f/64);
	m_labelHp->setPosition(ccp(origin.x + m_labelHp->getContentSize().width/2,
		origin.y + visibleSize.height - m_labelHp->getContentSize().height));
	this->addChild(m_labelHp, 10);

	m_itemBoom = CCMenuItemImage::create("icon_boom.png", "icon_boom.png", this, menu_selector(ObjectLayer::ActiveBoom));
	m_itemBoom->setPosition(ccp(origin.x + m_itemBoom->getContentSize().width/2 - visibleSize.width/2, 
		origin.y + m_itemBoom->getContentSize().height/2 - visibleSize.height/2));
	m_itemBoom->setVisible(false);
	CCMenu* menu = CCMenu::create(m_itemBoom, NULL);
	this->addChild(menu);

	m_labelBoom = CCLabelBMFont::create("0", "Mia_64.fnt");
	m_labelBoom->setPosition(ccp(origin.x + m_itemBoom->getContentSize().width + m_labelBoom->getContentSize().width,
		origin.y + m_itemBoom->getContentSize().height/4 + m_labelBoom->getContentSize().height/4));
	m_labelBoom->setVisible(false);
	this->addChild(m_labelBoom, 10);


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

//schedule generate enemy
void ObjectLayer::ScheduleGenerateEnemy( float dt )
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_difficulty = m_score;// + m_playedTime; //CCLOG("Difficulty: %f \tTime: %f", m_difficulty, m_playedTime);
	int movetype = G_MOVE_STRAINGH;

	float h = visibleSize.height;
	float w = visibleSize.width;

	Enemy* enemy = Enemy::create(m_difficulty, movetype);
	float enemyW = enemy->boundingBox().size.width;
			
	float x = (int)(CCRANDOM_0_1() * (visibleSize.width - enemyW));
	float y = visibleSize.height + enemy->boundingBox().size.height/2;

	enemy->setPosition(ccp(x + enemyW/2, y));

	m_arrEnemies->addObject(enemy);
	this->addChild(enemy);
}

//Player or enemy call to add bullet when fire
void ObjectLayer::AddBullet(Bullet* bullet)
{
	int type = bullet->getBulletType();

	if (type == G_BULLET_PLAYER_ID)
	{
		m_arrPlayerBullets->addObject(bullet);
		this->addChild(bullet);
	} 
	else if (type == G_BULLET_ENEMY_ID)
	{
		m_arrEnemyBullets->addObject(bullet);
		this->addChild(bullet);
	}
}

void ObjectLayer::AddItem( Item* item )
{
	m_arrItems->addObject(item);
	this->addChild(item);
}

void ObjectLayer::update( float delta )
{
	if (m_player->getHp() <= 0 && m_isEndGame == false)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
		this->unschedule(schedule_selector(ObjectLayer::ScheduleCheckCollision));
		this->unschedule(schedule_selector(ObjectLayer::ScheduleGenerateEnemy));
		this->unscheduleUpdate();
		this->setTouchEnabled(false);
		
		CCSequence* sequence = CCSequence::create(
			CCDelayTime::create(1),
			CCCallFunc::create(this, callfunc_selector(ObjectLayer::AfterDeadEffectCallback)),
			NULL
		);

		this->runAction(sequence);

		m_isEndGame = true;
	}

	//////////////////////////////////////////////////////////////////////////
	m_playedTime += delta;

	//CCLOG("HP: \t%d", m_player->getHp());
	//CCLOG("Score: \t%d", m_score);
	//CCLOG("Time: \t%f", m_playedTime);

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
	
	//Items
	CCARRAY_FOREACH(m_arrItems, it)
	{
		Item* item = dynamic_cast<Item*>(it);
		if (NULL != item)
		{
			//out of screen
			if (item->getPositionY() < 0)
			{
				this->removeChild(item);
				m_arrItems->removeObject(item);
			}
		}
	}

	//Enemy
	CCARRAY_FOREACH(m_arrEnemies, it)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(it);
	
		if (NULL != enemy)
		{
			//dead
			if (enemy->getHp() <= 0)
			{
				m_arrEnemies->removeObject(enemy);

				//item
				Item* item;
				float rd = CCRANDOM_0_1();
				
				if (rd <= G_ITEM_BULLET_RANDOM_PERCENT)
				{
					item = Item::create(G_ITEM_UPGRADE_BULLET, -0.3f, enemy->getPosition());
					this->AddItem(item);
				} 
				else if (rd <= G_ITEM_ARMOR_RANDOM_PERCENT)
				{
					item = Item::create(G_ITEM_ARMOR, -0.3f, enemy->getPosition());
					this->AddItem(item);
				} 
				else if (rd <= G_ITEM_BOOM_RANDOM_PERCENT)
				{
					item = Item::create(G_ITEM_BOOM, -0.3f, enemy->getPosition());
					this->AddItem(item);
				}
				
				//score
				m_score += enemy->getDamage();
				CCString* sscore = CCString::createWithFormat("%d", m_score);
				m_labelScore->setString(sscore->getCString());
			}

			//out of screen
			if (enemy->getPositionY() < - enemy->boundingBox().size.height)
			{
				this->removeChild(enemy);
				m_arrEnemies->removeObject(enemy);
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
	CCRect playerRect = m_player->boundingBox();

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
			CCARRAY_FOREACH(m_arrEnemies, it2)
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
						enemy->HitBullet(bullet->getDamage());
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Enemy's bullet -----VS------ Player
	CCARRAY_FOREACH(m_arrEnemyBullets, it1)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it1);
		if (NULL != bullet)
		{
			//pixel check collision	
			CCSprite* sprBullet = CCSprite::createWithTexture(bullet->getSprite()->getTexture());
			CCSprite* sprPlayer = CCSprite::createWithTexture(m_player->getSprite()->getTexture());
			sprBullet->setPosition(bullet->getPosition());
			sprPlayer->setPosition(m_player->getPosition());

			if (CollisionDetection::GetInstance()->areTheSpritesColliding(sprBullet, sprPlayer, true, _rt))
			//if(bullet->boundingBox().intersectsRect(playerRect))
			{
				this->removeChild(bullet);
				m_arrEnemyBullets->removeObject(bullet);

				//sound
				AudioManager::sharedAudioManager()->PlayEffect("explosion.wav");
				m_player->HitBullet(bullet->getDamage());

				//
				CCString* s = CCString::createWithFormat("HP: %d", (m_player->getHp() > 0) ? m_player->getHp() : 0);
				m_labelHp->setString(s->getCString());
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Enemy -----------VS------------- Player
	CCARRAY_FOREACH(m_arrEnemies, it1)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(it1);
		if (NULL != enemy)
		{
			CCRect enemyRect = enemy->boundingBox();

			if (enemyRect.intersectsRect(playerRect))
			{
				//sound
				AudioManager::sharedAudioManager()->PlayEffect("explosion.wav");
				
				m_player->HitBullet(1000);
				enemy->HitBullet(1000);
				m_arrEnemies->removeObject(enemy);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Items -----VS------ Player
	CCARRAY_FOREACH(m_arrItems, it1)
	{
		Item* item = dynamic_cast<Item*>(it1);
		if (NULL != item)
		{
			CCRect itemRect = item->boundingBox();

			if (playerRect.intersectsRect(itemRect))
			{
				int itemtype = item->getItemType();

				if (itemtype == G_ITEM_UPGRADE_BULLET)
				{
					m_player->UpgradeBullet();
				} 
				else if (itemtype == G_ITEM_ARMOR)
				{
					m_player->EnableArmor();
				}
				else if (itemtype == G_ITEM_BOOM)
				{
					this->IncreaseBoom();
				}
				
				this->removeChild(item);
				m_arrItems->removeObject(item);

				//sound
				AudioManager::sharedAudioManager()->PlayEffect("item.wav");
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

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);
	m_isEndGame = false;
	//reset
	m_player->setVisible(true);
	m_player->Restart();
	CCString* s = CCString::createWithFormat("HP: %d", (m_player->getHp() > 0) ? m_player->getHp() : 0);
	m_labelHp->setString(s->getCString());
	m_player->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.1));
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
	//	numberBoom
	
	m_playedTime = 0;
	m_score = 0;
	m_numberBoom = 0;

	m_arrEnemies->removeAllObjects();
	m_arrEnemyBullets->removeAllObjects();
	m_arrPlayerBullets->removeAllObjects();

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);
	m_isEndGame = false;
	m_player->setVisible(true);
	m_player->Restart();
	m_player->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height * 0.1));
	CCString* s = CCString::createWithFormat("HP: %d", (m_player->getHp() > 0) ? m_player->getHp() : 0);
	m_labelHp->setString(s->getCString());

	m_labelBoom->setVisible(false);
	m_itemBoom->setVisible(false);

	this->schedule(schedule_selector(ObjectLayer::ScheduleGenerateEnemy), m_timeToGenerateEnemy);
	this->schedule(schedule_selector(ObjectLayer::ScheduleCheckCollision), CCDirector::sharedDirector()->getAnimationInterval());
	this->scheduleUpdate();
}

void ObjectLayer::AfterDeadEffectCallback()
{
	m_player->Dead();

	MainGameScene* parent = (MainGameScene*) this->getParent();
	parent->showEndGame(m_score);
	
	//remove all enemy
	CCObject* it;
	CCARRAY_FOREACH(m_arrEnemies, it)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(it);
		if (NULL != enemy)
		{
			this->removeChild(enemy);
		}
	}

	m_arrEnemies->removeAllObjects();

	//remove all enemy's bullet
	CCARRAY_FOREACH(m_arrEnemyBullets, it)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it);
		if (NULL != bullet)
		{
			this->removeChild(bullet);
		}
	}

	m_arrEnemyBullets->removeAllObjects();
}

void ObjectLayer::IncreaseBoom()
{
	m_numberBoom++;
	m_numberBoom = (m_numberBoom < G_MAX_PLAYER_BOOM) ? m_numberBoom : G_MAX_PLAYER_BOOM;

	CCString* s = CCString::createWithFormat("%d", m_numberBoom);
	m_labelBoom->setString(s->getCString());

	m_labelBoom->setVisible(true);
	m_itemBoom->setVisible(true);
}

void ObjectLayer::ActiveBoom(CCObject* pSender)
{
	if (m_numberBoom > 0)
	{
		m_numberBoom--;

		CCString* s = CCString::createWithFormat("%d", m_numberBoom);
		m_labelBoom->setString(s->getCString());

		if (m_numberBoom == 0)
		{
			m_labelBoom->setVisible(false);
			m_itemBoom->setVisible(false);
		}

		//explosion all enemies
		CCObject* it;

		CCARRAY_FOREACH(m_arrEnemies, it)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(it);

			if (NULL != enemy)
			{
				enemy->HitBullet(1000);
			}
		}
	}
}
