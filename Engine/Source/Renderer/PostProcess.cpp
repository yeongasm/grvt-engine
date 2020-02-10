#include "GrvtPch.h"
#include "Include/Renderer/PostProcess.h"
#include "Include/Framework/Foundation/Interface.h"
#include "Include/Manager/Manager.h"

#include "Include/Framework/DefaultShaders/PostProcessing.h"

namespace Grvt
{

	PostProcessing::PostProcessing() :
		HDR(),
		BloomTarget0(),
		BloomTarget1(),
		HDRShader(nullptr),
		VerticalBlurKernel(nullptr),
		HorizontalBlurKernel(nullptr),
		BloomIteration(10),
		Exposure(2.5f),
		Gamma(0.5f),
		Bloom(true),
		RendererPtr(nullptr) {}


	PostProcessing::~PostProcessing() {}

	
	void PostProcessing::InitialiseHDRToneMapping()
	{
		/**
		* NOTE(Afiq): Should test and see what happens when using a 4 byte floating point framebuffer.
		*/
		ShaderImportInfo HDRShaderInfo;
		HDRShaderInfo.Name = "HDRShader";
		HDRShaderInfo.AddShaderToProgram(HDRBloomShader::VertexShader, GrvtShader_SourceType_Vertex);
		HDRShaderInfo.AddShaderToProgram(HDRBloomShader::FragmentShader, GrvtShader_SourceType_Fragment);

		HDRShader = GetResourceManager()->NewShaderProgram(HDRShaderInfo);

		Gfl::Pair<uint32, ObjHandle>& ColourAttachment0 = HDR.ColourAttachments.Insert(Gfl::Pair<uint32, ObjHandle>());
		Gfl::Pair<uint32, ObjHandle>& ColourAttachment1 = HDR.ColourAttachments.Insert(Gfl::Pair<uint32, ObjHandle>());
		
		ColourAttachment0.Key = RenderTarget_AttachPoint_Colour;
		ColourAttachment1.Key = RenderTarget_AttachPoint_Colour;

		// HDR ...
		BaseAPI::FramebufferBuildData HdrInfo;

		HdrInfo.Width  = RendererPtr->Width;
		HdrInfo.Height = RendererPtr->Height;

		BaseAPI::TextureBuildData HdrTexInfo;
		HdrTexInfo.Format = GL_RGBA;
		HdrTexInfo.InternalFormat = GL_RGBA16F;
		HdrTexInfo.Type = GL_FLOAT;
		HdrTexInfo.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_LINEAR});
		HdrTexInfo.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_LINEAR});

		HdrInfo.Attachments.Push(BaseAPI::TextureAttachment(&ColourAttachment0.Value, GL_COLOR_ATTACHMENT0, HdrTexInfo));
		HdrInfo.Attachments.Push(BaseAPI::TextureAttachment(&ColourAttachment1.Value, GL_COLOR_ATTACHMENT1, HdrTexInfo));

		HdrTexInfo.Format = GL_DEPTH_COMPONENT;
		HdrTexInfo.InternalFormat = GL_DEPTH_COMPONENT;

		Gfl::Pair<uint32, ObjHandle>& DepthAttachment = HDR.DepthAttachment;
		DepthAttachment.Key = RenderTarget_AttachPoint_Depth;

		HdrInfo.Attachments.Push(BaseAPI::TextureAttachment(&DepthAttachment.Value, GL_DEPTH_ATTACHMENT, HdrTexInfo));

		Middleware::GetBuildQueue()->QueueFramebufferForBuild(&HDR.Handle, HdrInfo);
	}

	
	void PostProcessing::InitialiseBloom()
	{
		ShaderImportInfo VKInfo;
		VKInfo.Name = "VerticalBlurKernel";
		VKInfo.AddShaderToProgram(BloomVerticalKernel::VertexShader, GrvtShader_SourceType_Vertex);
		VKInfo.AddShaderToProgram(BloomVerticalKernel::FragmentShader, GrvtShader_SourceType_Fragment);

		VerticalBlurKernel = GetResourceManager()->NewShaderProgram(VKInfo);

		ShaderImportInfo HKInfo;
		HKInfo.Name = "HorizontalBlurKernel";
		HKInfo.AddShaderToProgram(BloomHorizontalKernel::VertexShader, GrvtShader_SourceType_Vertex);
		HKInfo.AddShaderToProgram(BloomHorizontalKernel::FragmentShader, GrvtShader_SourceType_Fragment);

		HorizontalBlurKernel = GetResourceManager()->NewShaderProgram(HKInfo);

		// Ping pong Blur Framebuffers;
		// Framebuffer 1
		{
			Gfl::Pair<uint32, ObjHandle>& ColourAttachment = BloomTarget0.ColourAttachments.Insert(Gfl::Pair<uint32, ObjHandle>());
			ColourAttachment.Key = RenderTarget_AttachPoint_Colour;

			BaseAPI::FramebufferBuildData Blur;

			Blur.Width  = RendererPtr->Width;
			Blur.Height = RendererPtr->Height;

			BaseAPI::TextureBuildData TexInfo;

			TexInfo.Format = GL_RGB;
			TexInfo.InternalFormat = GL_RGB16F;
			TexInfo.Type = GL_FLOAT;
			TexInfo.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_LINEAR});
			TexInfo.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_LINEAR});
			TexInfo.Parameters.Push({GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE});
			TexInfo.Parameters.Push({GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE});

			Blur.Attachments.Push(BaseAPI::TextureAttachment(&ColourAttachment.Value, GL_COLOR_ATTACHMENT0, TexInfo));
			Middleware::GetBuildQueue()->QueueFramebufferForBuild(&BloomTarget0.Handle, Blur);
		}

		// Framebuffer 2
		{
			Gfl::Pair<uint32, ObjHandle>& ColourAttachment = BloomTarget1.ColourAttachments.Insert(Gfl::Pair<uint32, ObjHandle>());
			ColourAttachment.Key = RenderTarget_AttachPoint_Colour;

			BaseAPI::FramebufferBuildData Blur;

			Blur.Width = RendererPtr->Width;
			Blur.Height = RendererPtr->Height;

			BaseAPI::TextureBuildData TexInfo;

			TexInfo.Format = GL_RGB;
			TexInfo.InternalFormat = GL_RGB16F;
			TexInfo.Type = GL_FLOAT;
			TexInfo.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_LINEAR});
			TexInfo.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_LINEAR});
			TexInfo.Parameters.Push({GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE});
			TexInfo.Parameters.Push({GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE});

			Blur.Attachments.Push(BaseAPI::TextureAttachment(&ColourAttachment.Value, GL_COLOR_ATTACHMENT0, TexInfo));
			Middleware::GetBuildQueue()->QueueFramebufferForBuild(&BloomTarget1.Handle, Blur);
		}
	}


	void PostProcessing::Init(BaseRenderer* Renderer)
	{
		RendererPtr = Renderer;

		InitialiseHDRToneMapping();
		InitialiseBloom();
	}


	void PostProcessing::Free()
	{

	}

}