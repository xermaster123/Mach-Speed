#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class ModelProperties
{
public:
	ModelProperties() { }

	std::string m_filePath = "";

	glm::vec3 m_position = glm::vec3(0, 0, 0);
	glm::quat m_orientation = glm::quat(1, 0, 0, 0);
	glm::vec3 m_scale = glm::vec3(1, 1, 1);
};
