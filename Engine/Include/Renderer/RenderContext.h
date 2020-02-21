#pragma once
#ifndef GRAVITY_RENDERER_RENDERSERVICE
#define GRAVITY_RENDERER_RENDERSERVICE

#include "API/Graphics/GraphicsInterface.h"
#include "Renderer.h"
#include "Core/Engine.h"

namespace Grvt
{

	class RenderContext
	{
	private:

		GrvtEngine*		Engine;
		GraphicsDriver*	GlDriver;

	public:

		GraphicsInterface	GlInterface;
		GrvtRenderer*		GlRenderer;

		RenderContext();
		~RenderContext();

		void Execute(GrvtEngine* EnginePtr, GLADloadproc LoadProcFunc);
	};

	RenderContext* InitialiseRenderContext();
	void ShutdownRenderContext();
	
	ENGINE_API RenderContext*		GetRenderContext();
	ENGINE_API GraphicsInterface*	GetGraphicsInterface();
	ENGINE_API GrvtRenderer*		GetRenderer();

}


#endif // !GRAVITY_RENDERER_RENDERSERVICE