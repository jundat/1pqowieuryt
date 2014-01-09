#include "LoseDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"

USING_NS_CC;

bool LoseDialog::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCPoint pcenter = ccp(400, G_DESIGN_HEIGHT - 760);
	float textScale = 0.8f;

	CCSprite* bg = CCSprite::create("dialog.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg, -2);

	CCMenuItemImage* exitButton = CCMenuItemImage::create(
		"exit_button.png",
		"exit_button_press.png",
		this,
		menu_selector(LoseDialog::menuCallBack));

	exitButton->setScale(textScale);
	exitButton->setPosition(pcenter);

	CCMenu* menu = CCMenu::create(exitButton, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	CCString* s = CCString::createWithFormat("%d", m_score);
	CCLabelBMFont* lbScore = CCLabelBMFont::create(s->getCString(), "Mia_64.fnt");
	lbScore->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 30));
	this->addChild(lbScore);

    return true;
}

void LoseDialog::menuCallBack( CCObject* pSender )
{
	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

