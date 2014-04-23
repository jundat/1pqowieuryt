#include "Global.h"
#include "MenuScene.h"
#include "IntroScene.h"
#include "DataManager.h"
#include "ChooseLanguageDialog.h"
#include "NotLoggedInMenuScene.h"

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
    
    m_isLoggedIn = false;
    string fbId = DataManager::sharedDataManager()->GetFbID();
    if (fbId.compare("NULL") == 0) {
        m_isLoggedIn = false;
    } else {
        m_isLoggedIn = true;
    }

	m_sprLogo = CCSprite::create(G_LOGO);
	m_sprLogo->setPosition(G_LOGO_POSITION);
    m_sprLogo->setOpacity(0);
	this->addChild(m_sprLogo);
    

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
        CCScene *pScene;
        //if (this->m_isLoggedIn) {
        //    pScene = CCTransitionFade::create(0.5, MenuScene::scene());
        //} else {
            pScene = CCTransitionFade::create(0.5, NotLoggedInMenuScene::scene());
        //}
        CCDirector::sharedDirector()->replaceScene(pScene);
	}
}


void IntroScene::onEnterTransitionDidFinish()
{
	m_sprLogo->runAction(CCSequence::create(
        CCFadeIn::create(G_INTRO_FADE_IN_TIME),
        CCDelayTime::create(G_INTRO_DELAY_TIME),
        CCCallFunc::create(this, callfunc_selector(IntroScene::menuCallback)),
        NULL));
}

