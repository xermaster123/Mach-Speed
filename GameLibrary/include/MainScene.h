#pragma once
#include "Scene.h"
#include "LevelGateObject.h"
#include "AudioManager.h"
#include "AmmoObject.h"

class MainScene : public Scene
{
public:
	~MainScene() override;

	void update(float dt) override;
	void render() override;

	std::vector<std::shared_ptr<AmmoObject>> v_ammoObjects;
	AudioManager aManager = AudioManager();
};
