#include "LoseDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "AudioManager.h"
#include "DataManager.h"

USING_NS_CC;

bool LoseDialog::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	m_timerBar = NULL;
	m_timerNode = NULL;
	m_lbTimer = NULL;

	CCPoint pExit = ccp(400, 1280-813); // ccp(233, 1280-813);
	CCPoint pRevive = ccp(400, 1280-813); //ccp(565, 1280-813);
	CCPoint pTimerBar = ccp(400 - 161, 1280-740); //ccp(565, 1280-813);
	CCPoint pTimer = ccp(400, 1280-700); //ccp(565, 1280-813);

	float BUTTON_SCALE = 0.6722f;

	CCSprite* bg = CCSprite::create("dialog.png");
	bg->setPosition(ccp(400, 640));
	this->addChild(bg, -2);
	

	//diamon
	CCString* s = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetDiamon());
	m_lbDiamon = CCLabelTTF::create(s->getCString(), "Roboto-Medium.ttf", 48);
	m_lbDiamon->setColor(ccc3(0, 0, 0));
	m_lbDiamon->setAnchorPoint(ccp(1.0f, 0.5f));
	m_lbDiamon->setPosition(ccp(bg->getContentSize().width/2 - 10 + bg->getPositionX(), bg->getContentSize().height/2 - 40 + bg->getPositionY()));
	this->addChild(m_lbDiamon);


	CCSprite* sprDiamon = CCSprite::create("diamond.png");
	sprDiamon->setPosition(ccp(m_lbDiamon->getPositionX() - m_lbDiamon->getContentSize().width - sprDiamon->getContentSize().width/1.5f, m_lbDiamon->getPositionY()));
	this->addChild(sprDiamon);

	//////////////////////////////////////////////////////////////////////////

	m_itExitButton = CCMenuItemImage::create(
		"exit_button.png",
		"exit_button_press.png",
		this,
		menu_selector(LoseDialog::menuCallBack));
	m_itExitButton->setScale(BUTTON_SCALE);
	m_itExitButton->setPosition(pExit);
	

	m_itRevive = CCMenuItemImage::create(
		"hoisinh1.png",
		"hoisinh.png",
		"hoisinh.png",
		this,
		menu_selector(LoseDialog::reviveCallBack));
	m_itRevive->setScale(BUTTON_SCALE);
	m_itRevive->setPosition(pRevive);


	//REVIVE /////////////////////////////////////////////////////////////////
	//revive_button_press.png
	bool isJustRevive = DataManager::sharedDataManager()->GetIsJustRevived();
	if (isJustRevive == true)
	{
		m_itRevive->setVisible(false);
		//m_itRevive->setEnabled(false);
		//m_itRevive->setOpacity(0.60f * 255);
	}
	else //timer
	{
		m_itExitButton->setVisible(false);

		m_elapsedTime = 0;

		s = CCString::createWithFormat("%d", (int)G_WAIT_TO_REVIVE);
		m_lbTimer = CCLabelTTF::create(s->getCString(), "Roboto-Medium.ttf", 48);
		m_lbTimer->setFontFillColor(ccc3(0, 0, 0));
		m_lbTimer->setPosition(pTimer);
		m_lbTimer->setAnchorPoint(ccp(0.5f, 0.5f));
		this->addChild(m_lbTimer);

		m_timerBar = CCSprite::create("timer_bar.png");
		m_timerBar->setAnchorPoint(ccp(0.0f, 0.5f));
		m_timerBar->setPosition(pTimerBar);
		this->addChild(m_timerBar);
		
		m_timerNode = CCSprite::create("timer_node.png");
		m_timerNode->setAnchorPoint(ccp(0.0f, 0.5f));
		m_timerNode->setPosition(pTimerBar);
		this->addChild(m_timerNode);

		this->scheduleUpdate();
	}
	//////////////////////////////////////////////////////////////////////////
	
	CCMenu* menu = CCMenu::create(m_itExitButton, m_itRevive, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	CCLabelTTF* labelTitle = CCLabelTTF::create("Điểm", "Roboto-Medium.ttf", 64);
	labelTitle->setFontFillColor(ccc3(56, 56, 56));
	labelTitle->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 160)); //+40
	this->addChild(labelTitle);

	s = CCString::createWithFormat("%d", m_score);
	CCLabelTTF* lbScore = CCLabelTTF::create(s->getCString(), "Roboto-Medium.ttf", 96);
	lbScore->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 50));
	lbScore->setColor(ccc3(56, 56, 56));
	this->addChild(lbScore);

	//BREAK THE RECORD ///////////////////////////////////////////////////////
	int curScore = DataManager::sharedDataManager()->GetHighScore();
	if (m_score > curScore)
	{
		PLAY_GET_BOMB_EFFECT;

		breakRecord();
	}

	//////////////////////////////////////////////////////////////////////////

    return true;
}

