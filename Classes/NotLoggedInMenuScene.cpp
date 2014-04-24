#include "NotLoggedInMenuScene.h"
#include "MainGameScene.h"
#include "ScoreScene.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "QuitDialog.h"
#include "MenuScene.h"
#include "TextLoader.h"
#include <time.h>


USING_NS_CC;
USING_NS_CC_EXT;



CCScene* NotLoggedInMenuScene::scene()
{
    CCScene *scene = CCScene::create();
    NotLoggedInMenuScene *layer = NotLoggedInMenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool NotLoggedInMenuScene::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(195, 200, 201, 255)) )
    {
        return false;
    }
	
    float dt = 193;
    
    //
    //bg
    //
    m_bg = CCSprite::create("not_logged_bg.png");
	m_bg->setAnchorPoint(CCPointZero);
	m_bg->setPosition(CCPointZero);
	this->addChild(m_bg, 0);
    
    float SCALE_BUTTON = 1.163f;
    //
    //log in
    //
    m_facebookItem = CCMenuItemImage::create(
         "button.png",
         "button_down.png",
         this,
         menu_selector(NotLoggedInMenuScene::facebookCallback));
    m_facebookItem->setScale(SCALE_BUTTON);
    m_facebookItem->setPosition(ccp(214, 1280-662-dt));
    
    //
    //play test
    //
    m_playItem = CCMenuItemImage::create(
         "guest_mode_button.png",
         "guest_mode_button_down.png",
         this,
         menu_selector(NotLoggedInMenuScene::playCallback));
    m_playItem->setScale(SCALE_BUTTON);
    m_playItem->setPosition(ccp(589, 1280-662-dt));
    

    m_menu = CCMenu::create(m_playItem, m_facebookItem, NULL);
    m_menu->setPosition(CCPointZero);
    this->addChild(m_menu);
    
    //
    //text
    //
    CCLabelTTF* txtLogIn = CCLabelTTF::create(TXT("try_login"), G_FONT_NORMAL, 48);
    txtLogIn->setColor(ccBLACK);
    txtLogIn->setAnchorPoint(ccp(0.4f, 0.35f));
    txtLogIn->setPosition(m_facebookItem->getPosition());
    this->addChild(txtLogIn);
    
    CCLabelTTF* txtPlay = CCLabelTTF::create(TXT("try_play_test"), G_FONT_NORMAL, 36);
    txtPlay->setColor(ccBLACK);
    txtPlay->setAnchorPoint(ccp(0.5, 0.45f));
    txtPlay->setPosition(m_playItem->getPosition());
    this->addChild(txtPlay);
    
    //
    //image
    //
    CCSprite* sprFb = CCSprite::create("fb_button.png");
    sprFb->setPosition(ccp(91, 1280-651-dt));
    this->addChild(sprFb);
    
    CCLabelTTF* vendor = CCLabelTTF::create(TXT("vendor"), G_FONT_NORMAL, 32);
    vendor->setColor(ccBLACK);
    vendor->setPosition(ccp(400, vendor->getContentSize().height * 1.1f));
    this->addChild(vendor);
    
    this->initCloud();
    
    string fbId = DataManager::sharedDataManager()->GetFbID();
    if (fbId.compare("NULL") != 0) {
        this->scheduleOnce(schedule_selector(NotLoggedInMenuScene::scheduleTick), 1.0f);
    }
    
	this->setKeypadEnabled(true);
	STOP_BACKGROUND_MUSIC;
    return true;
}

void NotLoggedInMenuScene::initCloud()
{
    for (int i = 0; i < 8; ++i) {
        CCString* file = CCString::createWithFormat("cloud%d.png", 1 + (i % 3));
        CCSprite* c = CCSprite::create(file->getCString());
        this->addChild(c);
        
        float rdx = CCRANDOM_0_1() * 1000-200;
        float rdy = 1280 - CCRANDOM_0_1() * 600;
        
        c->setPosition(ccp(rdx, rdy));
        
        float rdDirectionX = CCRANDOM_0_1(); //0, 1
        
        float rdTime = 20 + CCRANDOM_0_1() * 10; // 15->20
        
        CCSequence* seq;
        if (rdDirectionX > 0.5f) {
            seq = CCSequence::create(
                                     CCMoveTo::create(rdTime, ccp(800 + 500, rdy)),
                                     CCMoveTo::create(rdTime, ccp(- 500, rdy)),
                                     NULL);
                                     } else {
                                         seq = CCSequence::create(
                                                                  CCMoveTo::create(rdTime, ccp(- 500, rdy)),
                                                                  CCMoveTo::create(rdTime, ccp(800 + 500, rdy)),
                                                                  NULL);
                                     }
        
        CCRepeatForever* repeat = CCRepeatForever::create(seq);
        c->runAction(repeat);
    }

}

void NotLoggedInMenuScene::gotoMainGame()
{
    GameClientManager::sharedGameClientManager()->setDelegate(NULL);
    
    //reset data
    DataManager::sharedDataManager()->ResetDataToPlayLocal();
    
    CCScene *pScene = CCTransitionFade::create(0.5, MainGameScene::scene());
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void NotLoggedInMenuScene::playCallback(CCObject* pSender)
{
    PLAY_GUN_RELOAD_EFFECT;

    gotoMainGame();
	this->setKeypadEnabled(false);
	this->setTouchEnabled(false);
}

void NotLoggedInMenuScene::keyBackClicked()
{
	exitCallback(NULL);
}

void NotLoggedInMenuScene::facebookCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;
    
    //
    //reset data when log in ok
    //
    DataManager::sharedDataManager()->ResetDataToLogIn();
    
    CCScene *scene = CCScene::create();
    MenuScene *layer = MenuScene::create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(scene);
    layer->facebookCallback(NULL);
}

void NotLoggedInMenuScene::exitCallback( CCObject* pSender )
{
	QuitDialog* dialog = QuitDialog::create();
    dialog->setYesNoDialogParent(this);
	this->addChild(dialog, 10);
	this->onShowDialog();
}

void NotLoggedInMenuScene::onShowDialog()
{
	m_menu->setEnabled(false);
	this->setKeypadEnabled(false);
}

void NotLoggedInMenuScene::onCloseDialog()
{
	m_menu->setEnabled(true);
	this->setKeypadEnabled(true);
    this->setTouchEnabled(true);
}

void NotLoggedInMenuScene::scheduleTick(float dt)
{
    m_facebookItem->selected();
    
    m_facebookItem->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.5f), CCCallFunc::create(this, callfunc_selector(NotLoggedInMenuScene::jumpMenu))));
}

void NotLoggedInMenuScene::jumpMenu()
{
    CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
    CCDirector::sharedDirector()->replaceScene(pScene);
}

