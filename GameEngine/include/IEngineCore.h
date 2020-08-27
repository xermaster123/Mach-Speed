#pragma once
#include <string>
#include <glm/mat4x2.hpp>
#include "Model.h"
#include "CameraComponent.h"
#include "ParticleEmitterComponent.h"

class Game;

class IEngineCore
{
public:

	virtual ~IEngineCore() {}

	virtual bool initWindow(int width, int height, std::string windowName) = 0;
	virtual bool runEngine(Game& game) = 0;
	
	// Move these to a proper renderer class later
	virtual void renderColouredBackground(float r, float g, float b) = 0;
	virtual void renderText(std::string text, float x, float y, float scale, glm::vec3 colour) = 0; // Added for font
	virtual void setCamera(const CameraComponent* camera) = 0;
	virtual void drawCube(const glm::mat4& modelMatrix) = 0;
	virtual void drawModel(Model* model, const glm::mat4& modelMatrix) = 0;
	virtual void drawParticle(ParticleEmitterComponent* particle) = 0;
	
	virtual void setWindowSize(double x, double y, double width, double height) = 0;
};
