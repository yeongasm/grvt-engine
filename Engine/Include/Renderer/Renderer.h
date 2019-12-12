#pragma once

#ifndef GRAVITY_BASE_RENDERER
#define GRAVITY_BASE_RENDERER

#include "RenderFoundation.h"

namespace Grvt
{

	/**
	* GrvtRenderer data structure.
	*
	* GrvtRenderer is an abstract class that acts like a shell for any custom implemented renderer.
	*/
	struct BaseRenderer
	{
		CommandBuffer			BackBuffer;
		CommandBuffer			FrontBuffer;
		GrvtShader*				ActiveShader;
		Gfl::Array<glm::mat4>	Lights;
		glm::vec2				RenderSize;
		

		BaseRenderer() {};
		virtual ~BaseRenderer() {};

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

}

#endif // !GRAVITY_BASE_RENDERER