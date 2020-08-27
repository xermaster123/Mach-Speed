#pragma once
#include "Game.h"
#include "AppLog.h"

class LevelEditor : public Game
{
public:
	LevelEditor();
	~LevelEditor();
	
	void update(float dt) override;
	void render() override;
	void setupScene(std::string filePath, bool newScene = false);

	bool loadLevel(std::string levelFile, Scene* scene);
	bool saveLevel(std::string saveLocation);
	void loadModel(std::string modelPath, Scene* scene);
	void removeLevel();

private:
	int m_currentLevel = 0;
	float spacing = 5.f;
	bool no_close = false;
	AppLog log = AppLog();
	string openLevel = "";
	GameObject* selectedObject = nullptr;
	int selectedObjectIndex = -1;

	//void setObjectProperties(GameObject & object, ModelProperties properties, bool pushBack = true) const;
	static void ShowHelpMarker(const char* desc);

	bool show_settings;
	bool show_about;

	void ShowSettingsWindow();
};
