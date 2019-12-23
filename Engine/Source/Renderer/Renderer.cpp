#include "GrvtPch.h"
#include "Renderer/Renderer.h"


Grvt::BaseRenderer* g_Renderer = nullptr;


namespace Grvt
{

	BaseRenderer::BaseRenderer() :
		BackBuffer(),
		FrontBuffer(),
		StateCache(),
		ActiveShader(nullptr),
		PosX(0),
		PosY(0),
		Width(0),
		Height(0) {}


	BaseRenderer* InitRenderer(BaseRenderer* RendererPtr)
	{
		if (g_Renderer)
		{
			return g_Renderer;
		}

		g_Renderer = RendererPtr;
		g_Renderer->Init();

		return g_Renderer;
	}


	BaseRenderer* GetRenderer()
	{
		return g_Renderer;
	}


	void ShutdownRenderer()
	{
		g_Renderer->Shutdown();	
		delete g_Renderer;
	}
}