#include "WaitForLifeDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"

USING_NS_CC;

bool WaitForLifeDialog::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCPoint pexit = ccp(250, G_DESIGN_HEIGHT - 760);
	CCPoint pask = ccp(G_DESIGN_WIDTH-250, G_DESIGN_HEIGHT - 760);
	float textScale = 0.6f;

	CCSprite* bg = CCSprite::create("dialog.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg, -2);

	CCMenuItemImage* exitButton = CCMenuItemImage::create(
		"exit_button.png",
		"exit_button_press.png",
		this,
		menu_selector(WaitForLifeDialog::exitCallback));
	exitButton->setScale(textScale);
	exitButton->setPosition(pexit);


	CCMenuItemImage* askButton = CCMenuItemImage::create(
		"ask_button.png",
		"ask_button_press.png",
		this,
		menu_selector(WaitForLifeDialog::askFriendCallback));
	askButton->setScale(textScale);
	askButton->setPosition(pask);

	CCMenu* menu = CCMenu::create(exitButton, askButton, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	CCLabelTTF* labelTitle = CCLabelTTF::create("Không đủ số mạng\nHãy chờ thêm", "Marker Felt.ttf", 64);
	labelTitle->setFontFillColor(ccc3(56, 56, 56));
	labelTitle->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 120));
	this->addChild(labelTitle);

// 	CCLabelTTF* lbTitle = CCLabelTTF::create("Không đủ số mạng\nHãy chờ thêm", "Marker Felt", 48);
// 	lbTitle->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
// 	lbTitle->setHorizontalAlignment(kCCTextAlignmentCenter);
// 	lbTitle->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 120));
// 	lbTitle->setFontFillColor(ccc3(56, 56, 56));
// 	this->addChild(lbTitle);


	CCString* s = CCString::createWithFormat("%d s", m_waitTime);
	CCLabelBMFont* lbScore = CCLabelBMFont::create(s->getCString(), "Mia_64.fnt");
	lbScore->setAlignment(kCCTextAlignmentCenter);
	lbScore->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	lbScore->setColor(ccc3(56, 56, 56));
	this->addChild(lbScore);

    return true;
}

void WaitForLifeDialog::exitCallback( CCObject* pSender )
{
	MenuScene* parent = (MenuScene*)this->getParent();
	parent->setTouchEnabled(true);
	parent->onCloseDialog();
	this->removeFromParent();
}

void WaitForLifeDialog::askFriendCallback( CCObject* pSender )
{
	MenuScene* parent = (MenuScene*)this->getParent();
	parent->setTouchEnabled(true);
	parent->onCloseDialog();
	this->removeFromParent();
}



