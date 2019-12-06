#pragma once
#ifndef GRAVITY_DEFERRED_RENDERER
#define GRAVITY_DEFERRED_RENDERER

#include "Renderer.h"
#include "RenderCache.h"
#include "RenderFoundation.h"
#include "Framework/Abstraction/Shader.h"

namespace Grvt
{

	class DeferredRenderer : public BaseRenderer
	{
	private:
		RenderCache			Cache;
		GrvtCommandBuffer*	CommandBuffer;
		GrvtShader*			ActiveShader;
	};

}

#endif // !GRAVITY_DEFERRED_RENDERER