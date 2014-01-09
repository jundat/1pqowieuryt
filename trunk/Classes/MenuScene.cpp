#include "MenuScene.h"
#include "MainGameScene.h"
#include "ScoreScene.h"
#include "SettingScene.h"
#include "AudioManager.h"
#include "DataManager.h"
#include <time.h>

USING_NS_CC;
USING_NS_CC_EXT;

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

	this->setKeypadEnabled(true);

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////

	CCSprite* bg = CCSprite::create("bg_stars.png");
	bg->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(bg, 0);
	
	CCSprite* menuTop = CCSprite::create("menu_top.png");
	menuTop->setPosition(ccp(visibleSize.width/2 + origin.x, -50 - menuTop->getContentSize().height/2 + visibleSize.height + origin.y));
	this->addChild(menuTop, 0);

    CCMenuItemImage *playItem = CCMenuItemImage::create(
                                        "new_button.png",
                                        "new_button_press.png",
                                        this,
                                        menu_selector(MenuScene::playCallback));
    
	playItem->setPosition(ccp(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height/2));

	//

	CCMenuItemImage *scoreItem = CCMenuItemImage::create(
		"score_button.png",
		"score_button_press.png",
		this,
		menu_selector(MenuScene::scoreCallback));

	scoreItem->setPosition(ccp(origin.x + visibleSize.width/2,
		playItem->getPositionY() - playItem->getContentSize().height/2 - scoreItem->getContentSize().height/2 - 10));

	//

	CCMenuItemImage *settingItem = CCMenuItemImage::create(
		"setting_button.png",
		"setting_button_press.png",
		this,
		menu_selector(MenuScene::settingCallback));

	settingItem->setPosition(ccp(origin.x + visibleSize.width/2,
		scoreItem->getPositionY() - scoreItem->getContentSize().height/2 - settingItem->getContentSize().height/2 - 10));

	//

	CCMenuItemImage *exitItem = CCMenuItemImage::create(
		"exit_button.png",
		"exit_button_press.png",
		this,
		menu_selector(MenuScene::exitCallback));

	exitItem->setPosition(ccp(origin.x + visibleSize.width/2,
		settingItem->getPositionY() - settingItem->getContentSize().height/2 - exitItem->getContentSize().height/2 - 10));

	//

    CCMenu* pMenu = CCMenu::create(playItem, scoreItem, settingItem, exitItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
	
	AudioManager::sharedAudioManager()->PlayBackground("background.ogg");

    return true;
}

void MenuScene::playCallback(CCObject* pSender)
{
	//check if last_player_life > 0
	int lastLife = DataManager::sharedDataManager()->GetLastPlayerLife();
	lastLife = (lastLife > G_MAX_PLAYER_LIFE) ? G_MAX_PLAYER_LIFE : lastLife;
	DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);

	CCLOG("Last life: %d", lastLife);

	if (lastLife > 0)
	{
// 		CCScene *pScene = MainGameScene::scene();
// 		CCDirector::sharedDirector()->replaceScene(pScene);

		CCScene *pScene = CCTransitionFade::create(0.5, MainGameScene::scene());
		CCDirector::sharedDirector()->replaceScene(pScene);

		CCLOG("LastLife > 0 -> Play");
	} 
	else
	{
		//get revive_life
		tm* lasttm = DataManager::sharedDataManager()->GetLastDeadTime();
		time_t lastTime = mktime(lasttm);
		time_t curTime = time(NULL);
		double seconds = difftime(curTime, lastTime);

		lastLife = (int)(seconds / G_PLAYER_TIME_TO_REVIVE);
		lastLife = (lastLife > G_MAX_PLAYER_LIFE) ? G_MAX_PLAYER_LIFE : lastLife;

		CCLOG("Revive Last life: %d", lastLife);

		if (lastLife > 0)
		{
			DataManager::sharedDataManager()->SetLastPlayerLife(lastLife);
			
// 			CCScene *pScene = MainGameScene::scene();
// 			CCDirector::sharedDirector()->replaceScene(pScene);
			CCScene *pScene = CCTransitionFade::create(0.5, MainGameScene::scene());
			CCDirector::sharedDirector()->replaceScene(pScene);

			CCLOG("Revive->LastLife > 0 -> Play");
		}
		else
		{
			CCMessageBox("Not enough life, wait for 10s!", "Info");
			CCLOG("Revive->LastLife < 0 -> Can not play");
			return;
		}
	}
}

void MenuScene::scoreCallback( CCObject* pSender )
{
	CCScene *pScene = CCTransitionFade::create(0.5, ScoreScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void MenuScene::settingCallback( CCObject* pSender )
{
	CCScene *pScene = CCTransitionFade::create(0.5, SettingScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void MenuScene::exitCallback( CCObject* pSender )
{
	CCDirector::sharedDirector()->end();
}

void MenuScene::keyBackClicked()
{
	exitCallback(NULL);
}
