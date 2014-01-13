/*
 * AudioManager.h
 *
 *  Created on: 20 Oct 2012
 *      Author: Loc
 */

#ifndef AUDIOMANAGER_H_
#define AUDIOMANAGER_H_

#include "SimpleAudioEngine.h"
#include "cocos2d.h"

class AudioManager
{
public:
	static AudioManager* sharedAudioManager();
	bool IsEnableBackground();
	bool IsEnableEffect();
	bool IsPlayingBackground();
	void SetEnableBackground(bool b);
	void SetEnableEffect(bool b);

	float GetVolumeMusic();
	void SetVolumeMusic(float _value);
	float GetVolumeFX();
	void SetVolumeFX(float _value);

	void LoadBackground(const char* path);
	void PlayBackground(const char* path, bool isPlayAgain = false, bool loop = true);
	void PauseBackground();
	void StopBackground();
	void PlayEffect(const char *path,  bool isLoop = false);

private:
	AudioManager();

	bool m_isPlayingBackground;
	bool m_bEnableBackground;
	bool m_bEnableEffect;
};

#endif /* AUDIOMANAGER_H_ */
