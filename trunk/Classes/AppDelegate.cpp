#include "AppDelegate.h"
#include "Global.h"
#include "MainGameScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "DataManager.h"
#include "ConfigLoader.h"
#include "LevelLoader.h"
#include "AudioManager.h"
#include "IntroScene.h"
#include <time.h>


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "EziSocialObject.h"
#include "EziSocialDelegate.h"
#include "EziFacebookFriend.h"
#endif



USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() 
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {

	ConfigLoader::shareConfigLoader();
	LevelLoader::shareLevelLoader();


	//check if first time install
	DataManager* DM = DataManager::sharedDataManager();
	int isInstall = DM->GetValueFromKey("IS_INSTALL");
	if (isInstall == 0)
	{
		//first install
		DM->SetValueFromKey("IS_INSTALL", 1);

		//save some value
		DM->SetLastPlayerLife(G_MAX_PLAYER_LIFE);
		DM->SetBoom(G_DEFAULT_BOOM);
	}	


    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	
#ifdef WIN32
	pEGLView->setFrameSize(G_DESIGN_WIDTH, G_DESIGN_HEIGHT);
	pEGLView->setFrameZoomFactor(G_SCALE_FACTOR);
#endif
	
    pDirector->setOpenGLView(pEGLView);
    pDirector->setAnimationInterval(1.0 / 60);
	
	CCSize realSize = pEGLView->getFrameSize();
	
	if (realSize.width < 320) //mini device
	{
		pEGLView->setDesignResolutionSize(G_DESIGN_WIDTH, G_DESIGN_HEIGHT, kResolutionExactFit);
	} 
	else
	{
		pEGLView->setDesignResolutionSize(G_DESIGN_WIDTH, G_DESIGN_HEIGHT, kResolutionShowAll);
	}
	
	//pDirector->setDisplayStats(true);


	AudioManager::sharedAudioManager()->SetVolumeMusic(0.25f);
	AudioManager::sharedAudioManager()->SetVolumeFX(0.25f);


    CCScene *pScene = MenuScene::scene(); //  MenuScene::scene(); //  MainGameScene::scene(); //
	pDirector->runWithScene(pScene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(DataManager::sharedDataManager()->GetProfileID() != std::string("NULL"))
	{
		CCLOG("SET AUTO CHECK INCOMING REQUEST");
		EziSocialObject::sharedObject()->setAutoCheckIncomingRequestsOnAppLaunch(true);
	}
#endif

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
