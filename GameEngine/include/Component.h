#pragma once
#include <string>
#include <iostream>

class Component
{
public:
	virtual ~Component() = default;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnMessage(const std::string m) = 0;
};