#pragma once
#include "ModelProperties.h"

class ModelComponent : public Component
{
public:
	void OnUpdate(float dt) override {}
	void OnMessage(const std::string m) override {}

	ModelProperties m_modelProperties;
};
