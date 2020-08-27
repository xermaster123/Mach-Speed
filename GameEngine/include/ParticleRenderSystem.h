#pragma once
#include "Particle.h"

#include <string>
#include <algorithm>
#include <stb_image/stb_image.h>
#include <iostream>

class ParticleSystemRenderer {
public:
	ParticleSystemRenderer(const unsigned int maxParticles) : m_maxParticles(maxParticles) {};
	void setTexture(std::string textureFilePath);
	void initialise();
	void update(Particle* particleData, int numberOfLiveParticles);
	void render(unsigned int numberOfParticles, const unsigned int shaderProgram);
	static unsigned int TextureFromFile(const char* filepath, const std::string& directory, bool gamma = false);
	unsigned int VAO;
private:
	unsigned int m_textureIndex;
	unsigned int VBO_Billboard;	// stays the same for each particles each frame
	unsigned int VBO_Particles;	// changes each frame

	const int m_maxParticles;
};