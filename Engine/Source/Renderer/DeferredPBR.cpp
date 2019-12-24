#include "GrvtPch.h"
#include "Framework/Foundation/Foundations.h"
#include "Renderer/DeferredPBR.h"
#include "Library/GrvtAlgorithms.h"


namespace Grvt
{

	DeferredPBR::DeferredPBR() : BaseRenderer(), SortedCommands(), SortedInstancedCommands() {}

	
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

			//if (Uniform.Type == GrvtShader_AttrType_Matrix)
			//{
			//	switch (Uniform.SubType)
			//	{
			//	case GrvtShader_AttrSubType_Matrix2:
			//		BaseAPI::Shader::GrShaderSetMat2Float(Uniform.Location, &Uniform.Cast<glm::mat2>()[0][0]);
			//		continue;
			//	case GrvtShader_AttrSubType_Matrix3:
			//		BaseAPI::Shader::GrShaderSetMat3Float(Uniform.Location, &Uniform.Cast<glm::mat3>()[0][0]);
			//		continue;
			//	case GrvtShader_AttrSubType_Matrix4:
			//		BaseAPI::Shader::GrShaderSetMat4Float(Uniform.Location, &Uniform.Cast<glm::mat4>()[0][0]);
			//		continue;
			//	}
			//}

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

		//BaseAPI::GrUnbindShaderProgram(Material->Shader->Handle);
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

	
	void DeferredPBR::Init()
	{
		printf("Initialising DeferredPBR renderer!\n");
		BackBuffer.Init();
		FrontBuffer.Init();
	}


	void DeferredPBR::Shutdown()
	{
		printf("Shuting down DeferredPBR renderer!\n");
		SortedCommands.Release();
		SortedInstancedCommands.Release();
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

		Gfl::Array<RenderCommand>& RenderCommands	 = FrontBuffer.RenderCommands;
		Gfl::Array<RenderCommand>& InstancedCommands = FrontBuffer.InstancedCommands;

		glViewport(0, 0, Width, Height);
		//glViewport(PosX, PosY, Width, Height);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		StateCache.SetAlphaBlend(CacheState_None, CacheState_None);
		StateCache.SetDepthFunc(DepthFunc_Less);
		StateCache.SetCullFace(CullFace_Back, FrontFace_CCW);
		StateCache.SetPolygonMode(PolygonFace_Front_And_Back, PolygonMode_Fill);

		// Sort render commands if they exist.
		if (RenderCommands.Length())
		{
			SortCommand(RenderCommands, SortedCommands);
		}

		// Sort instanced commands if they exist.
		if (InstancedCommands.Length())
		{
			SortCommand(InstancedCommands, SortedInstancedCommands);
		}

		RenderCommand* Command = nullptr;

		for (size_t Index : SortedCommands)
		{
			Command = &RenderCommands[Index];	

			if (ActiveShader != Command->Material->Shader)
			{
				ActiveShader = Command->Material->Shader;
			}

			BaseAPI::GrBindShaderProgram(ActiveShader->Handle);

			BaseAPI::Shader::GrShaderSetMat4FloatN(ActiveShader->Handle.Id, "Projection", &FrontBuffer.Projection[0][0]);
			BaseAPI::Shader::GrShaderSetMat4FloatN(ActiveShader->Handle.Id, "View", &FrontBuffer.View[0][0]);
			BaseAPI::Shader::GrShaderSetMat4FloatN(ActiveShader->Handle.Id, "Model", &Command->Transform[0][0]);

			RenderPushedCommand(Command, false);
		}

		StateCache.SetPolygonMode(PolygonFace_Front_And_Back, PolygonMode_Fill);

		SortedCommands.Empty();
		SortedInstancedCommands.Empty();
		FrontBuffer.Clear();
	}

}