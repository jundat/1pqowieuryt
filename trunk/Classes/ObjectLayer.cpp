#include "cocos2d.h"
#include "ObjectLayer.h"
#include "Global.h"
#include "AudioManager.h"
#include "Enemy.h"
#include "EffectLayer.h"
#include "MainGameScene.h"
#include "CollisionDetection.h"
#include "DataManager.h"
#include "cocos2d.h"

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

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);

	//////////////////////////////////////////////////////////////////////////

	m_arrEnemies = new CCArray();
	m_arrPlayerBullets = new CCArray();
	m_arrItems = new CCArray();

	m_arrEnemies->retain();
	m_arrPlayerBullets->retain();
	m_arrItems->retain();

	m_genTimeCounter = 0;

	m_isEndGame = false;
	m_killedEnemies = 0;
	m_score = 0;
	m_playedTime = 0;
	m_difficulty = 0;
	m_numberBoom = 0;

	m_player = Ship::create();
	m_player->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT * 0.1));
	this->addChild(m_player);

	CCString* s = CCString::createWithFormat("Your best: %d", DataManager::sharedDataManager()->GetCurrenHighScore());
	CCLabelBMFont* lbHighScore = CCLabelBMFont::create(s->getCString(), "Mia_64.fnt");
	lbHighScore->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(lbHighScore, 10);
	lbHighScore->runAction(CCSequence::create(
		CCDelayTime::create(2.0f),
		CCFadeOut::create(2.0f),
		NULL));

	CCSprite* temp = CCSprite::create("pause_0.png");
	float w = temp->getContentSize().width;
	float h = temp->getContentSize().height;
	
	m_labelScore = CCLabelBMFont::create("0", "Mia_64.fnt");
	m_labelScore->setScale(48.0f/64);
	m_labelScore->setPosition(ccp(2 * w, G_DESIGN_HEIGHT - h/2));
	m_labelScore->setAlignment(kCCTextAlignmentLeft);

	this->addChild(m_labelScore, 10);

// 	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
// 	s = CCString::createWithFormat("Life: %d", lastLife);
// 	m_labelHp = CCLabelBMFont::create(s->getCString(), "Mia_64.fnt");
// 	m_labelHp->setScale(48.0f/64);
// 	m_labelHp->setPosition(ccp(m_labelHp->getContentSize().width/2, G_DESIGN_HEIGHT - m_labelHp->getContentSize().height));
// 	this->addChild(m_labelHp, 10);

	m_itemBoom = CCMenuItemImage::create("icon_boom.png", "icon_boom.png", this, menu_selector(ObjectLayer::ActiveBoom));
	m_itemBoom->setPosition(ccp(m_itemBoom->getContentSize().width/2 - G_DESIGN_WIDTH/2, 
		m_itemBoom->getContentSize().height/2 - G_DESIGN_HEIGHT/2));
	m_itemBoom->setVisible(false);
	CCMenu* menu = CCMenu::create(m_itemBoom, NULL);
	this->addChild(menu);

	m_labelBoom = CCLabelBMFont::create("x0", "Mia_64.fnt");
	m_labelBoom->setScale(48.0f/64);
	m_labelBoom->setPosition(ccp(m_itemBoom->getContentSize().width + m_labelBoom->getContentSize().width,
		m_itemBoom->getContentSize().height/4 + m_labelBoom->getContentSize().height/4));
	m_labelBoom->setVisible(false);
	this->addChild(m_labelBoom, 10);

	
	m_labelDebug = CCLabelBMFont::create("debug", "Mia_64.fnt");
	m_labelDebug->setScale(32.0f/64);
	m_labelDebug->setPosition(ccp(0, G_DESIGN_HEIGHT/2));
	m_labelDebug->setVisible(false);
	this->addChild(m_labelDebug, 10);



	m_EffectLayer = EffectLayer::create();
	this->addChild(m_EffectLayer, 10);

	this->schedule(schedule_selector(ObjectLayer::ScheduleCheckCollision), CCDirector::sharedDirector()->getAnimationInterval());
	this->scheduleUpdate();

	//pixel check collision
	_rt  = CCRenderTexture::create(G_DESIGN_WIDTH, G_DESIGN_HEIGHT);
	_rt->retain();
	_rt->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
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
	m_difficulty = m_killedEnemies;

	float h = G_DESIGN_HEIGHT;
	float w = G_DESIGN_WIDTH;
	
	Enemy* enemy = Enemy::create(m_difficulty);

	float enemyW = enemy->boundingBox().size.width;
			
	float x = (int)(CCRANDOM_0_1() * (G_DESIGN_WIDTH - enemyW));
	float y = G_DESIGN_HEIGHT + enemy->boundingBox().size.height/2;

	enemy->setPosition(ccp(x + enemyW/2, y));

	m_arrEnemies->addObject(enemy);
	this->addChild(enemy);
}

