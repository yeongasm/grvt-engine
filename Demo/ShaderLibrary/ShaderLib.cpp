#include "pch.h"
#include "ShaderLib.h"


Gfl::String TestColourShader::VertexShader = R"(
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragWorldPos;
out vec3 Normal;

layout (std140, binding = 0) uniform Matrices
{
	mat4 Projection;
	mat4 View;
};

uniform mat4 Model;
uniform mat3 TrInvModel;

void main()
{
	FragWorldPos = vec3(Model * vec4(aPos, 1.0f));
	Normal = TrInvModel * aNormal;
	gl_Position = Projection * View * Model * vec4(aPos, 1.0f);
}
)";


Gfl::String TestColourShader::FragmentShader = R"(
#version 430 core

#define MAX_DIR_LIGHTS 8
#define MAX_POINT_LIGHTS 1000

out vec4 FragColour;

in vec3 Normal;
in vec3 FragWorldPos;

uniform float Near;
uniform float Far;
uniform float Shininess;
uniform vec3 Colour;
uniform vec3 ViewPos;

layout (std140, binding = 1) uniform LightUBO
{
	int TotalDirLight;
	int TotalPointLight;
	mat4 DirLights[MAX_DIR_LIGHTS];
	mat4 PointLights[MAX_POINT_LIGHTS];
};

float LinearizeDepth(float Depth)
{
	// Back to NDC.
	float z = Depth * 2.0f - 1.0f;
	return (2.0f * Near * Far) / (Far + Near - z * (Far - Near));
};

vec3 CalcDirectionalLighting(mat4 Light, vec3 Normal, vec3 ViewDir)
{
	vec3	LightPos	= vec3(Light[1][0], Light[1][1], Light[1][2]);
	vec3	LightColour = vec3(Light[2][0], Light[2][1], Light[2][2]);
	vec3	LightDir	= normalize(LightPos - FragWorldPos);
	vec3	HalfWayDir	= normalize(LightDir + ViewDir);
	
	LightDir = normalize(-LightDir);

	float Diff = max(dot(LightDir, Normal), 0);
	float Spec = pow(max(dot(ViewDir, HalfWayDir), 0.0f), Shininess);

	vec3 Ambient  = LightColour * (Colour * 1.0f);
	vec3 Diffused = LightColour * (Diff * Colour);
	vec3 Specular = LightColour * 0.5f * (Spec * Colour);

	Ambient  *= Light[0][0];
	Diffused *= Light[0][0];
	Specular *= Light[0][0];
	
	return Ambient + Diffused + Specular;
};

vec3 CalcPointLighting(mat4 Light, vec3 Normal, vec3 ViewDir)
{
	vec3	LightPos	= vec3(Light[1][0], Light[1][1], Light[1][2]);
	vec3	LightColour = vec3(Light[2][0], Light[2][1], Light[2][2]);
	vec3	LightDir	= normalize(LightPos - FragWorldPos);
	vec3	HalfWayDir	= normalize(LightDir + ViewDir);

	float Diff = max(dot(LightDir, Normal), 0.0f);
	float Spec = pow(max(dot(ViewDir, HalfWayDir), 0.0f), Shininess);

	// Attenuation
	float Distance = length(LightPos - FragWorldPos);
	float Attenuation = 1.0f / (Light[0][1] + Light[0][2] * Distance + Light[0][3] * (Distance * Distance));

	vec3 Ambient  = LightColour * (Colour * 1.0f);
	vec3 Diffused = LightColour * (Diff * Colour);
	vec3 Specular = LightColour * 0.5f * (Spec * Colour);

	Ambient  *= Attenuation;
	Diffused *= Attenuation;
	Specular *= Attenuation;

	Ambient  *= Light[0][0];
	Diffused *= Light[0][0];
	Specular *= Light[0][0];

	return Ambient + Diffused + Specular;
};

void main()
{
	float Blend = LinearizeDepth(gl_FragCoord.z) / Far;
	Blend = 1.0f - Blend;
	Blend = pow(Blend, 25.0f);

	vec3 Norm = normalize(Normal);
	vec3 ViewDirection = normalize(ViewPos - FragWorldPos);
	vec3 Result;

	for (int i = 0; i < TotalDirLight; i++)
	{
		Result += CalcDirectionalLighting(DirLights[i], Norm, ViewDirection);
	}

	for (int i = 0; i < TotalPointLight; i++)
	{
		Result += CalcPointLighting(PointLights[i], Norm, ViewDirection);
	}

	FragColour = mix(vec4(0.169f, 0.169f, 0.169f, 1.0f), vec4(Result, 1.0f), Blend);
}
)";


Gfl::String TestDiffShader::VertexShader = R"(
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

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

Gfl::String TestDiffShader::FragmentShader = R"(
#version 430 core
out vec4 FragColour;
in vec2 TexCoord;

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

layout (std140, binding = 0) uniform Matrices
{
	mat4 Projection;
	mat4 View;
};

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
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragWorldPos;
out vec3 Normal;

