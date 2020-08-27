#pragma once
#include "GameObject.h"

class TestGameObject : public GameObject
{
public:
	~TestGameObject() override { }
	bool m_success = false;

	void OnUpdate(float dt) override { }
	void OnMessage(const std::string m) override
	{
		if (m == "correct")
		{
			m_success = true;
		}
	}
};
