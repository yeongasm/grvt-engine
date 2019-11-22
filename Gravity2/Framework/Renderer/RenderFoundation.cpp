#include "stdafx.h"


GrvtRenderNode::GrvtRenderNode() :
	Material(nullptr), Mesh(nullptr), Amount(0), Mode(0) {}


GrvtRenderNode::GrvtRenderNode(const GrvtRenderNode& Other) { *this = Other; }


GrvtRenderNode& GrvtRenderNode::operator= (const GrvtRenderNode& Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Material	= Other.Material;
		Mesh		= Other.Mesh;
		Amount		= Other.Amount;
		Mode		= Other.Mode;
	}

	return *this;
}


GrvtRenderNode::GrvtRenderNode(GrvtRenderNode&& Other) { *this = Move(Other); }


GrvtRenderNode& GrvtRenderNode::operator= (GrvtRenderNode&& Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Material	= Other.Material;
		Mesh		= Other.Mesh;
		Amount		= Other.Amount;
		Mode		= Other.Mode;

		new (&Other) GrvtRenderNode();
	}

	return *this;
}


GrvtRenderNode::~GrvtRenderNode() {
	Material = nullptr;
	Mesh = nullptr;
	Amount = 0;
	Mode = 0;
}


GrvtRenderCommand::GrvtRenderCommand() :
	Nodes(), Instances(), Model() {}


GrvtRenderCommand::GrvtRenderCommand(const GrvtRenderCommand& Other) { *this = Other; }


GrvtRenderCommand& GrvtRenderCommand::operator= (const GrvtRenderCommand& Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Nodes		= Other.Nodes;
		Instances	= Other.Instances;
		Model		= Other.Model;
	}

	return *this;
}


GrvtRenderCommand::GrvtRenderCommand(GrvtRenderCommand&& Other) { *this = Move(Other); }


GrvtRenderCommand& GrvtRenderCommand::operator= (GrvtRenderCommand&& Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Nodes		= Other.Nodes;
		Instances	= Other.Instances;
		Model		= Other.Model;

		new (&Other) GrvtRenderCommand();
	}

	return *this;
}


GrvtRenderCommand::~GrvtRenderCommand() {
	Nodes.Release();
	Instances.Release();
	Model = glm::mat4();
}


GrvtCommandBuffer::GrvtCommandBuffer() :
	CompositeMatrix(), Lights(), RenderCommands(), InstanceCommands() {}


GrvtCommandBuffer::GrvtCommandBuffer(const GrvtCommandBuffer& Other) { *this = Other; }


GrvtCommandBuffer& GrvtCommandBuffer::operator= (const GrvtCommandBuffer& Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		CompositeMatrix		= Other.CompositeMatrix;
		Lights				= Other.Lights;
		RenderCommands		= Other.RenderCommands;
		InstanceCommands	= Other.InstanceCommands;
	}

	return *this;
}


GrvtCommandBuffer::GrvtCommandBuffer(GrvtCommandBuffer&& Other) { *this = Move(Other); }


GrvtCommandBuffer& GrvtCommandBuffer::operator= (GrvtCommandBuffer&& Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		CompositeMatrix		= Other.CompositeMatrix;
		Lights				= Other.Lights;
		RenderCommands		= Other.RenderCommands;
		InstanceCommands	= Other.InstanceCommands;

		new (&Other) GrvtCommandBuffer();
	}

	return *this;
}


GrvtCommandBuffer::~GrvtCommandBuffer() {
	CompositeMatrix = glm::mat4();
	Lights.Release();
	RenderCommands.Release();
	InstanceCommands.Release();
}