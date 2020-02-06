#include "pch.h"
#include "ShaderLib.h"


Gfl::String TestColourShader::VertexShader = R"(
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT
{
	vec3 Normal;
	vec3 FragWorldPos;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
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
	vs_out.FragPosLightSpace = LightSpaceMatrix * vec4(vs_out.FragWorldPos, 1.0f);
	
	gl_Position = Projection * View * Model * vec4(aPos, 1.0f);
}
)";


Gfl::String TestColourShader::FragmentShader = R"(
#version 430 core

#define MAX_LIGHTS 15

out vec4 FragColour;

in VS_OUT
{
	vec3 Normal;
	vec3 FragWorldPos;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
} fs_in;

uniform float Near;
uniform float Far;
uniform float Shininess;
uniform vec3  Colour;
uniform vec3  ViewPos;

uniform sampler2D DepthMap;
uniform samplerCube OmniDepthMaps[MAX_LIGHTS];

layout (std140, binding = 1) uniform LightUBO
{
	int  TotalPointLight;
	mat4 DirectionalLight;
	mat4 PointLights[MAX_LIGHTS];
};

// Array of offset direction for sampling
vec3 GridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float LinearizeDepth(float Depth)
{
	// Back to NDC.
	float z = Depth * 2.0f - 1.0f;
	return (2.0f * Near * Far) / (Far + Near - z * (Far - Near));
};

float DShadowCalculation(vec4 FragPosLightSpace)
{
	// Perform perspective divide.
	vec3 ProjCoord = FragPosLightSpace.xyz / FragPosLightSpace.w;
	
	// Transform to [0, 1] range.
	ProjCoord = ProjCoord * 0.5f + 0.5f;
	
	// Get closes depth value from light's perspective (using [0, 1] range FragPosLight as coodinates).
	float ClosestDepth = texture(DepthMap, ProjCoord.xy).r;
	// Get depth of current fragment from light's perspective.
	float CurrentDepth = ProjCoord.z;

	vec3 Normal = normalize(fs_in.Normal);
	vec3 LightDir = normalize(-vec3(DirectionalLight[1][0], DirectionalLight[1][1], DirectionalLight[1][2]));
	float Bias = max(0.05f * (1.0f - dot(Normal, LightDir)), DirectionalLight[1][3]); 

	float Shadow = 0.0f;
	vec2 TexelSize = 1.0f / textureSize(DepthMap, 0);

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float PCFDepth = texture(DepthMap, ProjCoord.xy + vec2(x, y) * TexelSize).r;
			Shadow += CurrentDepth - Bias > PCFDepth ? 1.0f : 0.0f;
		}
	}

	Shadow /= 9.0f;

	if (ProjCoord.z > 1.0f)
		Shadow = 0.0f;
	
	return Shadow;
};

float OShadowCalculation(int Index, vec3 LightPos, float FarPlane)
{
	vec3 FragToLight = fs_in.FragWorldPos - LightPos;
	//float ClosestDepth = texture(OmniDepthMaps[Index], FragToLight).r;
	//ClosestDepth *= FarPlane;
	float CurrentDepth = length(FragToLight);
	float Bias = 0.15f;
	float Shadow = 0.0f;
	
	int Samples = 20;

	float ViewDistance = length(ViewPos - fs_in.FragWorldPos);
	float DiskRadius = (1.0f + (ViewDistance / FarPlane)) / 25.0f;

	for (int i = 0; i < Samples; i++)
	{
		float ClosestDepth = texture(OmniDepthMaps[Index], FragToLight + GridSamplingDisk[i] * DiskRadius).r;
		ClosestDepth *= FarPlane;
		
		if (CurrentDepth - Bias > ClosestDepth)
			Shadow += 1.0f;
	}
	Shadow /= float(Samples);

	return Shadow;
};

vec3 CalcDirectionalLighting(vec3 Normal, vec3 ViewDir)
{
	float	Brightness  = DirectionalLight[0][0];
	vec3	LightDir	= vec3(DirectionalLight[1][0], DirectionalLight[1][1], DirectionalLight[1][2]);
	vec3	LightColour = vec3(DirectionalLight[2][0], DirectionalLight[2][1], DirectionalLight[2][2]);
	vec3	HalfWayDir	= normalize(LightDir + ViewDir);
	
	LightDir = normalize(-LightDir);

	float Diff = max(dot(LightDir, Normal), 0);
	float Spec = pow(max(dot(Normal, HalfWayDir), 0.0f), Shininess);

	vec3 Ambient  = LightColour * Colour * 0.3f;
	vec3 Diffused = LightColour * Diff * Colour;
	vec3 Specular = LightColour * Spec * Colour;

	Ambient  *= Brightness;
	Diffused *= Brightness;
	Specular *= Brightness;
	
	float Shadow = DShadowCalculation(fs_in.FragPosLightSpace);
	return (Ambient + (1.0f - Shadow) * (Diffused + Specular)) * Colour;
};

