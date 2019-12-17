#include "GrvtPch.h"
#include "Framework/Foundation/Foundations.h"
#include "Renderer/DeferredPBR.h"


namespace Grvt
{

	DeferredPBR::DeferredPBR() : BaseRenderer() {}

	
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

	
	void DeferredPBR::Init()
	{
		printf("Initialising DeferredPBR renderer!\n");
		BackBuffer.Init();
		FrontBuffer.Init();
	}


	void DeferredPBR::Shutdown()
	{
		printf("Shuting down DeferredPBR renderer!\n");
	}


	void DeferredPBR::Render()
	{
		/**
		* Swap the content's of the back buffer into the front buffer if the front buffer is empty.
		* When the renderer's front buffer is not empty, wait til it clears out before swapping with the back buffer.
		* Should disable the program to fill the back buffer when this process fails.
		*/
		if (FrontBuffer.IsEmpty/* && !BackBuffer.IsEmpty*/)
		{
			FrontBuffer = BackBuffer;
			FrontBuffer.IsEmpty = false;
			BackBuffer.Clear();
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		FrontBuffer.Clear();
	}

}