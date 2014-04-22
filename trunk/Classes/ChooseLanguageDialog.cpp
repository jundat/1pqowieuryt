#include "ChooseLanguageDialog.h"
#include "MainGameScene.h"
#include "NotLoggedInMenuScene.h"
#include "DataManager.h"
#include <time.h>
#include "cocos-ext.h"
#include "TextLoader.h"
#include "Global.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* ChooseLanguageDialog::scene()
{
	CCScene *scene = CCScene::create();
	ChooseLanguageDialog *layer = ChooseLanguageDialog::create();
	scene->addChild(layer);
	return scene;
}

bool ChooseLanguageDialog::init()
{
	if ( !CCLayerColor::initWithColor(G_DIM_COLOR) )
	{
		return false;
	}

	CCSprite* bg = CCSprite::create("bg_stars.png");
	bg->setPosition(ccp(400, 640));
	this->addChild(bg);

	CCScale9Sprite* dialog = CCScale9Sprite::create("dialog.png");
	dialog->setPosition(ccp(400, 640));
	dialog->setContentSize(CCSizeMake(678, 375));
	this->addChild(dialog);
	
	CCMenuItemImage* itVietnam = CCMenuItemImage::create("flag_vietnam.png", "flag_vietnam_down.png", this, menu_selector(ChooseLanguageDialog::vietnamCallback));
	itVietnam->setPosition(ccp(229, 1280-643));
	itVietnam->setScale(1.5f);

	CCMenuItemImage* itEnglish = CCMenuItemImage::create("flag_english.png", "flag_english_down.png", this, menu_selector(ChooseLanguageDialog::englishCallback));
	itEnglish->setPosition(ccp(572, 1280-643));
	itEnglish->setScale(1.5f);

	CCMenu* menu = CCMenu::create(itEnglish, itVietnam, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

    return true;
}

void ChooseLanguageDialog::vietnamCallback( CCObject* pSender )
{
	DataManager::sharedDataManager()->SetFirstTimeChooseLanguage(false);
	DataManager::sharedDataManager()->SetLanguage("Vietnamese");
	TextLoader::shareTextLoader()->setCurrentLanguage(LANGUAGE_VIETNAMESE);

	gotoMenu();
}


void ChooseLanguageDialog::englishCallback( CCObject* pSender )
{
	DataManager::sharedDataManager()->SetFirstTimeChooseLanguage(false);
	DataManager::sharedDataManager()->SetLanguage("English");
	TextLoader::shareTextLoader()->setCurrentLanguage(LANGUAGE_ENGLISH);

    gotoMenu();
}

void ChooseLanguageDialog::gotoMenu()
{
    CCScene *pScene = CCTransitionFade::create(0.5, NotLoggedInMenuScene::scene());
    CCDirector::sharedDirector()->replaceScene(pScene);
}