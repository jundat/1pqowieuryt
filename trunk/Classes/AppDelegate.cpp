#include "AppDelegate.h"
#include "Global.h"
#include "MainGameScene.h"
#include "NotLoggedInMenuScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "DataManager.h"
#include "ConfigLoader.h"
#include "LevelLoader.h"
#include "TextLoader.h"
#include "AudioManager.h"
#include "IntroScene.h"
#include <time.h>
#include "TestPostGetScene.h"
#include "Base64.h"
#include "Md5.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
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
	TextLoader::shareTextLoader();

	string lang = DataManager::sharedDataManager()->GetLanguage();
	if (lang.compare("English") == 0)
	{
		TextLoader::shareTextLoader()->setCurrentLanguage(LANGUAGE_ENGLISH);
	}
	else
	{
		TextLoader::shareTextLoader()->setCurrentLanguage(LANGUAGE_VIETNAMESE);
	}
	
	GameClientManager::sharedGameClientManager()->setUrls(
		G_URL_PLAYER_FB_PROFILE,
		G_URL_DEVICE_PROFILE,
		G_URL_FRIEND_LIST,
		G_URL_SCORE
		);

	//check if first time install
	DataManager* DM = DataManager::sharedDataManager();
	int isInstall = DM->GetValueFromKey(G_CHECK_INSTALLED);
	if (isInstall != G_INSTALLED)
	{
		//first install
		DM->SetValueFromKey(G_CHECK_INSTALLED, G_INSTALLED);

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
    pDirector->setAnimationInterval(1.0f / 60.0f);
	//pDirector->setDisplayStats(true);
	
	CCSize realSize = pEGLView->getFrameSize();
	
	if (realSize.width < G_SMALL_DEVIVE_SIZE) //mini device
	{
		pEGLView->setDesignResolutionSize(G_DESIGN_WIDTH, G_DESIGN_HEIGHT, kResolutionExactFit);
	} 
	else
	{
		pEGLView->setDesignResolutionSize(G_DESIGN_WIDTH, G_DESIGN_HEIGHT, kResolutionShowAll);
	}
	
	AudioManager::sharedAudioManager()->SetVolumeMusic(G_DEFAULT_VOLUME);
	AudioManager::sharedAudioManager()->SetVolumeFX(G_DEFAULT_VOLUME);


    CCScene *pScene = TestPostGetScene::scene(); // NotLoggedInMenuScene::scene();// IntroScene::scene(); // MenuScene::scene();// IntroScene::scene(); // MenuScene::scene(); // IntroScene::scene(); //
	pDirector->runWithScene(pScene);

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