vec3 CalcPointLighting(int Index, mat4 Light, vec3 Normal, vec3 ViewDir)
{
	float	Brightness	= Light[0][0];
	vec3	LightPos	= vec3(Light[1][0], Light[1][1], Light[1][2]);
	vec3	LightColour = vec3(Light[2][0], Light[2][1], Light[2][2]);
	vec3	LightDir	= normalize(LightPos - fs_in.FragWorldPos);
	vec3	HalfWayDir	= normalize(LightDir + ViewDir);

	float Diff = max(dot(LightDir, Normal), 0.0f);
	float Spec = pow(max(dot(Normal, HalfWayDir), 0.0f), Shininess);

	// Attenuation
	float Distance = length(LightPos - fs_in.FragWorldPos);
	float Attenuation = 1.0f / (Light[0][1] + Light[0][2] * Distance + Light[0][3] * (Distance * Distance));

	vec3 Ambient  = Colour * 0.3f;
	vec3 Diffused = LightColour * Diff * Colour;
	vec3 Specular = LightColour * Spec * Colour;

	Ambient  *= Attenuation;
	Diffused *= Attenuation;
	Specular *= Attenuation;

	Ambient  *= Brightness;
	Diffused *= Brightness;
	Specular *= Brightness;
	
	float Shadow = OShadowCalculation(Index, LightPos, Light[3][3]);

	return (Ambient + (1.0f - Shadow) * (Diffused + Specular)) * Colour;
};

void main()
{
	float Blend = LinearizeDepth(gl_FragCoord.z) / Far;
	Blend = 1.0f - Blend;
	Blend = pow(Blend, 25.0f);

	vec3 Norm = normalize(fs_in.Normal);
	vec3 ViewDirection = normalize(ViewPos - fs_in.FragWorldPos);
	vec3 Result;

	Result += CalcDirectionalLighting(Norm, ViewDirection);

	for (int i = 0; i < TotalPointLight; i++)
	{
		Result += CalcPointLighting(i, PointLights[i], Norm, ViewDirection);
	}

	//FragColour = mix(vec4(0.169f, 0.169f, 0.169f, 1.0f), vec4(Result, 1.0f), Blend);
	FragColour = vec4(Result, 1.0f);
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

out VS_OUT
{
	vec3 Normal;
	vec3 FragWorldPos;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
} vs_out;

layout (std140, binding = 0) uniform MatricesUBO
{
	mat4 Projection;
	mat4 View;
	mat4 LightSpaceMatrix;
};

uniform mat4 Model;
uniform mat3 TrInvModel;

uniform vec3 ViewPos;
uniform vec2 ScaleFactor;

void main()
{
	vs_out.Normal = TrInvModel * aNormal;
	vs_out.FragWorldPos = vec3(Model * vec4(aPos, 1.0f));
	vs_out.TexCoord = aTexCoord.xy + vec2(ViewPos.x, -ViewPos.z) * (1.0f / ScaleFactor * 0.05f);
	vs_out.FragPosLightSpace = LightSpaceMatrix * vec4(vs_out.FragWorldPos, 1.0f);

	gl_Position = Projection * View * Model * vec4(aPos, 1.0f);
})";


Gfl::String FloorShader::FragmentShader = R"(
#version 430 core

#define MAX_LIGHTS 15

out vec4 FragColour;

in VS_OUT
{
	vec3 Normal;
	vec3 FragWorldPos;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
} fs_in;

layout (std140, binding = 1) uniform LightUBO
{
	int TotalPointLight;
	mat4 DirectionalLight;
	mat4 PointLights[MAX_LIGHTS];
};

uniform sampler2D DepthMap;
uniform samplerCube OmniDepthMaps[MAX_LIGHTS];

uniform float Near;
uniform float Far;
uniform float Shininess;
uniform vec3  ViewPos;
uniform vec2  ScaleFactor;

uniform sampler2D FloorTexture;

vec2 Scale;

// Array of offset direction for sampling
vec3 GridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float LinearizeDepth(float Depth)
{
	// Back to NDC.
	float z = Depth * 2.0f - 1.0f;
	return (2.0f * Near * Far) / (Far + Near - z * (Far - Near));
};

float DShadowCalculation(vec4 FragPosLightSpace)
{
	// Perform perspective divide.
	vec3 ProjCoord = FragPosLightSpace.xyz / FragPosLightSpace.w;
	
	// Transform to [0, 1] range.
	ProjCoord = ProjCoord * 0.5f + 0.5f;
	
	// Get closes depth value from light's perspective (using [0, 1] range FragPosLight as coodinates).
	float ClosestDepth = texture(DepthMap, ProjCoord.xy).r;
	// Get depth of current fragment from light's perspective.
	float CurrentDepth = ProjCoord.z;

	vec3 Normal = normalize(fs_in.Normal);
	vec3 LightDir = normalize(-vec3(DirectionalLight[1][0], DirectionalLight[1][1], DirectionalLight[1][2]));
	float Bias = max(0.05f * (1.0f - dot(Normal, LightDir)), DirectionalLight[1][3]); 

	float Shadow = 0.0f;
	vec2 TexelSize = 1.0f / textureSize(DepthMap, 0);

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float PCFDepth = texture(DepthMap, ProjCoord.xy + vec2(x, y) * TexelSize).r;
			Shadow += CurrentDepth - Bias > PCFDepth ? 1.0f : 0.0f;
		}
	}

	Shadow /= 9.0f;

	if (ProjCoord.z > 1.0f)
		Shadow = 0.0f;
	
	return Shadow;
};

