#pragma once

#ifndef GRAVITY_BASE_RENDERER
#define GRAVITY_BASE_RENDERER

#include "RenderFoundation.h"
#include "RenderCache.h"

#include "PostProcess.h"

#include "Framework/Scene.h"

namespace Grvt
{

	/**
	* GrvtRenderer data structure.
	*
	* GrvtRenderer is an abstract class that acts like a shell for any custom implemented renderer.
	*/
	class Renderer
	{
	private:

		friend class	GrvtEngine;
		friend struct	CommandBuffer;
		friend class	GrvtScene;

		CommandBuffer	BackBuffer;
		CommandBuffer	FrontBuffer;

		RenderCache		StateCache;

		GrvtShader*		ActiveShader;
		GrvtMesh*		NDCPlane;


	public:

		uint32			PosX;
		uint32			PosY;
		uint32			Width;
		uint32			Height;
		
	public:

		Renderer();
		~Renderer();

		void Init();
		void Render();
		void Shutdown();
	};

}

#endif // !GRAVITY_BASE_RENDERER