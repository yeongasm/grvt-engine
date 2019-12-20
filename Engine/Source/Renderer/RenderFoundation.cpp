#include "GrvtPch.h"
#include "Renderer/RenderFoundation.h"


namespace Grvt
{


	RenderNode::RenderNode() :
		Handle(nullptr), Amount(1), Size(0), Mode(0), Indexed(true) {}


	RenderNode::RenderNode(const RenderNode& Other)
	{
		*this = Other;
	}


	RenderNode& RenderNode::operator= (const RenderNode& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Handle = Other.Handle;
			Amount = Other.Amount;
			Size = Other.Size;
			Mode = Other.Mode;
			Indexed = Other.Indexed;
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
			Handle = Other.Handle;
			Amount = Other.Amount;
			Size = Other.Size;
			Mode = Other.Mode;
			Indexed = Other.Indexed;

			new (&Other) RenderNode();
		}

		return *this;
	}


	RenderNode::~RenderNode()
	{
		Handle = nullptr;
		Amount = 0;
		Size = 0;
		Mode = 0;
	}


	RenderCommand::RenderCommand() :
		Nodes(), Instances(), Transform(1.0f), State(), Material(nullptr) {}


	RenderCommand::RenderCommand(const RenderCommand& Other) {
		*this = Other;
	}


	RenderCommand& RenderCommand::operator= (const RenderCommand& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Nodes = Other.Nodes;
			Instances = Other.Instances;
			Transform = Other.Transform;
			State = Other.State;
			Material = Other.Material;
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
			Nodes = Gfl::Move(Other.Nodes);
			Instances = Gfl::Move(Other.Instances);
			Transform = Gfl::Move(Other.Transform);
			State = Gfl::Move(Other.State);
			Material = Gfl::Move(Other.Material);

			new (&Other) RenderCommand();
		}

		return *this;
	}

	void RenderCommand::Empty()
	{
		Nodes.Empty();
		Instances.Empty();
	}


	RenderCommand::~RenderCommand()
	{
		Nodes.Release();
		Instances.Release();
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
		Lights(), ShadowMaps(), RenderCommands(), InstancedCommands(), IsEmpty(true) {}


	CommandBuffer::~CommandBuffer()
	{
		Lights.Release();
		ShadowMaps.Release();
		RenderCommands.Release();
		InstancedCommands.Release();
	}


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
			ShadowMaps		= Other.ShadowMaps;
			RenderCommands	= Other.RenderCommands;
			InstancedCommands = Other.InstancedCommands;
			IsEmpty = Other.IsEmpty;
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
			Lights				= Gfl::Move(Other.Lights);
			ShadowMaps			= Gfl::Move(Other.ShadowMaps);
			RenderCommands		= Gfl::Move(Other.RenderCommands);
			InstancedCommands	= Gfl::Move(Other.InstancedCommands);
			IsEmpty = Other.IsEmpty;

			new (&Other) CommandBuffer();
		}

		return *this;
	}
	

	void CommandBuffer::Init()
	{
		Lights.Reserve(64);
		ShadowMaps.Reserve(64);
		RenderCommands.Reserve(64);
		InstancedCommands.Reserve(64);
	}


	void CommandBuffer::Free()
	{
		Lights.Release();
		ShadowMaps.Release();
		RenderCommands.Release();
		InstancedCommands.Release();
		CustomCommands.clear();
	}


	void CommandBuffer::Clear()
	{
		Lights.Empty();
		ShadowMaps.Empty();
		RenderCommands.Empty();
		InstancedCommands.Empty();
		CustomCommands.clear();

		IsEmpty = true;
	}
}