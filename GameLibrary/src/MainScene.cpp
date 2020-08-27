#include "MainScene.h"
#include "ModelComponent.h"
#include "ColourComponent.h"
#include <GLFW/glfw3.h>
#include "RewardComponent.h"
#include "ShootComponent.h"

MainScene::~MainScene()
{
}

void MainScene::update(float dt)
{
	aManager.update();

	for (size_t i = 0; i < v_gameObjects.size(); ++i)
	{
		if (!v_gameObjects[i]->m_enabled) continue;
		
		v_gameObjects[i]->OnUpdate(dt);

		auto reward = v_gameObjects[i]->getComponent<RewardComponent>();

		if (reward != nullptr)
		{
			if (checkDistance(v_gameObjects[i].get(), m_player.get()) <= 2)
			{
				m_score += reward->m_amount;
				//v_gameObjects.erase(v_gameObjects.begin() + i);
				v_gameObjects[i]->m_enabled = false;
				aManager.playSound(3);
			}
		}

		auto shoot = v_gameObjects[i]->getComponent<ShootComponent>();
		if (shoot != nullptr && shoot->m_shoot && m_player != nullptr)
		{
			std::shared_ptr<AmmoObject> newBullet = std::make_shared<AmmoObject>();

			newBullet->getComponent<ModelComponent>()->m_modelProperties.m_filePath = shoot->m_modelPath;
			newBullet->getComponent<ModelComponent>()->m_modelProperties.m_position = v_gameObjects[i]->getComponent<TransformComponent>()->position();
			newBullet->getComponent<ModelComponent>()->m_modelProperties.m_scale = v_gameObjects[i]->getComponent<TransformComponent>()->scale();
			newBullet->getComponent<ModelComponent>()->m_modelProperties.m_orientation = v_gameObjects[i]->getComponent<TransformComponent>()->orientation();
			newBullet->getComponent<TransformComponent>()->setPosition(v_gameObjects[i]->getComponent<TransformComponent>()->position());
			newBullet->getComponent<TransformComponent>()->setOrientation(v_gameObjects[i]->getComponent<TransformComponent>()->orientation());
			newBullet->getComponent<TransformComponent>()->setScale(v_gameObjects[i]->getComponent<TransformComponent>()->scale());
			v_ammoObjects.push_back(newBullet);
			aManager.playSound(2);
			shoot->Shoot();
		}
	}

	// particle updating
	if (renderParticles)
	{
		for (auto &p : v_particles) {
			p->OnUpdate(dt);
		}
	}

	for (auto element : v_staticObjects)
	{
		element->OnUpdate(dt);
	}

	for (size_t i = 0; i < v_ammoObjects.size(); ++i)
	{
		v_ammoObjects[i]->OnUpdate(dt);
		
		if (glfwGetTime() - v_ammoObjects[i]->getComponent<LifetimeComponent>()->time >= 1.f)
		{
			v_ammoObjects.erase(v_ammoObjects.begin() + i);
		}

		if (checkDistance(v_ammoObjects[i].get(), m_player.get()) <= 1)
		{
			// restart level

			for (size_t i = 0; i < v_gameObjects.size(); ++i)
			{
				auto reward = v_gameObjects[i]->getComponent<RewardComponent>();

				if (reward != nullptr)
				{
					v_gameObjects[i]->m_enabled = true;
				}
			}

			m_player->getComponent<TransformComponent>()->setPosition(m_player->getComponent<ModelComponent>()->m_modelProperties.m_position);
			m_score = 0;

			v_ammoObjects.erase(v_ammoObjects.begin() + i);
			aManager.playSound(4);
		}
	}

	if (m_player != nullptr)
		m_player->OnUpdate(dt);

	if (m_gate != nullptr)
	{
		if (m_score >= m_gate->getComponent<GateComponent>()->m_scoreCheck)
		{
			if (checkDistance(m_player.get(), m_gate.get()) <= m_gate->getComponent<GateComponent>()->m_collideDistance)
			{
				m_levelCompleted = true;
			}
		}
	}
}

void MainScene::render()
{
	auto r = m_background.getComponent<RedComponent>()->m_colourValue;
	auto g = m_background.getComponent<GreenComponent>()->m_colourValue;
	auto b = m_background.getComponent<BlueComponent>()->m_colourValue;

	m_engineInterfacePtr->renderColouredBackground(r, g, b);

	if (m_camera == nullptr)
		m_engineInterfacePtr->setCamera(new CameraComponent());
	else
		m_engineInterfacePtr->setCamera(m_camera);


	// draw the objects
	for (size_t i = 0; i < v_gameObjects.size(); ++i)
	{
		if (!v_gameObjects[i]->m_enabled) continue;

		auto object = v_gameObjects[i];
		Model* objectModel = m_modelManager->getRef(object->getComponent<ModelComponent>()->m_modelProperties.m_filePath);
		m_engineInterfacePtr->drawModel(objectModel, object->getComponent<TransformComponent>()->getModelMatrix());
	}

	// draw the ammo objects
	for (size_t i = 0; i < v_ammoObjects.size(); ++i)
	{
		auto object = v_ammoObjects[i];
		Model* objectModel = m_modelManager->getRef(object->getComponent<ModelComponent>()->m_modelProperties.m_filePath);
		m_engineInterfacePtr->drawModel(objectModel, object->getComponent<TransformComponent>()->getModelMatrix());
	}

	// particle rendering
	if (renderParticles)
	{
		for (size_t i = 0; i < v_particles.size(); ++i) {
			auto object = v_particles[i];
			// only need the particle component attached to the object, not the object itself
			m_engineInterfacePtr->drawParticle(object->getComponent<ParticleEmitterComponent>());
		}
	}

	for (size_t i = 0; i < v_staticObjects.size(); ++i)
	{
		auto object = v_staticObjects[i];
		Model* objectModel = m_modelManager->getRef(object->getComponent<ModelComponent>()->m_modelProperties.m_filePath);
		m_engineInterfacePtr->drawModel(objectModel, object->getComponent<TransformComponent>()->getModelMatrix());
	}

	if (m_player != nullptr)
	{
		Model* playerModel = m_modelManager->getRef(m_player->getComponent<ModelComponent>()->m_modelProperties.m_filePath);
		if (playerModel != nullptr)
		{
			m_engineInterfacePtr->drawModel(playerModel, m_player->getComponent<TransformComponent>()->getModelMatrix());
		}
	}

	if (m_gate != nullptr)
	{
		Model* gateModel = m_modelManager->getRef(m_gate->getComponent<ModelComponent>()->m_modelProperties.m_filePath);
		if (gateModel != nullptr)
		{
			m_engineInterfacePtr->drawModel(gateModel, m_gate->getComponent<TransformComponent>()->getModelMatrix());
		}
	}

#if _DEBUG
	m_engineInterfacePtr->renderText("FPS: " + std::to_string(m_fps), 0.0f, 0.f, 0.5f, glm::vec3(0.f, 0.f, 0.f));
#endif
}