#pragma once
#ifndef GRAVITY_RENDERER_RENDERSERVICE
#define GRAVITY_RENDERER_RENDERSERVICE

#include "API/Graphics/GraphicsInterface.h"
#include "Renderer.h"
#include "Core/Engine.h"

namespace Grvt
{

	struct RenderContext
	{
		GrvtEngine* Engine;

		GraphicsDriver*		GlDriver;
		Renderer*			GlRenderer;

		GraphicsInterface	GlInterface;

		RenderContext();
		~RenderContext();

		void Execute(GrvtEngine* EnginePtr, GLADloadproc LoadProcFunc);
	};

}


#endif // !GRAVITY_RENDERER_RENDERSERVICE