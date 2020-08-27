#include "EditorScene.h"
#include "ModelComponent.h"
#include "ColourComponent.h"
#include "ParticleEmitterObject.h"

EditorScene::~EditorScene()
{
}

void EditorScene::update(float dt)
{
	for (auto element : v_gameObjects)
	{
		element->OnUpdate(dt);
	}

	if (m_player != nullptr)
		m_player->OnUpdate(dt);

	if (m_gate != nullptr)
	{
		if (checkDistance(m_player.get(), m_gate.get()) <= 2.f)
		{
			m_levelCompleted = true;
		}
	}
}

void EditorScene::render()
{
	if (m_engineInterfacePtr == nullptr) return;

	auto r = m_background.getComponent<RedComponent>()->m_colourValue;
	auto g = m_background.getComponent<GreenComponent>()->m_colourValue;
	auto b = m_background.getComponent<BlueComponent>()->m_colourValue;

	m_engineInterfacePtr->renderColouredBackground(r, g, b);

	if (m_camera == nullptr)
		m_engineInterfacePtr->setCamera(new CameraComponent());
	else
		m_engineInterfacePtr->setCamera(m_camera);


	// draw the cube
	for (size_t i = 0; i < v_gameObjects.size(); ++i)
	{
		auto object = v_gameObjects[i];

		if (object->getComponent<ParticleEmitterComponent>() != nullptr)
		{
			m_engineInterfacePtr->drawParticle(object.get()->getComponent<ParticleEmitterComponent>());
		}
		else
		{
			Model* objectModel = m_modelManager->getRef(object->getComponent<ModelComponent>()->m_modelProperties.m_filePath);
			if (objectModel != nullptr)
			{
				m_engineInterfacePtr->drawModel(objectModel, object->getComponent<TransformComponent>()->getModelMatrix());
			}
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