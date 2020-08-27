#version 430 core

in vec2 TextureUV;
in vec4 Colour;
in float Age;

out vec4 FragColour;

uniform sampler2D textureSample;

void main() {
	FragColour = texture(textureSample, TextureUV) * Colour;
}