#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#define ROTATION_ADJUSTMENT_VALUE 0.01f
#define TRANSLATION_ADJUSTMENT_VALUE 0.01f
#define SCALE_ADJUSTMENT_VALUE 0.001f

class TransformComponent : public Component
{
private:
	glm::vec3 m_position;
	glm::quat m_orientation;
	glm::vec3 m_scale;

public:
	void OnUpdate(float dt) override {}

	void OnMessage(const std::string m) override
	{

#pragma region Scaling
		//TODO Implement x, y, z scaling
		if (m == "scale+")
		{
			scaleUp(SCALE_ADJUSTMENT_VALUE, SCALE_ADJUSTMENT_VALUE, SCALE_ADJUSTMENT_VALUE);
			std::cout << "New scale: " << scale().x << "\n";
		}

		if (m == "scale-")
		{
			scaleUp(-SCALE_ADJUSTMENT_VALUE, -SCALE_ADJUSTMENT_VALUE, -SCALE_ADJUSTMENT_VALUE);
			std::cout << "New scale: " << scale().x << "\n";
		}
#pragma endregion 

#pragma region Translation
		if (m == "translatex+")
		{
			translate(TRANSLATION_ADJUSTMENT_VALUE, 0, 0);
			std::cout << "New X position: " << position().x << "\n";
		}

		if (m == "translatey+")
		{
			translate(0, TRANSLATION_ADJUSTMENT_VALUE, 0);
			std::cout << "New Y position: " << position().y << "\n";
		}

		if (m == "translatez+")
		{
			translate(0, 0, TRANSLATION_ADJUSTMENT_VALUE);
			std::cout << "New Z position: " << position().z << "\n";
		}

		if (m == "translatex-")
		{
			translate(-TRANSLATION_ADJUSTMENT_VALUE, 0, 0);
			std::cout << "New X position: " << position().x << "\n";
		}

		if (m == "translatey-")
		{
			translate(0, -TRANSLATION_ADJUSTMENT_VALUE, 0);
			std::cout << "New Y position: " << position().y << "\n";
		}

		if (m == "translatez-")
		{
			translate(0, 0, -TRANSLATION_ADJUSTMENT_VALUE);
			std::cout << "New Z position: " << position().z << "\n";
		}
#pragma endregion 

#pragma region Rotation
		if (m == "rotatex+")
		{
			rotate(ROTATION_ADJUSTMENT_VALUE, 1, 0, 0);
			std::cout << "New X angle: " << orientation().x << "\n";
		}

		if (m == "rotatey+")
		{
			rotate(ROTATION_ADJUSTMENT_VALUE, 0, 1, 0);
			std::cout << "New Y angle: " << orientation().y << "\n";
		}

		if (m == "rotatez+")
		{
			rotate(ROTATION_ADJUSTMENT_VALUE, 0, 0, 1);
			std::cout << "New Z angle: " << orientation().z << "\n";
		}

		if (m == "rotatex-")
		{
			rotate(-ROTATION_ADJUSTMENT_VALUE, 1, 0, 0);
			std::cout << "New X angle: " << orientation().x << "\n";
		}

		if (m == "rotatey-")
		{
			rotate(-ROTATION_ADJUSTMENT_VALUE, 0, 1, 0);
			std::cout << "New Y angle: " << orientation().y << "\n";
		}

		if (m == "rotatez-")
		{
			rotate(-ROTATION_ADJUSTMENT_VALUE, 0, 0, 1);
			std::cout << "New Z angle: " << orientation().z << "\n";
		}
#pragma endregion 

	}

	TransformComponent() : m_position(0), m_orientation(1, 0, 0, 0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orient) : m_position(pos), m_orientation(orient), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orient, const glm::vec3& scale) : m_position(pos), m_orientation(orient), m_scale(scale) {}

	const glm::vec3& position() const { return m_position; }
	const glm::quat& orientation() const { return m_orientation; }
	const glm::vec3& scale() const { return m_scale; }

	glm::mat4 getModelMatrix()
	{
		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_position);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
		glm::mat4 rotMatrix = glm::mat4_cast(m_orientation);
		return transMatrix * rotMatrix * scaleMatrix;
	}

	void translate(const glm::vec3 &v) { m_position += v; }
	void translate(float x, float y, float z) { m_position += glm::vec3(x, y, z); }
	void setPosition(float x, float y, float z) { m_position = glm::vec3(x, y, z); }
	void setPosition(glm::vec3 pos) { m_position = pos; }

	void rotate(float angle, const glm::vec3 &axis) { m_orientation *= glm::angleAxis(angle, axis * m_orientation); }
	void rotate(float angle, float x, float y, float z) { m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation); }

	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }

	void setOrientation(glm::quat ori) { m_orientation = ori; }
	void setOrientation(double pitch, double yaw, double roll) { m_orientation = glm::quat(glm::vec3(pitch, yaw, roll)); }

	void scale(const glm::vec3 &v) { m_scale *= v; }
	void scale(float x, float y, float z) { m_scale *= glm::vec3(x, y, z); }
	void scaleUp(const glm::vec3 &v) { m_scale += v; }
	void scaleUp(float x, float y, float z) { m_scale += glm::vec3(x, y, z); }
	void setScale(float x, float y, float z) { m_scale = glm::vec3(x, y, z); }
	void setScale(glm::vec3 scale) { m_scale = scale; }
};
