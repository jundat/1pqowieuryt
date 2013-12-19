#include "AppDelegate.h"
#include "Global.h"
#include "MainGameScene.h"
#include "MenuScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

#ifdef WIN32
	pEGLView->setFrameSize(G_DESIGN_WIDTH, G_DESIGN_HEIGHT);
	pEGLView->setFrameZoomFactor(G_SCALE_FACTOR);
#endif

    pDirector->setOpenGLView(pEGLView);

    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	pEGLView->setDesignResolutionSize(800, 1280, kResolutionExactFit);

    // create a scene. it's an autorelease object
    CCScene *pScene = MenuScene::scene(); // MainGameScene::scene(); // 
	
    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
