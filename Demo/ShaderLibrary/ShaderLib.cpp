#include "pch.h"
#include "ShaderLib.h"


Gfl::String TestShader::VertexShader = "\
#version 430 core\n\
layout (location = 0) in vec3 aPos;\n\
layout (location = 1) in vec3 aNormal;\n\
layout (location = 2) in vec2 aTexCoord;\n\
layout (location = 3) in vec3 aTangent;\n\
layout (location = 4) in vec3 aBitangent;\n\
\n\
out vec2 TexCoord;\n\
\n\
uniform mat4 Projection;\n\
uniform mat4 View;\n\
uniform mat4 Model;\n\
void main()\n\
{\n\
	TexCoord = aTexCoord;\n\
	gl_Position = Projection * View * Model * vec4(aPos, 1.0f);\n\
}\n\
";

Gfl::String TestShader::FragmentShader = "\
#version 430 core\n\
out vec4 FragColour;\n\
in vec2 TexCoord;\n\
\n\
uniform vec3 Colour;\n\
uniform sampler2D Albedo;\n\
\n\
void main()\n\
{\n\
	FragColour = texture(Albedo, TexCoord);\n\
}\n\
";

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

void main()
{
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
	float Alpha = LinearizeDepth(gl_FragCoord.z) / Far;
	Alpha = 1.0f - Alpha;
	vec2 Scale = ScaleFactor * 0.5f;
	vec3 Colour = texture(FloorTexture, TexCoord * Scale).rgb;
	FragColour = vec4(Colour, Alpha * 2.0f);
})";