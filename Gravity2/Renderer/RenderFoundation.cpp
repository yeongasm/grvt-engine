#include "stdafx.h"


GrvtRenderNode::GrvtRenderNode() :
	Material(nullptr), Handle(nullptr), Amount(0), Size(0), Mode(0) {}


GrvtRenderNode::GrvtRenderNode(const GrvtRenderNode& Other) { *this = Other; }


GrvtRenderNode& GrvtRenderNode::operator= (const GrvtRenderNode& Other) 
{
	_ASSERTE(this != &Other);

	if (this != &Other) 
	{
		Material	= Other.Material;
		Handle		= Other.Handle;
		Amount		= Other.Amount;
		Size		= Other.Size;
		Mode		= Other.Mode;
	}

	return *this;
}


GrvtRenderNode::GrvtRenderNode(GrvtRenderNode&& Other) { *this = Gfl::Move(Other); }


GrvtRenderNode& GrvtRenderNode::operator= (GrvtRenderNode&& Other) 
{
	_ASSERTE(this != &Other);

	if (this != &Other) 
	{
		Material	= Other.Material;
		Handle		= Other.Handle;
		Amount		= Other.Amount;
		Size		= Other.Size;
		Mode		= Other.Mode;

		new (&Other) GrvtRenderNode();
	}

	return *this;
}


GrvtRenderNode::~GrvtRenderNode() 
{
	Material = nullptr;
	Handle = nullptr;
	Amount = 0;
	Size = 0;
	Mode = 0;
}


GrvtRenderCommand::GrvtRenderCommand() :
	Nodes(), Instances(), Transform() {}


GrvtRenderCommand::GrvtRenderCommand(const GrvtRenderCommand& Other) { *this = Other; }


GrvtRenderCommand& GrvtRenderCommand::operator= (const GrvtRenderCommand& Other) 
{
	_ASSERTE(this != &Other);

	if (this != &Other) 
	{
		Nodes		= Other.Nodes;
		Instances	= Other.Instances;
		Transform	= Other.Transform;
	}

	return *this;
}


GrvtRenderCommand::GrvtRenderCommand(GrvtRenderCommand&& Other) { *this = Gfl::Move(Other); }


GrvtRenderCommand& GrvtRenderCommand::operator= (GrvtRenderCommand&& Other) 
{
	_ASSERTE(this != &Other);

	if (this != &Other) 
	{
		Nodes		= Other.Nodes;
		Instances	= Other.Instances;
		Transform	= Other.Transform;

		new (&Other) GrvtRenderCommand();
	}

	return *this;
}


GrvtRenderCommand::~GrvtRenderCommand() 
{
	Nodes.Release();
	Instances.Release();
	Transform = glm::mat4();
}


GrvtCommandBuffer::GrvtCommandBuffer() :
	CompositeMatrix(), Lights(), RenderCommands(), InstanceCommands() {}


GrvtCommandBuffer::GrvtCommandBuffer(const GrvtCommandBuffer& Other) { *this = Other; }


GrvtCommandBuffer& GrvtCommandBuffer::operator= (const GrvtCommandBuffer& Other) 
{
	_ASSERTE(this != &Other);

	if (this != &Other) 
	{
		CompositeMatrix		= Other.CompositeMatrix;
		Lights				= Other.Lights;
		RenderCommands		= Other.RenderCommands;
		InstanceCommands	= Other.InstanceCommands;
	}

	return *this;
}


GrvtRenderTarget::GrvtRenderTarget() :
	Handle(nullptr), Width(0), Height(0), AttachmentBitMask(0) {}


GrvtRenderTarget::~GrvtRenderTarget()
{
	Handle = nullptr;
	Width = Height = 0;
	AttachmentBitMask = 0;
}


GrvtRenderTarget::GrvtRenderTarget(const GrvtRenderTarget& Other) { *this = Other; }


GrvtRenderTarget& GrvtRenderTarget::operator= (const GrvtRenderTarget& Other)
{
	_ASSERTE(this != &Other);

	if (this != &Other)
	{
		Handle	= Other.Handle;
		Width	= Other.Width;
		Height	= Other.Height;
		AttachmentBitMask = Other.AttachmentBitMask;
	}

	return *this;
}


GrvtRenderTarget::GrvtRenderTarget(GrvtRenderTarget&& Other) { *this = Gfl::Move(Other); }


GrvtRenderTarget& GrvtRenderTarget::operator= (GrvtRenderTarget&& Other)
{
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Handle	= Other.Handle;
		Width	= Other.Width;
		Height	= Other.Height;
		AttachmentBitMask = Other.AttachmentBitMask;

		new (&Other) GrvtRenderTarget();
	}

	return *this;
}


GrvtCommandBuffer::GrvtCommandBuffer(GrvtCommandBuffer&& Other) { *this = Move(Other); }


GrvtCommandBuffer& GrvtCommandBuffer::operator= (GrvtCommandBuffer&& Other) 
{
	_ASSERTE(this != &Other);

	if (this != &Other) 
	{
		CompositeMatrix		= Other.CompositeMatrix;
		Lights				= Other.Lights;
		RenderCommands		= Other.RenderCommands;
		InstanceCommands	= Other.InstanceCommands;

		new (&Other) GrvtCommandBuffer();
	}

	return *this;
}


GrvtCommandBuffer::~GrvtCommandBuffer() 
{
	CompositeMatrix = glm::mat4();
	Lights.Release();
	RenderCommands.Release();
	InstanceCommands.Release();
}