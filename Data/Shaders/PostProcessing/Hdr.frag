#version 430 core

out vec4 FragColour;

in vec2 TexCoord;

layout (location = 0) uniform sampler2D HDRBuffer;
layout (location = 1) uniform sampler2D BloomKernel;
layout (location = 2) uniform float Exposure;
layout (location = 3) uniform float Gamma;
layout (location = 4) uniform float Bloom;

void main()
{
	vec3 HDRColour = texture(HDRBuffer, TexCoord).rgb;
	vec3 BloomColour = texture(BloomKernel, TexCoord).rgb;

	if (Bloom)
	{
		HDRColour += BloomColour;
	}

	// Exposure tone mapping
	vec3 Mapped = vec3(1.0f) - exp(-HDRColour * Exposure);

	// Gamma correction
	Mapped = pow(Mapped, vec3(1.0f / Gamma));

	FragColour = vec4(Mapped, 1.0f);
}