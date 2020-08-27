#pragma once
#include "InputHandler.h"
#include "IEngineCore.h"
#include "CameraComponent.h"
#include "ModelManager.h"
#include "GateComponent.h"

class Scene
{
public:
	virtual ~Scene() = default;

	virtual void update(float dt) = 0;
	virtual void render() = 0;
	bool renderParticles = true;

	float checkDistance(GameObject* object1, GameObject* object2) const
	{
		glm::vec3 pos1 = object1->getComponent<TransformComponent>()->position();
		glm::vec3 pos2 = object2->getComponent<TransformComponent>()->position();

		glm::vec3 temp = pos1 - pos2;
		return glm::sqrt(glm::dot(temp, temp));
	}

	bool wallCollisionCheck(GameObject* wall, GameObject* player) const
	{
		glm::vec3 wallPos = wall->getComponent<TransformComponent>()->position();
		glm::vec3 playerPos = player->getComponent<TransformComponent>()->position();
		glm::vec3 wallScale = wall->getComponent<TransformComponent>()->scale();

		// TODO Fix this by checking player height and width
		if (playerPos.x <= wallPos.x + wallScale.x / 2 && playerPos.x >= wallPos.x - wallScale.x / 2)
		{
			if (playerPos.y <= wallPos.y + wallScale.y / 2 && playerPos.y >= wallPos.y - wallScale.y / 2)
			{
				if (playerPos.z <= wallPos.z + wallScale.z / 2 && playerPos.z >= wallPos.z - wallScale.z / 2)
				{
					return true;
				}
			}
		}

		return false;
	}

	std::string getNextLevelPath()
	{
		for (size_t i = 0; i < v_gameObjects.size(); ++i)
		{
			if (v_gameObjects[i]->getComponent<GateComponent>() != nullptr)
			{
				return v_gameObjects[i]->getComponent<GateComponent>()->m_nextLevelPath;
			}
		}

		return "";
	}

	IEngineCore* m_engineInterfacePtr = nullptr;
	CameraComponent* m_camera = nullptr;
	std::shared_ptr<ModelManager> m_modelManager = std::make_shared<ModelManager>();

	std::vector<std::shared_ptr<GameObject>> v_gameObjects;
	std::vector<std::shared_ptr<GameObject>> v_particles;
	std::vector<std::shared_ptr<GameObject>> v_staticObjects;

	std::shared_ptr<GameObject> m_player;
	std::shared_ptr<GameObject> m_gate;
	GameObject m_background;

	bool m_levelCompleted = false;
	double m_fps;
	float m_score = 0;
};