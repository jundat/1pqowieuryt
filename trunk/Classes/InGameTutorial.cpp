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
	
	CCPoint pExit = ccp(400, 1280-813-60); // ccp(233, 1280-813);

	float BUTTON_SCALE = 0.6722f;

	CCScale9Sprite* bg = CCScale9Sprite::create("dialog.png");
	bg->setPosition(ccp(400, 640));
	bg->setContentSize(CCSizeMake(680, 600));
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

	string msg = "Quân địch đang tràn đến, những chiếc B52 sẽ ném bom phá hủy thành phố của ta.\
Hãy tiêu diệt chúng, đừng để quá 3 chiếc B52 vượt qua được phòng tuyến!";

	CCLabelTTF* lbMsg = CCLabelTTF::create(msg.c_str(), 
		"Roboto-Medium.ttf", 
		48, 
		CCSizeMake(9.0f * bg->getContentSize().width / 10.0f, 0), 
		kCCTextAlignmentCenter, 
		kCCVerticalTextAlignmentCenter);
	lbMsg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 50));
	lbMsg->setColor(ccc3(56, 56, 56));
	this->addChild(lbMsg);

    return true;
}

void InGameTutorial::menuCallBack( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;
	MainGameScene* maingame = (MainGameScene*) this->getParent();
	maingame->resumeCallback();
	//maingame->setKeypadEnabled(true);

	//remove dialog after animation
	this->removeFromParent();
}
