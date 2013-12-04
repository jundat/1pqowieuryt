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
	void SetEnableBackground(bool b);
	void SetEnableEffect(bool b);
	void LoadBackground(char* path);
	void PlayBackground(char* path, bool loop = true);
	void PauseBackground();
	void StopBackground();
	void PlayEffect(char *path,  bool isLoop = false);

private:
	AudioManager();

	bool m_bEnableBackground;
	bool m_bEnableEffect;
};

#endif /* AUDIOMANAGER_H_ */
