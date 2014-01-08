#include "AppDelegate.h"
#include "Global.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "DataManager.h"
#include "ConfigLoader.h"
#include "LevelLoader.h"
#include "AudioManager.h"
#include "GameClient.h"
#include <time.h>


USING_NS_CC;
using namespace CocosDenshion;


AppDelegate::AppDelegate() 
{

}

AppDelegate::~AppDelegate() 
{
}


bool AppDelegate::applicationDidFinishLaunching() {
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	
#ifdef WIN32
	pEGLView->setFrameSize(G_DESIGN_WIDTH, G_DESIGN_HEIGHT);
	pEGLView->setFrameZoomFactor(G_SCALE_FACTOR);
#endif
	
    pDirector->setOpenGLView(pEGLView);
    pDirector->setAnimationInterval(1.0 / 60);

	pEGLView->setDesignResolutionSize(G_DESIGN_WIDTH, G_DESIGN_HEIGHT, kResolutionExactFit);

    CCScene *pScene = MenuScene::scene(); //  MainGameScene::scene(); //
    pDirector->runWithScene(pScene);

	//////////////////////////////////////////////////////////////////////////

	ConfigLoader::shareConfigLoader();
	LevelLoader::shareLevelLoader();
	GameClient::sharedGameClient()->startGame();

	//////////////////////////////////////////////////////////////////////////

    return true;
}


void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}


void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
