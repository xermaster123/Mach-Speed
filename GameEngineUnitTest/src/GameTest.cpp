#include "stdafx.h"
#include "TestGame.h"
#include "TestScene.h"

TestGame::TestGame()
{
	m_currentScene = new TestScene();
}

TestGame::~TestGame()
{
}

void TestGame::update(float dt)
{
}

void TestGame::render()
{
}
