#include "LoseDialog.h"
#include "MainGameScene.h"
#include "NotLoggedInMenuScene.h"
#include "MenuScene.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "cocos-ext.h"
#include "GameClientObjects.h"
#include "BreakLeaderboardDialog.h"
#include "TextLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

bool LoseDialog::init()
{
	if ( !CCLayerColor::initWithColor(G_DIM_COLOR) )
	{
		return false;
	}
	
	m_timerBar = NULL;
	m_timerNode = NULL;
	m_lbTimer = NULL;
    m_waitDialog = NULL;

	CCPoint pExit = ccp(400, 1280-813);
	CCPoint pRevive = ccp(400, 1280-813);
	CCPoint pTimerBar = ccp(400 - 161, 1280-740);
	CCPoint pTimer = ccp(400, 1280-700);

	

	CCScale9Sprite* bg = CCScale9Sprite::create("dialog.png");
	bg->setPosition(ccp(400, 640));
	bg->setContentSize(CCSizeMake(680, 480));
	this->addChild(bg);
	

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

	MY_CREATE_MENU_BUTTON(exitButton, "button.png", "button_down.png", TXT("btn_end"), 
		"Roboto-Medium.ttf", 48, ccBLACK, pExit, this, LoseDialog::menuCallBack);
	m_itExitButton = exitButton;	


	MY_CREATE_MENU_BUTTON(reviveButton, "button.png", "button_down.png", TXT("btn_revive"), 
		"Roboto-Medium.ttf", 48, ccBLACK, pRevive, this, LoseDialog::reviveCallBack);
    reviveButton->setVisible(false);
	m_itRevive = reviveButton;
	
	//REVIVE /////////////////////////////////////////////////////////////////
	string fbId = DataManager::sharedDataManager()->GetFbID();
	bool isJustRevive = DataManager::sharedDataManager()->GetIsJustRevived();
    m_isLoggedIn = false;
    
    if (fbId.compare("NULL") == 0) {
        m_isLoggedIn = false;
        
        m_lbDiamon->setVisible(false);
        sprDiamon->setVisible(false);

    } else {
        m_isLoggedIn = true;
    }
    
	if (isJustRevive == true || m_isLoggedIn == false) //no revive
	{
		m_itRevive->setVisible(false);
	}
	else //have revive, init timer
	{
        m_itRevive->setVisible(true);
		m_itExitButton->setVisible(false);

		m_elapsedTime = 0;

		s = CCString::createWithFormat("%d", (int)G_WAIT_TO_REVIVE);
		m_lbTimer = CCLabelTTF::create(s->getCString(), "Roboto-Medium.ttf", 48);
		m_lbTimer->setColor(ccc3(0, 0, 0));
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

	CCLabelTTF* labelTitle = CCLabelTTF::create(TXT("lose_score"), "Roboto-Medium.ttf", 64);
	labelTitle->setColor(ccc3(56, 56, 56));
	labelTitle->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 160)); //+40
	this->addChild(labelTitle);

	s = CCString::createWithFormat("%d", m_score);
	CCLabelTTF* lbScore = CCLabelTTF::create(s->getCString(), "Roboto-Medium.ttf", 96);
	lbScore->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 50));
	lbScore->setColor(ccc3(56, 56, 56));
	this->addChild(lbScore);

	//BREAK THE RECORD ///////////////////////////////////////////////////////
	if (m_isBreakRecord && m_isLoggedIn == true)
	{
		PLAY_GET_BOMB_EFFECT;
		breakRecord();
	}

    GameClientManager::sharedGameClientManager()->setDelegate(this);
	//////////////////////////////////////////////////////////////////////////

    return true;
}

