#pragma once
#include "Component.h"

enum ObjectType
{
	Static = 0, // StaticEnvironmentObject
	Player = 1, // PlayerCharacter
	Gate = 2, // LevelGateObject
	Rotate = 3, // RotateEnvironmentObject
	Bounce = 4, // BounceEnvironmentObject
	ParticleEmitter = 5, // ParticleEmitterObject
	Coin = 6 // CoinObject
};

class LevelEditorComponent : public Component
{
public:
	LevelEditorComponent() { }

	ObjectType objectType = ObjectType::Static;
	
	// Level Gate Settings
	std::string nextLevel = "";
	
	// Coin Settings
	int rewardValue = 0;
	
	// Player Settings
	float playerSpeed = 0.f;

	// Particle Settings
	std::string particleTexture = "";
	float particleExpire = 10.f;

	void OnUpdate(float dt) override { }
	void OnMessage(const std::string m) override {  }
};