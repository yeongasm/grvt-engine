#pragma once

#ifndef GRAVITY_BASE_RENDERER
#define GRAVITY_BASE_RENDERER

#include "API/Graphics/GraphicsDriver.h"
#include "RenderFoundation.h"
#include "RenderCache.h"

#include "MaterialLibrary.h"
#include "PostProcess.h"

#include "Framework/Scene.h"

namespace Grvt
{

	/**
	* GrvtRenderer data structure.
	*
	* GrvtRenderer is an abstract class that acts like a shell for any custom implemented renderer.
	*/
	class Renderer
	{
	private:

		friend class	GrvtEngine;
		friend struct	CommandBuffer;
		friend class	GrvtScene;

		Gfl::Array<size_t> UnsortedCommands;
		Gfl::Array<size_t> UnsortedInstancedCommands;
		Gfl::Array<size_t> SortedCommands;
		Gfl::Array<size_t> SortedInstancedCommands;

		MaterialLibrary	MaterialLib;
		PostProcessing	PostProcess;
		RenderTarget	GBuffer;
		RenderCache		StateCache;

		CommandBuffer	BackBuffer;
		CommandBuffer	FrontBuffer;

		GrvtShader*		ActiveShader;
		GrvtMesh*		ScreenQuad;

		GfxHandle		ProjectionViewUBO;

	public:

		uint32			PosX;
		uint32			PosY;
		uint32			Width;
		uint32			Height;
		
	public:

		Renderer();
		~Renderer();

		void Init();
		void Shutdown();

		void Render(const GraphicsDriver* GlDriver);

		void CreateRenderCommandBuffer(GrvtScene* ActiveScene);
	};

}

#endif // !GRAVITY_BASE_RENDERER