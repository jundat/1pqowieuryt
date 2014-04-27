#include "cocos2d.h"
#include "ObjectLayer.h"
#include "Global.h"
#include "AudioManager.h"
#include "Enemy.h"
#include "EffectLayer.h"
#include "MainGameScene.h"
#include "DataManager.h"
#include "InGameTutorial.h"
#include "NDKHelper/NDKHelper.h"

USING_NS_CC;

static const int PLAYER_Y = 250;
static bool IS_SHOW_CHEAT = false;


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
	m_isPauseGame = false;
	m_isActiveBoom = false;
	m_killedEnemies = 0;
	m_score = 0;
	m_playedTime = 0;
	m_difficulty = 0;
	m_crossCount = 0;

	m_sprLazer = CCSprite::createWithSpriteFrameName("lazer.png");
	m_sprLazer->setVisible(false);
	this->addChild(m_sprLazer, 1);

	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray* animFramesFlying = CCArray::createWithCapacity(2);
	
	animFramesFlying->addObject(cache->spriteFrameByName("lazer.png"));
	animFramesFlying->addObject(cache->spriteFrameByName("lazer1.png"));

	CCAnimation* animationFlying = CCAnimation::createWithSpriteFrames(animFramesFlying, 0.1f);
	CCAnimate* animateFlying = CCAnimate::create(animationFlying);
	CCRepeatForever* actionLazer = CCRepeatForever::create(animateFlying);
	actionLazer->retain();
	m_sprLazer->runAction(actionLazer);



	m_player = Ship::create();
	m_player->setPosition(ccp(G_DESIGN_WIDTH/2, PLAYER_Y));
	this->addChild(m_player, 2);



	CCSprite* temp = CCSprite::create("pause_0.png");
	float w = temp->getContentSize().width;
	float h = temp->getContentSize().height;
	
	m_labelScore = CCLabelTTF::create("0", "Roboto-Medium.ttf", 52);
	m_labelScore->setColor(ccc3(0, 0, 0));
	m_labelScore->setAnchorPoint(ccp(0.0f, 0.5f));
	m_labelScore->setPosition(ccp(1.2f * w, G_DESIGN_HEIGHT - h/2));

	this->addChild(m_labelScore, 10);

	m_itemBoom = CCMenuItemImage::create("icon_boom.png", "icon_boomhover.png", this, menu_selector(ObjectLayer::ActiveBoom));
	m_itemBoom->setPosition(ccp(m_itemBoom->getContentSize().width/2, m_itemBoom->getContentSize().height/2));
	
    //
    //cheat
    //
    CCMenuItemImage* itAddScore = CCMenuItemImage::create("icon_boom.png", "icon_boomhover.png", this, menu_selector(ObjectLayer::addScore));
	itAddScore->setPosition(ccp(400, 1200));
    itAddScore->setVisible(IS_SHOW_CHEAT);
    
    
	CCMenu* menu = CCMenu::create(m_itemBoom, itAddScore, NULL);
    menu->setPosition(CCPointZero);
	this->addChild(menu, 10);
    

    
    
	CCString* sBoom = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetBoom());
	m_labelBoom = CCLabelTTF::create(sBoom->getCString(), "Roboto-Medium.ttf", 52);
	m_labelBoom->setAnchorPoint(ccp(0.0f, 0.5f));
	m_labelBoom->setColor(ccc3(0, 0, 0));
	m_labelBoom->setPosition(ccp(m_itemBoom->getContentSize().width,
		m_itemBoom->getContentSize().height/4 + m_labelBoom->getContentSize().height/4));
	this->addChild(m_labelBoom, 10);

	if (DataManager::sharedDataManager()->GetBoom() <= 0)
	{
		m_itemBoom->setVisible(false);
		m_labelBoom->setVisible(false);
	}

	//Cross
	CCSprite* sprCross = CCSprite::create("cross.png");
	int wCross = sprCross->getContentSize().width;
	int hCross = sprCross->getContentSize().height;

	for (int i = 0; i < CROSS_NUMBER; ++i)
	{
		m_arrSprCrosses[i] = CCSprite::create("cross.png");
		m_arrSprCrosses[i]->setPosition(ccp(800 - 10 + wCross/2 - (CROSS_NUMBER - i) * wCross, 1280 - hCross/2 - 10));
		this->addChild(m_arrSprCrosses[i], 10);
	}

	resetCrosses();

	m_EffectLayer = EffectLayer::create();
	this->addChild(m_EffectLayer, 10);

	m_enemyFactory = EnemyFactory::create();
	this->addChild(m_enemyFactory, 2);
	m_enemyFactory->update(-1.5f, m_score); //pause 2s before start generate enemies


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

void ObjectLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void ObjectLayer::AddEmemy( Enemy* enemy )
{
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
	CCLOG("Random Item: %f", rd);

	float rdw = CCRANDOM_0_1() * (7.0f / 8.0f * G_DESIGN_WIDTH) + G_DESIGN_WIDTH / 8.0f;

	if ( rd > 1.0f/4.0f ) // 2/3
	{
		item = Item::create(G_ITEM_UPGRADE_BULLET, -0.3f, ccp(rdw, 3.0f/4*G_DESIGN_HEIGHT));
		this->AddItem(item);
	}
	else
	{
		item = Item::create(G_ITEM_BOOM, -0.3f, ccp(rdw, 3.0f/4*G_DESIGN_HEIGHT));
		this->AddItem(item);
	}


	if (item != NULL)
	{
		PLAY_OUT_PORP_EFFECT;

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
					CCRect enemyRect = enemy->collisionBox();

					if (enemyRect.intersectsRect(bulletRect))
					{
                        bullet->stopAllActions();
						this->removeChild(bullet);
						m_arrPlayerBullets->removeObject(bullet);

						//sound
						switch(enemy->getEnemyType())
						{
						case 1:
							PLAY_ENEMY1_DOWN_EFFECT;
							break;

						case 2:
							PLAY_ENEMY2_DOWN_EFFECT;
							break;

						case 3:
							PLAY_ENEMY3_DOWN_EFFECT;
							break;
						}

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
			CCRect enemyRect = enemy->collisionBox();

			if (enemyRect.intersectsRect(playerRect))
			{
				//sound
				PLAY_ENEMY2_DOWN_EFFECT;

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
					PLAY_GET_DOUBLE_LAZER_EFFECT;
					m_player->UpgradeBullet();
				}
				else if (itemtype == G_ITEM_ARMOR)
				{
					m_player->EnableArmor();
				}
				else if (itemtype == G_ITEM_BOOM)
				{
					PLAY_GET_BOMB_EFFECT;
					this->IncreaseBoom();
				}

                item->stopAllActions();
				this->removeChild(item);
				m_arrItems->removeObject(item);
			}
		}
	}

	//Lazer
	if (m_sprLazer->isVisible())
	{
		CCRect lazerRect = m_sprLazer->boundingBox();
		
		CCARRAY_FOREACH(m_arrEnemies, it1)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(it1);
			if (NULL != enemy)
			{
				CCRect enemyRect = enemy->collisionBox();

				if (enemyRect.intersectsRect(lazerRect))
				{
					enemy->HitBullet(1000);
				}
			}
		}
	}
}

void ObjectLayer::AddBullet(Bullet* bullet)
{
	if (m_sprLazer->isVisible() == false) //not lazer
	{
		m_arrPlayerBullets->addObject(bullet);
		this->addChild(bullet);
	}
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

    enemy->stopAllActions();
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
        
        this->Vibrate(700);

		m_isEndGame = true;
	}

	ccDrawCircle(ccp(400, 640), 400, 90, 32, true);
	//////////////////////////////////////////////////////////////////////////
	m_playedTime += delta;
	
	//add score
	if (m_playedTime >= 1.0f)
	{
		m_playedTime -= 1.0f;

		m_score += G_POINTS_PER_SECOND;
		
		CCString* sscore = CCString::createWithFormat("%d", m_score);
		m_labelScore->setString(sscore->getCString());
	}



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
                bullet->stopAllActions();
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
                item->stopAllActions();
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
			}

			//out of screen
			if (enemy->getPositionY() < - enemy->boundingBox().size.height)
			{
				this->RemoveEnemy(enemy);
				m_arrEnemies->removeObject(enemy);

				//Cross
				if (enemy->getEnemyType() == 3) //BIG
				{
					IncreaseCross();
				}
			}
		}
	}

	//Lazer
	if (m_sprLazer->isVisible())
	{
		m_sprLazer->setScaleY(G_DESIGN_HEIGHT / m_sprLazer->getContentSize().height);
		//m_sprLazer->setScaleX(1.5f);
		m_sprLazer->setAnchorPoint(ccp(0.5f, 0.0f));
		m_sprLazer->setPosition(m_player->getPosition());
	}
}

void ObjectLayer::Revive()
{
	//keep:
	//	score
	//	difficulty
	//	player's Position
	//reset:
	//	player's HP

	resetCrosses();

	m_isEndGame = false;
	m_isPauseGame = false;

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);
	this->schedule(schedule_selector(ObjectLayer::ScheduleGenerateItem), G_TIME_TO_GENERATE_ITEM);
	this->schedule(schedule_selector(ObjectLayer::ScheduleCheckCollision), CCDirector::sharedDirector()->getAnimationInterval());
	this->scheduleUpdate();


	//reset
	m_player->setVisible(true);
	m_player->Restart();
	m_player->setPosition(ccp(G_DESIGN_WIDTH/2, PLAYER_Y));
}

