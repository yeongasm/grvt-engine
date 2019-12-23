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
uniform mat4 Projection;\n\
uniform mat4 View;\n\
uniform mat4 Model;\n\
void main()\n\
{\n\
	gl_Position = Projection * View * Model * vec4(aPos, 1.0f);\n\
}\n\
";

Gfl::String TestShader::FragmentShader = "\
#version 430 core\n\
out vec4 FragColour;\n\
\n\
void main()\n\
{\n\
	FragColour = vec4(1.0f);\n\
}\n\
";