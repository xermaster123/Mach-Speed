#pragma once
#include "Component.h"
#include "GLFW/glfw3.h"

class LifetimeComponent : public Component
{
public:
	void OnUpdate(float dt) override { }
	void OnMessage(const std::string m) override {}

	double time = glfwGetTime();
};