void ObjectLayer::Restart()
{
	//reset:
	//	difficulty (time, score)
	//	player's HP
	//	player's Position
	//	score

	m_playedTime = 0;
	m_killedEnemies = 0;
	m_score = 0;

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
            bullet->stopAllActions();
			this->removeChild(bullet);
		}
	}
	m_arrPlayerBullets->removeAllObjects();

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	this->setTouchEnabled(true);
	m_isEndGame = false;
	m_player->setVisible(true);
	m_player->Restart();
	m_player->setPosition(ccp(G_DESIGN_WIDTH/2, PLAYER_Y));
	
	m_labelScore->setString("0");
	m_labelScore->setHorizontalAlignment(kCCTextAlignmentLeft);
	
	resetCrosses();

	this->schedule(schedule_selector(ObjectLayer::ScheduleGenerateItem), G_TIME_TO_GENERATE_ITEM);
	this->schedule(schedule_selector(ObjectLayer::ScheduleCheckCollision), CCDirector::sharedDirector()->getAnimationInterval());
	this->scheduleUpdate();
}

void ObjectLayer::AfterDeadEffectCallback()
{
	//Lazer
	m_sprLazer->setVisible(false);

	m_player->Dead();

	MainGameScene* parent = (MainGameScene*) this->getParent();
	parent->showEndGame(m_score, m_killedEnemies);
	
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
	DataManager::sharedDataManager()->IncreaseBoom();
    
    
    //send to server
    MainGameScene* parent = (MainGameScene*) this->getParent();
    parent->addLaze();
    


	CCString* s = CCString::createWithFormat("x%d", DataManager::sharedDataManager()->GetBoom());
	m_labelBoom->setString(s->getCString());

	m_labelBoom->setVisible(true);
	m_itemBoom->setVisible(true);
}

void ObjectLayer::ActiveBoom(CCObject* pSender)
{
	if (m_isEndGame || m_isPauseGame || m_isActiveBoom)
	{
		return;
	}
	
	m_isActiveBoom = true;

	PLAY_LAZER_EFFECT;

	int m_numberBoom = DataManager::sharedDataManager()->GetBoom();

	if (m_numberBoom > 0)
	{
		m_numberBoom--;
		DataManager::sharedDataManager()->DecreaseBoom();
        
        //send to server
        MainGameScene* parent = (MainGameScene*) this->getParent();
        parent->useLaze();
        

		CCString* s = CCString::createWithFormat("x%d", m_numberBoom);
		m_labelBoom->setString(s->getCString());

		if (m_numberBoom == 0)
		{
			m_labelBoom->setVisible(false);
			m_itemBoom->setVisible(false);
		}

		m_sprLazer->setScaleY(G_DESIGN_HEIGHT / m_sprLazer->getContentSize().height);
		m_sprLazer->setAnchorPoint(ccp(0.5f, 0.0f));
		m_sprLazer->setPosition(m_player->getPosition());
		m_sprLazer->setVisible(true);

		this->schedule(schedule_selector(ObjectLayer::ScheduleStopLazer), G_PLAYER_LAZER_TIME);
	}
}

void ObjectLayer::ScheduleStopLazer( float dt )
{
	m_isActiveBoom = false;

	this->unschedule(schedule_selector(ObjectLayer::ScheduleStopLazer));
	m_sprLazer->setVisible(false);
}

void ObjectLayer::Pause()
{
	m_isPauseGame = true;

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
	m_isPauseGame = false;

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

void ObjectLayer::IncreaseCross()
{
	PLAY_CROSS_EFFECT;
	Vibrate();

	m_crossCount++;
	
	//show
	CCSprite* spr = CCSprite::create("crossActive.png");
	m_arrSprCrosses[m_crossCount - 1]->setTexture(spr->getTexture());
	m_arrSprCrosses[m_crossCount - 1]->runAction(CCSequence::createWithTwoActions(
		CCScaleTo::create(0.2f, 1.5f),
		CCScaleTo::create(0.2f, 1.0f)
		));

	//check if lose game
	if (m_crossCount >= CROSS_NUMBER)
	{
		//sound
		PLAY_ENEMY2_DOWN_EFFECT;

		m_player->HitBullet(1000); //die
	}
}

void ObjectLayer::resetCrosses()
{
	m_crossCount = 0;

	for (int i = 0; i < CROSS_NUMBER; ++i)
	{
		CCSprite* spr = CCSprite::create("cross.png");
		m_arrSprCrosses[i]->setTexture(spr->getTexture());
	}
}

void ObjectLayer::Vibrate(int _time)
{
	CCLOG("Vibrate");
    
    CCDictionary* prms = CCDictionary::create();
	prms->setObject(CCString::createWithFormat("%d", _time), "time");
    
	SendMessageWithParams(string("Vibrate"), prms);
}


void ObjectLayer::addScore(CCObject* pSender)
{
    m_score += 250000;
    //this->ActiveBoom(NULL);
    //this->resetCrosses();
    m_player->UpgradeBullet();
}
