#pragma once
#include "GameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "ParticleEmitterComponent.h"

class StaticEnvironmentObject : public GameObject
{
public:
	StaticEnvironmentObject()
	{
		addComponent(new TransformComponent);
		addComponent(new ModelComponent);
		addComponent(new ParticleEmitterComponent);
	}

	~StaticEnvironmentObject() { }

	void OnUpdate(float dt) override { }
	void OnMessage(const std::string m) override { }
};
