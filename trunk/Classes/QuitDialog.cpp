#include "QuitDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "DataManager.h"
#include <time.h>
#include "cocos-ext.h"
#include "TextLoader.h"
#include "YesNoDialogParent.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool QuitDialog::init()
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
		"Roboto-Medium.ttf", 48, ccBLACK, pexit, this, QuitDialog::yesCallback);

	MY_CREATE_MENU_BUTTON(askButton, "button.png", "button_down.png", TXT("btn_no"), 
		"Roboto-Medium.ttf", 48, ccBLACK, pask, this, QuitDialog::noCallback);
	
	CCMenu* menu = CCMenu::create(exitButton, askButton, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	CCLabelTTF* labelTitle = CCLabelTTF::create(TXT("quit_ask"), 
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

void QuitDialog::yesCallback( CCObject* pSender )
{
	CCDirector::sharedDirector()->end();
}


void QuitDialog::noCallback( CCObject* pSender )
{
	m_yesnoParent->onCloseDialog();
	this->removeFromParent();
}


void QuitDialog::keyBackClicked()
{
	noCallback(NULL);
}
