#include "..\include\ParticleRenderSystem.h"

void ParticleSystemRenderer::setTexture(std::string textureFilePath) {
	m_textureIndex = TextureFromFile(textureFilePath.c_str(), "assets/particles/");
}

void ParticleSystemRenderer::initialise() {
	// create the arrays
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// quad to represent the particle
	static const GLfloat position[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};
	// load into buffer
	glGenBuffers(1, &VBO_Billboard);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Billboard);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

	// vbo for non-static data
	glGenBuffers(1, &VBO_Particles);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Particles);
	// initialise empty
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(Particle), NULL, GL_STREAM_DRAW);
}

void ParticleSystemRenderer::update(Particle* particleData, int numberOfLiveParticles) {
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Particles);
	// set to null before being overwritten
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(Particle), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numberOfLiveParticles * sizeof(Particle), particleData);
}

void ParticleSystemRenderer::render(unsigned int numberOfParticles, const unsigned int shaderProgram) {
	glBindVertexArray(VAO); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set vertex attribute pointers
	// billboard
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Billboard);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// position and size
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Particles);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
	// colours
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, colour));
	// age
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, age));


	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureIndex);
	glUniform1i(glGetUniformLocation(shaderProgram, "textureSample"), 0);

	glVertexAttribDivisor(0, 0);	// billboard: always reuse the 4 vertices
	glVertexAttribDivisor(1, 1);	// position: update every frame
	glVertexAttribDivisor(2, 1);	// colour: update every frame
	glVertexAttribDivisor(3, 1);	// age : update every frame

	// draw the instanced particles
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numberOfParticles);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

unsigned int ParticleSystemRenderer::TextureFromFile(const char* filepath, const std::string& directory, bool gamma) {
	std::string filename = std::string(filepath);
	filename = directory + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* textureData = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (textureData) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(textureData);
	}
	else {
		std::cout << "Texture failed to load from: " << filepath << std::endl;
		stbi_image_free(textureData);
	}

	return textureID;
}