void LoseDialog::menuCallBack( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	//revive set to false
	DataManager::sharedDataManager()->SetIsJustRevived(false);

	//////////////////////////////////////////////////////////////////////////

	bool isBreakoutLeaderboard = false; //true;

	if (m_isBreakRecord)
	{
		//check if breakout leaderboard
		CCArray* arrHigherFriends = DataManager::sharedDataManager()->GetHigherFriends();
		int highscore = DataManager::sharedDataManager()->GetHighScore();

		int count = arrHigherFriends->count();
		CCLOG(" --------------------- GET HIGHER FRIENDS ---- %d -----------------", count);

		// 	FacebookAccount* acc = new FacebookAccount("100000770031652", "Dũng Đinh Nguyễn Anh", "dungdna@gmail.com", 123456);
		// 	BreakLeaderboardDialog* dialog = BreakLeaderboardDialog::create(acc);
		// 	this->addChild(dialog);

		for (int i = 0; i < count; ++i)
		{
			CCObject* obj = arrHigherFriends->objectAtIndex(i);
			FacebookAccount* acc = dynamic_cast<FacebookAccount*>(obj);
			CCLOG("%s: %s: %d", acc->m_fbId.c_str(), acc->m_fbName.c_str(), acc->m_score);

			if(acc && acc->m_score < highscore)
			{
				isBreakoutLeaderboard = true;

				//remove from list and save again
				arrHigherFriends->removeObject(obj);
				DataManager::sharedDataManager()->SetHigherFriends(arrHigherFriends);

				//remove all image to show BreakLeaderboard dialog
				this->removeAllChildren();

				//post status
				CCLOG("GET HIM: %s, %s, %d", acc->m_fbId.c_str(), acc->m_fbName.c_str(), acc->m_score);
				BreakLeaderboardDialog* dialog = BreakLeaderboardDialog::create(acc);
				this->addChild(dialog);

				break;
			}
		}
		CCLOG(" >>>>>>>>>>>>>>>>>> GET HIGHER FRIENDS <<<<<<<<<<<<<<<<<<");
	}

	
	//////////////////////////////////////////////////////////////////////////
	//NO BreakLeaderboard
	if (!isBreakoutLeaderboard)
	{
        CCScene *pScene;
        if (this->m_isLoggedIn) {
            pScene = CCTransitionFade::create(0.5, MenuScene::scene());
        } else {
            pScene = CCTransitionFade::create(0.5, NotLoggedInMenuScene::scene());
        }
        
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}

void LoseDialog::reviveCallBack( CCObject* pSender )
{
    int diamond = DataManager::sharedDataManager()->GetDiamon();
    if (diamond >= G_DIAMON_PER_LIFE) {
        this->showWaitDialog(TXT("buy_revive"));
        
        string fbid = DataManager::sharedDataManager()->GetFbID();
        GameClientManager::sharedGameClientManager()->buyItem(fbid.c_str(), string(G_ITEM_REVIVE), 1, string("Hoi_sinh"));
    } else {
        PLAY_OUT_PORP_EFFECT;
        CCMessageBox(TXT("lose_lack_diamond"), TXT("lose_error_caption"));
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
	CCSprite* sprKyLuc;

	string lang = DataManager::sharedDataManager()->GetLanguage();
	if (lang.compare("English") == 0)
	{
		sprKyLuc = CCSprite::create("en_kiluc.png");
	} 
	else
	{
		sprKyLuc = CCSprite::create("vn_kiluc.png");
	}

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


void LoseDialog::showWaitDialog(string title)
{
    CCLOG("LoseDialog::showWaitDialog");
    this->setKeypadEnabled(false);
    
    
    if (m_waitDialog != NULL) {
        m_waitDialog->m_refCount++;
        
    } else {
        m_waitDialog = WaitDialog::create();
        m_waitDialog->m_refCount = 1;
        m_waitDialog->setTitle(title);
        
        this->addChild(m_waitDialog, 100); // =1
    }
}

void LoseDialog::closeWaitDialog()
{
    CCLOG("LoseDialog::closeWaitDialog");
    if (m_waitDialog != NULL) {
        m_waitDialog->m_refCount--;
        
        if (m_waitDialog->m_refCount <= 0) {
            this->removeChild(m_waitDialog);
            m_waitDialog = NULL;

            
            this->setKeypadEnabled(true);
        }
    }
}



void LoseDialog::onBuyItemCompleted(bool isSuccess, int newCoin, std::string itemType, int itemCount, std::string uniqueTag)
{
    if (isSuccess) {
        CCLOG("ItemType= %s", itemType.c_str());
        CCLOG("Count= %d", itemCount);
        CCLOG("uniqueTag= %s", uniqueTag.c_str());
        
        DataManager::sharedDataManager()->SetDiamon(newCoin);
        
        PLAY_GET_DOUBLE_LAZER_EFFECT;
        
		//////////////////////////////////////////////////////////////////////////
		//unvisiable
		m_lbTimer->setVisible(false);
		m_timerBar->setVisible(false);
		m_timerNode->setVisible(false);
        
		m_itRevive->setEnabled(false);
		m_itRevive->setOpacity(0.6f * 255);
		this->unscheduleUpdate();
        
		CCString* s = CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetDiamon());
		m_lbDiamon->setString(s->getCString());
        
        //resume game
        closeReviveAnimation();

    } else {
        PLAY_OUT_PORP_EFFECT;
        CCMessageBox(TXT("buy_revive_error"), TXT("error_caption"));
    }
    
    this->closeWaitDialog();
}
