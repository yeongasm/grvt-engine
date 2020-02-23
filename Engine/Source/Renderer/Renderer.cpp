#include "GrvtPch.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderContext.h"
#include "Library/GrvtAlgorithms.h"


namespace Grvt
{


	//static uint32 PreviousWidth  = 0;
	//static uint32 PreviousHeight = 0;

	void GrvtRenderer::SortCommand(const Gfl::Array<RenderCommand>& Commands, Gfl::Array<size_t>& SortedCommands)
	{
		Gfl::Array<Gfl::Pair<uint64, size_t>> CommandWeight;

		for (RenderCommand& Command : Commands)
		{
			size_t Index = Commands.IndexOf(Command);

			if (!Command.Sort)
			{
				UnsortedCommands.Push(Index);
				continue;
			}

			uint64 Weight = ((uint64)Command.Material->Shader->Handle.Id << 52)
				| ((uint64)Command.State.PolygonFace << 48)
				| ((uint64)Command.State.PolygonMode << 44)
				| ((uint64)Command.State.CullFace << 40)
				| ((uint64)Command.State.FrontFace << 36)
				| ((uint64)Command.State.DepthFunc << 28)
				| ((uint64)Command.State.DstBlend << 14)
				| ((uint64)Command.State.SrcBlend);

			CommandWeight.Push({Weight, Index});
		}

		Gfl::QuickSort(CommandWeight);

		if (SortedCommands.Size() < CommandWeight.Length())
		{
			SortedCommands.Reserve(CommandWeight.Length(), false);
		}

		for (Gfl::Pair<uint64, size_t>& Sorted : CommandWeight)
		{
			SortedCommands.Push(Sorted.Value);
		}
	}


