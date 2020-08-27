#pragma once
#include "Component.h"
#include "TransformComponent.h"

class BounceComponent : public Component
{
	float m_bouncePos;
	float m_low;
	float m_high;
	float m_change;
	bool m_down;
	TransformComponent * m_transform;
	

public:
	BounceComponent(TransformComponent * transform, float initial, float low, float high, float change)
	{
		m_bouncePos = initial;
		m_low = low;
		m_high = high;
		m_change = change;
		m_down = false;
		m_transform = transform;
	}

	void OnUpdate(float dt) override 
	{
		glm::vec3 pos = m_transform->position();
		float translation = 0;

		if (m_bouncePos >= m_high) m_down = true;
		if (m_bouncePos <= m_low) m_down = false;

		m_down ? translation = -m_change : translation = m_change;
		translation = translation * dt;
		if (translation > 0.08f || translation < -0.08f) translation = 0.f;

		m_transform->translate(0, translation, 0);
		m_bouncePos += translation;
	}

	void OnMessage(const std::string m) override {}
};
