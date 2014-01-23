#include "cocos2d.h"
#include "ObjectLayer.h"
#include "Global.h"
#include "AudioManager.h"
#include "Enemy.h"
#include "EffectLayer.h"
#include "MainGameScene.h"
#include "CollisionDetection.h"
#include "DataManager.h"

USING_NS_CC;

bool ObjectLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);

	//////////////////////////////////////////////////////////////////////////

	Enemy::S_NUMBER_BIG = 0;
	Enemy::S_NUMBER_MED = 0;

	m_arrEnemies = new CCArray();
	m_arrPlayerBullets = new CCArray();
	m_arrItems = new CCArray();

	m_arrEnemies->retain();
	m_arrPlayerBullets->retain();
	m_arrItems->retain();
	
	m_isEndGame = false;
	m_killedEnemies = 0;
	m_score = 0;
	m_playedTime = 0;
	m_difficulty = 0;
	m_numberBoom = 0;

	m_player = Ship::create();
	m_player->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT * 0.1));
	this->addChild(m_player);

	CCSprite* temp = CCSprite::create("pause_0.png");
	float w = temp->getContentSize().width;
	float h = temp->getContentSize().height;
	
	m_labelScore = CCLabelBMFont::create("0", "Mia_64.fnt");
	m_labelScore->setScale(0.8f);
	m_labelScore->setAlignment(kCCTextAlignmentLeft);
	m_labelScore->setPosition(ccp(2 * w, G_DESIGN_HEIGHT - h/2));

	this->addChild(m_labelScore, 10);

	m_itemBoom = CCMenuItemImage::create("icon_boom.png", "icon_boom.png", this, menu_selector(ObjectLayer::ActiveBoom));
	m_itemBoom->setPosition(ccp(m_itemBoom->getContentSize().width/2 - G_DESIGN_WIDTH/2, 
		m_itemBoom->getContentSize().height/2 - G_DESIGN_HEIGHT/2));
	m_itemBoom->setVisible(false);
	CCMenu* menu = CCMenu::create(m_itemBoom, NULL);
	this->addChild(menu, 10);

	m_labelBoom = CCLabelBMFont::create("x0", "Mia_64.fnt");
	m_labelBoom->setPosition(ccp(m_itemBoom->getContentSize().width + m_labelBoom->getContentSize().width,
		m_itemBoom->getContentSize().height/4 + m_labelBoom->getContentSize().height/4));
	m_labelBoom->setVisible(false);
	this->addChild(m_labelBoom, 10);

	
	m_EffectLayer = EffectLayer::create();
	this->addChild(m_EffectLayer, 10);

	m_enemyFactory = EnemyFactory::create();
	this->addChild(m_enemyFactory);

	this->schedule(schedule_selector(ObjectLayer::ScheduleGenerateItem), G_TIME_TO_GENERATE_ITEM);
	this->schedule(schedule_selector(ObjectLayer::ScheduleCheckCollision), CCDirector::sharedDirector()->getAnimationInterval());
	this->scheduleUpdate();

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

void ObjectLayer::AddEmemy( Enemy* enemy )
{
	float h = G_DESIGN_HEIGHT;
	float w = G_DESIGN_WIDTH;

	float enemyW = enemy->boundingBox().size.width;

	float x = (int)(CCRANDOM_0_1() * (G_DESIGN_WIDTH - enemyW));
	float y = G_DESIGN_HEIGHT + enemy->boundingBox().size.height/2;

	enemy->setPosition(ccp(x + enemyW/2, y));

	m_arrEnemies->addObject(enemy);
	this->addChild(enemy);
}

