#pragma once

#ifndef GRAVITY_DEFERRED_RENDERER
#define GRAVITY_DEFERRED_RENDERER

#include "Renderer.h"

namespace Grvt
{

	class DeferredPBR : public BaseRenderer
	{
	private:

		/**
		* Stores the index of the draw calls.
		*/
		Gfl::Array<uint32> SortedCommands;
		Gfl::Array<uint32> SortedInstancedCommands;

		void RenderMesh(RenderNode* Node);
		void SortCommand(const Gfl::Array<RenderCommand>& Commands, Gfl::Array<uint32>& SortedCommand);

	public:

		ENGINE_API DeferredPBR();
		ENGINE_API ~DeferredPBR();

		DeferredPBR(const DeferredPBR& Other)				= delete;
		DeferredPBR& operator= (const DeferredPBR& Other)	= delete;

		DeferredPBR(DeferredPBR&& Other)					= delete;
		DeferredPBR& operator= (DeferredPBR&& Other)		= delete;

		/**
		* Initialises the renderer.
		*/
		void Init();

		/**
		* Shuts down the renderer.
		*/
		void Shutdown();

		/**
		* Renders the scene that is pushed into the renderer.
		*/
		void Render();


	};

}

#endif // !GRAVITY_DEFERRED_RENDERER