#pragma once
#include <glm/glm.hpp>

// TODO Implement debug draw method.
class DebugDrawManager
{
public:
	void addLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& colour, float duration = 0.0f, bool depthEnabled = true);
	void addSphere(const glm::vec3& centrePos, const float radius, const glm::vec3& colour, float duration = 0.0f, bool depthEnabled = true);
	void addCross(const glm::vec3& centrePos, const float height, const float width, const glm::vec3& colour, float duration = 0.0f, bool depthEnabled = true);
	void addAABB(const glm::vec3& minCoords, const glm::vec3& maxCoords, const glm::vec3& colour, float duration = 0.0f, bool depthEnabled = true);
};

extern DebugDrawManager g_debugDrawManager;