void LoseDialog::menuCallBack( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	//revive set to false
	DataManager::sharedDataManager()->SetIsJustRevived(false);

	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void LoseDialog::reviveCallBack( CCObject* pSender )
{
	//check if enough Diamon
	int diamon = DataManager::sharedDataManager()->GetDiamon();
	if (diamon >= G_DIAMON_PER_LIFE)
	{
		DataManager::sharedDataManager()->SetDiamon(diamon - G_DIAMON_PER_LIFE);

		//animation diamon
		CCSprite* sprDiamon = CCSprite::create("sub_diamond.png");
		sprDiamon->setPosition(m_itRevive->getPosition());
		this->addChild(sprDiamon);

		PLAY_GET_DOUBLE_LAZER_EFFECT;


		//////////////////////////////////////////////////////////////////////////
		//unvisiable
		m_lbTimer->setVisible(false);
		m_timerBar->setVisible(false);
		m_timerNode->setVisible(false);

		//m_itRevive->setVisible(false);
		m_itRevive->setEnabled(false);
		m_itRevive->setOpacity(0.6f * 255);
		this->unscheduleUpdate();

		//m_lbDiamond

		CCString* s = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetDiamon());
		m_lbDiamon->setString(s->getCString());

		//////////////////////////////////////////////////////////////////////////


		sprDiamon->runAction(CCSequence::createWithTwoActions(
			CCSpawn::createWithTwoActions(
				CCJumpTo::create(1.0f, ccp(sprDiamon->getPositionX(), - sprDiamon->getContentSize().height), 600, 1),
				CCFadeOut::create(1.0f)
			),
			CCCallFunc::create(this, callfunc_selector(LoseDialog::closeReviveAnimation))
			));
	}
	else //Need more diamon
	{
		PLAY_OUT_PORP_EFFECT;

		CCMessageBox("Thông tin", "Bạn đã hết kim cương!");

		//visitble
		m_itExitButton->setVisible(true);

		//unvisiable
		m_lbTimer->setVisible(false);
		m_timerBar->setVisible(false);
		m_timerNode->setVisible(false);

		m_itRevive->setVisible(false);
		//m_itRevive->setEnabled(false);
		//m_itRevive->setOpacity(0.6f * 255);
		this->unscheduleUpdate();
	}
}

void LoseDialog::breakRecord()
{
	initParticle();

	float DELAY_BEFORE_START = 0.3f;
	float TIME_FOR_KY_LUC = 0.3f;
	float TIME_TO_SCALE_KY_LUC = 0.3f;
	float SCALE_ZERO = 0.01f;
	float SCALE_KY_LUC = 1.0f;
	float SCALE_ANH_SANG = 1.5f;
	float TIME_TO_SCALE_ANH_SANG = 0.5f;
	float ROTATE = 180.0f;

	//Anh sang
	CCSprite* sprAnhsang = CCSprite::create("anhsang.png");
	sprAnhsang->setPosition(ccp(138, 1280-478));
	sprAnhsang->setScale(SCALE_ZERO);
	this->addChild(sprAnhsang);
	sprAnhsang->runAction(
		CCSequence::create(
			CCDelayTime::create(DELAY_BEFORE_START + TIME_FOR_KY_LUC),
			CCSpawn::createWithTwoActions( //in
				CCScaleTo::create(TIME_TO_SCALE_ANH_SANG, SCALE_ANH_SANG),
				CCRotateBy::create(TIME_TO_SCALE_ANH_SANG, ROTATE)
			),
			CCSpawn::createWithTwoActions( //out
				CCScaleTo::create(TIME_TO_SCALE_ANH_SANG, SCALE_ZERO),
				CCRotateBy::create(TIME_TO_SCALE_ANH_SANG, ROTATE)
			),
			NULL
		)
	);


	//Ngoi sao
	CCSprite* sprKyLuc = CCSprite::create("kiluc.png");
	sprKyLuc->setPosition(ccp(138, 1280-475));
	sprKyLuc->setScale(1280.0f / sprKyLuc->getContentSize().height);
	this->addChild(sprKyLuc);

	sprKyLuc->runAction(
		CCSequence::createWithTwoActions(
			CCDelayTime::create(DELAY_BEFORE_START),
			CCSpawn::createWithTwoActions(
				CCScaleTo::create(TIME_TO_SCALE_KY_LUC, SCALE_KY_LUC),
				CCFadeIn::create(TIME_TO_SCALE_KY_LUC)
			)
		)
	);
}

