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
    
    CCScale9Sprite* sprNormal1 = CCScale9Sprite::create("button.png");
	sprNormal1->setContentSize(CCSizeMake(256, 256));
    
    CCScale9Sprite* sprSelect1 = CCScale9Sprite::create("button_down.png");
	sprSelect1->setContentSize(CCSizeMake(256, 256));
    
    CCMenuItemSprite* itLogIn = CCMenuItemSprite::create(sprNormal1, sprSelect1, this, menu_selector(TryPlayDialog::yesCallback));
    itLogIn->setPosition(ccp(230, 640));
    
    
    
    CCScale9Sprite* sprNormal2 = CCScale9Sprite::create("button.png");
	sprNormal2->setContentSize(CCSizeMake(256, 256));
    
    CCScale9Sprite* sprSelect2 = CCScale9Sprite::create("button_down.png");
	sprSelect2->setContentSize(CCSizeMake(256, 256));
    
    CCMenuItemSprite* itTry = CCMenuItemSprite::create(sprNormal2, sprSelect2, this, menu_selector(TryPlayDialog::noCallback));
    itTry->setPosition(ccp(570, 640));
    
    
    
    CCMenu* menu = CCMenu::create(itLogIn, itTry, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu);

    CCLabelTTF* lbLogIn = CCLabelTTF::create("Log In", G_FONT_NORMAL, 64);
    lbLogIn->setPosition(itLogIn->getPosition());
    lbLogIn->setColor(ccBLACK);
    this->addChild(lbLogIn);
    
    CCLabelTTF* lbTry = CCLabelTTF::create("Try", G_FONT_NORMAL, 64);
    lbTry->setPosition(itTry->getPosition());
    lbTry->setColor(ccBLACK);
    this->addChild(lbTry);
    
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

