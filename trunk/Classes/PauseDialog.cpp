#include "PauseDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "AudioManager.h"
#include "MyMacro.h"
#include "TextLoader.h"

USING_NS_CC;

bool PauseDialog::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCPoint  presume = ccp(400, G_DESIGN_HEIGHT/2 + 100);
	CCPoint  pexit= ccp(400, G_DESIGN_HEIGHT/2 - 100);

	MY_CREATE_MENU_BUTTON(exitButton, "button.png", "button_down.png", TXT("btn_end"), 
		"Roboto-Medium.ttf", 48, ccBLACK, pexit, this, PauseDialog::menuCallback);

	MY_CREATE_MENU_BUTTON(resumeButton, "button.png", "button_down.png", TXT("btn_resume"), 
		"Roboto-Medium.ttf", 48, ccBLACK, presume, this, PauseDialog::resumeCallBack);
	
	CCMenu* menu = CCMenu::create(exitButton, resumeButton, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	return true;
}

void PauseDialog::menuCallback( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void PauseDialog::resumeCallBack( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	MainGameScene* parent = (MainGameScene*) this->getParent();
	parent->resumeCallback();
	this->removeFromParent();
}

void PauseDialog::restartCallBack( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	MainGameScene* parent = (MainGameScene*) this->getParent();
	parent->restartCallback();
	this->removeFromParent();
}
