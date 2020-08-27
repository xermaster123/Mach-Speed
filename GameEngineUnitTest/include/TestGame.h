#pragma once
#include "Game.h"

class TestGame : public Game
{
public:
	TestGame();
	~TestGame();

	void update(float dt) override;
	void render() override;
};

