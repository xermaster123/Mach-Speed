#pragma once
#include "Scene.h"

class EditorScene : public Scene
{
public:
	~EditorScene() override;

	void update(float dt) override;
	void render() override;
};
