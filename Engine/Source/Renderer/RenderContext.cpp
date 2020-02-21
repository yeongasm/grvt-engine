#include "GrvtPch.h"
#include "Renderer/RenderContext.h"


namespace Grvt
{

	RenderContext* g_RenderCtx = nullptr;


	RenderContext::RenderContext() :
		Engine(nullptr), GlDriver(nullptr), GlInterface(), GlRenderer(nullptr) {}


	RenderContext::~RenderContext() {}


	void RenderContext::Execute(GrvtEngine* EnginePtr, GLADloadproc LoadProcFunc)
	{
		Engine = EnginePtr;

		glfwMakeContextCurrent(Engine->Window);

		if (!gladLoadGLLoader(LoadProcFunc))
		{
			// Failed to initialise GLAD.
			_ASSERTE(false);

			exit(0);
		}

		GlDriver	= new GraphicsDriver();
		GlRenderer	= new GrvtRenderer();

		GlInterface.Init(GlDriver);

		GlRenderer->Init(Engine->PosX, Engine->PosY, Engine->Width, Engine->Height, GlDriver);
	}


	RenderContext* InitialiseRenderContext()
	{
		g_RenderCtx = new RenderContext();

		return g_RenderCtx;
	}


	void ShutdownRenderContext()
	{
		g_RenderCtx->GlRenderer->Shutdown();
		g_RenderCtx->GlInterface.Shutdown();

		delete g_RenderCtx;
	}


	RenderContext* GetRenderContext()
	{
		return g_RenderCtx;
	}


	GraphicsInterface* GetGraphicsInterface()
	{
		return &g_RenderCtx->GlInterface;
	}


	GrvtRenderer* GetRenderer()
	{
		return g_RenderCtx->GlRenderer;
	}
}