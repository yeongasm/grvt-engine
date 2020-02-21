#pragma once

#ifndef GRAVITY_BASE_RENDERER
#define GRAVITY_BASE_RENDERER

#include "API/Graphics/GraphicsDriver.h"
#include "Framework/Scene.h"
#include "Framework/Material.h"

#include "RenderFoundation.h"
#include "RenderCache.h"
#include "PostProcess.h"

#include "Core/Engine.h"

namespace Grvt
{

	/**
	* GrvtRenderer data structure.
	*
	* GrvtRenderer is an abstract class that acts like a shell for any custom implemented renderer.
	*/
	class GrvtRenderer
	{
	private:

		friend class	GrvtEngine;
		friend struct	CommandBuffer;
		friend class	GrvtScene;
		friend class	PostProcessing;

		friend ENGINE_API void ExecuteEngine();

		Gfl::Array<size_t> UnsortedCommands;
		Gfl::Array<size_t> UnsortedInstancedCommands;
		Gfl::Array<size_t> SortedCommands;
		Gfl::Array<size_t> SortedInstancedCommands;

		PostProcessing	PostProcess;
		RenderTarget	GBuffer;
		RenderCache		StateCache;

		CommandBuffer	BackBuffer;
		CommandBuffer	FrontBuffer;

		GraphicsDriver* GfxDriver;

		GrvtShader*		ActiveShader;
		GrvtMesh*		ScreenQuad;

		GrvtShader*		DeferredPassShader;
		GrvtShader*		DeferredDirectionalShader;
		GrvtShader*		DeferredPointShader;

		GfxHandle		ProjectionViewUBO;

		void SortCommand(const Gfl::Array<RenderCommand>& Commands, Gfl::Array<size_t>& SortedCommands);

		void RenderMesh(RenderNode& Node);
		void RenderMesh(GrvtMesh& Mesh);

		void UpdateMaterial(GrvtMaterial* Material);

		void RenderPushedCommand(const RenderCommand& Command, bool UpdateState);

	public:

		uint32			PosX;
		uint32			PosY;
		uint32			Width;
		uint32			Height;

		GrvtRenderer();
		~GrvtRenderer();

		void Init(uint32 X, uint32 Y, uint32 Width, uint32 Height, GraphicsDriver* Driver);
		void Shutdown();

		void Render();
	};

}

#endif // !GRAVITY_BASE_RENDERER