#include "LoseDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "AudioManager.h"

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

	CCLabelTTF* labelTitle = CCLabelTTF::create("Điểm", "Marker Felt.ttf", 64);
	labelTitle->setFontFillColor(ccc3(56, 56, 56));
	labelTitle->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 120));
	this->addChild(labelTitle);

	CCString* s = CCString::createWithFormat("%d", m_score);
	CCLabelTTF* lbScore = CCLabelTTF::create(s->getCString(), "Marker Felt.ttf", 64);
	lbScore->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 30));
	lbScore->setColor(ccc3(56, 56, 56));
	this->addChild(lbScore);

    return true;
}

void LoseDialog::menuCallBack( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

