#pragma once
#include "GameObject.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "BounceComponent.h"
#include "BounceEnvironmentObject.h"

class PlayerCharacterObject : public GameObject
{
	enum CameraViewState
	{
		firstPersonCamera, 
		thirdPersonCamera
	};
	
	glm::vec3 m_translationVector = glm::vec3(0, 0, 0);
	float m_rotateAmount = 0.f;
	float m_speed = 1.f;
	float m_rotateSpeed = 2.f;
	CameraViewState m_cameraState;
	
public:
	PlayerCharacterObject(ModelProperties properties, float speed) : m_cameraState(CameraViewState::thirdPersonCamera)
	{
		addComponent(new TransformComponent());
		addComponent(new ModelComponent());
		addComponent(new CameraComponent());

		getComponent<TransformComponent>()->translate(properties.m_position);
		getComponent<TransformComponent>()->scale(properties.m_scale);
		getComponent<TransformComponent>()->setOrientation(properties.m_orientation);
		getComponent<ModelComponent>()->m_modelProperties = properties;

		addComponent(new BounceComponent(getComponent<TransformComponent>(), properties.m_position.y, -0.42f, -0.33f, 0.08f));

		m_speed = speed;
	}

	~PlayerCharacterObject() { }

	void OnUpdate(float dt) override
	{
		// Get the transform component details
		TransformComponent * transform = getComponent<TransformComponent>();

		switch (m_cameraState)
		{
		case CameraViewState::firstPersonCamera:
			getComponent<CameraComponent>()->m_position = -1.0f * transform->position();
			getComponent<CameraComponent>()->m_orientation = glm::inverse(transform->orientation());
			break;
		case CameraViewState::thirdPersonCamera:
			const glm::quat inverseOrientation = glm::inverse(transform->orientation());
			getComponent<CameraComponent>()->m_position = -1.0f * (transform->position() + glm::vec3(0, 3.5f, 12) * inverseOrientation);
			getComponent<CameraComponent>()->m_orientation = inverseOrientation;
			break;
		}

		getComponent<BounceComponent>()->OnUpdate(dt);

		if (m_translationVector.z != 0 || m_translationVector.x != 0)
		{
			auto newPos = m_translationVector * dt * inverse(transform->orientation());
			auto newPosX = newPos.x + transform->position().x;
			auto newPosY = newPos.y + transform->position().y;
			auto newPosZ = newPos.z + transform->position().z;

			transform->setPosition(newPosX, newPosY, newPosZ);

			m_translationVector = glm::vec3(0, 0, 0);
		}

		if (m_rotateAmount != 0)
		{
			transform->yaw(m_rotateAmount * dt);
			m_rotateAmount = 0.f;
		} 
	}

	void OnMessage(const std::string m) override
	{
		if (m == "SetCameraFirstPerson")
		{
			m_cameraState = CameraViewState::firstPersonCamera;
		}

		if (m == "SetCameraThirdPerson")
		{
			m_cameraState = CameraViewState::thirdPersonCamera;
		}

		if (m.compare(0, 12, "rotateCamera") == 0)
		{
			// Same rotation for both first and third person camera
			TransformComponent* transform = getComponent<TransformComponent>();

			if (m == "rotateCameraLeft")
			{
				m_rotateAmount = m_rotateSpeed;
			}

			if (m == "rotateCameraRight")
			{
				m_rotateAmount = -m_rotateSpeed;
			}
		}

		if (m.compare(0, 10, "movePlayer") == 0)
		{
			// Determine the correct translation vector
			if (m == "movePlayerForward") m_translationVector.z = -m_speed;
			if (m == "movePlayerBackward") m_translationVector.z = m_speed;

			if (m == "movePlayerLeft") m_translationVector.x = -m_speed;
			if (m == "movePlayerRight") m_translationVector.x = m_speed;
		}
	}
};