	void GrvtRenderer::RenderMesh(RenderNode& Node)
	{
		GfxDriver->BindVertexArray(*Node.Handle);

		if (Node.Indexed)
		{
			glDrawElements(Node.Mode, Node.Size, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(Node.Mode, 0, Node.Size);
		}

		GfxDriver->UnbindVertexArray(*Node.Handle);
	}


	void GrvtRenderer::RenderMesh(GrvtMesh& Mesh)
	{
		GfxDriver->BindVertexArray(Mesh.Vao);

		if (Mesh.Indices.Length())
		{
			glDrawElements(GL_TRIANGLES, Mesh.Size, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, Mesh.Size);
		}

		GfxDriver->UnbindVertexArray(Mesh.Vao);
	}


	void GrvtRenderer::UpdateMaterial(GrvtMaterial* Material)
	{
		for (auto& Uniform : Material->Uniforms)
		{
			switch (Uniform.second.Type)
			{
			case Gfx_Type_Boolean:
				GfxDriver->SetUniformBool(ActiveShader->Uniforms[Uniform.first].Location, Uniform.second.Boolean);
				continue;
			case Gfx_Type_Integer:
				GfxDriver->SetUniformInt(ActiveShader->Uniforms[Uniform.first].Location, Uniform.second.Integer);
				continue;
			case Gfx_Type_Float:
				GfxDriver->SetUniformFloat(ActiveShader->Uniforms[Uniform.first].Location, Uniform.second.Float);
				continue;
			case Gfx_Type_Vec2f:
				GfxDriver->SetUniformVec2F(ActiveShader->Uniforms[Uniform.first].Location, glm::value_ptr(Uniform.second.Vec2));
				continue;
			case Gfx_Type_Vec3f:
				GfxDriver->SetUniformVec3F(ActiveShader->Uniforms[Uniform.first].Location, glm::value_ptr(Uniform.second.Vec3));
				continue;
			case Gfx_Type_Vec4f:
				GfxDriver->SetUniformVec4F(ActiveShader->Uniforms[Uniform.first].Location, glm::value_ptr(Uniform.second.Vec4));
				continue;
			case Gfx_Type_Mat2f:
				GfxDriver->SetUniformMat2F(ActiveShader->Uniforms[Uniform.first].Location, glm::value_ptr(Uniform.second.Mat2));
				continue;
			case Gfx_Type_Mat3f:
				GfxDriver->SetUniformMat3F(ActiveShader->Uniforms[Uniform.first].Location, glm::value_ptr(Uniform.second.Mat3));
				continue;
			case Gfx_Type_Mat4f:
				GfxDriver->SetUniformMat4F(ActiveShader->Uniforms[Uniform.first].Location, glm::value_ptr(Uniform.second.Mat4));
				continue;
			default:
				GfxDriver->SetUniformInt(ActiveShader->Uniforms[Uniform.first].Location, Uniform.second.Integer);
				continue;
			}
		}
	}


	void GrvtRenderer::DeleteFramebuffer(RenderTarget& Framebuffer)
	{
		if (Framebuffer.DepthStencilAttachment.Value.Id)
		{
			GetGraphicsInterface()->QueueHandleForDelete(Gfl::Move(Framebuffer.DepthStencilAttachment.Value), HandleType::Handle_Texture);
		}

		if (Framebuffer.DepthAttachment.Value.Id)
		{
			GetGraphicsInterface()->QueueHandleForDelete(Gfl::Move(Framebuffer.DepthAttachment.Value), HandleType::Handle_Texture);
		}

		for (RenderTarget::AttachmentPoint& Attachment : Framebuffer.ColourAttachments)
		{
			if (Attachment.Value.Id)
			{
				GetGraphicsInterface()->QueueHandleForDelete(Gfl::Move(Attachment.Value), HandleType::Handle_Texture);
			}
		}

		if (Framebuffer.Handle.Id)
		{
			GetGraphicsInterface()->QueueHandleForDelete(Gfl::Move(Framebuffer.Handle), HandleType::Handle_Framebuffer);
		}
	}


	void GrvtRenderer::DeleteFramebufferInstant(RenderTarget& Framebuffer)
	{
		if (Framebuffer.DepthStencilAttachment.Value.Id)
		{
			GfxDriver->DeleteTexture(Framebuffer.DepthStencilAttachment.Value);
		}

		if (Framebuffer.DepthAttachment.Value.Id)
		{
			GfxDriver->DeleteTexture(Framebuffer.DepthAttachment.Value);
		}

		for (RenderTarget::AttachmentPoint& Attachment : Framebuffer.ColourAttachments)
		{
			if (Attachment.Value.Id)
			{
				GfxDriver->DeleteTexture(Attachment.Value);
			}
		}

		if (Framebuffer.Handle.Id)
		{
			GfxDriver->DeleteFramebuffer(Framebuffer.Handle);
		}
	}


	void GrvtRenderer::RenderPushedCommand(const RenderCommand& Command, bool UpdateState)
	{
		GrvtMaterial* Material = Command.Material;

		if (UpdateState)
		{
			StateCache.SetAlphaBlend(Command.State.SrcBlend, Command.State.DstBlend);
			StateCache.SetDepthFunc(Command.State.DepthFunc);
			StateCache.SetCullFace(Command.State.CullFace, Command.State.FrontFace);
			StateCache.SetPolygonMode(Command.State.PolygonFace, Command.State.PolygonMode);
		}

		for (TexturePair& Pair : Material->Textures)
		{
			GfxDriver->ActiveTexture(Pair.Key);
			GfxDriver->BindTexture(*Pair.Value);
		}

		UpdateMaterial(Material);

		for (RenderNode& Node : Command.Nodes)
		{
			RenderMesh(Node);
		}
	}


	GrvtRenderer::GrvtRenderer() :
		UnsortedCommands(),
		UnsortedInstancedCommands(),
		SortedCommands(),
		SortedInstancedCommands(),
		RenderTargets(),
		PostProcess(),
		GBuffer(),
		StateCache(),
		BackBuffer(),
		FrontBuffer(),
		GfxDriver(nullptr),
		ActiveShader(nullptr),
		ScreenQuad(nullptr), 
		ProjectionViewUBO(),
		RtIdCount(0),
		PosX(0),
		PosY(0),
		Width(0),
		Height(0) {}


	GrvtRenderer::~GrvtRenderer() {}


	void GrvtRenderer::Init(uint32 X, uint32 Y, uint32 Width, uint32 Height, GraphicsDriver* Driver)
	{
		PosX = X;
		PosY = Y;

		this->Width  = Width;
		this->Height = Height;

		GfxDriver = Driver;

		PostProcess.Init(this);
	}

	
	void GrvtRenderer::Shutdown()
	{
		if (ProjectionViewUBO.Id)
		{
			GfxDriver->DeleteBuffer(ProjectionViewUBO);
		}

		BackBuffer.Free();
		FrontBuffer.Free();

		PostProcess.Free();

		DeleteFramebuffer(GBuffer);
		
		for (auto& It : RenderTargets)
		{
			DeleteFramebuffer(It.second);
		}

		UnsortedCommands.Release();
		UnsortedInstancedCommands.Release();

		SortedCommands.Release();
		UnsortedCommands.Release();

		GfxDriver		= nullptr;
		ActiveShader	= nullptr;
		ScreenQuad		= nullptr;

		DeferredPassShader			= nullptr;
		DeferredDirectionalShader	= nullptr;
		DeferredPointShader			= nullptr;
	}


	void GrvtRenderer::Render()
	{

	}


	size_t GrvtRenderer::NewDepthMap(bool Cubemap)
	{
		auto Pair = RenderTargets.emplace(RtIdCount++, RenderTarget());

		size_t Handle = Pair.first->first;

		RenderTarget& Framebuffer = Pair.first->second;

		Gfl::Pair<uint32, GfxHandle>& DepthAttachment = Framebuffer.DepthAttachment;
		DepthAttachment.Key = RenderTarget_AttachPoint_Depth;

		Driver::FramebufferBuildData FBuild;
		Driver::TextureBuildData TBuild;

		TBuild.Target	= Gfx_Texture_2D;
		TBuild.Type		= Gfx_Type_Float;
		TBuild.Format	= Gfx_Format_Depth;
		TBuild.InternalFormat = Gfx_Format_Depth;

		TBuild.Parameters.Push({ Gfx_TexParam_MagFilter, Gfx_Mip_Nearest });
		TBuild.Parameters.Push({ Gfx_TexParam_MinFilter, Gfx_Mip_Nearest });

		if (Cubemap)
		{
			TBuild.Target = Gfx_Texture_CubeMap;

			TBuild.Parameters.Push({ Gfx_TexParam_WrapS, Gfx_Wrap_ClampToEdge });
			TBuild.Parameters.Push({ Gfx_TexParam_WrapT, Gfx_Wrap_ClampToEdge });
			TBuild.Parameters.Push({ Gfx_TexParam_WrapR, Gfx_Wrap_ClampToEdge });
		}
		else
		{
			TBuild.Parameters.Push({ Gfx_TexParam_WrapS, Gfx_Wrap_ClampToBorder });
			TBuild.Parameters.Push({ Gfx_TexParam_WrapT, Gfx_Wrap_ClampToBorder });
			TBuild.BorderColour[0] = 1.0f;
			TBuild.BorderColour[1] = 1.0f;
			TBuild.BorderColour[2] = 1.0f;
			TBuild.BorderColour[3] = 1.0f;
		}
		
		FBuild.Attachments.Push(Driver::FrameTexAttachment(&DepthAttachment.Value, Gfx_Attachment_Depth, TBuild));

		GetRenderContext()->GlInterface.QueueFramebufferForBuild(Framebuffer.Handle, FBuild);
		
		return Handle;
	}

	bool GrvtRenderer::DeleteRenderTarget(size_t RenderTargetIndex)
	{
		if (RenderTargetIndex == -1)
		{
			return false;
		}

		RenderTarget& Framebuffer = RenderTargets[RenderTargetIndex];

		DeleteFramebuffer(Framebuffer);

		RenderTargets.erase(RenderTargetIndex);

		return true;
	}

}