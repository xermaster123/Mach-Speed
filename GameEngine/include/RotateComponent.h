#pragma once
#include "Component.h"
#include "TransformComponent.h"

class RotateComponent : public Component
{

	float m_rotateAmount = 1;
	glm::vec3 m_rotationAxis = glm::vec3(0, 1, 0);
	TransformComponent * m_transform;


public:
	RotateComponent(TransformComponent * transform, float rotateAmount, glm::vec3 rotationAxis)
	{
		m_rotateAmount = rotateAmount;
		m_rotationAxis = rotationAxis;
		m_transform = transform;
	}

	void OnUpdate(float dt) override
	{
		m_transform->rotate(m_rotateAmount * dt, m_rotationAxis.x, m_rotationAxis.y, m_rotationAxis.z);
	}

	void OnMessage(const std::string m) override {}
};
