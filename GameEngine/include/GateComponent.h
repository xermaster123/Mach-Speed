#pragma once
#include "Component.h"

class GateComponent : public Component
{
public:
	GateComponent(std::string level, float collide, float score)
	{
		m_nextLevelPath = level;
		m_collideDistance = collide;
		m_scoreCheck = score;
	}
	
	std::string m_nextLevelPath;
	float m_collideDistance;
	float m_scoreCheck;

	void OnUpdate(float dt) override {}
	void OnMessage(const std::string m) override {}
};
