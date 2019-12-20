#include "GrvtPch.h"
#include "Framework/Foundation/Foundations.h"
#include "Renderer/DeferredPBR.h"
#include "Library/GrvtAlgorithms.h"


namespace Grvt
{

	DeferredPBR::DeferredPBR() : BaseRenderer(), SortedCommands(), SortedInstancedCommands() {}

	
	DeferredPBR::~DeferredPBR() {}


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


	void DeferredPBR::SortCommand(const Gfl::Array<RenderCommand>& Commands, Gfl::Array<uint32>& SortedCommand)
	{
		Gfl::Array<Gfl::Pair<uint64, size_t>> CommandWeight;

		for (RenderCommand& Command : Commands)
		{
			size_t Index = Commands.IndexOf(Command);
			uint64 Weight = ((uint64)Command.Material->ShaderHandle->Id << 52)
							| ((uint64)Command.State.PolygonFace		<< 48)		
							| ((uint64)Command.State.PolygonMode		<< 44)
							| ((uint64)Command.State.CullFace			<< 40)
							| ((uint64)Command.State.FrontFace			<< 36)
							| ((uint64)Command.State.DepthFunc			<< 28)
							| ((uint64)Command.State.DstBlend			<< 14)
							| ((uint64)Command.State.SrcBlend);
			
			CommandWeight.Push({Weight, Index});
		}

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

		Gfl::Array<RenderCommand>& RenderCommands	 = FrontBuffer.RenderCommands;
		Gfl::Array<RenderCommand>& InstancedCommands = FrontBuffer.InstancedCommands;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		
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

		for (uint32 Index : SortedCommands)
		{
			Command = &RenderCommands[Index];

			// TODO(Afiq):
			// Render the normal commands.
		}

		FrontBuffer.Clear();
	}

}