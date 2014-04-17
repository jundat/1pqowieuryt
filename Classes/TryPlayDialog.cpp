#include "TryPlayDialog.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "DataManager.h"
#include "TextLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool TryPlayDialog::init()
{
	if ( !CCLayerColor::initWithColor(G_DIM_COLOR) )
	{
		return false;
	}
    
	CCPoint pexit = ccp(250, G_DESIGN_HEIGHT - 810);
	CCPoint pask = ccp(G_DESIGN_WIDTH-250, G_DESIGN_HEIGHT - 810);
    
	CCScale9Sprite* dialog = CCScale9Sprite::create("dialog.png");
	dialog->setPosition(ccp(400, 640));
	dialog->setContentSize(CCSizeMake(680, 480));
	this->addChild(dialog);
    
	MY_CREATE_MENU_BUTTON(exitButton, "button.png", "button_down.png", "Log In",
                          "Roboto-Medium.ttf", 48, ccBLACK, pexit, this, TryPlayDialog::yesCallback);
    
	MY_CREATE_MENU_BUTTON(askButton, "button.png", "button_down.png", "Try",
                          "Roboto-Medium.ttf", 48, ccBLACK, pask, this, TryPlayDialog::noCallback);
	
	CCMenu* menu = CCMenu::create(exitButton, askButton, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);
    
	CCLabelTTF* labelTitle = CCLabelTTF::create("Connect Facebook to get more friends, more gifts, ...",
                                                "Roboto-Medium.ttf",
                                                48,
                                                CCSizeMake(600, 0),
                                                kCCTextAlignmentCenter,
                                                kCCVerticalTextAlignmentCenter);
	labelTitle->setColor(ccBLACK);
	labelTitle->setPosition(ccp(400, 700));
	this->addChild(labelTitle);
    
	this->setKeypadEnabled(true);
    return true;
    
    return true;
}

void TryPlayDialog::yesCallback( CCObject* pSender )
{
	MenuScene* menu = (MenuScene*) this->getParent();
    menu->facebookLogInOut();
    
    menu->onCloseDialog();
    this->removeFromParent();
}


void TryPlayDialog::noCallback( CCObject* pSender )
{
	GameClientManager::sharedGameClientManager()->setDelegate(NULL);
    
    CCScene *pScene = CCTransitionFade::create(0.5, MainGameScene::scene());
    CCDirector::sharedDirector()->replaceScene(pScene);
}

