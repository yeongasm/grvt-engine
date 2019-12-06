#pragma once
#ifndef GRAVITY_RENDER_FOUNDATION
#define GRAVITY_RENDER_FOUNDATION

#include "Minimal.h"
#include "Framework/Abstraction/Material.h"
#include "Framework/Foundation/Foundations.h"
#include "RenderCache.h"

namespace Grvt
{

	/**
	* GrvtRenderNode data structure.
	*
	* A render node is the most basic unit required to render an object onto the screen.
	* @param Mode		- The mode that this node will be rendered with. E.g: GL_TRIANGLES, GL_LINES, etc.
	*/
	struct GrvtRenderNode
	{
		GrvtMaterial* Material;
		ObjHandle* Handle;
		size_t			Amount;
		uint32			Size;
		uint32			Mode;

		GrvtRenderNode();
		~GrvtRenderNode();

		GrvtRenderNode(const GrvtRenderNode& Other);
		GrvtRenderNode& operator= (const GrvtRenderNode& Other);

		GrvtRenderNode(GrvtRenderNode&& Other);
		GrvtRenderNode& operator= (GrvtRenderNode&& Other);
	};


	struct RenderState;


	/**
	* GrvtRenderCommand data structure.
	*
	* A render command represent a "model" that will be rendered onto the screen.
	*
	* @param Nodes		- An array of GrvtRenderNode.
	* @param Instances	- An array of 4x4 matrices that represents the model matrix of each instance.
	* @param Model		- For render commands that are not instanced, this would represent the object's model matrix.
	*/
	struct GrvtRenderCommand
	{
		Gfl::Array<GrvtRenderNode>	Nodes;
		glm::mat4					Transform;
		RenderState					State;

		GrvtRenderCommand();
		~GrvtRenderCommand();

		GrvtRenderCommand(const GrvtRenderCommand& Other);
		GrvtRenderCommand& operator= (const GrvtRenderCommand& Other);

		GrvtRenderCommand(GrvtRenderCommand&& Other);
		GrvtRenderCommand& operator= (GrvtRenderCommand&& Other);
	};


	/**
	* GrvtRenderTarget data structure.
	*/
	struct GrvtRenderTarget
	{
		ObjHandle* Handle;
		uint32		Width;
		uint32		Height;
		uint8		AttachmentBitMask;

		GrvtRenderTarget();
		~GrvtRenderTarget();

		GrvtRenderTarget(const GrvtRenderTarget& Other);
		GrvtRenderTarget& operator= (const GrvtRenderTarget& Other);

		GrvtRenderTarget(GrvtRenderTarget&& Other);
		GrvtRenderTarget& operator= (GrvtRenderTarget&& Other);
	};


	/**
	* GrvtRenderBuffer data structure.
	*
	*/
	struct GrvtCommandBuffer
	{
		glm::mat4						ViewProjection;
		Gfl::Array<glm::mat4>			Lights;
		Gfl::Array<GrvtRenderCommand>	RenderCommands;
		glm::vec2						ViewportSize;


		GrvtCommandBuffer();
		~GrvtCommandBuffer();

		GrvtCommandBuffer(const GrvtCommandBuffer& Other);
		GrvtCommandBuffer& operator= (const GrvtCommandBuffer& Other);

		GrvtCommandBuffer(GrvtCommandBuffer&& Other);
		GrvtCommandBuffer& operator= (GrvtCommandBuffer&& Other);
	};

}


#endif // !GRAVITY_RENDER_FOUNDATION