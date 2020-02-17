#include "GrvtPch.h"
#include "Renderer/RenderContext.h"


namespace Grvt
{

	RenderContext::RenderContext() :
		Engine(nullptr), GlDriver(nullptr), GlInterface(nullptr), GlRenderer(nullptr) {}


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
		GlRenderer	= new Renderer();

		GlInterface.Init(GlDriver);
	}
}