#include "Global.h"
#include "MenuScene.h"
#include "IntroScene.h"
#include "DataManager.h"
#include "ChooseLanguageDialog.h"

USING_NS_CC;


CCScene* IntroScene::scene()
{
    CCScene *scene = CCScene::create();
    IntroScene *layer = IntroScene::create();
    scene->addChild(layer);
    return scene;
}

bool IntroScene::init()
{
    if ( !CCLayerColor::initWithColor(G_CCWHITE4) )
    {
        return false;
    }

	CCSprite* sprLogo = CCSprite::create(G_LOGO);
	sprLogo->setPosition(G_LOGO_POSITION);
	sprLogo->runAction(CCSequence::create(
		CCFadeIn::create(G_INTRO_FADE_IN_TIME),
		CCDelayTime::create(G_INTRO_DELAY_TIME),
		CCCallFunc::create(this, callfunc_selector(IntroScene::menuCallback)),
		NULL));
	this->addChild(sprLogo);

    return true;
}

void IntroScene::menuCallback()
{
	bool isFirstTimeChooseLanguage = DataManager::sharedDataManager()->GetFirstTimeChooseLanguage();

	if (isFirstTimeChooseLanguage)
	{
		CCScene *pScene = CCTransitionFade::create(G_INTRO_TRANSITION_MENU_TIME, ChooseLanguageDialog::scene(), ccWHITE);
		CCDirector::sharedDirector()->replaceScene(pScene);
	} 
	else
	{
		CCScene *pScene = CCTransitionFade::create(G_INTRO_TRANSITION_MENU_TIME, MenuScene::scene(), ccWHITE);
		CCDirector::sharedDirector()->replaceScene(pScene);
	}
}
