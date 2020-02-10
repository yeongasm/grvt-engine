#include "Framework/DefaultShaders/PostProcessing.h"


Gfl::String HDRBloomShader::VertexShader = R"(
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	gl_Position = vec4(aPos, 1.0f);
}
)";


Gfl::String HDRBloomShader::FragmentShader = R"(
#version 430 core
out vec4 FragColour;

in vec2 TexCoord;

layout (location = 0) uniform sampler2D HDRBuffer;
layout (location = 1) uniform sampler2D BloomKernel;
layout (location = 2) uniform float Exposure;
layout (location = 3) uniform float Gamma;
layout (location = 4) uniform bool Bloom;

void main()
{
	vec3 HDRColour = texture(HDRBuffer, TexCoord).rgb;
	vec3 BloomColour = texture(BloomKernel, TexCoord).rgb;

	if (Bloom)
	{
		HDRColour+= BloomColour;
	}

	// Exposure tone mapping
	vec3 Mapped = vec3(1.0f) - exp(-HDRColour * Exposure);
	// Gamme correction
	Mapped = pow(Mapped, vec3(1.0f / Gamma));

	FragColour = vec4(Mapped, 1.0f);
}
)";


Gfl::String BloomVerticalKernel::VertexShader = R"(
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	gl_Position = vec4(aPos, 1.0f);
}
)";


Gfl::String BloomVerticalKernel::FragmentShader = R"(
#version 430 core

out vec4 FragColour;

in vec2 TexCoord;

layout (location = 0) uniform sampler2D Image;
uniform float Weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
	// Gets size of a single texel.
	vec2 TexOffset = 1.0f / textureSize(Image, 0);
	vec3 Result = texture(Image, TexCoord).rgb * Weight[0];

	// NOTE(Afiq): Samples should be a uniform!
	for (int i = 0; i < 5; i++)
	{
		Result += texture(Image, TexCoord + vec2(TexOffset.x * i, 0.0f)).rgb * Weight[i];
		Result += texture(Image, TexCoord - vec2(TexOffset.x * i, 0.0f)).rgb * Weight[i];
	}

	FragColour = vec4(Result, 1.0f);
}
)";


Gfl::String BloomHorizontalKernel::VertexShader = R"(
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	gl_Position = vec4(aPos, 1.0f);
}
)";


Gfl::String BloomHorizontalKernel::FragmentShader = R"(
#version 430 core

out vec4 FragColour;

in vec2 TexCoord;

uniform sampler2D Image;
uniform float Weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
	// Gets size of a single texel.
	vec2 TexOffset = 1.0f / textureSize(Image, 0);
	vec3 Result = texture(Image, TexCoord).rgb * Weight[0];

	// NOTE(Afiq): Samples should be a uniform!
	for (int i = 0; i < 5; i++)
	{
		Result += texture(Image, TexCoord + vec2(0.0f, TexOffset.y * i)).rgb * Weight[i];
		Result += texture(Image, TexCoord - vec2(0.0f, TexOffset.y * i)).rgb * Weight[i];
	}

	FragColour = vec4(Result, 1.0f);
}
)";