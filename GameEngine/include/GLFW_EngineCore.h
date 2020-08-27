#pragma once
#include "IEngineCore.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include "CameraComponent.h"
#include "Game.h"

class GLFW_EngineCore : public IEngineCore
{
	GLFWwindow* m_window;
	GLuint m_defaultShaderProgram;
	GLuint m_fontShaderProgram; // Added for font
	GLuint m_particleShaderProgram;

	// Holds all state information relevant to a character as loaded using FreeType
	// based on https://learnopengl.com/#!In-Practice/Text-Rendering
	struct Character 
	{
		GLuint textureID; // ID handle of the glyph texture
		glm::ivec2 size; // Size of glyph
		glm::ivec2 bearing; // Offset from baseline to left/top of glyph
		GLuint advance; // Horizontal offset to advance to next glyph
	};

	std::map<GLchar, Character> Characters; // Added for font
	GLuint font_VAO, font_VBO; // Added for font

	static int m_screenWidth;
	static int m_screenHeight;
	static std::vector<bool> m_keyBuffer;
	//static std::vector<bool> m_oldKeyBuffer;
	static const int m_keyBufferSize = 400;

	static void mouseMoveCallbackEvent(GLFWwindow* window, double xPos, double yPos);
	static void keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void windowResizeCallbackEvent(GLFWwindow* window, int width, int height);

	void loadShader(std::string vertexShaderFile, std::string fragmentShaderFile, GLuint& shaderProgram); // Added for font
	void setDefaultShaders();
	void initCubeModel();
	void setupDefaultFont(); // Added for font

public:
	~GLFW_EngineCore() override;

	bool initWindow(int width, int height, std::string windowName) override;
	bool runEngine(Game& game) override;
	void renderColouredBackground(float r, float g, float b) override;
	void renderText(std::string text, float x, float y, float scale, glm::vec3 colour) override; // Added for font
	void setWindowSize(double x, double y, double width, double height) override;

	void setCamera(const CameraComponent* camera) override;
	void drawCube(const glm::mat4& modelMatrix) override;
	void drawModel(Model* model, const glm::mat4& modelMatrix) override;
	void drawParticle(ParticleEmitterComponent* particle) override;

	glm::vec3 camera_position;
};