#include "GrvtPch.h"
#include "Framework/Foundation/Foundations.h"
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
		DepthMap(),
		BGColour(0.0f, 0.0f, 0.0f),
		SortedCommands(), 
		SortedInstancedCommands(),
		ScreenQuad(nullptr),
		DepthPassShader(nullptr),
		SimpleDepthDebug(nullptr),
		ProjectionViewUBO(),
		LightUBO(),
		DirLightTransformUBO() {}

	
	DeferredPBR::~DeferredPBR() {}


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

		for (TexturePair& Pair : Material->Textures)
		{
			glActiveTexture(GL_TEXTURE0 + Pair.Key);
			BaseAPI::GrBindTexture(*Pair.Value);
		}

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


	void DeferredPBR::RenderDirectionalLightingPass(const RenderCommand* Command)
	{
		// Update Model uniform.
		glm::mat4 CommandTransform = Command->Transform;

		BaseAPI::Shader::GrShaderSetMat4Float(1, &CommandTransform[0][0]);
		
		for (RenderNode& Node : Command->Nodes)
		{
			RenderMesh(&Node);
		}
	}

	
	void DeferredPBR::Init()
	{
		BackBuffer.Init();
		FrontBuffer.Init();

		BGColour = glm::vec3(0.169f, 0.169f, 0.169f);

		BaseAPI::GrCreateBufferObject(ProjectionViewUBO, GL_UNIFORM_BUFFER);
		BaseAPI::GrCreateBufferObject(LightUBO, GL_UNIFORM_BUFFER);
		BaseAPI::GrCreateBufferObject(DirLightTransformUBO, GL_UNIFORM_BUFFER);

		BaseAPI::GrBindBufferObject(ProjectionViewUBO);
		glBufferData(ProjectionViewUBO.Target, 128, nullptr, GL_STATIC_DRAW);
		glBindBufferBase(ProjectionViewUBO.Target, 0, ProjectionViewUBO.Id);
		BaseAPI::GrUnbindBufferObject(ProjectionViewUBO);

		BaseAPI::GrBindBufferObject(LightUBO);
		size_t TotalSize = sizeof(uint32) + sizeof(uint32) + (64 * 8) + (64 * 1000);
		glBufferData(LightUBO.Target, TotalSize, nullptr, GL_STATIC_DRAW);
		glBindBufferBase(LightUBO.Target, 1, LightUBO.Id);
		BaseAPI::GrUnbindBufferObject(LightUBO);

		BaseAPI::GrBindBufferObject(DirLightTransformUBO);
		TotalSize = sizeof(uint32) * 64 * 8;
		glBufferData(DirLightTransformUBO.Target, TotalSize, nullptr, GL_STATIC_DRAW);
		glBindBufferBase(DirLightTransformUBO.Target, 2, DirLightTransformUBO.Id);
		BaseAPI::GrUnbindBufferObject(DirLightTransformUBO);

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
			SInfo.Name = "DebugDepthMap";
			SInfo.AddShaderToProgram(SimpleDebugShader::VertexShader, GrvtShader_SourceType_Vertex);
			SInfo.AddShaderToProgram(SimpleDebugShader::FragmentShader, GrvtShader_SourceType_Fragment);

			SimpleDepthDebug = Grvt::GetResourceManager()->NewShaderProgram(SInfo);
		}

		// Set up shadow map for directional lighting.
		{
			Gfl::Pair<uint32, ObjHandle>& DepthAttachment = DepthMap.DepthAttachment;
			DepthAttachment.Key = RenderTarget_AttachPoint_Depth;

			BaseAPI::FramebufferBuildData FBuild;
			BaseAPI::TextureBuildData TBuild;

			TBuild.Type = GL_FLOAT;
			TBuild.Format = GL_DEPTH_COMPONENT;
			TBuild.InternalFormat = GL_DEPTH_COMPONENT;
			TBuild.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_NEAREST});
			TBuild.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_NEAREST});
			TBuild.Parameters.Push({GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER});
			TBuild.Parameters.Push({GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER});
			TBuild.BorderColour[0] = 1.0f;
			TBuild.BorderColour[1] = 1.0f;
			TBuild.BorderColour[2] = 1.0f;
			TBuild.BorderColour[3] = 1.0f;

			FBuild.Attachments.Push(BaseAPI::TextureAttachment(&DepthAttachment.Value, GL_DEPTH_ATTACHMENT, TBuild));

			BaseAPI::BuildFramebuffer(DepthMap.Handle, FBuild);
		}
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
		if (PreviousWidth != Width && PreviousHeight != Height)
		{
			BaseAPI::GrBindFramebuffer(DepthMap.Handle);
			BaseAPI::GrBindTexture(DepthMap.DepthAttachment.Value);
			glTexImage2D(DepthMap.DepthAttachment.Value.Target, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

			glFramebufferTexture2D(DepthMap.Handle.Target, GL_DEPTH_ATTACHMENT, DepthMap.DepthAttachment.Value.Target, DepthMap.DepthAttachment.Value.Id, 0);
			BaseAPI::GrUnbindTexture(DepthMap.DepthAttachment.Value);
			BaseAPI::GrUnbindFramebuffer(DepthMap.Handle);
			
			DepthMap.Width  = Width;
			DepthMap.Height = Height;

			PreviousWidth  = Width;
			PreviousHeight = Height;
		}

		Gfl::Array<RenderCommand>& RenderCommands	 = FrontBuffer.RenderCommands;
		Gfl::Array<RenderCommand>& InstancedCommands = FrontBuffer.InstancedCommands;

		// NOTE(Afiq):
		// Get rid of this once we've learned about some anti-aliasing techniques.
		glEnable(GL_MULTISAMPLE);
		glViewport(0, 0, Width, Height);
		//glViewport(PosX, PosY, Width, Height);
		glClearColor(BGColour.x, BGColour.y, BGColour.z, 1.0f);
		
		BaseAPI::GrBindBufferObject(ProjectionViewUBO);
		glBufferSubData(ProjectionViewUBO.Target,  0, 64, &FrontBuffer.Projection[0][0]);
		glBufferSubData(ProjectionViewUBO.Target, 64, 64,       &FrontBuffer.View[0][0]);

		size_t TotalDirLights = FrontBuffer.DirectionalLights.Length();
		size_t TotalPointLights = FrontBuffer.PointLights.Length();

		BaseAPI::GrBindBufferObject(LightUBO);
		glBufferSubData(LightUBO.Target,   0,        sizeof(uint32),					   &TotalDirLights);
		glBufferSubData(LightUBO.Target,   4,        sizeof(uint32),					 &TotalPointLights);
		glBufferSubData(LightUBO.Target,  16,   TotalDirLights * 64, FrontBuffer.DirectionalLights.First());
		glBufferSubData(LightUBO.Target, 528, TotalPointLights * 64,	   FrontBuffer.PointLights.First());
		BaseAPI::GrUnbindBufferObject(LightUBO);

		// Sort render commands if they exist.
		if (RenderCommands.Length())
			SortCommand(RenderCommands, SortedCommands);

		// Sort instanced commands if they exist.
		if (InstancedCommands.Length())
			SortCommand(InstancedCommands, SortedInstancedCommands);

		RenderCommand* Command = nullptr;

		// Render scene from directional light's perspective.
		size_t NumDirectionalLights = FrontBuffer.DirectionalLights.Length();

		if (ActiveShader != DepthPassShader)
			ActiveShader = DepthPassShader;
		BaseAPI::GrBindFramebuffer(DepthMap.Handle);
		glClear(GL_DEPTH_BUFFER_BIT);
		//BaseAPI::GrBindBufferObject(DirLightTransformUBO);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		BaseAPI::GrBindShaderProgram(ActiveShader->Handle);
		BaseAPI::Shader::GrShaderSetMat4Float(0, &FrontBuffer.LightSpaceTransforms[0][0][0]);
		for (size_t i = 0; i < NumDirectionalLights; i++)
		{
			//glBufferSubData(DirLightTransformUBO.Target, 0, 4, &i);
			//glBufferSubData(DirLightTransformUBO.Target, 16, NumDirectionalLights * 64, FrontBuffer.LightSpaceTransforms.First());

			for (size_t Index : SortedCommands)
			{
				Command = &RenderCommands[Index];

				RenderDirectionalLightingPass(Command);
			}
		}

		//BaseAPI::GrUnbindBufferObject(DirLightTransformUBO);
		BaseAPI::GrUnbindFramebuffer(DepthMap.Handle);
		BaseAPI::GrUnbindShaderProgram(ActiveShader->Handle);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		StateCache.SetAlphaBlend(CacheState_None, CacheState_None);
		StateCache.SetDepthFunc(DepthFunc_Less);
		StateCache.SetCullFace(CullFace_Back, FrontFace_CCW);
		StateCache.SetPolygonMode(PolygonFace_Front_And_Back, PolygonMode_Fill);

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

		// Render To Screen Quad.
		BaseAPI::GrBindShaderProgram(SimpleDepthDebug->Handle);

		glm::mat4 transform(1.0f);
		transform = glm::scale(transform, glm::vec3(0.5f));
		transform = glm::translate(transform, glm::vec3(5.0, -5.0f, 0.0f));

		//BaseAPI::Shader::GrShaderSetMat4Float(0, &transform[0][0]);
		BaseAPI::Shader::GrShaderSetInt(0, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, DepthMap.DepthAttachment.Value.Id);

		for (GrvtMesh& Mesh : ScreenQuad->Meshes)
		{
			glBindVertexArray(Mesh.Vao.Id);
			glDrawElements(GL_TRIANGLES, Mesh.Size, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		BaseAPI::GrUnbindShaderProgram(SimpleDepthDebug->Handle);

		SortedCommands.Empty();
		SortedInstancedCommands.Empty();
		FrontBuffer.Clear();
	}

}