float OShadowCalculation(int Index, vec3 LightPos, float FarPlane)
{
	vec3 FragToLight = fs_in.FragWorldPos - LightPos;
	//float ClosestDepth = texture(OmniDepthMaps[Index], FragToLight).r;
	//ClosestDepth *= FarPlane;
	float CurrentDepth = length(FragToLight);
	float Bias = 0.15f;
	float Shadow = 0.0f;
	
	int Samples = 20;

	float ViewDistance = length(ViewPos - fs_in.FragWorldPos);
	float DiskRadius = (1.0f + (ViewDistance / FarPlane)) / 25.0f;

	for (int i = 0; i < Samples; i++)
	{
		float ClosestDepth = texture(OmniDepthMaps[Index], FragToLight + GridSamplingDisk[i] * DiskRadius).r;
		ClosestDepth *= FarPlane;
		
		if (CurrentDepth - Bias > ClosestDepth)
			Shadow += 1.0f;
	}
	Shadow /= float(Samples);

	return Shadow;
};

vec3 CalcDirectionalLighting(vec3 Normal, vec3 ViewDir)
{
	float	Brightness  = DirectionalLight[0][0];
	vec3	LightDir	= vec3(DirectionalLight[1][0], DirectionalLight[1][1], DirectionalLight[1][2]);
	vec3	LightColour = vec3(DirectionalLight[2][0], DirectionalLight[2][1], DirectionalLight[2][2]);
	vec3	HalfWayDir	= normalize(LightDir + ViewDir);
	
	LightDir = normalize(-LightDir);

	float Diff = max(dot(LightDir, Normal), 0);
	float Spec = pow(max(dot(Normal, HalfWayDir), 0.0f), Shininess);

	vec3 Ambient  = LightColour * texture(FloorTexture, fs_in.TexCoord * Scale).rgb * 0.3f;
	vec3 Diffused = LightColour * Diff * texture(FloorTexture, fs_in.TexCoord * Scale).rgb;
	vec3 Specular = LightColour * Spec * texture(FloorTexture, fs_in.TexCoord * Scale).rgb;
	
	Ambient  *= Brightness;
	Diffused *= Brightness;
	Specular *= Brightness;
	
	float Shadow = DShadowCalculation(fs_in.FragPosLightSpace);
	return (Ambient + (1.0f - Shadow) * (Diffused + Specular)) * texture(FloorTexture, fs_in.TexCoord * Scale).rgb;
};

vec3 CalcPointLighting(int Index, mat4 Light, vec3 Normal, vec3 ViewDir)
{
	float	Brightness	= Light[0][0];
	vec3	LightPos	= vec3(Light[1][0], Light[1][1], Light[1][2]);
	vec3	LightColour = vec3(Light[2][0], Light[2][1], Light[2][2]);
	vec3	LightDir	= normalize(LightPos - fs_in.FragWorldPos);
	vec3	HalfWayDir	= normalize(LightDir + ViewDir);

	float Diff = max(dot(LightDir, Normal), 0.0f);
	float Spec = pow(max(dot(Normal, HalfWayDir), 0.0f), Shininess);

	// Attenuation
	float Distance = length(LightPos - fs_in.FragWorldPos);
	float Attenuation = 1.0f / (Light[0][1] + Light[0][2] * Distance + Light[0][3] * (Distance * Distance));

	vec3 Ambient  = LightColour * texture(FloorTexture, fs_in.TexCoord * Scale).rgb * 0.3f;
	vec3 Diffused = LightColour * Diff * texture(FloorTexture, fs_in.TexCoord * Scale).rgb;
	vec3 Specular = LightColour * Spec * texture(FloorTexture, fs_in.TexCoord * Scale).rgb;

	Ambient  *= Attenuation;
	Diffused *= Attenuation;
	Specular *= Attenuation;

	Ambient  *= Brightness;
	Diffused *= Brightness;
	Specular *= Brightness;

	float Shadow = OShadowCalculation(Index, LightPos, Light[3][3]);
	//return Ambient + Diffused + Specular;	
	return (Ambient + (1.0f - Shadow) * (Diffused + Specular)) * texture(FloorTexture, fs_in.TexCoord * Scale).rgb;
};

void main()
{
	float Blend = LinearizeDepth(gl_FragCoord.z) / Far;
	Blend = 1.0f - Blend;
	Blend = pow(Blend, 25.0f);
	Scale = ScaleFactor * 5.0f;
	
	vec3 Norm = normalize(fs_in.Normal);
	vec3 ViewDirection = normalize(ViewPos - fs_in.FragWorldPos);
	vec3 Colour;

	Colour += CalcDirectionalLighting(Norm, ViewDirection);

	for (int i = 0; i < TotalPointLight; i++)
	{
		Colour += CalcPointLighting(i, PointLights[i], Norm, ViewDirection);
	}

	FragColour = vec4(Colour, 1.0f);
})";