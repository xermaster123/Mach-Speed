#pragma once
#include "Component.h"
#include "GLFW/glfw3.h"

class ShootComponent : public Component
{
	double time = glfwGetTime();

public:
	bool m_shoot = false;
	std::string m_modelPath;

	void OnUpdate(float dt) override 
	{
		if (glfwGetTime() - time >= 0.75f)
		{
			m_shoot = true;
		}
	}

	void Shoot()
	{
		time = glfwGetTime();
		m_shoot = false;
	}

	void OnMessage(const std::string m) override {}
};