void ObjectLayer::ScheduleGenerateItem( float dt )
{
	Item* item = NULL;
	float rd = CCRANDOM_0_1();
	float rdw = CCRANDOM_0_1() * (7.0f / 8.0f * G_DESIGN_WIDTH) + G_DESIGN_WIDTH / 8.0f;

	if (rd < 0.5f)
	{
		if (m_player->getBulletLevel() < G_MAX_PLAYER_BULLET_LEVEL)
		{
			item = Item::create(G_ITEM_UPGRADE_BULLET, -0.3f, ccp(rdw, 3.0f/4*G_DESIGN_HEIGHT));
			this->AddItem(item);
		}
		else
		{
			if (this->getNumberBoom() < G_MAX_PLAYER_BOOM)
			{
				item = Item::create(G_ITEM_BOOM, -0.3f, ccp(rdw, 3.0f/4*G_DESIGN_HEIGHT));
				this->AddItem(item);
			}
		}
	}
	else
	{
		if (this->getNumberBoom() < G_MAX_PLAYER_BOOM)
		{
			item = Item::create(G_ITEM_BOOM, -0.3f, ccp(rdw, 3.0f/4*G_DESIGN_HEIGHT));
			this->AddItem(item);
		}
		else
		{
			if (m_player->getBulletLevel() < G_MAX_PLAYER_BULLET_LEVEL)
			{
				item = Item::create(G_ITEM_UPGRADE_BULLET, -0.3f, ccp(rdw, 3.0f/4*G_DESIGN_HEIGHT));
				this->AddItem(item);
			}
		}
	}


	if (item != NULL)
	{
		CCAction* ac = CCJumpTo::create(1.5f, ccp(rdw, -2-item->boundingBox().size.height), 3.0f * G_DESIGN_HEIGHT/4.0f, 1);
		item->runAction(ac);
	}
}

void ObjectLayer::ScheduleCheckCollision(float dt)
{
	CCObject* it1 = NULL;
	CCRect playerRect = m_player->collisionBox();

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

				m_player->HitBullet(1);
				enemy->HitBullet(1000);
				m_arrEnemies->removeObject(enemy);
			}
		}
	}

	CCRect playerBigRect = m_player->bigcollisionBox();

	//////////////////////////////////////////////////////////////////////////
	//Items -----VS------ Player
	CCARRAY_FOREACH(m_arrItems, it1)
	{
		Item* item = dynamic_cast<Item*>(it1);
		if (NULL != item)
		{
			CCRect itemRect = item->boundingBox();

			if (playerBigRect.intersectsRect(itemRect))
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

void ObjectLayer::AddBullet(Bullet* bullet)
{
	int type = bullet->getBulletType();

	m_arrPlayerBullets->addObject(bullet);
	this->addChild(bullet);
}

void ObjectLayer::AddItem( Item* item )
{
	m_arrItems->addObject(item);
	this->addChild(item);
}

void ObjectLayer::RemoveEnemy( Enemy* enemy )
{
	switch(enemy->getEnemyType()) 
	{
	case 1:
		break;
	case 2:
		Enemy::S_NUMBER_MED--;
		break;
	case 3:
		Enemy::S_NUMBER_BIG--;
		CCLOG("Remove: %d <<<<<<<<<<<<", Enemy::S_NUMBER_BIG);
		break;
	}

	this->removeChild(enemy);
}

void ObjectLayer::update( float delta )
{
	if (m_player->getHp() <= 0 && m_isEndGame == false)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
		this->setTouchEnabled(false);
		this->unschedule(schedule_selector(ObjectLayer::ScheduleCheckCollision));
		this->unscheduleUpdate();

		m_isEndGame = true;
	}

	//////////////////////////////////////////////////////////////////////////
	m_playedTime += delta;

	m_enemyFactory->update(delta, m_score);

	CCObject* it = NULL;

	//Player's bullets
	CCARRAY_FOREACH(m_arrPlayerBullets, it)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it);
		if (NULL != bullet)
		{
			//out of screen
			if (bullet->getPositionY() > G_DESIGN_HEIGHT)
			{
				this->removeChild(bullet);
				m_arrPlayerBullets->removeObject(bullet);
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
				m_killedEnemies++;

				switch (enemy->getEnemyType())
				{
				case 1:
					m_score += G_ENEMY_1_SCORE;
					break;
				case 2:
					m_score += G_ENEMY_2_SCORE;
					break;
				case 3:
					m_score += G_ENEMY_3_SCORE;
					break;
				}
				
				CCString* sscore = CCString::createWithFormat("%d", m_score);
				m_labelScore->setString(sscore->getCString());
				m_labelScore->setAlignment(kCCTextAlignmentLeft);
			}

			//out of screen
			if (enemy->getPositionY() < - enemy->boundingBox().size.height)
			{
				this->RemoveEnemy(enemy);
				m_arrEnemies->removeObject(enemy);
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

	m_isEndGame = false;
	//reset
	m_player->setVisible(true);
	m_player->Restart();
	m_player->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT * 0.1));
	this->schedule(schedule_selector(ObjectLayer::ScheduleGenerateItem), G_TIME_TO_GENERATE_ITEM);
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
	m_killedEnemies = 0;
	m_score = 0;
	m_numberBoom = 0;

	m_enemyFactory->Reset();

	//remove all enemy
	CCObject* it;
	CCARRAY_FOREACH(m_arrEnemies, it)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(it);
		if (NULL != enemy)
		{
			this->RemoveEnemy(enemy);
		}
	}

	m_arrEnemies->removeAllObjects();
	
	Enemy::S_NUMBER_BIG = 0;
	Enemy::S_NUMBER_MED = 0;

	CCARRAY_FOREACH(m_arrPlayerBullets, it)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(it);
		if (NULL != bullet)
		{
			this->removeChild(bullet);
		}
	}
	m_arrPlayerBullets->removeAllObjects();

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);
	m_isEndGame = false;
	m_player->setVisible(true);
	m_player->Restart();
	m_player->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT * 0.1));
	
	m_labelBoom->setVisible(false);
	m_itemBoom->setVisible(false);

	m_labelScore->setString("0");
	m_labelScore->setAlignment(kCCTextAlignmentLeft);
	

	this->schedule(schedule_selector(ObjectLayer::ScheduleGenerateItem), G_TIME_TO_GENERATE_ITEM);
	this->schedule(schedule_selector(ObjectLayer::ScheduleCheckCollision), CCDirector::sharedDirector()->getAnimationInterval());
	this->scheduleUpdate();
}

