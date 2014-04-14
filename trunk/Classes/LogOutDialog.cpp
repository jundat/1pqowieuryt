#include "LogOutDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "DataManager.h"
#include <time.h>
#include "cocos-ext.h"
#include "TextLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool LogOutDialog::init()
{
	if ( !CCLayerColor::initWithColor(G_DIM_COLOR) )
	{
		return false;
	}

	CCPoint pexit = ccp(250, G_DESIGN_HEIGHT - 810);
	CCPoint pask = ccp(G_DESIGN_WIDTH-250, G_DESIGN_HEIGHT - 810);

	CCScale9Sprite* dialog = CCScale9Sprite::create("dialog.png");
	dialog->setPosition(ccp(400, 640));
	dialog->setContentSize(CCSizeMake(680, 480));
	this->addChild(dialog);

	MY_CREATE_MENU_BUTTON(exitButton, "button.png", "button_down.png", TXT("btn_yes"), 
		"Roboto-Medium.ttf", 48, ccBLACK, pexit, this, LogOutDialog::yesCallback);

	MY_CREATE_MENU_BUTTON(askButton, "button.png", "button_down.png", TXT("btn_no"), 
		"Roboto-Medium.ttf", 48, ccBLACK, pask, this, LogOutDialog::noCallback);
	
	CCMenu* menu = CCMenu::create(exitButton, askButton, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	CCLabelTTF* labelTitle = CCLabelTTF::create(TXT("fb_logout_ask"), 
		"Roboto-Medium.ttf", 
		48, 
		CCSizeMake(600, 0), 
		kCCTextAlignmentCenter, 
		kCCVerticalTextAlignmentCenter);
	labelTitle->setColor(ccBLACK);
	labelTitle->setPosition(ccp(400, 700));
	this->addChild(labelTitle);

	this->setKeypadEnabled(true);
    return true;
}

void LogOutDialog::yesCallback( CCObject* pSender )
{
	MenuScene* parent = (MenuScene*)this->getParent();

	parent->facebookLogInOut();

	parent->setTouchEnabled(true);
	parent->onCloseDialog();
	this->removeFromParent();
}


void LogOutDialog::noCallback( CCObject* pSender )
{
	MenuScene* parent = (MenuScene*)this->getParent();
	parent->setTouchEnabled(true);
	parent->onCloseDialog();
	this->removeFromParent();
}


void LogOutDialog::keyBackClicked()
{
	noCallback(NULL);
}
