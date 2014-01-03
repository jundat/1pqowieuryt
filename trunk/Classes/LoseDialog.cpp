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

	CCPoint pcenter = ccp(400, G_DESIGN_HEIGHT-783);
	CCPoint pcancel = ccp(259, G_DESIGN_HEIGHT-783);
	CCPoint pok = ccp(541, G_DESIGN_HEIGHT-783);
	float textScale = 0.6f;

	CCSprite* bg = CCSprite::create("dialog.png");
	bg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
	this->addChild(bg, -2);
	
	if (m_canBeRevived)
	{
		CCMenuItemImage* cancelButton = CCMenuItemImage::create(
			"button.png",
			"buttonPress.png",
			this,
			menu_selector(LoseDialog::CancelCallBack));

		cancelButton->setScale(0.6f);
		cancelButton->setPosition(pcancel);

		CCMenuItemImage* okButton = CCMenuItemImage::create(
			"button.png",
			"buttonPress.png",
			this,
			menu_selector(LoseDialog::OkCallBack));
		okButton->setScale(0.6f);
		okButton->setPosition(pok);

		CCMenu* menu = CCMenu::create(cancelButton, okButton, NULL);
		menu->setPosition(origin);
		this->addChild(menu);
		
		CCLabelBMFont* cancel = CCLabelBMFont::create("Again", "Mia_64.fnt");
		cancel->setScale(textScale);
		cancel->setPosition(pcancel);
		this->addChild(cancel);

		CCLabelBMFont* ok = CCLabelBMFont::create("Revive", "Mia_64.fnt");
		ok->setScale(textScale);
		ok->setPosition(pok);
		this->addChild(ok);
	} 
	else
	{
		CCMenuItemImage* cancelButton = CCMenuItemImage::create(
			"button.png",
			"buttonPress.png",
			this,
			menu_selector(LoseDialog::CancelCallBack));

		cancelButton->setScale(0.6f);
		cancelButton->setPosition(pcenter);

		CCMenu* menu = CCMenu::create(cancelButton, NULL);
		menu->setPosition(origin);
		this->addChild(menu);

		CCLabelBMFont* cancel = CCLabelBMFont::create("Again", "Mia_64.fnt");
		cancel->setScale(textScale);
		cancel->setPosition(pcenter);
		this->addChild(cancel);
	}
	
	CCLabelBMFont* lbTitle = CCLabelBMFont::create("You lose !", "Mia_64.fnt");
	lbTitle->setPosition(ccp(visibleSize.width/2, visibleSize.height/2 + 150));
	this->addChild(lbTitle);

	CCLabelBMFont* lbMsg = CCLabelBMFont::create("Score", "Mia_64.fnt");
	lbMsg->setPosition(ccp(visibleSize.width/2, visibleSize.height/2 + 50));
	this->addChild(lbMsg);

	CCString* s = CCString::createWithFormat("%d", m_score);
	CCLabelBMFont* lbScore = CCLabelBMFont::create(s->getCString(), "Mia_64.fnt");
	lbScore->setPosition(ccp(visibleSize.width/2, visibleSize.height/2 - 50));
	this->addChild(lbScore);

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
