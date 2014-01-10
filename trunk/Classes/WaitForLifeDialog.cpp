#include "WaitForLifeDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "DataManager.h"
#include <time.h>

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

	CCLabelTTF* labelTitle = CCLabelTTF::create("Không đủ số mạng\nHãy chờ thêm", "Marker Felt.ttf", 48);
	labelTitle->setFontFillColor(ccc3(56, 56, 56));
	labelTitle->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2 + 120));
	this->addChild(labelTitle);

	int mins = (int)m_waitTime/60;
	int seconds = (int)m_waitTime%60;

	CCString* s = CCString::createWithFormat("%d phút %d giây", mins, seconds);
	m_lbTime = CCLabelTTF::create(s->getCString(), "Marker Felt", 48);
	m_lbTime->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	m_lbTime->setColor(ccc3(56, 56, 56));
	this->addChild(m_lbTime);

	this->schedule(schedule_selector(WaitForLifeDialog::ScheduleTick), 1);

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
	CCMessageBox("I\'ll give you 5 lifes, ok?", "Info");
	DataManager::sharedDataManager()->SetLastPlayerLife(5);

	MenuScene* parent = (MenuScene*)this->getParent();
	parent->setTouchEnabled(true);
	parent->onCloseDialog();
	this->removeFromParent();
}

void WaitForLifeDialog::ScheduleTick( float dt )
{
	m_waitTime -= dt;

	if (m_waitTime < 0)
	{
		this->unschedule(schedule_selector(WaitForLifeDialog::ScheduleTick));
		MenuScene* parent = (MenuScene*)this->getParent();
		parent->onCompletedWaiting();
	}
	else
	{
		int mins = (int)m_waitTime/60;
		int seconds = (int)m_waitTime % 60;

		CCString* s = CCString::createWithFormat("%d phút %d giây", mins, seconds);
		m_lbTime->setString(s->getCString());
	}
}



