#pragma once
#include "Component.h"

class RewardComponent : public Component
{
public:
	RewardComponent(int amount)
	{
		m_amount = amount;
	}

	void OnUpdate(float dt) override {}
	void OnMessage(const std::string m) override {}

	int m_amount;
};
