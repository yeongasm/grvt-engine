#pragma once

#ifndef GRAVITY_POST_PROCESS
#define GRAVITY_POST_PROCESS

#include "RenderFoundation.h"

namespace Grvt
{


	struct PostProcessFx
	{
		RenderTarget	Frame;
		GrvtShader*		Shader;

		PostProcessFx();
		~PostProcessFx();
	};


	class PostProcessing
	{
	public:

		PostProcessFx Hdr;

		PostProcessing();
		~PostProcessing();

	private:

		PostProcessing(const PostProcessing& Other)				= delete;
		PostProcessing& operator= (const PostProcessing& Other) = delete;

		PostProcessing(PostProcessing&& Other)					= delete;
		PostProcessing& operator= (PostProcessing&& Other)		= delete;

	public:

		void Init(const BaseRenderer* Renderer);
		void Free();

	};

}


#endif // !GRAVITY_POST_PROCESS