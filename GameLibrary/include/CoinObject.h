#pragma once
#include "GameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "BounceComponent.h"
#include "RewardComponent.h"

class CoinObject : public GameObject
{
public:
	CoinObject(int coinWorth)
	{
		addComponent(new TransformComponent);
		addComponent(new ModelComponent);
		addComponent(new RotateComponent(getComponent<TransformComponent>(), 1.f, glm::vec3(0, 1, 0)));
		addComponent(new RewardComponent(coinWorth));
	}

	~CoinObject() { }

	void OnUpdate(float dt) override 
	{
		getComponent<RotateComponent>()->OnUpdate(dt);
	}

	void OnMessage(const std::string m) override { }
};
