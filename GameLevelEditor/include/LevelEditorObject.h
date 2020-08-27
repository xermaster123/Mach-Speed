#pragma once
#include "GameObject.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "LevelEditorComponent.h"

class LevelEditorObject : public GameObject
{
public:
	LevelEditorObject(ModelProperties properties)
	{
		addComponent(new TransformComponent());
		addComponent(new ModelComponent());
		addComponent(new LevelEditorComponent());

		getComponent<TransformComponent>()->translate(properties.m_position);
		getComponent<TransformComponent>()->scale(properties.m_scale);
		getComponent<TransformComponent>()->setOrientation(properties.m_orientation);
		getComponent<ModelComponent>()->m_modelProperties = properties;
	}

	LevelEditorObject(ModelProperties properties, std::string filePath, glm::vec3 offset, glm::vec4 colour, float size, float maxR, float minR, bool falling, float expire)
	{
		addComponent(new TransformComponent());
		addComponent(new ModelComponent());
		addComponent(new LevelEditorComponent());

		getComponent<TransformComponent>()->translate(properties.m_position);
		getComponent<TransformComponent>()->scale(properties.m_scale);
		getComponent<TransformComponent>()->setOrientation(properties.m_orientation);
		getComponent<ModelComponent>()->m_modelProperties = properties;

		addComponent(new ParticleEmitterComponent(filePath, offset, colour, size, maxR, minR, falling, expire));
	}

	LevelEditorObject(ModelProperties properties, std::string levelPath, float collide, float score)
	{
		addComponent(new TransformComponent());
		addComponent(new ModelComponent());
		addComponent(new LevelEditorComponent());

		getComponent<TransformComponent>()->translate(properties.m_position);
		getComponent<TransformComponent>()->scale(properties.m_scale);
		getComponent<TransformComponent>()->setOrientation(properties.m_orientation);
		getComponent<ModelComponent>()->m_modelProperties = properties;

		addComponent(new GateComponent(levelPath, collide, score));
	}

	~LevelEditorObject() { }

	void OnUpdate(float dt) override 
	{
		auto particle = getComponent<ParticleEmitterComponent>();

		if (particle != nullptr)
		{
			particle->OnUpdate(dt);
		}
	}
	void OnMessage(const std::string m) override { }
};
