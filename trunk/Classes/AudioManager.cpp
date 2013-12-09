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
	
}

AudioManager* AudioManager::sharedAudioManager()
{
	if (m_Instance == 0)
		m_Instance = new AudioManager();
	return m_Instance;

}

bool AudioManager::IsEnableBackground()
{
	return m_bEnableBackground;
}


bool AudioManager::IsEnableEffect()
{
	return m_bEnableEffect;
}


void AudioManager::SetEnableBackground(bool b)
{
	m_bEnableBackground = b;
}


void AudioManager::SetEnableEffect(bool b)
{
	m_bEnableEffect = b;
}


void AudioManager::LoadBackground(const char* path)
{
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(path).c_str());
}


void AudioManager::PlayBackground(const char* path, bool loop)
{
	if (m_bEnableBackground)
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(path).c_str(), loop);
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
	if (m_bEnableEffect)
		SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(path).c_str(), isLoop);
}


