#pragma once

#ifndef GRAVITY_BASE_RENDERER
#define GRAVITY_BASE_RENDERER

#include "RenderFoundation.h"
#include "RenderCache.h"

namespace Grvt
{

	/**
	* GrvtRenderer data structure.
	*
	* GrvtRenderer is an abstract class that acts like a shell for any custom implemented renderer.
	*/
	struct BaseRenderer
	{
		CommandBuffer	BackBuffer;
		CommandBuffer	FrontBuffer;
		RenderCache		StateCache;
		GrvtShader*		ActiveShader;
		uint32			Width;
		uint32			Height;
		

		ENGINE_API BaseRenderer();
		ENGINE_API virtual ~BaseRenderer() {};

		/**
		* Initialises the renderer.
		* Called during engine's startup.
		*/
		virtual void Init() = 0;

		/**
		* Renders data supplied onto the screen.
		* Called every tick.
		*/
		virtual void Render() = 0;

		/**
		* Releases all resource held by the engine.
		* called during engine's shutdown.
		*/
		virtual void Shutdown() = 0;
	};

	ENGINE_API	BaseRenderer*	InitRenderer(BaseRenderer* RendererPtr);
	ENGINE_API	BaseRenderer*	GetRenderer();
				void			ShutdownRenderer();

}

#endif // !GRAVITY_BASE_RENDERER