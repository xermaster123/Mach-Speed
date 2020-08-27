#pragma once
#include "GameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "ParticleEmitterComponent.h"
#include "RotateComponent.h"
#include "GateComponent.h"

class LevelGateObject : public GameObject
{
public:
	LevelGateObject(std::string level, float collide, float score)
	{
		addComponent(new TransformComponent);
		addComponent(new ModelComponent);
		addComponent(new GateComponent(level, collide, score));
		addComponent(new RotateComponent(getComponent<TransformComponent>(), 1.0f, glm::vec3(0, 0, 1)));
	}

	~LevelGateObject() { }

	void OnUpdate(float dt) override
	{
		getComponent<RotateComponent>()->OnUpdate(dt);
	}
	void OnMessage(const std::string m) override { }
};
