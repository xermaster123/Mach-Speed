#pragma once
#include "IEngineCore.h"
#include "Scene.h"

class Game
{
public:
	virtual ~Game() {}

	std::vector<InputHandler> m_inputHandler;
	Scene* m_currentScene;

	virtual void update(float dt) = 0;
	virtual void render() = 0;

	int m_windowWidth = 0;
	int m_windowHeight = 0;
	
};
