#pragma once
#ifndef GRAVITY_RENDERER_RENDERSERVICE
#define GRAVITY_RENDERER_RENDERSERVICE

#include "API/Graphics/GraphicsDriver.h"
#include "Renderer.h"

namespace Grvt
{

	class RenderService
	{
	private:

		GraphicsDriver* GlDriver;
		Renderer*		Renderer;

		void Init();
		void Shutdown();

	public:

		RenderService();
		~RenderService();

		void Execute(GLFWwindow* Window, GLADloadproc ProcAddress);
	};

}


#endif // !GRAVITY_RENDERER_RENDERSERVICE