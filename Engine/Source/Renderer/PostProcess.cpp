#include "GrvtPch.h"
#include "Include/Renderer/PostProcess.h"
#include "Include/Framework/Foundation/Interface.h"
#include "Include/Manager/Manager.h"

namespace Grvt
{

	PostProcessFx::PostProcessFx() :
		Frame(), Shader(nullptr) {}


	PostProcessFx::~PostProcessFx() {}


	PostProcessing::PostProcessing() :
		Hdr() {}


	PostProcessing::~PostProcessing() {}

	
	void PostProcessing::Init(const BaseRenderer* Renderer)
	{
		/**
		* NOTE(Afiq): Should test and see what happens when using a 4 byte floating point framebuffer.
		*/
		{
			Gfl::Pair<uint32, ObjHandle>& ColorAttachment = Hdr.Frame.ColourAttachments.Insert(Gfl::Pair<uint32, ObjHandle>());
			ColorAttachment.Key = RenderTarget_AttachPoint_Colour;

			// HDR ...
			BaseAPI::FramebufferBuildData HdrInfo;

			BaseAPI::TextureBuildData HdrTexInfo;
			HdrTexInfo.Width  = Renderer->Width;
			HdrTexInfo.Height = Renderer->Height;
			HdrTexInfo.Format = GL_RGBA;
			HdrTexInfo.InternalFormat = GL_RGBA16F;
			HdrTexInfo.Type = GL_FLOAT;
			HdrTexInfo.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_LINEAR});
			HdrTexInfo.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_LINEAR});

			HdrInfo.Attachments.Push(BaseAPI::TextureAttachment(&ColorAttachment.Value, GL_COLOR_ATTACHMENT0, HdrTexInfo));

			HdrTexInfo.Format = GL_DEPTH_COMPONENT;
			HdrTexInfo.InternalFormat = GL_DEPTH_COMPONENT;

			Gfl::Pair<uint32, ObjHandle>& DepthAttachment = Hdr.Frame.DepthAttachment;
			DepthAttachment.Key = RenderTarget_AttachPoint_Depth;

			HdrInfo.Attachments.Push(BaseAPI::TextureAttachment(&ColorAttachment.Value, GL_DEPTH_ATTACHMENT, HdrTexInfo));

			Middleware::GetBuildQueue()->QueueFramebufferForBuild(&Hdr.Frame.Handle, HdrInfo);
		}
	}


	void PostProcessing::Free()
	{

	}

}