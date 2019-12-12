#include "GrvtPch.h"
#include "Renderer/RenderFoundation.h"


namespace Grvt
{


	RenderNode::RenderNode() :
		Material(nullptr), Handle(nullptr), Amount(0), Size(0), Mode(0) {}


	RenderNode::RenderNode(const RenderNode& Other)
	{
		*this = Other;
	}


	RenderNode& RenderNode::operator= (const RenderNode& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Material = Other.Material;
			Handle = Other.Handle;
			Amount = Other.Amount;
			Size = Other.Size;
			Mode = Other.Mode;
		}

		return *this;
	}


	RenderNode::RenderNode(RenderNode&& Other)
	{
		*this = Gfl::Move(Other);
	}


	RenderNode& RenderNode::operator= (RenderNode&& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Material = Other.Material;
			Handle = Other.Handle;
			Amount = Other.Amount;
			Size = Other.Size;
			Mode = Other.Mode;

			new (&Other) RenderNode();
		}

		return *this;
	}


	RenderNode::~RenderNode()
	{
		Material = nullptr;
		Handle = nullptr;
		Amount = 0;
		Size = 0;
		Mode = 0;
	}


	RenderCommand::RenderCommand() :
		Nodes(), Transform(), State() {}


	RenderCommand::RenderCommand(const RenderCommand& Other) {
		*this = Other;
	}


	RenderCommand& RenderCommand::operator= (const RenderCommand& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Nodes = Other.Nodes;
			Transform = Other.Transform;
			State = Other.State;
		}

		return *this;
	}


	RenderCommand::RenderCommand(RenderCommand&& Other) {
		*this = Gfl::Move(Other);
	}


	RenderCommand& RenderCommand::operator= (RenderCommand&& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Nodes = Other.Nodes;
			Transform = Other.Transform;
			State = Other.State;

			new (&Other) RenderCommand();
		}

		return *this;
	}


	RenderCommand::~RenderCommand()
	{
		Nodes.Release();
		Transform = glm::mat4();
	}


	RenderTarget::RenderTarget() :
		Handle(nullptr), Width(0), Height(0), AttachmentBitMask(0) {}


	RenderTarget::~RenderTarget()
	{
		Handle = nullptr;
		Width = Height = 0;
		AttachmentBitMask = 0;
	}


	RenderTarget::RenderTarget(const RenderTarget& Other) {
		*this = Other;
	}


	RenderTarget& RenderTarget::operator= (const RenderTarget& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Handle = Other.Handle;
			Width = Other.Width;
			Height = Other.Height;
			AttachmentBitMask = Other.AttachmentBitMask;
		}

		return *this;
	}


	RenderTarget::RenderTarget(RenderTarget&& Other) {
		*this = Gfl::Move(Other);
	}


	RenderTarget& RenderTarget::operator= (RenderTarget&& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other) {
			Handle = Other.Handle;
			Width = Other.Width;
			Height = Other.Height;
			AttachmentBitMask = Other.AttachmentBitMask;

			new (&Other) RenderTarget();
		}

		return *this;
	}


	CommandBuffer::CommandBuffer() :
		Lights(), RenderCommands() {}


	CommandBuffer::CommandBuffer(const CommandBuffer& Other) 
	{
		*this = Other;
	}


	CommandBuffer& CommandBuffer::operator= (const CommandBuffer& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other) 
		{
			Lights			= Other.Lights;
			RenderCommands	= Other.RenderCommands;
		}

		return *this;
	}


	CommandBuffer::CommandBuffer(CommandBuffer&& Other)
	{
		*this = Gfl::Move(Other);
	}


	CommandBuffer& CommandBuffer::operator= (CommandBuffer&& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Lights			= Other.Lights;
			RenderCommands	= Other.RenderCommands;

			new (&Other) CommandBuffer();
		}

		return *this;
	}


	CommandBuffer::~CommandBuffer()
	{
		Lights.Release();
		RenderCommands.Release();
	}

}