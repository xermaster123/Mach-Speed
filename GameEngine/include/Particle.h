#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

struct Particle {
	// initialised as dead
	Particle() : age(-1.0f), cameraSqDist(-1.0f) {};

	// variables passed to shader
	glm::vec3 position;
	float size;
	glm::vec4 colour;

	// otehr variables
	glm::vec3 velocity;
	float age, expiration;
	float cameraSqDist;

	bool operator<(const Particle& that) const {
		return this->cameraSqDist > that.cameraSqDist;
	}
};