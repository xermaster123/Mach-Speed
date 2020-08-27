#version 430 core

// input data
layout (location = 0) in vec3 BillboardVertexes;
layout (location = 1) in vec4 particlePosition;
layout (location = 2) in vec4 particleColours;
layout (location = 3) in float particleAge;

// uniforms
uniform vec3 CameraRight_WorldSpace;
uniform vec3 CameraUp_WorldSpace;
uniform mat4 ViewProjectionMatrix;

// output
out vec2 TextureUV;
out vec4 Colour;
out float Age;

void main() {
	float particleSize = particlePosition.w;
	vec3 particleCenter_WorldSpace = particlePosition.xyz;
	
	vec3 VertexPosition_WorldSpace = particleCenter_WorldSpace
		+ CameraRight_WorldSpace * BillboardVertexes.x * particleSize
		+ CameraUp_WorldSpace * BillboardVertexes.y * particleSize;
	
	TextureUV = BillboardVertexes.xy + vec2(0.5, 0.5);
	Colour = particleColours;
	Age = particleAge;
	gl_Position = ViewProjectionMatrix * vec4(VertexPosition_WorldSpace, 1.0);
}