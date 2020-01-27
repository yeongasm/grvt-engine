#include "Framework/DefaultShaders/ShadowMaps.h"


Gfl::String DDepthMapShader::VertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

#define MAX_DIR_LIGHT 8

layout (std140, binding = 3) uniform Transforms
{
	uniform int  Index;
	uniform mat4 LightSpaceTransforms[MAX_DIR_LIGHT];
}

uniform mat4 Model;

void main()
{
	gl_Position = LightSpaceTransforms[Index] * Model * vec4(aPos, 1.0f);
}
)";

Gfl::String DDepthMapShader::FragmentShader = R"(
#version 330 core

void main()
{
	//gl_FragDepth = gl_FragCoord.z
}
)";