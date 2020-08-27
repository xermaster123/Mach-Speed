#pragma once
#include "fmod\fmod.hpp"
#include"fmod\common.h"
#include <map>
#include <Game.h>

class AudioInformation
{
public:
	FMOD::Sound *sound;
	FMOD::Channel *channel;
};

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void setupAudio(const char * path, bool loop = false);
	void playSound(int number);
	void update();

private:
	std::vector<AudioInformation> audioFiles;

	FMOD::System		*system;
	FMOD::Sound			*sound1, *sound2, *sound3, *sound4, *sound5;
	FMOD::Channel		*channel = 0;
	void				*extradriverdata = 0;
};

