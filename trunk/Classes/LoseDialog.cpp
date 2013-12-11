#include "LoseDialog.h"
#include "MainGameScene.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool LoseDialog::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////

	CCSprite* bg = CCSprite::create("lose_dialog.png");
	bg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
	this->addChild(bg, -2);
	
	CCMenuItemImage* cancelButton = CCMenuItemImage::create(
		"button.png",
		"buttonPress.png",
		this,
		menu_selector(LoseDialog::CancelCallBack));

	cancelButton->setScale(0.6f);
	cancelButton->setPosition(ccp(origin.x + 254, origin.y + visibleSize.height - 965 + 20));

	CCMenuItemImage* okButton = CCMenuItemImage::create(
		"button.png",
		"buttonPress.png",
		this,
		menu_selector(LoseDialog::OkCallBack));
	okButton->setScale(0.6f);
	okButton->setPosition(ccp(origin.x + 546, origin.y + visibleSize.height - 965 + 20));

	CCMenu* menu = CCMenu::create(cancelButton, okButton, NULL);
	menu->setPosition(origin);
	this->addChild(menu);

	CCLabelTTF* cancel = CCLabelTTF::create("Cancel", "Marker Felt.ttf", 32);
	cancel->setFontFillColor(ccc3(0,0,0));
	cancel->setColor(ccc3(0,0,0));
	cancel->setPosition(ccp(origin.x + 254, origin.y + visibleSize.height - 965 + 20));
	this->addChild(cancel);

	CCLabelTTF* ok = CCLabelTTF::create("Ok", "Marker Felt.ttf", 32);
	ok->setFontFillColor(ccc3(0,0,0));
	ok->setColor(ccc3(0,0,0));
	ok->setPosition(ccp(origin.x + 546, origin.y + visibleSize.height - 965 + 20));
	this->addChild(ok);

    return true;
}

void LoseDialog::CancelCallBack( CCObject* pSender )
{
	MainGameScene* parent = (MainGameScene*) this->getParent();
	parent->cancelCallback();
	this->removeFromParent();
}

void LoseDialog::OkCallBack( CCObject* pSender )
{
	MainGameScene* parent = (MainGameScene*) this->getParent();
	parent->okCallback();
	this->removeFromParent();
}
