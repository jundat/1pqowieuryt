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
	string content = "....1972 Cuộc tập kích của địch diễn ra liên tục nhiều ngày với trọng tâm là các cuộc ném bom của B-52 vào ban đêm....";

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
	string msg = "Không được để lọt lưới trên 3 chiếc B-52 !!!";

	CCLabelTTF* lbMsg = CCLabelTTF::create(msg.c_str(), 
		"Roboto-Medium.ttf", 
		32, 
		CCSizeMake(200, 0), 
		kCCTextAlignmentCenter, 
		kCCVerticalTextAlignmentCenter);
	lbMsg->setPosition(ccp(395, 1280-931));
	lbMsg->setColor(ccc3(0,0,0));
	bg->addChild(lbMsg);
	
	m_itExitButton = CCMenuItemImage::create(
		"catcanh.png",
		"catcanh1.png",
		this,
		menu_selector(InGameTutorial::menuCallBack));
	m_itExitButton->setScale(BUTTON_SCALE);
	m_itExitButton->setPosition(pExit);
	

	CCMenu* menu = CCMenu::create(m_itExitButton, NULL);
	menu->setPosition(CCPointZero);
	bg->addChild(menu);

	//////////////////////////////////////////////////////////////////////////
// 	bg->runAction(
// 		CCSequence::create(
// 		CCFadeIn::create(0.2f),
// 		CCDelayTime::create(0.2f),
// 		CCBlink::create(0.5f, 4),
// 		CCDelayTime::create(0.25f),
// 		CCBlink::create(0.6f, 6),
// 		CCDelayTime::create(0.1f),
// 		CCBlink::create(0.8f, 7),
// 		CCFadeOut::create(0.01f),
// 		CCDelayTime::create(2.0f),
// 		CCFadeIn::create(1.0f),
// 		NULL)
// 		);


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
