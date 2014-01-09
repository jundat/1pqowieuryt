#include "PauseDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"

USING_NS_CC;

bool PauseDialog::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCPoint  presume = ccp(400, G_DESIGN_HEIGHT/2 + 200);
	CCPoint prestart = ccp(400, G_DESIGN_HEIGHT/2);
	CCPoint  pexit= ccp(400, G_DESIGN_HEIGHT/2 - 200);
	float textScale = 0.8f;

	CCMenuItemImage* exitButton = CCMenuItemImage::create(
		"exit_button.png",
		"exit_button_press.png",
		this,
		menu_selector(PauseDialog::menuCallback));
	exitButton->setScale(textScale);
	exitButton->setPosition(pexit);

	CCMenuItemImage* resumeButton = CCMenuItemImage::create(
		"resume.png",
		"resume1.png",
		this,
		menu_selector(PauseDialog::resumeCallBack));
	resumeButton->setScale(textScale);
	resumeButton->setPosition(presume);

	CCMenuItemImage* restartButton = CCMenuItemImage::create(
		"restart.png",
		"restart1.png",
		this,
		menu_selector(PauseDialog::restartCallBack));
	restartButton->setScale(textScale);
	restartButton->setPosition(prestart);

	CCMenu* menu = CCMenu::create(exitButton, resumeButton, restartButton, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	return true;
}

void PauseDialog::menuCallback( CCObject* pSender )
{
	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void PauseDialog::resumeCallBack( CCObject* pSender )
{
	MainGameScene* parent = (MainGameScene*) this->getParent();
	parent->resumeCallback();
	this->removeFromParent();
}

void PauseDialog::restartCallBack( CCObject* pSender )
{
	MainGameScene* parent = (MainGameScene*) this->getParent();
	parent->restartCallback();
	this->removeFromParent();
}
