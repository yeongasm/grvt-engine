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
		ShaderInfo.PathToVertexShader	= "Data\\Shaders\\PostProcessing\\Hdr.vert";
		ShaderInfo.PathToFragmentShader = "Data\\Shaders\\PostProcessing\\Hdr.frag";

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
		HdrTexInfo.Format = Gfx_Format_Rgba;
		HdrTexInfo.InternalFormat = Gfx_Format_Rgba16F;
		HdrTexInfo.Type = Gfx_Type_Float;
		HdrTexInfo.Parameters.Push({ Gfx_TexParam_MinFilter, Gfx_Mip_Linear});
		HdrTexInfo.Parameters.Push({ Gfx_TexParam_MagFilter, Gfx_Mip_Linear });

		HdrInfo.Attachments.Push(Driver::FrameTexAttachment(&ColourAttachment0.Value, Gfx_Attachment_Colour0, HdrTexInfo));
		HdrInfo.Attachments.Push(Driver::FrameTexAttachment(&ColourAttachment1.Value, Gfx_Attachment_Colour1, HdrTexInfo));

		HdrTexInfo.Format = Gfx_Format_Depth;
		HdrTexInfo.InternalFormat = Gfx_Format_Depth;

		Gfl::Pair<uint32, GfxHandle>& DepthAttachment = HDR.DepthAttachment;
		DepthAttachment.Key = RenderTarget_AttachPoint_Depth;

		HdrInfo.Attachments.Push(Driver::FrameTexAttachment(&DepthAttachment.Value, Gfx_Attachment_Depth, HdrTexInfo));

		GfxDriver->BuildFramebuffer(HDR.Handle, HdrInfo);
	}

	
	void PostProcessing::InitialiseBloom(GraphicsDriver* GfxDriver)
	{
		ShaderImportInfo VKInfo;
		VKInfo.Name = "VerticalBlurKernel";
		VKInfo.PathToVertexShader = "Data\\Shaders\\PostProcessing\\BloomVertical.vert";
		VKInfo.PathToFragmentShader = "Data\\Shaders\\PostProcessing\\BloomVertical.frag";

		VerticalBlurKernel = GetResourceManager()->NewShaderProgram(VKInfo);

		ShaderImportInfo HKInfo;
		HKInfo.Name = "HorizontalBlurKernel";
		HKInfo.PathToVertexShader = "Data\\Shaders\\PostProcessing\\BloomHorizontal.vert";
		HKInfo.PathToFragmentShader = "Data\\Shaders\\PostProcessing\\BloomHorizontal.frag";

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

			TexInfo.Format = Gfx_Format_Rgb;
			TexInfo.InternalFormat = Gfx_Format_Rgb16F;
			TexInfo.Type = Gfx_Type_Float;
			TexInfo.Parameters.Push({ Gfx_TexParam_MinFilter, Gfx_Mip_Linear });
			TexInfo.Parameters.Push({ Gfx_TexParam_MagFilter, Gfx_Mip_Linear });
			TexInfo.Parameters.Push({ Gfx_TexParam_WrapS, Gfx_Wrap_ClampToEdge });
			TexInfo.Parameters.Push({ Gfx_TexParam_WrapT, Gfx_Wrap_ClampToEdge });

			Blur.Attachments.Push(Driver::FrameTexAttachment(&ColourAttachment.Value, Gfx_Attachment_Colour0, TexInfo));
			
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

			TexInfo.Format = Gfx_Format_Rgb;
			TexInfo.InternalFormat = Gfx_Format_Rgb16F;
			TexInfo.Type = Gfx_Type_Float;
			TexInfo.Parameters.Push({ Gfx_TexParam_MinFilter, Gfx_Mip_Linear });
			TexInfo.Parameters.Push({ Gfx_TexParam_MagFilter, Gfx_Mip_Linear });
			TexInfo.Parameters.Push({ Gfx_TexParam_WrapS, Gfx_Wrap_ClampToEdge });
			TexInfo.Parameters.Push({ Gfx_TexParam_WrapT, Gfx_Wrap_ClampToEdge });

			Blur.Attachments.Push(Driver::FrameTexAttachment(&ColourAttachment.Value, Gfx_Attachment_Colour0, TexInfo));
			
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