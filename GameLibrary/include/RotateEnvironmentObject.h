#pragma once
#include "GameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "RotateComponent.h"

class RotateEnvironmentObject : public GameObject
{
public:
	RotateEnvironmentObject()
	{
		addComponent(new TransformComponent);
		addComponent(new ModelComponent);
		addComponent(new RotateComponent(getComponent<TransformComponent>(), 1.0f, glm::vec3(0, 1, 0)));
	}

	~RotateEnvironmentObject() { }

	void OnUpdate(float dt) override
	{
		getComponent<RotateComponent>()->OnUpdate(dt);
	}
	void OnMessage(const std::string m) override { }
};
