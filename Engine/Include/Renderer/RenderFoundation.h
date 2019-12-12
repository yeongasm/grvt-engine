#pragma once

#ifndef GRAVITY_RENDER_FOUNDATION
#define GRAVITY_RENDER_FOUNDATION

#include <map>
#include "Minimal.h"
#include "Framework/Abstraction/Material.h"
#include "Framework/Foundation/Foundations.h"
#include "RenderCache.h"

namespace Grvt
{

	/**
	* RenderNode data structure.
	*
	* A render node is the most basic unit required to render an object onto the screen.
	* @param Mode		- The mode that this node will be rendered with. E.g: GL_TRIANGLES, GL_LINES, etc.
	*/
	struct RenderNode
	{
		GrvtMaterial*	Material;
		ObjHandle*		Handle;
		size_t			Amount;
		uint32			Size;
		uint32			Mode;

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
		glm::mat4				Transform;
		RenderState				State;

		RenderCommand();
		~RenderCommand();

		RenderCommand(const RenderCommand& Other);
		RenderCommand& operator= (const RenderCommand& Other);

		RenderCommand(RenderCommand&& Other);
		RenderCommand& operator= (RenderCommand&& Other);
	};


	/**
	* RenderTarget data structure.
	*/
	struct RenderTarget
	{
		ObjHandle*	Handle;
		uint32		Width;
		uint32		Height;
		uint8		AttachmentBitMask;

		RenderTarget();
		~RenderTarget();

		RenderTarget(const RenderTarget& Other);
		RenderTarget& operator= (const RenderTarget& Other);

		RenderTarget(RenderTarget&& Other);
		RenderTarget& operator= (RenderTarget&& Other);
	};


	/**
	* CommandBuffer data structure.
	*
	*/
	struct CommandBuffer
	{
		Gfl::Array<glm::mat4>		Lights;
		Gfl::Array<RenderCommand>	RenderCommands;
		Gfl::Array<RenderCommand>	InstancedCommands;
		std::map<RenderTarget, Gfl::Array<RenderCommand>> CustomCommands;

		CommandBuffer();
		~CommandBuffer();

		CommandBuffer(const CommandBuffer& Other);
		CommandBuffer& operator= (const CommandBuffer& Other);

		CommandBuffer(CommandBuffer&& Other);
		CommandBuffer& operator= (CommandBuffer&& Other);
	};

}


#endif // !GRAVITY_RENDER_FOUNDATION