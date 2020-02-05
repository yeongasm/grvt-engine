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
		Gfl::Array<size_t> UnsortedCommands;
		Gfl::Array<size_t> SortedCommands;
		Gfl::Array<size_t> UnsortedInstacedCommands;
		Gfl::Array<size_t> SortedInstancedCommands;

		glm::vec3	BGColour;

		GrvtModel*	ScreenQuad;
		GrvtShader* DepthPassShader;
		GrvtShader* OmniDepthPassShader;
		GrvtShader* SimpleDepthDebug;

		ObjHandle	ProjectionViewUBO;
		ObjHandle	LightUBO;

		/**
		* Updates the materials inside the render command.
		*/
		void UpdateMaterial(GrvtMaterial* SourceMaterial);

		/**
		* Render pushed commands.
		*/
		void RenderPushedCommand(const RenderCommand* Command, bool UpdateState);
		
		/**
		* Render the nodes in the command.
		*/
		void RenderMesh(RenderNode* Node);

		/**
		* Sort commands that are pushed into the renderer.
		*/
		void SortCommand(const Gfl::Array<RenderCommand>& Commands, Gfl::Array<size_t>& SortedCommand);

		/**
		* Render to depth map.
		*/
		void RenderDirectionalLightingPass(RenderCommand* Command);

		/**
		* Render to omni depth map.
		*/
		void RenderPointLightsPass(size_t Index, RenderCommand* Command);

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