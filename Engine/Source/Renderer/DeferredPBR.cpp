#include "GrvtPch.h"
#include "Framework/Foundation/Interface.h"
#include "Framework/DefaultShaders/GBuffer.h"
#include "Framework/DefaultShaders/ShadowMaps.h"
#include "Framework/DefaultShaders/SimpleDebug.h"
#include "Renderer/DeferredPBR.h"
#include "Library/GrvtAlgorithms.h"
#include "Manager/Manager.h"


namespace Grvt
{

	static uint32 PreviousWidth		= 0;
	static uint32 PreviousHeight	= 0;


	DeferredPBR::DeferredPBR() :
		BaseRenderer(),
		BGColour(0.0f, 0.0f, 0.0f),
		SortedCommands(), 
		SortedInstancedCommands(),
		ScreenQuad(nullptr),
		DepthPassShader(nullptr),
		OmniDepthPassShader(nullptr),
		GBufferPass(nullptr),
		ProjectionViewUBO(),
		LightUBO() {}

	
	DeferredPBR::~DeferredPBR() {}


	void DeferredPBR::InitialiseGBuffer()
	{
		ShaderImportInfo ShaderInfo;

		ShaderInfo.Name = "GBufferShader";
		ShaderInfo.AddShaderToProgram(GBufferShader::VertexShader, GrvtShader_SourceType_Vertex);
		ShaderInfo.AddShaderToProgram(GBufferShader::FragmentShader, GrvtShader_SourceType_Fragment);

		GBufferPass = GetResourceManager()->NewShaderProgram(ShaderInfo);

		BaseAPI::FramebufferBuildData Data;

		Data.Width  = Width;
		Data.Height = Height;

		Gfl::Pair<uint32, ObjHandle>& PositionBuffer = GBuffer.ColourAttachments.Insert(Gfl::Pair<uint32, ObjHandle>());
		Gfl::Pair<uint32, ObjHandle>& NormalBuffer = GBuffer.ColourAttachments.Insert(Gfl::Pair<uint32, ObjHandle>());
		Gfl::Pair<uint32, ObjHandle>& AlbedoBuffer = GBuffer.ColourAttachments.Insert(Gfl::Pair<uint32, ObjHandle>());
		
		Gfl::Pair<uint32, ObjHandle>& DepthStencilBuffer = GBuffer.DepthStencilAttachment;

		PositionBuffer.Key = GrvtFramebuffer_Attachment_Colour;
		NormalBuffer.Key = GrvtFramebuffer_Attachment_Colour;
		AlbedoBuffer.Key = GrvtFramebuffer_Attachment_Colour;

		DepthStencilBuffer.Key = GrvtFramebuffer_Attachment_DepthStencil;

		BaseAPI::TextureBuildData TexData;
		TexData.Type = GL_FLOAT;
		TexData.Format = GL_RGB16F;
		TexData.InternalFormat = GL_RGB;
		TexData.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_NEAREST});
		TexData.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_NEAREST});

		Data.Attachments.Push(BaseAPI::TextureAttachment(&PositionBuffer.Value, GL_COLOR_ATTACHMENT0, TexData));
		Data.Attachments.Push(BaseAPI::TextureAttachment(&NormalBuffer.Value, GL_COLOR_ATTACHMENT1, TexData));

		TexData.Type = GL_UNSIGNED_BYTE;
		TexData.Format = GL_RGBA;
		TexData.InternalFormat = GL_RGBA;

		Data.Attachments.Push(BaseAPI::TextureAttachment(&AlbedoBuffer.Value, GL_COLOR_ATTACHMENT2, TexData));

		TexData.Type = GL_FLOAT;
		TexData.Format = GL_DEPTH_STENCIL;
		TexData.InternalFormat = GL_DEPTH_STENCIL;

		Data.Attachments.Push(BaseAPI::TextureAttachment(&DepthStencilBuffer.Value, GL_DEPTH_STENCIL_ATTACHMENT, TexData));

		Middleware::GetBuildQueue()->QueueFramebufferForBuild(&GBuffer.Handle, Data);
	}


	void DeferredPBR::UpdateMaterial(GrvtMaterial* SourceMaterial)
	{
		for (UniformAttr& Uniform : SourceMaterial->Uniforms)
		{
			if (Uniform.Type == GrvtShader_AttrType_Boolean)
			{
				BaseAPI::Shader::GrShaderSetBool(Uniform.Location, Uniform.Cast<bool>());
				continue;
			}

			if (Uniform.Type == GrvtShader_AttrType_Integer)
			{
				BaseAPI::Shader::GrShaderSetInt(Uniform.Location, Uniform.Cast<int32>());
				continue;
			}

			if (Uniform.Type == GrvtShader_AttrType_Float)
			{
				BaseAPI::Shader::GrShaderSetFloat(Uniform.Location, Uniform.Cast<float32>());
				continue;
			}

			if (Uniform.Type == GrvtShader_AttrType_Vector)
			{
				switch (Uniform.SubType)
				{
				case GrvtShader_AttrSubType_Vector2:
					BaseAPI::Shader::GrShaderSetVec2Float(Uniform.Location, &Uniform.Cast<glm::vec2>()[0]);
					continue;
				case GrvtShader_AttrSubType_Vector3:
					BaseAPI::Shader::GrShaderSetVec3Float(Uniform.Location, &Uniform.Cast<glm::vec3>()[0]);
					continue;
				case GrvtShader_AttrSubType_Vector4:
					BaseAPI::Shader::GrShaderSetVec4Float(Uniform.Location, &Uniform.Cast<glm::vec4>()[0]);
					continue;
				}
			}

			if (Uniform.Type == GrvtShader_AttrType_Matrix)
			{
				switch (Uniform.SubType)
				{
				case GrvtShader_AttrSubType_Matrix2:
					BaseAPI::Shader::GrShaderSetMat2Float(Uniform.Location, &Uniform.Cast<glm::mat2>()[0][0]);
					continue;
				case GrvtShader_AttrSubType_Matrix3:
					BaseAPI::Shader::GrShaderSetMat3Float(Uniform.Location, &Uniform.Cast<glm::mat3>()[0][0]);
					continue;
				case GrvtShader_AttrSubType_Matrix4:
					BaseAPI::Shader::GrShaderSetMat4Float(Uniform.Location, &Uniform.Cast<glm::mat4>()[0][0]);
					continue;
				}
			}

			if (Uniform.Type == GrvtShader_AttrType_Sampler)
			{
				BaseAPI::Shader::GrShaderSetInt(Uniform.Location, Uniform.Cast<int32>());
				continue;
			}
		}
	}


	void DeferredPBR::RenderPushedCommand(const RenderCommand* Command, bool UpdateState)
	{
		GrvtMaterial* Material = Command->Material;

		if (UpdateState)
		{
			StateCache.SetAlphaBlend(Command->State.SrcBlend, Command->State.DstBlend);
			StateCache.SetDepthFunc(Command->State.DepthFunc);
			StateCache.SetCullFace(Command->State.CullFace, Command->State.FrontFace);
			StateCache.SetPolygonMode(Command->State.PolygonFace, Command->State.PolygonMode);
		}

		/*for (TexturePair& Pair : Material->Textures)
		{
			glActiveTexture(GL_TEXTURE0 + Pair.Key);
			BaseAPI::GrBindTexture(*Pair.Value);
		}*/

		/*if (FrontBuffer.DepthMap)
		{
			glActiveTexture(GL_TEXTURE0 + GrvtTexture_Type_ShadowMap);
			BaseAPI::GrBindTexture(FrontBuffer.DepthMap->DepthAttachment.Value);
		}

		for (size_t i = 0; i < FrontBuffer.OmniDepthMaps.Length(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + GrvtTexture_Type_OmniShadowMap + (uint32)i);
			BaseAPI::GrBindTexture(FrontBuffer.OmniDepthMaps[i]->DepthAttachment.Value);
		}*/

		UpdateMaterial(Material);
		
		for (RenderNode& Node : Command->Nodes)
		{
			RenderMesh(&Node);
		}
	}


	void DeferredPBR::RenderMesh(RenderNode* Node)
	{
		BaseAPI::GrBindVertexArray(*Node->Handle);

		if (Node->Indexed)
		{
			glDrawElements(Node->Mode, Node->Size, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(Node->Mode, 0, Node->Size);
		}

		BaseAPI::GrUnbindVertexArray();
	}


	void DeferredPBR::SortCommand(const Gfl::Array<RenderCommand>& Commands, Gfl::Array<size_t>& SortedCommand)
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
							| ((uint64)Command.State.PolygonFace		<< 48)		
							| ((uint64)Command.State.PolygonMode		<< 44)
							| ((uint64)Command.State.CullFace			<< 40)
							| ((uint64)Command.State.FrontFace			<< 36)
							| ((uint64)Command.State.DepthFunc			<< 28)
							| ((uint64)Command.State.DstBlend			<< 14)
							| ((uint64)Command.State.SrcBlend);
			
			CommandWeight.Push({Weight, Index});
		}

		// NOTE(Afiq):
		// Implement a robust Sort() algoritm.
		// Sort() algoritm would make use of BubbleSort, QuickSort and HeapSort.
		Gfl::QuickSort(CommandWeight);

		if (SortedCommand.Size() < Commands.Length())
		{
			SortedCommand.Reserve(CommandWeight.Length(), false);
		}

		for (Gfl::Pair<uint64, size_t>& Sorted : CommandWeight)
		{
			SortedCommand.Push(Sorted.Value);
		}
	}


	void DeferredPBR::RenderDirectionalLightingPass(RenderCommand* Command)
	{
		BaseAPI::Shader::GrShaderSetMat4Float(0, glm::value_ptr(FrontBuffer.DirLightSpaceTransform));
		BaseAPI::Shader::GrShaderSetMat4Float(1, &Command->Transform[0][0]);
		
		for (RenderNode& Node : Command->Nodes)
		{
			RenderMesh(&Node);
		}
	}


	void DeferredPBR::RenderPointLightsPass(size_t Index, RenderCommand* Command)
	{
		BaseAPI::Shader::GrShaderSetFloat(0, FrontBuffer.PointLights[Index][3][3]);
		BaseAPI::Shader::GrShaderSetVec3Float(1, &FrontBuffer.PointLights[Index][1][0]);
		BaseAPI::Shader::GrShaderSetMat4Float(2, &Command->Transform[0][0]);

		for (uint32 i = 0; i < 6; i++)
		{
			BaseAPI::Shader::GrShaderSetMat4Float(3 + i, glm::value_ptr(FrontBuffer.PointLightSpaceTransforms[i]));
		}

		for (RenderNode& Node : Command->Nodes)
		{
			RenderMesh(&Node);
		}
	}


	void DeferredPBR::RenderScreenQuad()
	{
		GrvtMesh* Quad = &ScreenQuad->Meshes[0];
		BaseAPI::GrBindVertexArray(Quad->Vao);
		glDrawElements(GL_TRIANGLES, Quad->Size, GL_UNSIGNED_INT, 0);
		BaseAPI::GrUnbindVertexArray();
	}

	
	void DeferredPBR::Init()
	{
		BackBuffer.Init();
		FrontBuffer.Init();

		BGColour = glm::vec3(0.169f, 0.169f, 0.169f);

		BaseAPI::GrCreateBufferObject(ProjectionViewUBO, GL_UNIFORM_BUFFER);
		BaseAPI::GrCreateBufferObject(LightUBO, GL_UNIFORM_BUFFER);

		BaseAPI::GrBindBufferObject(ProjectionViewUBO);
		glBufferData(ProjectionViewUBO.Target, 192, nullptr, GL_STATIC_DRAW);
		glBindBufferBase(ProjectionViewUBO.Target, 0, ProjectionViewUBO.Id);
		BaseAPI::GrUnbindBufferObject(ProjectionViewUBO);

		BaseAPI::GrBindBufferObject(LightUBO);
		glBufferData(LightUBO.Target, 1028, nullptr, GL_STATIC_DRAW);
		glBindBufferBase(LightUBO.Target, 1, LightUBO.Id);
		BaseAPI::GrUnbindBufferObject(LightUBO);

		ScreenQuad = Grvt::GetResourceManager()->GetModel("Quad");

		{
			ShaderImportInfo SInfo;
			SInfo.Name = "RenderToDepthMap";
			SInfo.AddShaderToProgram(DDepthMapShader::VertexShader, GrvtShader_SourceType_Vertex);
			SInfo.AddShaderToProgram(DDepthMapShader::FragmentShader, GrvtShader_SourceType_Fragment);

			DepthPassShader = Grvt::GetResourceManager()->NewShaderProgram(SInfo);
		}

		{
			ShaderImportInfo SInfo;
			SInfo.Name = "RenderToOmniDepthMap";
			SInfo.AddShaderToProgram(ODepthMapShader::VertexShader, GrvtShader_SourceType_Vertex);
			SInfo.AddShaderToProgram(ODepthMapShader::GeometryShader, GrvtShader_SourceType_Geometry);
			SInfo.AddShaderToProgram(ODepthMapShader::FragmentShader, GrvtShader_SourceType_Fragment);

			OmniDepthPassShader = Grvt::GetResourceManager()->NewShaderProgram(SInfo);
		}

		/*{
			ShaderImportInfo SInfo;
			SInfo.Name = "DebugDepthMap";
			SInfo.AddShaderToProgram(SimpleDebugShader::VertexShader, GrvtShader_SourceType_Vertex);
			SInfo.AddShaderToProgram(SimpleDebugShader::FragmentShader, GrvtShader_SourceType_Fragment);

			SimpleDepthDebug = Grvt::GetResourceManager()->NewShaderProgram(SInfo);
		}*/

		InitialiseGBuffer();

		// Initialise post processing effects.
		//PostProcess.Init(this);
	}


	void DeferredPBR::Shutdown()
	{
		ScreenQuad = nullptr;
		SortedCommands.Release();
		SortedInstancedCommands.Release();

		BaseAPI::GrDeleteBufferObject(ProjectionViewUBO);
		BaseAPI::GrDeleteBufferObject(LightUBO);
	}


	void DeferredPBR::Render()
	{
		// Swap the content's of the back buffer into the front buffer if the front buffer is empty.
		// When the renderer's front buffer is not empty, wait til it clears out before swapping with the back buffer.
		// Should disable the program to fill the back buffer when this process fails.
		if (FrontBuffer.IsEmpty && !BackBuffer.IsEmpty)
		{
			FrontBuffer = BackBuffer;
			FrontBuffer.IsEmpty = false;
			BackBuffer.Clear();
		}

		// Don't render anything when screen is minimised.
		if (!Width || !Height)
		{
			return;
		}

		// Dynamically update shadow map's render target size.
		// Not needed for a omni depth map since cube maps need to be identical in width & height.
		// Hence they are set upon creation.
		if (PreviousWidth != Width && PreviousHeight != Height)
		{
			if (FrontBuffer.DepthMap)
			{
				if (!FrontBuffer.DepthMap->Handle.Id)
					goto UpdateGlobalUBOs;

				BaseAPI::GrBindFramebuffer(FrontBuffer.DepthMap->Handle);
				BaseAPI::GrBindTexture(FrontBuffer.DepthMap->DepthAttachment.Value);
				glTexImage2D(FrontBuffer.DepthMap->DepthAttachment.Value.Target, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

				glFramebufferTexture2D(FrontBuffer.DepthMap->Handle.Target, GL_DEPTH_ATTACHMENT, FrontBuffer.DepthMap->DepthAttachment.Value.Target, FrontBuffer.DepthMap->DepthAttachment.Value.Id, 0);
				BaseAPI::GrUnbindTexture(FrontBuffer.DepthMap->DepthAttachment.Value);
				BaseAPI::GrUnbindFramebuffer(FrontBuffer.DepthMap->Handle);

				FrontBuffer.DepthMap->Width  = Width;
				FrontBuffer.DepthMap->Height = Height;
			}

			PreviousWidth  = Width;
			PreviousHeight = Height;
		}

UpdateGlobalUBOs:

		Gfl::Array<RenderCommand>& RenderCommands	 = FrontBuffer.RenderCommands;
		Gfl::Array<RenderCommand>& InstancedCommands = FrontBuffer.InstancedCommands;

		// NOTE(Afiq):
		// Get rid of this once we've learned about some anti-aliasing techniques.
		//glEnable(GL_MULTISAMPLE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, Width, Height);
		//glViewport(PosX, PosY, Width, Height);
		glClearColor(BGColour.x, BGColour.y, BGColour.z, 1.0f);
		
		BaseAPI::GrBindBufferObject(ProjectionViewUBO);
		glBufferSubData(ProjectionViewUBO.Target,   0,	  64,				glm::value_ptr(FrontBuffer.Projection));
		glBufferSubData(ProjectionViewUBO.Target,  64,    64,					  glm::value_ptr(FrontBuffer.View));
		glBufferSubData(ProjectionViewUBO.Target, 128,	  64,	glm::value_ptr(FrontBuffer.DirLightSpaceTransform));
		BaseAPI::GrUnbindBufferObject(ProjectionViewUBO);

		size_t TotalPointLights = FrontBuffer.PointLights.Length();

		BaseAPI::GrBindBufferObject(LightUBO);
		glBufferSubData(LightUBO.Target,   0,        sizeof(uint32),							   &TotalPointLights);
		glBufferSubData(LightUBO.Target,  16,					 64,	glm::value_ptr(FrontBuffer.DirectionalLight));
		glBufferSubData(LightUBO.Target,  80, TotalPointLights * 64,				 FrontBuffer.PointLights.First());
		BaseAPI::GrUnbindBufferObject(LightUBO);

		// Sort render commands if they exist.
		if (RenderCommands.Length())
			SortCommand(RenderCommands, SortedCommands);

		// Sort instanced commands if they exist.
		if (InstancedCommands.Length())
			SortCommand(InstancedCommands, SortedInstancedCommands);

		// Render scene from directional light's perspective.
		/*
		if (FrontBuffer.DepthMap)
		{
			if (!FrontBuffer.DepthMap->Handle.Id)
				goto SkipDirectionalLightingPass;

			if (ActiveShader != DepthPassShader)
				ActiveShader = DepthPassShader;

			BaseAPI::GrBindShaderProgram(ActiveShader->Handle);

			BaseAPI::GrBindFramebuffer(FrontBuffer.DepthMap->Handle);
			glClear(GL_DEPTH_BUFFER_BIT);

			for (RenderCommand& Command : FrontBuffer.ShadowCommands)
			{
				RenderDirectionalLightingPass(&Command);
			}

			BaseAPI::GrUnbindFramebuffer(FrontBuffer.DepthMap->Handle);
			BaseAPI::GrUnbindShaderProgram(ActiveShader->Handle);
		}
		*/

//SkipDirectionalLightingPass:

		if (ActiveShader != GBufferPass)
			ActiveShader = GBufferPass;

		BaseAPI::GrBindShaderProgram(ActiveShader->Handle);
		BaseAPI::GrBindFramebuffer(GBuffer.Handle);

		StateCache.SetAlphaBlend(CacheState_None, CacheState_None);
		StateCache.SetDepthFunc(DepthFunc_Less);
		StateCache.SetCullFace(CullFace_Back, FrontFace_CCW);
		StateCache.SetPolygonMode(PolygonFace_Front_And_Back, PolygonMode_Fill);

		RenderCommand* Command = nullptr;

		for (size_t Index : UnsortedCommands)
		{
			Command = &RenderCommands[Index];

			RenderPushedCommand(Command, true);
		}

		for (size_t Index : SortedCommands)
		{
			Command = &RenderCommands[Index];

			RenderPushedCommand(Command, true);
		}

		BaseAPI::GrUnbindFramebuffer(GBuffer.Handle);
		BaseAPI::GrUnbindShaderProgram(ActiveShader->Handle);

		// TODO(Afiq): We have to figure out a better method than this. Perhaps point lights and it's light space transform should be coupled.
		/*
		if (ActiveShader != OmniDepthPassShader)
			ActiveShader = OmniDepthPassShader;

		for (size_t i = 0; i < FrontBuffer.PointLights.Length(); i++)
		{
			if (!FrontBuffer.OmniDepthMaps[i]->Handle.Id)
				continue;

			glViewport(0, 0, FrontBuffer.OmniDepthMaps[i]->Width, FrontBuffer.OmniDepthMaps[i]->Height);
			BaseAPI::GrBindFramebuffer(FrontBuffer.OmniDepthMaps[i]->Handle);
			glClear(GL_DEPTH_BUFFER_BIT);
			
			BaseAPI::GrBindShaderProgram(ActiveShader->Handle);

			for (RenderCommand& Command : FrontBuffer.ShadowCommands)
			{
				RenderPointLightsPass(i, &Command);
			}

			std::deque<glm::mat4>::iterator First = FrontBuffer.PointLightSpaceTransforms.begin();
			std::deque<glm::mat4>::iterator Sixth = First + 6;

			FrontBuffer.PointLightSpaceTransforms.erase(First, Sixth);

			BaseAPI::GrUnbindFramebuffer(FrontBuffer.OmniDepthMaps[i]->Handle);
		}
		*/
		/*
		BaseAPI::GrUnbindShaderProgram(ActiveShader->Handle);
		BaseAPI::GrBindFramebuffer(PostProcess.HDR.Handle);

		glViewport(0, 0, Width, Height);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		StateCache.SetAlphaBlend(CacheState_None, CacheState_None);
		StateCache.SetDepthFunc(DepthFunc_Less);
		StateCache.SetCullFace(CullFace_Back, FrontFace_CCW);
		StateCache.SetPolygonMode(PolygonFace_Front_And_Back, PolygonMode_Fill);

		RenderCommand* Command = nullptr;

		for (size_t Index : UnsortedCommands)
		{
			Command = &RenderCommands[Index];

			if (ActiveShader != Command->Material->Shader)
				ActiveShader = Command->Material->Shader;

			BaseAPI::GrBindShaderProgram(ActiveShader->Handle);
			RenderPushedCommand(Command, true);
		}

		// Render scene
		for (size_t Index : SortedCommands)
		{
			Command = &RenderCommands[Index];	

			if (ActiveShader != Command->Material->Shader)
				ActiveShader = Command->Material->Shader;

			BaseAPI::GrBindShaderProgram(ActiveShader->Handle);
			RenderPushedCommand(Command, true);
		}

		StateCache.SetPolygonMode(PolygonFace_Front_And_Back, PolygonMode_Fill);
		
		// Render Sky boxes last if it exists.
		if (FrontBuffer.SkyBox.Nodes.Length())
		{
			RenderCommand& SkyBox = FrontBuffer.SkyBox;

			StateCache.SetAlphaBlend(CacheState_None, CacheState_None);
			StateCache.SetCullFace(CacheState_None, CacheState_None);
			StateCache.SetDepthFunc(DepthFunc_LEqual);

			glm::mat4 SkyBoxViewMat = glm::mat4(glm::mat3(FrontBuffer.View));

			BaseAPI::GrBindBufferObject(ProjectionViewUBO);
			glBufferSubData(ProjectionViewUBO.Target, 64, 64, &SkyBoxViewMat[0][0]);
			BaseAPI::GrUnbindBufferObject(ProjectionViewUBO);

			ActiveShader = SkyBox.Material->Shader;

			BaseAPI::GrBindShaderProgram(ActiveShader->Handle);
			RenderPushedCommand(&SkyBox, false);
		}

		BaseAPI::GrUnbindFramebuffer(PostProcess.HDR.Handle);

		// Execute Bloom Kernel ----------------------------------------------------------------
		// TODO(Afiq):
		// Once deferred, ssao and pbr is done, revisit bloom and implement it similar to how EARenderer does it.

		bool Horizontal = true;
		ObjHandle* BloomTexture = &PostProcess.HDR.ColourAttachments[1].Value;

		ActiveShader = PostProcess.HorizontalBlurKernel;

		for (uint32 i = 0; i < PostProcess.BloomIteration; i++)
		{
			if (Horizontal)
			{
				ActiveShader = PostProcess.HorizontalBlurKernel;
				BaseAPI::GrBindFramebuffer(PostProcess.BloomTarget0.Handle);
			}
			else
			{
				ActiveShader = PostProcess.VerticalBlurKernel;
				BaseAPI::GrBindFramebuffer(PostProcess.BloomTarget1.Handle);
			}

			BaseAPI::GrBindShaderProgram(ActiveShader->Handle);
			BaseAPI::Shader::GrShaderSetInt(0, 0);
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, BloomTexture->Id);
			glGenerateMipmap(BloomTexture->Target);

			RenderScreenQuad();

			if (Horizontal)
			{
				BloomTexture = &PostProcess.BloomTarget0.ColourAttachments[0].Value;
			}
			else
			{
				BloomTexture = &PostProcess.BloomTarget1.ColourAttachments[0].Value;
			}

			Horizontal ^= true;
		}

		//BaseAPI::GrUnbindFramebuffer(PostProcess.BloomTarget1.Handle);
		// Render To Screen Quad ---------------------------------------------------------------

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (ActiveShader != PostProcess.HDRShader)
			ActiveShader = PostProcess.HDRShader;

		BaseAPI::GrBindShaderProgram(ActiveShader->Handle);
		
		BaseAPI::Shader::GrShaderSetInt(  0, 0);
		BaseAPI::Shader::GrShaderSetInt(  1, 1);
		BaseAPI::Shader::GrShaderSetFloat(2, PostProcess.Exposure);
		BaseAPI::Shader::GrShaderSetFloat(3, PostProcess.Gamma);
		BaseAPI::Shader::GrShaderSetBool( 4, PostProcess.Bloom);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, PostProcess.HDR.ColourAttachments[0].Value.Id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, PostProcess.BloomTarget0.ColourAttachments[0].Value.Id);

		RenderScreenQuad();

		BaseAPI::GrUnbindShaderProgram(ActiveShader->Handle);
		*/
		// -------------------------------------------------------------------------------------

		UnsortedCommands.Empty();
		SortedCommands.Empty();
		UnsortedInstacedCommands.Empty();
		SortedInstancedCommands.Empty();
		FrontBuffer.Clear();
	}


	void DeferredPBR::Exposure(float32 Value)
	{
		PostProcess.Exposure = Value;
	}


	void DeferredPBR::Gamma(float32 Value)
	{
		PostProcess.Gamma = Value;
	}


	void DeferredPBR::InitialisePostProcessing()
	{
		PostProcess.Init(this);
	}


}