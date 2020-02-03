#include "Framework/DefaultShaders/ShadowMaps.h"


Gfl::String DDepthMapShader::VertexShader = R"(
#version 430 core
layout (location = 0) in vec3 aPos;


uniform mat4 LightSpaceTransform;
uniform mat4 Model;

void main()
{
	gl_Position = LightSpaceTransform * Model * vec4(aPos, 1.0f);
}
)";


Gfl::String DDepthMapShader::FragmentShader = R"(
#version 430 core

void main()
{
	//gl_FragDepth = gl_FragCoord.z;
}
)";


Gfl::String ODepthMapShader::VertexShader = R"(
#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 Model;

void main()
{
	gl_Position = model * vec3(aPos, 1.0f);
}
)";


Gfl::String ODepthMapShader::GeometryShader = R"(
#version 430 core
layout (triangles) in
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 ShadowMatrices[6];

out vec4 FragPos;

void main()
{
	for (int Face = 0; Face < 6; Face++)
	{
		gl_Layer = face;
		for (int i = 0; i < 3; i++)
		{
			FragPos = gl_in[i].gl_Position;
			gl_Position = ShadowMatrices[Face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}
)";


Gfl::String ODepthMapShader::FragmentShader = R"(
#version 430 core
in vec4 FragPos;

uniform vec3 LightPos;
uniform float FarPlane;

void main()
{
	float LightDistance = length(FragPos.xyz - LightPos);
	LightDistance = LightDistance / FarPlane;
	gl_FragDepth = LightDistance;
}
)";