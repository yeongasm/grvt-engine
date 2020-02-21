#include "GrvtPch.h"
#include "Renderer/PostProcess.h"

#include "API/Graphics/GraphicsDriver.h"
#include "Core/Engine.h"

namespace Grvt
{

	PostProcessing::PostProcessing() :
		HDR(),
		BloomTarget0(),
		BloomTarget1(),
		HDRShader(nullptr),
		VerticalBlurKernel(nullptr),
		HorizontalBlurKernel(nullptr),
		BloomIteration(9),
		Exposure(2.5f),
		Gamma(0.5f),
		Bloom(true),
		Renderer(nullptr) {}


	PostProcessing::~PostProcessing() {}

	
	void PostProcessing::InitialiseHDRToneMapping(GraphicsDriver* GfxDriver)
	{
		/**
		* NOTE(Afiq): Should test and see what happens when using a 4 byte floating point framebuffer.
		*/
		ShaderImportInfo ShaderInfo;
		ShaderInfo.Name = "HDRShader";
		ShaderInfo.PathToVertexShader	= "";
		ShaderInfo.PathToFragmentShader = "";

		HDRShader = GetResourceManager()->NewShaderProgram(ShaderInfo);

		Gfl::Pair<uint32, GfxHandle>& ColourAttachment0 = HDR.ColourAttachments.Insert(Gfl::Pair<uint32, GfxHandle>());
		Gfl::Pair<uint32, GfxHandle>& ColourAttachment1 = HDR.ColourAttachments.Insert(Gfl::Pair<uint32, GfxHandle>());
		
		ColourAttachment0.Key = RenderTarget_AttachPoint_Colour;
		ColourAttachment1.Key = RenderTarget_AttachPoint_Colour;

		// HDR ...
		Driver::FramebufferBuildData HdrInfo;

		HdrInfo.Width  = Renderer->Width;
		HdrInfo.Height = Renderer->Height;

		Driver::TextureBuildData HdrTexInfo;
		HdrTexInfo.Format = GL_RGBA;
		HdrTexInfo.InternalFormat = GL_RGBA16F;
		HdrTexInfo.Type = GL_FLOAT;
		HdrTexInfo.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_LINEAR});
		HdrTexInfo.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_LINEAR});

		HdrInfo.Attachments.Push(Driver::FrameTexAttachment(&ColourAttachment0.Value, GL_COLOR_ATTACHMENT0, HdrTexInfo));
		HdrInfo.Attachments.Push(Driver::FrameTexAttachment(&ColourAttachment1.Value, GL_COLOR_ATTACHMENT1, HdrTexInfo));

		HdrTexInfo.Format = GL_DEPTH_COMPONENT;
		HdrTexInfo.InternalFormat = GL_DEPTH_COMPONENT;

		Gfl::Pair<uint32, GfxHandle>& DepthAttachment = HDR.DepthAttachment;
		DepthAttachment.Key = RenderTarget_AttachPoint_Depth;

		HdrInfo.Attachments.Push(Driver::FrameTexAttachment(&DepthAttachment.Value, GL_DEPTH_ATTACHMENT, HdrTexInfo));

		GfxDriver->BuildFramebuffer(HDR.Handle, HdrInfo);
	}

	
	void PostProcessing::InitialiseBloom(GraphicsDriver* GfxDriver)
	{
		ShaderImportInfo VKInfo;
		VKInfo.Name = "VerticalBlurKernel";
		VKInfo.PathToVertexShader = "";
		VKInfo.PathToFragmentShader = "";

		VerticalBlurKernel = GetResourceManager()->NewShaderProgram(VKInfo);

		ShaderImportInfo HKInfo;
		HKInfo.Name = "HorizontalBlurKernel";
		HKInfo.PathToVertexShader = "";
		HKInfo.PathToFragmentShader = "";

		HorizontalBlurKernel = GetResourceManager()->NewShaderProgram(HKInfo);

		// Ping pong Blur Framebuffers;
		// Framebuffer 1
		{
			Gfl::Pair<uint32, GfxHandle>& ColourAttachment = BloomTarget0.ColourAttachments.Insert(Gfl::Pair<uint32, GfxHandle>());
			ColourAttachment.Key = RenderTarget_AttachPoint_Colour;

			Driver::FramebufferBuildData Blur;

			Blur.Width  = Renderer->Width;
			Blur.Height = Renderer->Height;

			Driver::TextureBuildData TexInfo;

			TexInfo.Format = GL_RGB;
			TexInfo.InternalFormat = GL_RGB16F;
			TexInfo.Type = GL_FLOAT;
			TexInfo.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_LINEAR});
			TexInfo.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_LINEAR});
			TexInfo.Parameters.Push({GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE});
			TexInfo.Parameters.Push({GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE});

			Blur.Attachments.Push(Driver::FrameTexAttachment(&ColourAttachment.Value, GL_COLOR_ATTACHMENT0, TexInfo));
			
			GfxDriver->BuildFramebuffer(BloomTarget0.Handle, Blur);
		}

		// Framebuffer 2
		{
			Gfl::Pair<uint32, GfxHandle>& ColourAttachment = BloomTarget1.ColourAttachments.Insert(Gfl::Pair<uint32, GfxHandle>());
			ColourAttachment.Key = RenderTarget_AttachPoint_Colour;

			Driver::FramebufferBuildData Blur;

			Blur.Width  = Renderer->Width;
			Blur.Height = Renderer->Height;

			Driver::TextureBuildData TexInfo;

			TexInfo.Format = GL_RGB;
			TexInfo.InternalFormat = GL_RGB16F;
			TexInfo.Type = GL_FLOAT;
			TexInfo.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_LINEAR});
			TexInfo.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_LINEAR});
			TexInfo.Parameters.Push({GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE});
			TexInfo.Parameters.Push({GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE});

			Blur.Attachments.Push(Driver::FrameTexAttachment(&ColourAttachment.Value, GL_COLOR_ATTACHMENT0, TexInfo));
			
			GfxDriver->BuildFramebuffer(BloomTarget1.Handle, Blur);
		}
	}


	void PostProcessing::Init(GrvtRenderer* RendererPtr)
	{
		Renderer = RendererPtr;

		InitialiseHDRToneMapping(RendererPtr->GfxDriver);
		InitialiseBloom(RendererPtr->GfxDriver);
	}


	void PostProcessing::Free()
	{
		Renderer->GfxDriver->DeleteBuffer(HDR.Handle);
		Renderer->GfxDriver->DeleteBuffer(BloomTarget0.Handle);
		Renderer->GfxDriver->DeleteBuffer(BloomTarget1.Handle);
	}

}