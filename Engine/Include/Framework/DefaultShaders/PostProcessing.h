#pragma once

#ifndef GRAVITY_DEFAULTSHADER_POSTPROCESSING
#define GRAVITY_DEFAULTSHADER_POSTPROCESSING


#include "Library/GrvtString.h"


struct HDRBloomShader
{
	static Gfl::String VertexShader;
	static Gfl::String FragmentShader;
};


struct BloomVerticalKernel
{
	static Gfl::String VertexShader;
	static Gfl::String FragmentShader;
};


struct BloomHorizontalKernel
{
	static Gfl::String VertexShader;
	static Gfl::String FragmentShader;
};


#endif // !GRAVITY_DEFAULTSHADER_POSTPROCESSING