#pragma once

#ifndef GRAVITY_DEFAULTSHADER_SHADOWMAPS
#define GRAVITY_DEFAULTSHADER_SHADOWMAPS

#include "Library/GrvtString.h"


struct DDepthMapShader
{
	static Gfl::String VertexShader;
	static Gfl::String FragmentShader;
};


struct ODepthMapShader
{
	static Gfl::String VertexShader;
	static Gfl::String GeometryShader;
	static Gfl::String FragmentShader;
};


#endif // !GRAVITY_DEFAULTSHADER_SHADOWMAPS