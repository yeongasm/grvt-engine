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