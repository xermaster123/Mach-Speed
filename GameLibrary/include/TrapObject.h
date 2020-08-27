#pragma once
#include "GameObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"

class TrapObject : public GameObject
{
	glm::vec3 targetVector;
	glm::vec3 playerPosition;
	glm::vec3 turretPosition;

public:
	TrapObject()
	{
		addComponent(new TransformComponent);
		addComponent(new ModelComponent);
	}

	~TrapObject() {}

	void OnUpdate(float dt) override
	{
		TransformComponent* transform = getComponent<TransformComponent>();
		glm::vec3 pos = transform->position();
		glm::quat ori = transform->orientation();

		float distx = pos.x - playerPosition.x;
		float disty = pos.y - playerPosition.y;
		float distz = pos.z - playerPosition.z;

		if (distz <= 15 && distz >= -15 && distx <= 15 && distx >= -15)
		{
			transform->setPosition(pos.x, 5.0, pos.z);
		}
		if (distz > 15 || distz < -15 || distx > 15 || distx < -15)
		{
			transform->setPosition(pos.x, -4.0, pos.z);
		}
		if (distz <= 5 && distz >= -5 && distx <= 5 && distx >= -5)
		{
			//cout << "collision" << endl;
		}

	}
	void OnMessage(const std::string m) override
	{

	}
};