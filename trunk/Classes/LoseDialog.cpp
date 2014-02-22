#include "LoseDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "AudioManager.h"
#include "DataManager.h"

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
	bg->setPosition(ccp(400, 640));
	this->addChild(bg, -2);
	
	CCMenuItemImage* exitButton = CCMenuItemImage::create(
		"exit_button.png",
		"exit_button_press.png",
		this,
		menu_selector(LoseDialog::menuCallBack));

	exitButton->setScale(textScale);
	exitButton->setPosition(pcenter);
	
	//REVIVE /////////////////////////////////////////////////////////////////
	//revive_button_press.png


	//CCDrawingPrimitives
	//////////////////////////////////////////////////////////////////////////

	CCMenu* menu = CCMenu::create(exitButton, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

	CCLabelTTF* labelTitle = CCLabelTTF::create("Điểm", "Roboto-Medium.ttf", 64);
	labelTitle->setFontFillColor(ccc3(56, 56, 56));
	labelTitle->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 120));
	this->addChild(labelTitle);

	CCString* s = CCString::createWithFormat("%d", m_score);
	CCLabelTTF* lbScore = CCLabelTTF::create(s->getCString(), "Roboto-Medium.ttf", 64);
	lbScore->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 30));
	lbScore->setColor(ccc3(56, 56, 56));
	this->addChild(lbScore);

	//BREAK THE RECORD ///////////////////////////////////////////////////////
	int curScore = DataManager::sharedDataManager()->GetHighScore();
	if (m_score > curScore)
	{
		PLAY_GET_BOMB_EFFECT;

		CCSprite* sprBreakRecord = CCSprite::create("break_record.png");
		sprBreakRecord->setPosition(ccp(174, 1280-515));
		sprBreakRecord->setScale(0.01f);
		this->addChild(sprBreakRecord);

		sprBreakRecord->runAction(
			CCSequence::createWithTwoActions(
				CCDelayTime::create(0.3f),
				CCScaleTo::create(0.3f, 1.0f))
			);
	}

	//////////////////////////////////////////////////////////////////////////

    return true;
}

void LoseDialog::menuCallBack( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;

	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

