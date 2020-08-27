#pragma once
#include "GameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "ParticleEmitterComponent.h"

class ParticleEmitterObject : public GameObject{
public:
	ParticleEmitterObject() {
		addComponent(new TransformComponent);
		addComponent(new ModelComponent);
		addComponent(new ParticleEmitterComponent);
	};
	ParticleEmitterObject(std::string filePath, glm::vec3 offset, glm::vec4 colour,
		float size, float maxR, float minR, bool falling, float expire) {
		addComponent(new TransformComponent);
		addComponent(new ModelComponent);
		addComponent(new ParticleEmitterComponent(filePath, offset, colour,
			size, maxR, minR, falling, expire));
	};
	~ParticleEmitterObject() {};

	void OnUpdate(float dt) { getComponent<ParticleEmitterComponent>()->OnUpdate(dt); };
	void OnMessage(const std::string m) {};
};