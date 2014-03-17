#include "QuitDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "DataManager.h"
#include <time.h>
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool QuitDialog::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCPoint pexit = ccp(250, G_DESIGN_HEIGHT - 810);
	CCPoint pask = ccp(G_DESIGN_WIDTH-250, G_DESIGN_HEIGHT - 810);
	float textScale = 0.6f;


	CCScale9Sprite* dialog = CCScale9Sprite::create("dialog.png");
	dialog->setPosition(ccp(400, 640));
	dialog->setContentSize(CCSizeMake(680, 480));
	this->addChild(dialog, -2);
	
	CCMenuItemImage* exitButton = CCMenuItemImage::create(
		"yes.png",
		"yesDown.png",
		this,
		menu_selector(QuitDialog::yesCallback));
	exitButton->setScale(textScale);
	exitButton->setPosition(pexit);


	CCMenuItemImage* askButton = CCMenuItemImage::create(
		"no.png",
		"noDown.png",
		this,
		menu_selector(QuitDialog::noCallback));
	askButton->setScale(textScale);
	askButton->setPosition(pask);

	CCMenu* menu = CCMenu::create(exitButton, askButton, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	CCLabelTTF* labelTitle = CCLabelTTF::create("Bạn có muốn\nthoát game?", "Roboto-Medium.ttf", 48);
	labelTitle->setFontFillColor(ccc3(56, 56, 56));
	labelTitle->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 50));
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
	MenuScene* parent = (MenuScene*)this->getParent();
	parent->setTouchEnabled(true);
	parent->onCloseDialog();
	this->removeFromParent();
}


void QuitDialog::keyBackClicked()
{
	noCallback(NULL);
}
