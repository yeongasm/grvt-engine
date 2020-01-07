#include "pch.h"
#include "ShaderLib.h"


Gfl::String TestShader::VertexShader = R"(
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
void main()
{
	TexCoord = aTexCoord;
	gl_Position = Projection * View * Model * vec4(aPos, 1.0f);
}
)";

Gfl::String TestShader::FragmentShader = R"(
#version 430 core
out vec4 FragColour;
in vec2 TexCoord;

uniform vec3 Colour;
uniform sampler2D Albedo;

void main()
{
	FragColour = texture(Albedo, TexCoord);
}
)";

Gfl::String SkyboxShader::VertexShader = R"(
#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 Projection;
uniform mat4 View;

void main()
{
	TexCoord = aPos;
	vec4 Position = Projection * View * vec4(aPos, 1.0f);
	gl_Position = Position.xyww;
}
)";

Gfl::String SkyboxShader::FragmentShader = R"(
#version 430 core
out vec4 FragColour;

in vec3 TexCoord;

uniform samplerCube CubeMap;

void main()
{
	FragColour = texture(CubeMap, TexCoord);
}
)";

Gfl::String FloorShader::VertexShader = R"(
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

uniform vec3 ViewPos;

void main()
{
	//vec4 FixedPos = Projection * View * vec4(aPos.x, aPos.y - ViewPos.y, aPos.z, 1.0f);
	//TexCoord = normalize(FixedPos.xz);
	TexCoord = aTexCoord;
	gl_Position = Projection * View * Model * vec4(aPos, 1.0f);
})";


Gfl::String FloorShader::FragmentShader = R"(
#version 430 core
out vec4 FragColour;

in vec2 TexCoord;

uniform float Near;
uniform float Far;
uniform vec2 ScaleFactor;
uniform sampler2D FloorTexture;

float LinearizeDepth(float Depth)
{
	// Back to NDC.
	float z = Depth * 2.0f - 1.0f;
	return (2.0f * Near * Far) / (Far + Near - z * (Far - Near));
}

void main()
{
	//float Blend = LinearizeDepth(gl_FragCoord.z) / Far;
	//Blend = 1.0f - Blend;
	//Blend = pow(Blend, 50.0f);
	//vec2 Scale = ScaleFactor * 0.5f;
	//vec4 Colour = texture(FloorTexture, TexCoord * Scale).rgba;
	//FragColour = mix(vec4(0.169f, 0.169f, 0.169f, 1.0f), Colour, Blend);
	FragColour = texture(FloorTexture, TexCoord);
})";