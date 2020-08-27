#pragma once
#include "GameObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "LifetimeComponent.h"

class AmmoObject : public GameObject
{
	glm::vec3 m_translationVector = glm::vec3(0, 0, 65);

public:
	AmmoObject()
	{
		addComponent(new TransformComponent);
		addComponent(new ModelComponent);
		addComponent(new LifetimeComponent);
	}

	~AmmoObject() {}

	void OnUpdate(float dt) override
	{
		auto transform = getComponent<TransformComponent>();

		auto newPos = m_translationVector * dt * inverse(transform->orientation());
		auto newPosX = newPos.x + transform->position().x;
		auto newPosY = newPos.y + transform->position().y;
		auto newPosZ = newPos.z + transform->position().z;

		transform->setPosition(newPosX, newPosY, newPosZ);
	}

	void OnMessage(const std::string m) override
	{

	}
};