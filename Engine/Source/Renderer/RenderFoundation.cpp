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
		Nodes(), Instances(), Transform(1.0f), State(), Material(nullptr), Sort(false) {}


	RenderCommand::RenderCommand(const RenderCommand& Other) {
		*this = Other;
	}


	RenderCommand& RenderCommand::operator= (const RenderCommand& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Nodes		= Other.Nodes;
			Instances	= Other.Instances;
			Transform	= Other.Transform;
			State		= Other.State;
			Material	= Other.Material;
			Sort		= Other.Sort;
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
			Nodes		= Gfl::Move(Other.Nodes);
			Instances	= Gfl::Move(Other.Instances);
			Transform	= Gfl::Move(Other.Transform);
			State		= Gfl::Move(Other.State);
			Material	= Gfl::Move(Other.Material);
			Sort		= Gfl::Move(Other.Sort);

			new (&Other) RenderCommand();
		}

		return *this;
	}

	void RenderCommand::Empty()
	{
		Nodes.Empty();
		Instances.Empty();
		Material = nullptr;
	}


	RenderCommand::~RenderCommand()
	{
		Nodes.Release();
		Instances.Release();
		Transform = glm::mat4();
	}


	RenderTarget::RenderTarget() :
		ColourAttachments(), DepthAttachment(), DepthStencilAttachment(), Handle(), Width(0), Height(0) {}


	RenderTarget::~RenderTarget()
	{
		Width  = 0;
		Height = 0;
		ColourAttachments.Release();
	}

	RenderTarget::RenderTarget(RenderTarget&& Other) 
	{
		*this = Gfl::Move(Other);
	}


	RenderTarget& RenderTarget::operator= (RenderTarget&& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other) {
			Handle	= Gfl::Move(Other.Handle);
			Width	= Gfl::Move(Other.Width);
			Height	= Gfl::Move(Other.Height);
			
			new (&Other) RenderTarget();
		}

		return *this;
	}


	CommandBuffer::CommandBuffer() :
		Projection(0.0f),
		View(0.0f),
		DirectionalLight(0.0f),
		PointLights(),
		LightSpaceTransforms(),
		RenderCommands(), 
		InstancedCommands(),
		DepthMap(nullptr),
		IsEmpty(true) {}


	CommandBuffer::~CommandBuffer()
	{
		PointLights.Release();
		LightSpaceTransforms.Release();
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
			Projection			= Other.Projection;
			View				= Other.View;
			DirectionalLight    = Other.DirectionalLight;
			PointLights			= Other.PointLights;

			LightSpaceTransforms = Other.LightSpaceTransforms;

			DepthMap			= Other.DepthMap;
			OmniDepthMaps		= Other.OmniDepthMaps;
			RenderCommands		= Other.RenderCommands;
			InstancedCommands	= Other.InstancedCommands;
			SkyBox				= Other.SkyBox;

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
			Projection			= Gfl::Move(Other.Projection);
			View				= Gfl::Move(Other.View);
			DirectionalLight	= Gfl::Move(Other.DirectionalLight);
			PointLights			= Gfl::Move(Other.PointLights);

			LightSpaceTransforms = Gfl::Move(Other.LightSpaceTransforms);

			DepthMap			= Gfl::Move(DepthMap);
			OmniDepthMaps		= Gfl::Move(OmniDepthMaps);
			RenderCommands		= Gfl::Move(Other.RenderCommands);
			InstancedCommands	= Gfl::Move(Other.InstancedCommands);
			SkyBox				= Gfl::Move(Other.SkyBox);

			IsEmpty = Other.IsEmpty;

			new (&Other) CommandBuffer();
		}

		return *this;
	}
	

	void CommandBuffer::Init()
	{
		PointLights.Reserve(64);
		LightSpaceTransforms.Reserve(64);
		OmniDepthMaps.Reserve(64);

		RenderCommands.Reserve(64);
		InstancedCommands.Reserve(64);
	}


	void CommandBuffer::Free()
	{
		Projection = glm::mat4();
		View = glm::mat4();
		PointLights.Release();
		LightSpaceTransforms.Release();
		OmniDepthMaps.Release();
		RenderCommands.Release();
		InstancedCommands.Release();
		SkyBox.Empty();
	}


	void CommandBuffer::Clear()
	{
		PointLights.Empty();
		LightSpaceTransforms.Empty();
		OmniDepthMaps.Empty();
		RenderCommands.Empty();
		InstancedCommands.Empty();
		SkyBox.Empty();

		DepthMap = nullptr;

		IsEmpty = true;
	}
}