layout (std140, binding = 0) uniform MatricesUBO
{
	mat4 Projection;
	mat4 View;
};

uniform mat4 Model;
uniform mat3 TrInvModel;

uniform vec3 ViewPos;
uniform vec2 ScaleFactor;

void main()
{
	Normal = TrInvModel * aNormal;
	FragWorldPos = vec3(Model * vec4(aPos, 1.0f));
	TexCoord = aTexCoord.xy + vec2(ViewPos.x, -ViewPos.z) * (1.0f / ScaleFactor * 0.05f);
	gl_Position = Projection * View * Model * vec4(aPos, 1.0f);
})";


Gfl::String FloorShader::FragmentShader = R"(
#version 430 core

#define MAX_DIR_LIGHTS 8
#define MAX_POINT_LIGHTS 1000

out vec4 FragColour;

in vec2 TexCoord;
in vec3 FragWorldPos;
in vec3 Normal;

layout (std140, binding = 1) uniform LightUBO
{
	int TotalDirLight;
	int TotalPointLight;
	mat4 DirLights[MAX_DIR_LIGHTS];
	mat4 PointLights[MAX_POINT_LIGHTS];
};

uniform float Near;
uniform float Far;
uniform float Shininess;
uniform vec3 ViewPos;
uniform vec2 ScaleFactor;

uniform sampler2D FloorTexture;

vec2 Scale;

float LinearizeDepth(float Depth)
{
	// Back to NDC.
	float z = Depth * 2.0f - 1.0f;
	return (2.0f * Near * Far) / (Far + Near - z * (Far - Near));
}

vec3 CalcDirectionalLighting(mat4 Light, vec3 Normal, vec3 ViewDir)
{
	vec3	LightPos	= vec3(Light[1][0], Light[1][1], Light[1][2]);
	vec3	LightColour = vec3(Light[2][0], Light[2][1], Light[2][2]);
	vec3	LightDir	= normalize(LightPos - FragWorldPos);
	vec3	HalfWayDir	= normalize(LightDir + ViewDir);
	
	LightDir = normalize(-LightDir);

	float Diff = max(dot(LightDir, Normal), 0);
	float Spec = pow(max(dot(ViewDir, HalfWayDir), 0.0f), Shininess);

	vec3 Ambient  = LightColour * (texture(FloorTexture, TexCoord * Scale).rgb * 1.0f);
	vec3 Diffused = LightColour * (Diff * texture(FloorTexture, TexCoord * Scale).rgb);
	vec3 Specular = LightColour * 0.0f * (Spec * texture(FloorTexture, TexCoord * Scale).rgb);
	
	Ambient  *= Light[0][0];
	Diffused *= Light[0][0];
	Specular *= Light[0][0];
	
	return Ambient + Diffused + Specular;
};

vec3 CalcPointLighting(mat4 Light, vec3 Normal, vec3 ViewDir)
{
	vec3	LightPos	= vec3(Light[1][0], Light[1][1], Light[1][2]);
	vec3	LightColour = vec3(Light[2][0], Light[2][1], Light[2][2]);
	vec3	LightDir	= normalize(LightPos - FragWorldPos);
	vec3	HalfWayDir	= normalize(LightDir + ViewDir);

	float Diff = max(dot(LightDir, Normal), 0.0f);
	float Spec = pow(max(dot(ViewDir, HalfWayDir), 0.0f), Shininess);

	// Attenuation
	float Distance = length(LightPos - FragWorldPos);
	float Attenuation = 1.0f / (Light[0][1] + Light[0][2] * Distance + Light[0][3] * (Distance * Distance));

	vec3 Ambient  = LightColour * (texture(FloorTexture, TexCoord * Scale).rgb * 1.0f);
	vec3 Diffused = LightColour * (Diff * texture(FloorTexture, TexCoord * Scale).rgb);
	vec3 Specular = LightColour * 0.5f * (Spec * texture(FloorTexture, TexCoord * Scale).rgb);

	Ambient  *= Attenuation;
	Diffused *= Attenuation;
	Specular *= Attenuation;

	Ambient  *= Light[0][0];
	Diffused *= Light[0][0];
	Specular *= Light[0][0];

	return Ambient + Diffused + Specular;
};

void main()
{
	float Blend = LinearizeDepth(gl_FragCoord.z) / Far;
	Blend = 1.0f - Blend;
	Blend = pow(Blend, 25.0f);
	Scale = ScaleFactor * 5.0f;
	
	vec3 Norm = normalize(Normal);
	vec3 ViewDirection = normalize(ViewPos - FragWorldPos);
	vec3 Colour;
	
	for (int i = 0; i < TotalDirLight; i++)
	{
		Colour += CalcDirectionalLighting(DirLights[i], Norm, ViewDirection);
	}

	for (int i = 0; i < TotalPointLight; i++)
	{
		Colour += CalcPointLighting(PointLights[i], Norm, ViewDirection);
	}

	FragColour = mix(vec4(0.169f, 0.169f, 0.169f, 1.0f), vec4(Colour, 1.0f), Blend);
})";