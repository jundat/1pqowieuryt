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
    m_facebookItem->setPosition(ccp(214, 1280-662));
    
    //
    //play test
    //
    m_playItem = CCMenuItemImage::create(
         "button.png",
         "button_down.png",
         this,
         menu_selector(NotLoggedInMenuScene::playCallback));
    m_playItem->setScale(SCALE_BUTTON);
    m_playItem->setPosition(ccp(589, 1280-662));
    

    m_menu = CCMenu::create(m_playItem, m_facebookItem, NULL);
    m_menu->setPosition(CCPointZero);
    this->addChild(m_menu);
    
    CCPoint ANCHOR_TEXT = ccp(0.4f, 0.3f);
    //
    //text
    //
    CCLabelTTF* txtLogIn = CCLabelTTF::create(TXT("try_login"), G_FONT_NORMAL, 48);
    txtLogIn->setColor(ccBLACK);
    txtLogIn->setAnchorPoint(ANCHOR_TEXT);
    txtLogIn->setPosition(m_facebookItem->getPosition());
    this->addChild(txtLogIn);
    
    CCLabelTTF* txtPlay = CCLabelTTF::create(TXT("try_play_test"), G_FONT_NORMAL, 48);
    txtPlay->setColor(ccBLACK);
    txtPlay->setAnchorPoint(ANCHOR_TEXT);
    txtPlay->setPosition(m_playItem->getPosition());
    this->addChild(txtPlay);
    
    //
    //image
    //
    CCSprite* sprFb = CCSprite::create("fb_button.png");
    sprFb->setPosition(ccp(91, 1280-651));
    this->addChild(sprFb);
    
    CCSprite* sprPlay = CCSprite::create("fire_button.png");
    sprPlay->setPosition(ccp(474, 1280-654));
    this->addChild(sprPlay);
    
    
	
	this->setKeypadEnabled(true);
	STOP_BACKGROUND_MUSIC;
    return true;
}

void NotLoggedInMenuScene::playStartAnimation(int lastLife)
{
    this->gotoMainGame();
}

void NotLoggedInMenuScene::gotoMainGame()
{
    GameClientManager::sharedGameClientManager()->setDelegate(NULL);
    
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

    //facebookLogInOut();
    
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

