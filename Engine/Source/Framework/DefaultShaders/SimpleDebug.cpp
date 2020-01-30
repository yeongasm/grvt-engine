#include "Framework/DefaultShaders/SimpleDebug.h"


Gfl::String SimpleDebugShader::VertexShader = R"(
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = vec4(aPos, 1.0f);
}
)";

Gfl::String SimpleDebugShader::FragmentShader = R"(
#version 430 core

out vec4 FragColour;
in vec2 TexCoords;

uniform sampler2D DebugTexture;
uniform float Near;
uniform float Far;

float LinearizeDepth(float Depth)
{
	float Z = Depth * 2.0f - 1.0f;
	return (2.0f * Near * Far) / (Far + Near - Z * (Far - Near));
};

void main()
{
	float DepthValue = texture(DebugTexture, TexCoords).r;
	FragColour =  vec4(vec3(DepthValue), 1.0f);
}
)";