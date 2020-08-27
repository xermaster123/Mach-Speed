#pragma once
#include "GameObject.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

class CameraObject : public GameObject
{
	enum CameraViewState
	{
		firstPersonCamera,
		thirdPersonCamera
	};

public:
	CameraObject() : m_cameraState(CameraViewState::firstPersonCamera)
	{
		addComponent(new TransformComponent());
		addComponent(new CameraComponent());
	}

	~CameraObject() { }

	void OnUpdate(float dt) override
	{
		// Get the transform component details
		TransformComponent * transform = getComponent<TransformComponent>();

		switch (m_cameraState)
		{
		case CameraViewState::firstPersonCamera:
			// First person view - just have the camera in place on the player
			// Need inverse pos and orientation
			// Needs to be correctly positioned so it is on the models face
			getComponent<CameraComponent>()->m_position = -1.0f * transform->position();
			getComponent<CameraComponent>()->m_orientation = glm::inverse(transform->orientation());
			break;
		case CameraViewState::thirdPersonCamera:
			// Third person view - stay at a set distance from the player
			// Needs to be correctly positioned so it scales with the model.
			//glm::quat ori = transform->orientation() * glm::angleAxis(80.f, glm::vec3(0, 1, 0) * transform->orientation());
			const glm::quat inverseOrientation = glm::inverse(transform->orientation());
			getComponent<CameraComponent>()->m_position = -1.0f * (transform->position() + glm::vec3(0, 2.5f, 10) * inverseOrientation); // Original 0, 150, 500 //edited 0, 2.5f, 10
			getComponent<CameraComponent>()->m_orientation = inverseOrientation;
			break;
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
			float speed = 0.1f;

			if (m == "rotateCameraLeft")
			{
				transform->yaw(speed);
			}

			if (m == "rotateCameraRight")
			{
				transform->yaw(-speed);
			}

			if (m == "rotateCameraUp")
			{
				//transform->pitch(-speed);
			}

			if (m == "rotateCameraDown")
			{
				//transform->pitch(speed);
			}
		}

		if (m.compare(0, 10, "movePlayer") == 0)
		{
			// Determine the correct translation vector
			glm::vec3 translationVector(0, 0, 0);
			const float movementspeed = 0.45f;

			if (m == "movePlayerForward") translationVector.z = -movementspeed;
			if (m == "movePlayerBackward") translationVector.z = movementspeed;

			if (m == "movePlayerLeft") translationVector.x = -movementspeed;
			if (m == "movePlayerRight") translationVector.x = movementspeed;

			if (m == "movePlayerUp") translationVector.y = movementspeed;
			if (m == "movePlayerDown") translationVector.y = -movementspeed;

			TransformComponent* transform = getComponent<TransformComponent>();
			transform->translate(translationVector * inverse(transform->orientation()));
		}

		OnUpdate(0);
	}

	CameraViewState m_cameraState;
};

