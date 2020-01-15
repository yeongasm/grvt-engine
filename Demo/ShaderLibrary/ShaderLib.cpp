#include "pch.h"
#include "ShaderLib.h"


Gfl::String TestColourShader::VertexShader = R"(
#version 430 core
layout (location = 0) in vec3 aPos;

layout (std140, binding = 0) uniform Matrices
{
	uniform mat4 Projection;
	uniform mat4 View;
};

uniform mat4 Model;

void main()
{
	gl_Position = Projection * View * Model * vec4(aPos, 1.0f);
}
)";


Gfl::String TestColourShader::FragmentShader = R"(
#version 430 core
out vec4 FragColour;

uniform vec3 Colour;
uniform float Near;
uniform float Far;

float LinearizeDepth(float Depth)
{
	// Back to NDC.
	float z = Depth * 2.0f - 1.0f;
	return (2.0f * Near * Far) / (Far + Near - z * (Far - Near));
}

void main()
{
	float Blend = LinearizeDepth(gl_FragCoord.z) / Far;
	Blend = 1.0f - Blend;
	Blend = pow(Blend, 20.0f);
	FragColour = mix(vec4(0.169f, 0.169f, 0.169f, 1.0f), vec4(Colour, 1.0f), Blend);;
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
	uniform mat4 Projection;
	uniform mat4 View;
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
	uniform mat4 Projection;
	uniform mat4 View;
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

#define MAX_LIGHTS 1000

out vec4 FragColour;

in vec2 TexCoord;
in vec3 FragWorldPos;
in vec3 Normal;

layout (std140, binding = 1) uniform LightUBO
{
	uniform int NumLights;
	mat4 Lights[MAX_LIGHTS];
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
	float Brightness = Light[0][0];

	vec3	LightPos	= vec3(Light[1][0], Light[1][1], Light[1][2]);
	vec3	LightColour = vec3(Light[2][0], Light[2][1], Light[2][2]) * Brightness;
	vec3	LightDir	= normalize(LightPos - FragWorldPos);
	vec3	ReflectDir	= reflect(-LightDir, Normal);
	
	float Diff = max(dot(LightDir, Normal), 0);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0f), Shininess);

	vec3 Ambient  = LightColour * (texture(FloorTexture, TexCoord * Scale).rgb * 1.0f);
	vec3 Diffused = LightColour * (Diff * texture(FloorTexture, TexCoord * Scale).rgb);
	vec3 Specular = LightColour * 0.5f * (Spec * texture(FloorTexture, TexCoord * Scale).rgb);
	
	return (Ambient + Diffused + Specular) * Light[3][0];
};

vec3 CalcPointLighting(mat4 Light, vec3 Normal, vec3 ViewDir)
{
	float Brightness = Light[0][0];

	vec3	LightPos	= vec3(Light[1][0], Light[1][1], Light[1][2]);
	vec3	LightColour = vec3(Light[2][0], Light[2][1], Light[2][2]) * Brightness;
	vec3	LightDir	= normalize(LightPos - FragWorldPos);
	vec3	ReflectDir	= reflect(-LightDir, Normal);

	float Diff = max(dot(LightDir, Normal), 0.0f);
	float Spec = pow(max(dot(ViewDir, Normal), 0.0f), Shininess);

	// Attenuation
	float Distance = length(LightPos - FragWorldPos);
	float Attenuation = 1.0f / (Light[0][1] + Light[0][2] * Distance + Light[0][3] * (Distance * Distance));

	vec3 Ambient  = LightColour * (texture(FloorTexture, TexCoord * Scale).rgb * 1.0f);
	vec3 Diffused = LightColour * (Diff * texture(FloorTexture, TexCoord * Scale).rgb);
	vec3 Specular = LightColour * 0.5f * (Spec * texture(FloorTexture, TexCoord * Scale).rgb);

	Ambient  *= Attenuation;
	Diffused *= Attenuation;
	Specular *= Attenuation;

	return (Ambient + Diffused + Specular) * Light[3][1];
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
	
	for (int i = 0; i < NumLights; i++)
	{
		Colour += CalcDirectionalLighting(Lights[i], Norm, ViewDirection);
		Colour += CalcPointLighting(Lights[i], Norm, ViewDirection);
	}

	FragColour = mix(vec4(0.169f, 0.169f, 0.169f, 1.0f), vec4(Colour, 1.0f), Blend);
})";