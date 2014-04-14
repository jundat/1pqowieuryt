#include "InGameTutorial.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "cocos-ext.h"
#include "GameClientObjects.h"
#include "BreakLeaderboardDialog.h"
#include "TextLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool InGameTutorial::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(0,0,0,255)) )
    {
        return false;
    }
	
	CCPoint pExit = ccp(400, 1280-1176); // ccp(233, 1280-813);

	float BUTTON_SCALE = 0.6722f;

	CCSprite* bg = CCSprite::create("intro.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg);

	//main content
	string content = string(TXT("tut_content"));

	CCLabelTTF* lbContent = CCLabelTTF::create(content.c_str(), 
		"Roboto-Medium.ttf", 
		36, 
		CCSizeMake(680, 0), 
		kCCTextAlignmentCenter, 
		kCCVerticalTextAlignmentCenter);
	lbContent->setPosition(ccp(400, 650));
	lbContent->setColor(ccc3(120, 120, 120));
	bg->addChild(lbContent);

	//misson
	string msg = string(TXT("tut_mission"));

	CCLabelTTF* lbMsg = CCLabelTTF::create(msg.c_str(), 
		"Roboto-Medium.ttf", 
		32, 
		CCSizeMake(200, 0), 
		kCCTextAlignmentCenter, 
		kCCVerticalTextAlignmentCenter);
	lbMsg->setPosition(ccp(395, 1280-931));
	lbMsg->setColor(ccc3(0,0,0));
	bg->addChild(lbMsg);

	MY_CREATE_MENU_BUTTON(exitButton, "button.png", "button_down.png", TXT("btn_start"), 
		"Roboto-Medium.ttf", 48, ccBLACK, pExit, this, InGameTutorial::menuCallBack);
	m_itExitButton = exitButton;
		
	CCMenu* menu = CCMenu::create(m_itExitButton, NULL);
	menu->setPosition(CCPointZero);
	bg->addChild(menu);

    return true;
}

void InGameTutorial::menuCallBack( CCObject* pSender )
{
	PLAY_BUTTON_EFFECT;
	MainGameScene* maingame = (MainGameScene*) this->getParent();
	maingame->resumeCallback();
	this->removeFromParent();
}
