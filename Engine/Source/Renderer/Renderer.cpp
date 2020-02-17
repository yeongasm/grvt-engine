#include "GrvtPch.h"
#include "Renderer/Renderer.h"


namespace Grvt
{

	Renderer::Renderer() :
		UnsortedCommands(),
		UnsortedInstancedCommands(),
		SortedCommands(),
		SortedInstancedCommands(),
		MaterialLib(),
		PostProcess(),
		GBuffer(),
		StateCache(),
		BackBuffer(),
		FrontBuffer(),
		ActiveShader(nullptr),
		ScreenQuad(nullptr),
		ProjectionViewUBO(),
		PosX(0),
		PosY(0),
		Width(0),
		Height(0) {}


	Renderer::~Renderer() {}


	void Renderer::Init()
	{

	}

	
	void Renderer::Shutdown()
	{

	}


	void Renderer::Render(const GraphicsDriver* GlDriver)
	{

	}


	void Renderer::CreateRenderCommandBuffer(GrvtScene* ActiveScene)
	{

	}


}