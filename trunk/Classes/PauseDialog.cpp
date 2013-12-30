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

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////

	CCPoint pcenter = ccp(400, G_DESIGN_HEIGHT-783);
	CCPoint pcancel = ccp(259, G_DESIGN_HEIGHT-783);
	CCPoint pok = ccp(541, G_DESIGN_HEIGHT-783);
	float textScale = 0.6f;

	CCSprite* bg = CCSprite::create("dialog.png");
	bg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
	this->addChild(bg, -2);

	CCMenuItemImage* cancelButton = CCMenuItemImage::create(
		"button.png",
		"buttonPress.png",
		this,
		menu_selector(PauseDialog::menuCallback));

	cancelButton->setScale(0.6f);
	cancelButton->setPosition(pcancel);

	CCMenuItemImage* okButton = CCMenuItemImage::create(
		"button.png",
		"buttonPress.png",
		this,
		menu_selector(PauseDialog::resumeCallBack));
	okButton->setScale(0.6f);
	okButton->setPosition(pok);

	CCMenu* menu = CCMenu::create(cancelButton, okButton, NULL);
	menu->setPosition(origin);
	this->addChild(menu);

	CCLabelBMFont* cancel = CCLabelBMFont::create("MENU", "Mia_64.fnt");
	cancel->setScale(textScale);
	cancel->setPosition(pcancel);
	this->addChild(cancel);

	CCLabelBMFont* ok = CCLabelBMFont::create("RESUME", "Mia_64.fnt");
	ok->setScale(textScale);
	ok->setPosition(pok);
	this->addChild(ok);
	
	CCLabelBMFont* msg = CCLabelBMFont::create("PAUSE", "Mia_64.fnt");
	msg->setPosition(ccp(visibleSize.width/2, visibleSize.height/2 + 100));
	this->addChild(msg);

	return true;
}

void PauseDialog::menuCallback( CCObject* pSender )
{
	CCScene *pScene = MenuScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void PauseDialog::resumeCallBack( CCObject* pSender )
{
	MainGameScene* parent = (MainGameScene*) this->getParent();
	parent->resumeCallback();
	this->removeFromParent();
}
