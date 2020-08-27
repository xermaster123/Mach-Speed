#pragma once
#include "Component.h"
#include "TransformComponent.h"

class AimComponent : public Component
{
public:
	AimComponent() {};
	~AimComponent() {};

	void OnUpdate(float dt) override {};
	void OnMessage(const std::string m) override {};

	glm::quat setTarget(TransformComponent* shooter, glm::vec3 targetVector) {
		float distx = shooter->position().x - targetVector.x;
		float disty = shooter->position().y - targetVector.y;
		float distz = shooter->position().z - targetVector.z;
		float tandist = distx / distz;
		angleyaw = atan(tandist);
		orientationX = shooter->orientation().x;
		orientationY = shooter->orientation().y;

		//Turrent aiming the x axis of the player
		if (angleyaw > orientationY)
		{
			shooter->rotate(0.01, 0, 1, 0);
		}
		if (angleyaw < orientationY)
		{
			shooter->rotate(-0.01, 0, 1, 0);
		}
		//Turrent aiming the z axis of the player
		float tanheight = disty / distz;
		float anglepitch = atan(tanheight);

		if (-anglepitch > orientationX)
		{
			shooter->rotate(0.01, 1, 0, 0);
		}
		if (-anglepitch < orientationX)
		{
			shooter->rotate(-0.01, 1, 0, 0);
		}

		return shooter->orientation();
	}

	bool targetCheck() {
		angleDistTaryaw = angleyaw - orientationY;
		if (angleDistTaryaw <= 0.1 && angleDistTaryaw >= -0.1)
		{
			return true;
		}
		return false;
	}

private:
	float angleyaw;
	float orientationX, orientationY;
	float angleDistTaryaw;
};