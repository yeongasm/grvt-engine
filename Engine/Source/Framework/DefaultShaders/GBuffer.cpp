#include "Framework/DefaultShaders/GBuffer.h"


Gfl::String GBufferShader::VertexShader = R"(
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT
{
	vec3 FragWorldPos;
	vec3 Normal;
	vec2 TexCoord;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
	mat4 Projection;
	mat4 View;
	mat4 LightSpaceMatrix;
};

uniform mat3 TrInvModel;
uniform mat4 Model;

void main()
{
	vs_out.FragWorldPos = vec3(Model * vec4(aPos, 1.0f));
	vs_out.Normal = TrInvModel * aNormal;
	vs_out.TexCoord = aTexCoord;

	gl_Position = Projection * View * Model * vec4(aPos, 1.0f);
}
)";


Gfl::String GBufferShader::FragmentShader = R"(
#version 430 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

in VS_OUT
{
	vec3 FragWorldPos;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;

//uniform vec3 Colour;
//uniform sampler2D DiffusedTexture;

void main()
{
	gPosition = fs_in.FragWorldPos;
	gNormal = normalize(fs_in.Normal);
	gAlbedo = vec4(fs_in.TexCoord, 0.0f, 0.0f);
}

)";