void LoseDialog::initParticle()
{
// 	m_emitter = new CCParticleSystemQuad();
// 	m_emitter->initWithTotalParticles(300);
// 	m_emitter->autorelease();
// 
// #if WIN32
// 	//nothing
// #else
// 	m_emitter->release();    // win32 : Remove this line
// #endif
// 	
// 	m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("sao.png") );
// 
// 	// duration
// 	m_emitter->setDuration(-1);
// 
// 	// gravity
// 	m_emitter->setGravity(CCPointZero);
// 
// 	// angle
// 	m_emitter->setAngle(90);
// 	m_emitter->setAngleVar(360);
// 
// 	// speed of particles
// 	m_emitter->setSpeed(160);
// 	m_emitter->setSpeedVar(20);
// 
// 	// radial
// 	m_emitter->setRadialAccel(-120);
// 	m_emitter->setRadialAccelVar(0);
// 
// 	// tagential
// 	m_emitter->setTangentialAccel(30);
// 	m_emitter->setTangentialAccelVar(0);
// 
// 	// emitter position
// 	m_emitter->setPosition( ccp(160,240) );
// 	m_emitter->setPosVar(CCPointZero);
// 
// 	// life of particles
// 	m_emitter->setLife(3);
// 	m_emitter->setLifeVar(1);
// 
// 	// spin of particles
// 	m_emitter->setStartSpin(0);
// 	m_emitter->setStartSpinVar(0);
// 	m_emitter->setEndSpin(0);
// 	m_emitter->setEndSpinVar(2000);
// 
// 	// color of particles
// 	ccColor4F startColor = {0.5f, 0.5f, 0.5f, 1.0f};
// 	m_emitter->setStartColor(startColor);
// 
// 	ccColor4F startColorVar = {0.5f, 0.5f, 0.5f, 1.0f};
// 	m_emitter->setStartColorVar(startColorVar);
// 
// 	ccColor4F endColor = {0.1f, 0.1f, 0.1f, 0.2f};
// 	m_emitter->setEndColor(endColor);
// 
// 	ccColor4F endColorVar = {0.1f, 0.1f, 0.1f, 0.2f};    
// 	m_emitter->setEndColorVar(endColorVar);
// 
// 	// size, in pixels
// 	m_emitter->setStartSize(30.0f);
// 	m_emitter->setStartSizeVar(00.0f);
// 	m_emitter->setEndSize(kParticleStartSizeEqualToEndSize);
// 
// 	// emits per second
// 	m_emitter->setEmissionRate(m_emitter->getTotalParticles()/m_emitter->getLife());
// 
// 	// additive
// 	m_emitter->setBlendAdditive(false);
// 
// 
// 	// SET POSITION //////////////////////////
// 	m_emitter->setPosition( ccp(138, 1280-475) );
// 
// 	//START,i dont know //////////////////////////////////////////////////////////////////////////
}

void LoseDialog::update( float delta )
{
	if (m_timerBar != NULL)
	{
		m_elapsedTime += delta;

		if (m_elapsedTime < G_WAIT_TO_REVIVE) //scale
		{
			CCString* s = CCString::createWithFormat("%d", (int)(1 + (G_WAIT_TO_REVIVE - m_elapsedTime)));
			m_lbTimer->setString(s->getCString());

			float originW = m_timerBar->getContentSize().width;
			float originH = m_timerBar->getContentSize().height;

			m_timerNode->setTextureRect(CCRectMake(0, 0, ((G_WAIT_TO_REVIVE - m_elapsedTime) / G_WAIT_TO_REVIVE) * originW, originH));
		}
		else
		{
			//unvisiable
			m_itExitButton->setVisible(true);
			m_lbTimer->setVisible(false);

			m_timerBar->setVisible(false);
			m_timerNode->setVisible(false);

			//m_itRevive->setEnabled(false);
			m_itRevive->setVisible(false);
			//m_itRevive->setOpacity(0.6f * 255);
			this->unscheduleUpdate();
		}
	}	
}

void LoseDialog::closeReviveAnimation()
{
	MainGameScene* maingame = (MainGameScene*) this->getParent();
	maingame->reviveCallback();

	//remove dialog after animation
	this->removeFromParent();
}
