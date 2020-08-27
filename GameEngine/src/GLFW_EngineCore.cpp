#include "GLFW_EngineCore.h"
#include <fstream>
#include <sstream>
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h> // Added for font
#include FT_FREETYPE_H 

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"
#include "imgui/imgui_internal.h"

std::vector<bool> GLFW_EngineCore::m_keyBuffer;
//std::vector<bool> GLFW_EngineCore::m_oldKeyBuffer;
int GLFW_EngineCore::m_screenWidth;
int GLFW_EngineCore::m_screenHeight;

GLFW_EngineCore::~GLFW_EngineCore()
{
	// cleanup
	glfwTerminate();
}

bool GLFW_EngineCore::initWindow(int width, int height, std::string windowName)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	m_screenWidth = width;
	m_screenHeight = height;

	m_window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW m_window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(m_window, true);
	ImGui::StyleColorsDark();

	glfwMakeContextCurrent(m_window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		return false;
	}

	// callback functions
	glfwSetFramebufferSizeCallback(m_window, windowResizeCallbackEvent);
	glfwSetKeyCallback(m_window, keyCallbackEvent);
	glfwSetCursorPosCallback(m_window, mouseMoveCallbackEvent);

	// make space for the keybuffer
	m_keyBuffer.resize(m_keyBufferSize);
	std::fill(m_keyBuffer.begin(), m_keyBuffer.end(), false);

	//m_oldKeyBuffer.resize(m_keyBufferSize);
	//std::fill(m_oldKeyBuffer.begin(), m_oldKeyBuffer.end(), false);

	// set the shaders to the given default ones
	setDefaultShaders();
	setupDefaultFont(); // Added for font

	// set the drawable model as a cube
	// note: this will be changed later when we can draw many kinds of objects
	initCubeModel();

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// enable alpha transparency - Added for font
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

