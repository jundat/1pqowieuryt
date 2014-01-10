﻿#include "MenuScene.h"
#include "SettingScene.h"
#include "AudioManager.h"
#include "Global.h"

USING_NS_CC;

CCScene* SettingScene::scene()
{
    CCScene *scene = CCScene::create();
    SettingScene *layer = SettingScene::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool SettingScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	this->setKeypadEnabled(true);

    /////////////////////////////

	CCSprite* bg = CCSprite::create("bg_stars.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg, 0);

	CCSprite* setting_top = CCSprite::create("setting_top.png");
	setting_top->setPosition(ccp(G_DESIGN_WIDTH/2, - 110 + G_DESIGN_HEIGHT));
	this->addChild(setting_top, 0);


	CCMenuItemImage *backItem = CCMenuItemImage::create(
		"back.png",
		"back1.png",
		this,
		menu_selector(SettingScene::menuCallback));
	backItem->setPosition(ccp(G_DESIGN_WIDTH/2, backItem->getContentSize().height - 3));


	CCMenuItem* soundOn = CCMenuItemImage::create("sound_on.png", NULL, NULL);
	CCMenuItem* soundOff = CCMenuItemImage::create("sound_off.png", NULL, NULL);
	soundToggle = CCMenuItemToggle::createWithTarget(this,  menu_selector(SettingScene::soundCallback), soundOn, soundOff, NULL);
	
	if(AudioManager::sharedAudioManager()->IsEnableBackground())
	{
		soundToggle->setSelectedIndex(0);
	}
	else
	{
		soundToggle->setSelectedIndex(1);
	}
	
	soundToggle->setPosition(ccp(400, 1280-384));

    CCMenu* pMenu = CCMenu::create(backItem, soundToggle, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

	//////////////////////////////////////////////////////////////////////////
	//VOLUME
	CCLabelTTF* labelMusic = CCLabelTTF::create("Nhạc nền", "Marker Felt.ttf", 64);
	labelMusic->setFontFillColor(ccc3(56, 56, 56));
	labelMusic->setPosition(ccp(400, 1280-546));
	this->addChild(labelMusic);
	sliderMusic=CCControlSlider::create(
		CCSprite::create("sliderbarBG.png"),
		CCSprite::create("sliderbar.png"),
		CCSprite::create("slidernode.png"));
	sliderMusic->setMinimumValue(0.0f);
	sliderMusic->setMaximumValue(1.0f);
	sliderMusic->setValue(1.0f);
	sliderMusic->setPosition(400, 1280-649);
	sliderMusic->setEnabled(true);
	sliderMusic->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingScene::volumeMusicCallBack), CCControlEventValueChanged);
	this->addChild(sliderMusic);

	CCLabelTTF* labelFX = CCLabelTTF::create("Nhạc nền", "Marker Felt.ttf", 64);
	labelFX->setFontFillColor(ccc3(56, 56, 56));
	labelFX->setPosition(ccp(400, 1280-802));
	this->addChild(labelFX);
	sliderFX=CCControlSlider::create(
		CCSprite::create("sliderbarBG.png"),
		CCSprite::create("sliderbar.png"),
		CCSprite::create("slidernode.png"));
	sliderFX->setValue(1.0f);
	sliderFX->setMinimumValue(0.0f);
	sliderFX->setMaximumValue(1.0f);
	sliderFX->setPosition(400, 1280-905);
	sliderFX->setEnabled(true);
	sliderFX->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingScene::volumeFXCallBack),CCControlEventValueChanged);
	this->addChild(sliderFX);
	//////////////////////////////////////////////////////////////////////////
	

    return true;
}

void SettingScene::menuCallback(CCObject* pSender)
{
	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void SettingScene::soundCallback( CCObject* pSender )
{
	if(AudioManager::sharedAudioManager()->IsEnableBackground())
	{
		AudioManager::sharedAudioManager()->SetEnableBackground(false);
		AudioManager::sharedAudioManager()->SetEnableEffect(false);

		//
		AudioManager::sharedAudioManager()->StopBackground();
	}
	else
	{
		AudioManager::sharedAudioManager()->SetEnableBackground(true);
		AudioManager::sharedAudioManager()->SetEnableEffect(true);

		//
		AudioManager::sharedAudioManager()->PlayBackground("background.ogg");
	}
}

void SettingScene::volumeMusicCallBack(CCObject* sender,CCControlEvent pEvent)
{
	AudioManager::sharedAudioManager()->SetVolumeMusic(sliderMusic->getValue());
}

void SettingScene::volumeFXCallBack(CCObject* sender,CCControlEvent pEvent)
{
	AudioManager::sharedAudioManager()->SetVolumeFX(sliderFX->getValue());
}

void SettingScene::keyBackClicked()
{
	menuCallback(NULL);
}
