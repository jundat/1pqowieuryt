#include "MenuScene.h"
#include "MainGameScene.h"
#include "SettingScene.h"
#include "AudioManager.h"

USING_NS_CC;

CCScene* MenuScene::scene()
{
    CCScene *scene = CCScene::create();
    MenuScene *layer = MenuScene::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////

	CCSprite* bg = CCSprite::create("bg_menu.png");
	bg->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(bg, 0);

	CCSprite* menuTop = CCSprite::create("menu_top.png");
	menuTop->setPosition(ccp(visibleSize.width/2 + origin.x, - 280 + visibleSize.height + origin.y));
	this->addChild(menuTop, 0);

    CCMenuItemImage *playItem = CCMenuItemImage::create(
                                        "new_button.png",
                                        "new_button_press.png",
                                        this,
                                        menu_selector(MenuScene::playCallback));
    
	playItem->setPosition(ccp(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height/2));

	CCMenuItemImage *settingItem = CCMenuItemImage::create(
		"setting_button.png",
		"setting_button_press.png",
		this,
		menu_selector(MenuScene::settingCallback));

	settingItem->setPosition(ccp(origin.x + visibleSize.width/2,
		playItem->getPositionY() - playItem->getContentSize().height/2 - settingItem->getContentSize().height/2 - 10));


	CCMenuItemImage *exitItem = CCMenuItemImage::create(
		"exit_button.png",
		"exit_button_press.png",
		this,
		menu_selector(MenuScene::exitCallback));

	exitItem->setPosition(ccp(origin.x + visibleSize.width/2,
		settingItem->getPositionY() - settingItem->getContentSize().height/2 - exitItem->getContentSize().height/2 - 10));


    CCMenu* pMenu = CCMenu::create(playItem, settingItem, exitItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

	AudioManager::sharedAudioManager()->PlayBackground("background.ogg");

    return true;
}


void MenuScene::playCallback(CCObject* pSender)
{
    CCScene *pScene = MainGameScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void MenuScene::settingCallback( CCObject* pSender )
{
	CCScene *pScene = SettingScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void MenuScene::exitCallback( CCObject* pSender )
{
	CCDirector::sharedDirector()->end();
}
