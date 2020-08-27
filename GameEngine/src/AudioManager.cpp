#include "AudioManager.h"
#include <fstream>

AudioManager::AudioManager()
{
	FMOD::System_Create(&system);
	system->init(32, FMOD_INIT_NORMAL, extradriverdata);

	// load in sounds
	setupAudio("assets/audio/raceloop.mp3", true);
	setupAudio("assets/audio/button2.mp3");
	setupAudio("assets/audio/cannon.wav");
	setupAudio("assets/audio/coin.wav");
	setupAudio("assets/audio/death.mp3");

	playSound(0);
}

AudioManager::~AudioManager()
{
	for (size_t i = 0; i < audioFiles.size(); i++)
	{
		audioFiles[i].sound->release();
		//audioFiles[i].channel->release();
	}

	system->close();
	system->release();
}

void AudioManager::setupAudio(const char * path, bool loop)
{
	AudioInformation info = AudioInformation();

	system->createSound(path, FMOD_DEFAULT, 0, &info.sound);

	if (loop)
	{
		info.sound->setMode(FMOD_LOOP_NORMAL);
	}

	audioFiles.push_back(info);
}

void AudioManager::playSound(int number)
{
	system->playSound(audioFiles[number].sound, 0, false, &audioFiles[number].channel);
}

void AudioManager::update()
{
	system->update();
}