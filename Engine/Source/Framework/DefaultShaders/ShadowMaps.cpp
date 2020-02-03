#include "Framework/DefaultShaders/ShadowMaps.h"


Gfl::String DDepthMapShader::VertexShader = R"(
#version 430 core
layout (location = 0) in vec3 aPos;


uniform mat4 LightSpaceTransform;
uniform mat4 Model;

void main()
{
	gl_Position = LightSpaceTransform * Model * vec4(aPos, 1.0f);
}
)";

Gfl::String DDepthMapShader::FragmentShader = R"(
#version 430 core

void main()
{
	//gl_FragDepth = gl_FragCoord.z;
}
)";