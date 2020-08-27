#pragma once
#include "Component.h"
#include "ParticleRenderSystem.h"
#include <random>

class ParticleEmitterComponent : public Component {
public:
	// defualt constructor - snow effect
	ParticleEmitterComponent(const unsigned int maxParticles = 10000) : m_particleRenderer(maxParticles), m_maxParticles(maxParticles){
		// initialised variables
		m_particles = new Particle[m_maxParticles];
		m_particles->expiration = 10.0f;	// set default expiration time
		m_lastUsedParticleIndex = 0;
		m_numLiveParticles = 0;
		m_timeSinceLastParticleGen = 0;
		m_size = 1.0f;
		m_offset = glm::vec3(0.0f, 100.0f, 0.0f);
		m_colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_maxRange = 500.0f;
		m_minRange = 250.0f;
		// set up renderer
		m_particleRenderer.initialise();
		m_particleRenderer.setTexture("snow.png");
		// are particles falling or rising? if one is true, negate the other
		b_falling = true;
		b_rising = false;
	};
	// constructor
	ParticleEmitterComponent(std::string textureFilePath, glm::vec3 offset, glm::vec4 colour, float size, float maxRange, float minRange, bool falling, float expire,
		const unsigned int maxParticles = 10000) : m_particleRenderer(maxParticles), m_maxParticles(maxParticles) {
		// initialised variables
		m_particles = new Particle[m_maxParticles];
		m_particles->expiration = m_expire = expire;	// set default expiration time
		m_lastUsedParticleIndex = 0;
		m_numLiveParticles = 0;
		m_timeSinceLastParticleGen = 0;
		m_size = size;
		m_offset = offset;
		m_colour = colour;
		m_maxRange = maxRange;
		m_minRange = minRange;
		// set up renderer
		m_particleRenderer.initialise();
		m_particleRenderer.setTexture(textureFilePath);
		// are particles falling or rising? if one is true, negate the other
		b_falling = falling;
		if (b_falling) { b_rising = false; }
		else { b_rising = true; }
	};

	// update the particles
	void OnUpdate(float dt) override {
		// update particles appropriately
		if (b_falling) {
			updateFallingParticles(dt);
		}
		else if (b_rising) {
			updateRisingParticles(dt);
		}
		// sort particles by camera distance squared
		std::sort(&m_particles[0], &m_particles[m_maxParticles]);
		// instantiate any new particles
		m_timeSinceLastParticleGen += dt;
		if (m_timeSinceLastParticleGen > 0.16) {
			int particleIndex = findUnusedParticle();
			m_particles[particleIndex].age = 0.0f;
			m_particles[particleIndex].expiration = m_expire;
			// start close to floor - going upwards particles
			float randX = ((rand() % RAND_MAX) / (float)RAND_MAX) * m_maxRange - m_minRange;
			float randZ = ((rand() % RAND_MAX) / (float)RAND_MAX) * m_maxRange - m_minRange;
			m_particles[particleIndex].position = glm::vec3(m_offset.x + randX, m_offset.y, m_offset.z + randZ);
			m_particles[particleIndex].velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			m_particles[particleIndex].colour = m_colour;
			m_particles[particleIndex].size = m_size;
			// reset timer
			m_timeSinceLastParticleGen = 0;
		}
		// pass to gpu
		m_particleRenderer.update(m_particles, m_numLiveParticles);
	};
	void OnMessage(const std::string m) override {};

	// if particlaes are going downwards, update them
	void updateFallingParticles(float dt) {
		m_numLiveParticles = 0;
		// simulate the particles
		for (int i = 0; i < m_maxParticles; i++) {
			Particle& p = m_particles[i];	// shortcut
			// check if alive
			if (p.age < p.expiration) {
				// if yes decrease, life reamining
				p.age += dt;
				// another life check
				if (p.age < p.expiration) {
					// simulate basic physics
					p.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * dt;
					p.position += p.velocity * dt;
					p.cameraSqDist = glm::length2(p.position - m_cameraPos);
				}
				else {
					// particle dead, needs to be placed at end of buffer
					p.cameraSqDist = -1.0f;
				}
				// update counter
				m_numLiveParticles++;
			}
		}
	}
	// update particles that are going up
	void updateRisingParticles(float dt) {
		m_numLiveParticles = 0;
		// simulate the particles
		for (int i = 0; i < m_maxParticles; i++) {
			Particle& p = m_particles[i];	// shortcut
			// check if alive
			if (p.age < p.expiration) {
				// if yes decrease, life reamining
				p.age += dt;
				// another life check
				if (p.age < p.expiration) {
					// simulate basic physics
					p.velocity += glm::vec3(0.0f, 9.81f, 0.0f) * dt;
					p.position += p.velocity * dt;
					p.cameraSqDist = glm::length2(p.position - m_cameraPos);
				}
				else {
					// particle dead, needs to be placed at end of buffer
					p.cameraSqDist = -1.0f;
				}
				// update counter
				m_numLiveParticles++;
			}
		}
	}

	// render the particles
	void render(GLuint shader) { m_particleRenderer.render(m_numLiveParticles, shader); };

	// finds the next unused particle (age > expiration)
	int findUnusedParticle() {
		// search last used particles
		for (int i = m_lastUsedParticleIndex; i < m_maxParticles; i++) {
			// check each particle in the container
			if (m_particles[i].age > m_particles[i].expiration) {
				// dead particle found
				m_lastUsedParticleIndex = i;
				return i;
			}
		}
		// linear search
		for (int i = 0; i < m_lastUsedParticleIndex; i++) {
			if (m_particles[i].age > m_particles[i].expiration) {
				// dead particle found
				m_lastUsedParticleIndex = i;
				return i;
			}
		}
		// all particles are still in use
		return 0;
	};

	glm::vec3 m_cameraPos;

//private:
	Particle* m_particles;
	ParticleSystemRenderer m_particleRenderer;
	int m_maxParticles;
	int m_lastUsedParticleIndex;
	int m_numLiveParticles;
	float m_timeSinceLastParticleGen;
	// partcile data that is passe in via constructor
	float m_size, m_expire;
	bool b_falling, b_rising;	// what kind of particle is it
	float m_maxRange, m_minRange;	// max and min range that particles will be spawn from
	glm::vec3 m_offset;	// position where the particles originate from
	glm::vec4 m_colour;	// colour data of the particle
};