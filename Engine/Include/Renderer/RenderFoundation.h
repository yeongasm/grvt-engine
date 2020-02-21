#pragma once

#ifndef GRAVITY_RENDER_FOUNDATION
#define GRAVITY_RENDER_FOUNDATION

#include "Minimal.h"
#include "API/Graphics/GraphicsDriver.h"

#include "Framework/Material.h"
#include "RenderCache.h"

namespace Grvt
{

	/**
	* RenderNode data structure.
	*
	* A render node is the most basic unit required to render an object onto the screen.
	*/
	struct RenderNode
	{
		GfxHandle*		Handle;
		size_t			Amount;
		uint32			Size;
		uint32			Mode;
		bool			Indexed;

		RenderNode();
		~RenderNode();

		RenderNode(const RenderNode& Other);
		RenderNode& operator= (const RenderNode& Other);

		RenderNode(RenderNode&& Other);
		RenderNode& operator= (RenderNode&& Other);
	};


	struct RenderState;


	/**
	* RenderCommand data structure.
	*
	* A render command represent a "model" that will be rendered onto the screen.
	*
	* @param Nodes		- An array of RenderNode.
	* @param Instances	- An array of 4x4 matrices that represents the model matrix of each instance.
	* @param Model		- For render commands that are not instanced, this would represent the object's model matrix.
	*/
	struct RenderCommand
	{
		Gfl::Array<RenderNode>	Nodes;
		Gfl::Array<glm::mat4>	Instances;
		glm::mat4				Transform;
		RenderState				State;
		GrvtMaterial*			Material;
		bool					Sort;

		RenderCommand();
		~RenderCommand();

		RenderCommand(const RenderCommand& Other);
		RenderCommand& operator= (const RenderCommand& Other);

		RenderCommand(RenderCommand&& Other);
		RenderCommand& operator= (RenderCommand&& Other);

		void Empty();
	};


	enum RenderTargetAttachPoints : uint32
	{
		RenderTarget_AttachPoint_None			= 0x00,
		RenderTarget_AttachPoint_Depth			= 0x01,
		RenderTarget_AttachPoint_DepthStencil	= 0x02,
		RenderTarget_AttachPoint_Colour			= 0x03
	};

	/**
	* RenderTarget data structure.
	* Width and Height will follow renderer's dimensions when it is not specified.
	*/
	struct RenderTarget
	{
		using AttachmentPoint  = Gfl::Pair<uint32, GfxHandle>;
		using AttachmentPoints = Gfl::Array<AttachmentPoint>;

		AttachmentPoints ColourAttachments;
		AttachmentPoint	 DepthAttachment;
		AttachmentPoint	 DepthStencilAttachment;

		GfxHandle	Handle;
		uint32		Width;
		uint32		Height;

		RenderTarget();
		~RenderTarget();

		RenderTarget(RenderTarget&& Other);
		RenderTarget& operator= (RenderTarget&& Other);
	};


	/**
	* CommandBuffer data structure.
	*
	*/
	struct CommandBuffer
	{
		glm::mat4 Projection;
		glm::mat4 View;

		glm::mat4		DirectionalLight;
		glm::mat4		DirLightSpaceTransform;
		RenderTarget*	DepthMap;

		Gfl::Array<glm::mat4>		PointLights;
		std::deque<glm::mat4>		PointLightSpaceTransforms;
		Gfl::Array<RenderTarget*>	OmniDepthMaps;

		Gfl::Array<RenderCommand>	RenderCommands;
		Gfl::Array<RenderCommand>	InstancedCommands;
		Gfl::Array<RenderCommand>	ShadowCommands;

		RenderCommand				SkyBox;				// Skyboxes will be rendered last. Where it stands in a deferred pipeline? Probably last as well...
		
		bool IsEmpty;

		ENGINE_API CommandBuffer();
		ENGINE_API ~CommandBuffer();

		CommandBuffer(const CommandBuffer& Other);
		CommandBuffer& operator= (const CommandBuffer& Other);

		CommandBuffer(CommandBuffer&& Other);
		CommandBuffer& operator= (CommandBuffer&& Other);

		void Init();
		void Free();
		void Clear();
	};

}


#endif // !GRAVITY_RENDER_FOUNDATION