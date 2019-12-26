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
uniform vec3 Colour;\n\
\n\
void main()\n\
{\n\
	FragColour = vec4(Colour, 1.0f);\n\
}\n\
";


Gfl::String FloorShader::VertexShader = R"(
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
	TexCoord = aTexCoord;
	FragPos  = vec3(Model * vec4(aPos, 1.0f));
	gl_Position = Projection * View * Model * vec4(aPos, 1.0f);
})";


//Gfl::String FloorShader::FragmentShader = R"(
//#version 430 core
//out vec4 FragColour;
//
//uniform vec3 GridColour;
//uniform vec3 ViewPos;
//
//in vec2 TexCoord;
//in vec3 FragPos;
//
//
//})";


Gfl::String FloorShader::FragmentShader = R"(
#version 430 core
out vec4 FragColour;

uniform vec3 GridColour;
uniform vec3 ViewPos;

in vec2 TexCoord;
in vec3 FragPos;

const float Divisions = 1000.0f;
const float LineWidth = 1.0f;
const float Step = 100.0f;
const float SubDivisions = 4.0f;

vec4 CalcGrid(float Div)
{
	// Pick a coordinate to visualize in a grid.
	vec2 Coord = TexCoord * Div;

	// Compute anti-aliased world space grids.
	vec2 Grid = abs(fract(Coord - 0.5f) - 0.5f) / fwidth(Coord);
	float Line = min(Grid.x, Grid.y);
	float LineResult = LineWidth - min(Line, LineWidth);

	// Visualize the grid lines directly.
	vec3 FakeViewPos = ViewPos;
	FakeViewPos.y = 0.0f;
	
	return vec4(vec3(LineResult) * GridColour, 0.05 * LineResult); 
};

void main()
{
	float Divs = Divisions / pow(2, round((abs(ViewPos.z) - Step / SubDivisions) / Step));
	vec4 Grid1 = CalcGrid(Divs) + CalcGrid(Divs / SubDivisions);

	Divs = Divisions / pow(2, round((abs(ViewPos.z + 50.0f) - Step / SubDivisions) / Step));
	vec4 Grid2 = CalcGrid(Divs) + CalcGrid(Divs / SubDivisions);

	//float Alpha = mod(abs(ViewPos.y), Step);
	float Alpha = 0.0f;

	FragColour = mix(Grid1, Grid2, Alpha);

	//vec3 PseudoViewPos = vec3(ViewPos.x, FragPos.y, ViewPos.z);
	//float DistanceToCamera = max(distance(FragPos, PseudoViewPos) - abs(ViewPos.y), 0);
	float DistanceFromCamera = distance(FragPos, ViewPos);	

	float AlphaDecreaseDistance = 20.0f;
	float DecreaseDistance = 30.0f;
	
	//if (DistanceFromCamera > AlphaDecreaseDistance)
	//{
		float NormalisedDistanceCamera = clamp(DistanceFromCamera - AlphaDecreaseDistance, 0.0f, DecreaseDistance) / DecreaseDistance;
		//FragColour.a = 0.0f;
		FragColour.a *= clamp(1.0f - NormalisedDistanceCamera, 0.0f, 1.0f);
	//}
})";