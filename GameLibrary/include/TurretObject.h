#pragma once
#include "GameObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "ShootComponent.h"

class TurretObject : public GameObject
{

public:
	TurretObject()
	{
		addComponent(new TransformComponent);
		addComponent(new ModelComponent);
		addComponent(new ShootComponent);
	}

	~TurretObject() { }

	void OnUpdate(float dt) override 
	{
		getComponent<ShootComponent>()->OnUpdate(dt);
	}

	void OnMessage(const std::string m) override {}
};