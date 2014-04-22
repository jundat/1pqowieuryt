#include "NotLoggedInMenuScene.h"
#include "MainGameScene.h"
#include "ScoreScene.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "QuitDialog.h"
#include "MenuScene.h"
#include <time.h>
#include "TextLoader.h"

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

	this->setKeypadEnabled(true);
	
	string lang = DataManager::sharedDataManager()->GetLanguage();
	if (lang.compare("English") == 0)
	{
		m_bg = CCSprite::create("en_bg_menu.png");
	}
	else
	{
		m_bg = CCSprite::create("vn_bg_menu.png");
	}	
	
	m_bg->setAnchorPoint(CCPointZero);
	m_bg->setPosition(CCPointZero);
	this->addChild(m_bg, 0);
	
    //
    //new button border
    //
    m_sprNewButtonBorderOut = CCSprite::create("new_button_border_out.png");
    m_sprNewButtonBorderIn = CCSprite::create("new_button_border_in.png");
    
    m_sprNewButtonBorderOut->setPosition(ccp(400, 1280-813));
    m_sprNewButtonBorderIn->setPosition(ccp(400, 1280-813));
    
    this->addChild(m_sprNewButtonBorderOut);
    this->addChild(m_sprNewButtonBorderIn);
    
    
    //
    //disable play button animation in local mode
    //
    
    string en_start_normal = string("en_new_button.png");
    string en_start_press = string("en_new_button_press.png");
    
    string vn_start_normal = string("vn_new_button.png");
    string vn_start_press = string("vn_new_button_press.png");
    
	//
	if (lang.compare("English") == 0)
	{
		m_playItem = CCMenuItemImage::create(
			"en_new_button.png",
			"en_new_button_press.png",
			this,
			menu_selector(NotLoggedInMenuScene::playCallback));
	} 
	else
	{
		m_playItem = CCMenuItemImage::create(
			"vn_new_button.png",
			"vn_new_button_press.png",
			this,
			menu_selector(NotLoggedInMenuScene::playCallback));
	}

	m_playItem->setPosition(ccp(400, 1280-813));
    
    
    //Log in facebook
    CCMenuItemImage* itFacebook = CCMenuItemImage::create("button.png", "button_down.png", "button_down.png", this, menu_selector(NotLoggedInMenuScene::facebookCallback));
    itFacebook->setPosition(ccp(400, 2*itFacebook->getContentSize().height));

    m_menu = CCMenu::create(m_playItem, itFacebook, NULL);
    m_menu->setPosition(CCPointZero);
    this->addChild(m_menu, 1);

	
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

    //
    //animation
    //
    
    m_sprNewButtonBorderOut->runAction(
        CCSequence::createWithTwoActions(
            CCRotateTo::create(0.15f, 40),
            CCRotateTo::create(0.15f, 0)
        ));
    m_sprNewButtonBorderIn->runAction(
        CCSequence::create(
            CCRotateTo::create(0.15f, -40),
            CCRotateTo::create(0.15f, 0),
            CCCallFunc::create(this, callfunc_selector(NotLoggedInMenuScene::gotoMainGame)),
            NULL
        ));
    
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
    CCLOG("NotLoggedInMenuScene:: onCloseDialog");
	m_menu->setEnabled(true);
    CCLOG("1");
	this->setKeypadEnabled(true);
    CCLOG("2");
    this->setTouchEnabled(true);
    CCLOG("3");
}

