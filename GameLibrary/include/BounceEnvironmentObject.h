#pragma once
#include "GameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "BounceComponent.h"

class BounceEnvironmentObject : public GameObject
{
public:
	BounceEnvironmentObject()
	{
		addComponent(new TransformComponent);
		addComponent(new ModelComponent);
		addComponent(new BounceComponent(getComponent<TransformComponent>(), getComponent<TransformComponent>()->position().y, 0.03f, 0.15f, 0.1f));
	}

	~BounceEnvironmentObject() { }

	void OnUpdate(float dt) override
	{
		getComponent<BounceComponent>()->OnUpdate(dt);
	}

	void OnMessage(const std::string m) override { }
};