bool GLFW_EngineCore::runEngine(Game& game)
{
	// For speed computation
	double oldTimeSinceStart = 0;
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	// message loop
	while (!glfwWindowShouldClose(m_window))
	{
		if (game.m_currentScene != nullptr)
		{
			game.m_currentScene->m_engineInterfacePtr = this;
		}

		const double timeSinceStart = glfwGetTime();
		nbFrames++;

		// If last prinf() was more than 1 sec ago // set fps and reset timer
		if (timeSinceStart - lastTime >= 1.0) {
			//game.m_currentScene->m_fps = (1000.0 / double(nbFrames));
			if (game.m_currentScene != nullptr) game.m_currentScene->m_fps = double(nbFrames);
			nbFrames = 0;
			lastTime += 1.0;
		}

		const float deltatime = static_cast<float>(timeSinceStart - oldTimeSinceStart);
		oldTimeSinceStart = timeSinceStart;

		glfwGetFramebufferSize(m_window, &game.m_windowWidth, &game.m_windowHeight);
		
		game.update(deltatime);
		renderColouredBackground(0, 0, 0);
		game.render();

		if (game.m_inputHandler.size() != 0)
		{
			for (InputHandler element : game.m_inputHandler)
			{
				element.handleInputs(m_keyBuffer);
			}
		}

		// swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	return true;
}

void GLFW_EngineCore::renderColouredBackground(float r, float g, float b)
{
	// clear the screen to the specified r, g, b values
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Added for font
// screen is between 0 and 1 with (0,0) at the bottom left corner
// typical use: m_engineInterfacePtr->renderText("Awesome Game Debug Mode", 0.05f, 0.05f, 1.0f, glm::vec3(0.9, 0.9f, 0.1f));
void GLFW_EngineCore::renderText(std::string text, float x, float y, float scale, glm::vec3 colour)
{
	// set the window to orthographic
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_screenWidth), 0.0f, static_cast<float>(m_screenHeight));

	float pixelValueX = x * m_screenWidth;
	float pixelValueY = y * m_screenHeight;

	glUseProgram(m_fontShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(m_fontShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// Activate corresponding render state	
	glUniform3f(glGetUniformLocation(m_fontShaderProgram, "textColour"), colour.x, colour.y, colour.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(font_VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = pixelValueX + ch.bearing.x * scale;
		GLfloat ypos = pixelValueY - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, font_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		pixelValueX += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// go back to default shader
	glUseProgram(m_defaultShaderProgram);
}

//-----------------------------Private functions------------------------------
void GLFW_EngineCore::mouseMoveCallbackEvent(GLFWwindow * window, double xPos, double yPos)
{
}

void GLFW_EngineCore::keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN || key > m_keyBufferSize)
	{
		return;
	}
	m_keyBuffer[key] = ((action == GLFW_PRESS || action == GLFW_REPEAT));

	ImGuiIO& io = ImGui::GetIO();
	if (action == GLFW_PRESS)
		io.KeysDown[key] = true;
	if (action == GLFW_RELEASE)
		io.KeysDown[key] = false;

	(void)mods; // Modifiers are not reliable across systems
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void GLFW_EngineCore::windowResizeCallbackEvent(GLFWwindow* window, int width, int height)
{
	// change the opengl viewport to match the new m_window size
	m_screenWidth = width;
	m_screenHeight = height;
	glViewport(0, 0, width, height);
}

// Added for font
void GLFW_EngineCore::loadShader(std::string vertexShaderFile, std::string fragmentShaderFile, GLuint & shaderProgram)
{
	// Load contents of vertex file
	std::ifstream inFile(vertexShaderFile);
	if (!inFile) {
		string errorMsg = "Error opening shader file: " + vertexShaderFile + "\n";
		fprintf(stderr, errorMsg.c_str());
		exit(1);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	std::string codeStr(code.str());
	const GLchar* vertex_shader[] = { codeStr.c_str() };

	// Load contents of fragment file
	std::ifstream inFile2(fragmentShaderFile);
	if (!inFile2) {
		string errorMsg = "Error opening shader file: " + fragmentShaderFile + "\n";
		fprintf(stderr, errorMsg.c_str());
		exit(1);
	}

	std::stringstream code2;
	code2 << inFile2.rdbuf();
	inFile2.close();
	std::string codeStr2(code2.str());
	const GLchar* fragment_shader[] = { codeStr2.c_str() };

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertex_shader, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragment_shader, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// loading some default shaders to get things up and running
void GLFW_EngineCore::setDefaultShaders()
{
	loadShader("assets/shaders/defaultShader.vert", "assets/shaders/defaultShader.frag", m_defaultShaderProgram);
	loadShader("assets/shaders/fontShader.vert", "assets/shaders/fontShader.frag", m_fontShaderProgram);
	loadShader("assets/shaders/particleShader.vert", "assets/shaders/particleShader.frag", m_particleShaderProgram);

	// set the default shader
	glUseProgram(m_defaultShaderProgram);
}

// a simple function to initialise a cube model in memory
void GLFW_EngineCore::initCubeModel()
{
	// set up vertex and normal data
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

// Added for font
// based on tutorial from https://learnopengl.com/#!In-Practice/Text-Rendering
void GLFW_EngineCore::setupDefaultFont()
{
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(GLuint(face->glyph->bitmap.width), GLuint(face->glyph->bitmap.rows)),
			glm::ivec2(GLuint(face->glyph->bitmap_left), GLuint(face->glyph->bitmap_top)),
			GLuint(face->glyph->advance.x)
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &font_VAO);
	glGenBuffers(1, &font_VBO);
	glBindVertexArray(font_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, font_VBO);
	// dynamic draw as the text may change frequently
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLFW_EngineCore::setCamera(const CameraComponent* camera)
{
	camera_position = camera->m_position;
	glUseProgram(m_defaultShaderProgram);
	// set the view and projection components of our shader to the camera values
	if ((float)m_screenWidth == 0 || (float)m_screenHeight == 0) return;
	glm::mat4 viewMatrix = camera->getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera->m_fov), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 1000.0f);
	
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

	// be sure to activate shader when setting uniforms/drawing objects
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "objectColour"), 1.0f, 0.6f, 0.61f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightPos"), 0.0f, 2.0f, -2.0f);
	glUniform3fv(glGetUniformLocation(m_defaultShaderProgram, "viewPos"), 1, glm::value_ptr(camera->position()));

	// particle shader
	glUseProgram(m_particleShaderProgram);
	glm::mat4 vpMatrix = projection * viewMatrix;
	glUniform3f(glGetUniformLocation(m_particleShaderProgram, "CameraRight_WorldSpace"), viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	glUniform3f(glGetUniformLocation(m_particleShaderProgram, "CameraUp_WorldSpace"), viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
	glUniformMatrix4fv(glGetUniformLocation(m_particleShaderProgram, "ViewProjectionMatrix"), 1, GL_FALSE, &vpMatrix[0][0]);

	glUseProgram(m_defaultShaderProgram);
}

void GLFW_EngineCore::drawCube(const glm::mat4& modelMatrix)
{
	// set the model component of our shader to the cube model
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// the only thing we can draw so far is the cube, so we know it is bound already
	// this will obviously have to change later
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void GLFW_EngineCore::drawModel(Model* model, const glm::mat4& modelMatrix)
{
	glUseProgram(m_defaultShaderProgram);
	// set the model component of our shader to the object model
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	model->render(m_defaultShaderProgram);
}
void GLFW_EngineCore::drawParticle(ParticleEmitterComponent* particle) {
	glUseProgram(m_particleShaderProgram); 
	particle->m_cameraPos = camera_position;
	particle->render(m_particleShaderProgram);
}

void GLFW_EngineCore::setWindowSize(double x, double y, double width, double height)
{
	glViewport(GLint(x), GLint(y), GLsizei(width), GLsizei(height));
}
