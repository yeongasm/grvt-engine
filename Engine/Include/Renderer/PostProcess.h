#pragma once

#ifndef GRAVITY_POST_PROCESS
#define GRAVITY_POST_PROCESS

#include "RenderFoundation.h"

namespace Grvt
{

	/**
	* Data structure to hold all post processing effects.
	* End goal would include HDR, Bloom, Vignette, SSAO, TXAA
	*
	* There should be a method to include the material system into post processing.
	*/
	class PostProcessing
	{
	public:

		RenderTarget	HDR;
		RenderTarget	BloomTarget0;
		RenderTarget	BloomTarget1;

		GrvtShader*		HDRShader;
		GrvtShader*		VerticalBlurKernel;
		GrvtShader*		HorizontalBlurKernel;

		uint32	BloomIteration;
		float32	Exposure;
		float32	Gamma;

		bool	Bloom;

		PostProcessing();
		~PostProcessing();

	private:

		BaseRenderer* RendererPtr;

		PostProcessing(const PostProcessing& Other)				= delete;
		PostProcessing& operator= (const PostProcessing& Other) = delete;

		PostProcessing(PostProcessing&& Other)					= delete;
		PostProcessing& operator= (PostProcessing&& Other)		= delete;

		void InitialiseHDRToneMapping();
		void InitialiseBloom();

	public:

		void Init(BaseRenderer* Renderer);
		void Free();

	};

}


#endif // !GRAVITY_POST_PROCESS