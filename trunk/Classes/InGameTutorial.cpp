#include "InGameTutorial.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "cocos-ext.h"
#include "GameClientObjects.h"
#include "BreakLeaderboardDialog.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool InGameTutorial::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	CCPoint pExit = ccp(400, 1280-813); // ccp(233, 1280-813);

	float BUTTON_SCALE = 0.6722f;

	CCScale9Sprite* bg = CCScale9Sprite::create("dialog.png");
	bg->setPosition(ccp(400, 640));
	bg->setContentSize(CCSizeMake(680, 480));
	this->addChild(bg, -2);
	
	m_itExitButton = CCMenuItemImage::create(
		"resume.png",
		"resume1.png",
		this,
		menu_selector(InGameTutorial::menuCallBack));
	m_itExitButton->setScale(BUTTON_SCALE);
	m_itExitButton->setPosition(pExit);
	

	CCMenu* menu = CCMenu::create(m_itExitButton, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

// 	CCLabelTTF* labelTitle = CCLabelTTF::create("Hướng dẫn", "Roboto-Medium.ttf", 64);
// 	labelTitle->setFontFillColor(ccc3(56, 56, 56));
// 	labelTitle->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 200)); //+40
// 	this->addChild(labelTitle);

	CCLabelTTF* lbScore = CCLabelTTF::create("B52 đã xuất hiện\nHãy tiêu diệt B52\nĐừng để lọt quá 3 B52!", "Roboto-Medium.ttf", 48);
	lbScore->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 50));
	lbScore->setColor(ccc3(56, 56, 56));
	this->addChild(lbScore);

    return true;
}

void InGameTutorial::menuCallBack( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;
	ObjectLayer* maingame = (ObjectLayer*) this->getParent();
	maingame->Resume();

	//remove dialog after animation
	this->removeFromParent();
}
