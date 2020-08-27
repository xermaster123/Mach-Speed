#pragma once
#include "Scene.h"

class TestScene : public Scene
{
public:
	TestScene() { }
	~TestScene() { }

	void update(float dt) override { }
	void render() override { }
};