void ObjectLayer::AfterDeadEffectCallback()
{
	m_player->Dead();

	MainGameScene* parent = (MainGameScene*) this->getParent();
	parent->showEndGame(m_score, m_killedEnemies);
	DataManager::sharedDataManager()->SetCurrentHighScore(m_score);



	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	this->setTouchEnabled(false);
	this->unschedule(schedule_selector(ObjectLayer::ScheduleGenerateItem));
	this->unschedule(schedule_selector(ObjectLayer::ScheduleCheckCollision));
	this->unscheduleUpdate();
	
	//remove all enemy
	CCObject* it;
	CCARRAY_FOREACH(m_arrEnemies, it)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(it);
		if (NULL != enemy)
		{
			this->RemoveEnemy(enemy);
		}
	}

	m_arrEnemies->removeAllObjects();
}

void ObjectLayer::IncreaseBoom()
{
	m_numberBoom++;
	m_numberBoom = (m_numberBoom < G_MAX_PLAYER_BOOM) ? m_numberBoom : G_MAX_PLAYER_BOOM;

	CCString* s = CCString::createWithFormat("x%d", m_numberBoom);
	m_labelBoom->setString(s->getCString());

	m_labelBoom->setVisible(true);
	m_itemBoom->setVisible(true);
}

void ObjectLayer::ActiveBoom(CCObject* pSender)
{
	if (m_numberBoom > 0)
	{
		m_numberBoom--;

		CCString* s = CCString::createWithFormat("x%d", m_numberBoom);
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

void ObjectLayer::Pause()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	this->setTouchEnabled(false);
	this->unschedule(schedule_selector(ObjectLayer::ScheduleGenerateItem));
	this->unschedule(schedule_selector(ObjectLayer::ScheduleCheckCollision));
	this->unscheduleUpdate();

	m_player->unscheduleUpdate();

	CCObject* it;

	//enemy
	CCARRAY_FOREACH(m_arrEnemies, it)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(it);

		if (NULL != enemy)
		{
			enemy->unscheduleUpdate();
		}
	}

	//items
	CCARRAY_FOREACH(m_arrItems, it)
	{
		Item* item = dynamic_cast<Item*>(it);

		if (NULL != item)
		{
			item->unscheduleUpdate();
		}
	}
}

void ObjectLayer::Resume()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);
	this->schedule(schedule_selector(ObjectLayer::ScheduleGenerateItem), G_TIME_TO_GENERATE_ITEM);
	this->schedule(schedule_selector(ObjectLayer::ScheduleCheckCollision), CCDirector::sharedDirector()->getAnimationInterval());
	this->scheduleUpdate();

	m_player->scheduleUpdate();

	CCObject* it;

	//enemies
	CCARRAY_FOREACH(m_arrEnemies, it)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(it);

		if (NULL != enemy)
		{
			enemy->scheduleUpdate();
		}
	}

	//items
	CCARRAY_FOREACH(m_arrItems, it)
	{
		Item* item = dynamic_cast<Item*>(it);

		if (NULL != item)
		{
			item->scheduleUpdate();
		}
	}
}
