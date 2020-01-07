#pragma once

#ifndef GRAVITY_SHADER_LIB
#define GRAVITY_SHADER_LIB

#include <Minimal.h>


struct TestColourShader
{
	static Gfl::String VertexShader;
	static Gfl::String FragmentShader;
};


struct TestDiffShader
{
	static Gfl::String VertexShader;
	static Gfl::String FragmentShader;
};


struct SkyboxShader
{
	static Gfl::String VertexShader;
	static Gfl::String FragmentShader;
};


struct FloorShader
{
	static Gfl::String VertexShader;
	static Gfl::String FragmentShader;
};


//struct BlinnShader
//{
//	static Gfl::String VertexShader;
//	static Gfl::String FragmentShader;
//};

#endif // !GRAVITY_SHADER_LIB