//Player or enemy call to add bullet when fire
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

void ObjectLayer::update( float delta )
{
	if (m_player->getHp() <= 0 && m_isEndGame == false)
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
		this->setTouchEnabled(false);
		this->unschedule(schedule_selector(ObjectLayer::ScheduleCheckCollision));
		this->unschedule(schedule_selector(ObjectLayer::ScheduleGenerateEnemy));
		this->unscheduleUpdate();
		
		CCSequence* sequence = CCSequence::create(
			CCDelayTime::create(1.8f),
			CCCallFunc::create(this, callfunc_selector(ObjectLayer::AfterDeadEffectCallback)),
			NULL
		);

		this->runAction(sequence);

		m_isEndGame = true;
	}

	//////////////////////////////////////////////////////////////////////////
	m_playedTime += delta;

	m_genTimeCounter += delta;
	if (m_genTimeCounter >= Enemy::S_GENERATE_TIME)
	{
		m_genTimeCounter = 0;
		ScheduleGenerateEnemy(delta);
	}

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

				//item
				Item* item = NULL;
				float rd = CCRANDOM_0_1();
				float rdw = CCRANDOM_0_1() * (7.0f / 8.0f * G_DESIGN_WIDTH) + G_DESIGN_WIDTH / 8.0f;
				CCLOG("Random Item: %f", rd);
				if (rd <= G_ITEM_BULLET_RANDOM_PERCENT)
				{ CCLOG("----------------- bullet item ");
					if (m_player->getBulletLevel() < G_MAX_PLAYER_BULLET_LEVEL)
					{
						item = Item::create(G_ITEM_UPGRADE_BULLET, -0.3f, ccp(rdw, 3.0f/4*G_DESIGN_HEIGHT));
						this->AddItem(item);
					}
				}
				else if (rd <= G_ITEM_BULLET_RANDOM_PERCENT + G_ITEM_BOOM_RANDOM_PERCENT)
				{ CCLOG("----------------- boom item ");
					if (this->getNumberBoom() < G_MAX_PLAYER_BOOM)
					{
						item = Item::create(G_ITEM_BOOM, -0.3f, ccp(rdw, 3.0f/4*G_DESIGN_HEIGHT));
						this->AddItem(item);
					}
				}

				if (item != NULL)
				{
					CCAction* ac = CCJumpTo::create(1.5f, ccp(rdw, -2-item->boundingBox().size.height), G_DESIGN_HEIGHT/2, 1);
					item->runAction(ac);
				}

				m_killedEnemies++;

				switch (enemy->getEnemyType())
				{
				case 1:
					m_score += G_SCORE_1;
					break;
				case 2:
					m_score += G_SCORE_2;
					break;
				case 3:
					m_score += G_SCORE_3;
					break;
				}
				
				CCString* sscore = CCString::createWithFormat("%d", m_score);
				m_labelScore->setString(sscore->getCString());
				DataManager::sharedDataManager()->SetCurrentHighScore(m_score);
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

// 	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
// 	CCString* s = CCString::createWithFormat("Life: %d", lastLife);
// 	m_labelHp->setString(s->getCString());

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);

	m_genTimeCounter = 0;
	m_isEndGame = false;
	//reset
	m_player->setVisible(true);
	m_player->Restart();
	m_player->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT * 0.1));
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

// 	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
// 	CCString* s = CCString::createWithFormat("Life: %d", lastLife);
// 	m_labelHp->setString(s->getCString());

	m_genTimeCounter = 0;
	m_playedTime = 0;
	m_killedEnemies = 0;
	m_score = 0;
	m_numberBoom = 0;

	m_arrEnemies->removeAllObjects();
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

	this->schedule(schedule_selector(ObjectLayer::ScheduleCheckCollision), CCDirector::sharedDirector()->getAnimationInterval());
	this->scheduleUpdate();
}

void ObjectLayer::AfterDeadEffectCallback()
{
	m_player->Dead();

	MainGameScene* parent = (MainGameScene*) this->getParent();
	parent->showEndGame(m_score, m_killedEnemies);
	
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
	this->unschedule(schedule_selector(ObjectLayer::ScheduleCheckCollision));
	this->unschedule(schedule_selector(ObjectLayer::ScheduleGenerateEnemy));
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
	this->schedule(schedule_selector(ObjectLayer::ScheduleCheckCollision), CCDirector::sharedDirector()->getAnimationInterval());
	this->scheduleUpdate();

	m_genTimeCounter = 0;
	m_player->scheduleUpdate();

	CCObject* it;
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
