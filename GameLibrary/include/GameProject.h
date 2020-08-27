#pragma once
#include "Game.h"
#include "ModelProperties.h"
#include "fmod\fmod.hpp"
#include "fmod\common.h"

class GameProject : public Game
{
public:
	GameProject();
	~GameProject();

	void update(float dt) override;
	void render() override;
	void setupScene();

	bool loadLevel(std::string levelFile);
	void removeLevel();

private:
	int m_currentLevel = 0;

	// UI Stuff
	bool isMenuOpen = false;
	bool OptionsOpen = false;
	bool HelpOpen = false;
	bool* particlePtr;
	float Brightness = 50;
	float* BrightPtr = &Brightness;
	float MasterVolume = 50;
	float* mVolumePtr = &MasterVolume;
	float MusicVolume = 100;
	float* MusicVolumePtr = &MusicVolume;

	void setObjectProperties(std::shared_ptr<GameObject> object, ModelProperties properties, bool pushBack = true) const;
};