/*
 * AudioManager.cpp
 *
 *  Created on: 20 Oct 2012
 *      Author: Loc
 */

#include "cocos2d.h"
USING_NS_CC;
#include "AudioManager.h"
using namespace CocosDenshion;


static AudioManager *m_Instance;

AudioManager::AudioManager()
{
	m_isPlayingBackground = false;
	m_bEnableBackground = CCUserDefault::sharedUserDefault()->getBoolForKey("IS_ENABLE_SOUND_BACKGROUND", true);
	m_bEnableEffect = CCUserDefault::sharedUserDefault()->getBoolForKey("IS_ENABLE_SOUND_EFFECT", true);
}

AudioManager* AudioManager::sharedAudioManager()
{
	if (m_Instance == 0) {
		m_Instance = new AudioManager();
	}
	return m_Instance;

}

bool AudioManager::IsEnableBackground()
{
	m_bEnableBackground = CCUserDefault::sharedUserDefault()->getBoolForKey("IS_ENABLE_SOUND_BACKGROUND", true);
	return m_bEnableBackground;
}


bool AudioManager::IsEnableEffect()
{
	m_bEnableEffect = CCUserDefault::sharedUserDefault()->getBoolForKey("IS_ENABLE_SOUND_EFFECT", true);
	return m_bEnableEffect;
}


void AudioManager::SetEnableBackground(bool b)
{
	m_bEnableBackground = b;
	CCUserDefault::sharedUserDefault()->setBoolForKey("IS_ENABLE_SOUND_BACKGROUND", m_bEnableBackground);
	CCUserDefault::sharedUserDefault()->flush();
}


void AudioManager::SetEnableEffect(bool b)
{
	m_bEnableEffect = b;
	CCUserDefault::sharedUserDefault()->setBoolForKey("IS_ENABLE_SOUND_EFFECT", m_bEnableEffect);
	CCUserDefault::sharedUserDefault()->flush();
}


void AudioManager::LoadBackground(const char* path)
{
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(path).c_str());
}


void AudioManager::PlayBackground(const char* path, bool isPlayAgain, bool loop)
{
	if (m_bEnableBackground) {
		if (IsPlayingBackground())
		{
			if (isPlayAgain)
			{
				SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(path).c_str(), loop);
			}
		}
		else
		{
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(path).c_str(), loop);
		}
	}
}

void AudioManager::PauseBackground()
{
	if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
}

void AudioManager::StopBackground()
{
	if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(false);
	}
}

void AudioManager::PlayEffect(const char *path, bool isLoop)
{
	if (m_bEnableEffect) {
		SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(path).c_str(), isLoop);
	}
}

bool AudioManager::IsPlayingBackground()
{
	return SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
}

void AudioManager::SetVolumeMusic(float _value)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(_value);
	CCUserDefault::sharedUserDefault()->setFloatForKey("SOUND_BACKGROUND_VOLUME", _value);
	CCUserDefault::sharedUserDefault()->flush();
}

void AudioManager::SetVolumeFX(float _value)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(_value);
	CCUserDefault::sharedUserDefault()->setFloatForKey("SOUND_EFFECT_VOLUME", _value);
	CCUserDefault::sharedUserDefault()->flush();
}

