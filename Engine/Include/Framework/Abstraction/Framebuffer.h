#pragma once

#ifndef GRAVITY_ABSTRACTION_FRAMEBUFFER
#define GRAVITY_ABSTRACTION_FRAMEBUFFER

#include "Minimal.h"
#include "Framework/Foundation/Foundations.h"

namespace Grvt
{

	/**
	* Revamp Status: Completed!
	*/


	/**
	*/
	enum AttachComponent : uint32
	{
		GrvtFramebuffer_AttachComponent_None			= 0xFF, /** On first init only */
		GrvtFramebuffer_AttachComponent_Texture			= 0x00,
		GrvtFramebuffer_AttachComponent_RenderBuffer	= 0x01, /** Renderbuffers are currently not being supported */
		GrvtFramebuffer_AttachComponent_Cubemap			= 0x02
	};


	/**
	* Specify the framebuffer's sub attachment type.
	*/
	enum AttachmentType : uint32
	{
		GrvtFramebuffer_Attachment_None			= 0x00, /** On first init only */
		GrvtFramebuffer_Attachment_Colour		= 0x01,
		GrvtFramebuffer_Attachment_Depth		= 0x02,
		GrvtFramebuffer_Attachment_DepthStencil = 0x04
	};


	class GrvtFramebuffer;


	/**
	*/
	class FramebufferCreationInfo
	{
	private:

		Gfl::Array<Gfl::Pair<AttachComponent, AttachmentType>> Attachments;

		friend class GrvtFramebuffer;

	public:

		Gfl::String	Name;
		uint32		Width;
		uint32		Height;

		/**
		* Pushes a new attachment for the to be created framebuffer.
		* If the specified attachment type already exists, then Gravity will ignore it and use the existing one unless the specified type is popped away first.
		* The statement above is only true for attachments of type Depth and DepthStencil.
		*/
		ENGINE_API bool AddAttachment(AttachComponent Component, AttachmentType Type);

		/**
		* Pops a previously added attachment from the to be created framebuffer.
		* Default count is 1.
		*/
		ENGINE_API void PopAttachment(AttachmentType Type, size_t Count = 1);
	};


	struct FrmBfrAttachment
	{
		ObjHandle		Handle;
		AttachComponent Component;
		AttachmentType	Type;
		bool			Floats;

		FrmBfrAttachment();
		~FrmBfrAttachment();

		FrmBfrAttachment(const FrmBfrAttachment& Other)					= delete;
		FrmBfrAttachment& operator= (const FrmBfrAttachment& Other)		= delete;

		FrmBfrAttachment(FrmBfrAttachment&& Other);
		FrmBfrAttachment& operator= (FrmBfrAttachment&& Other);
	};


	/**
	* Revamped GrvtFramebuffer object.
	*/
	class GrvtFramebuffer
	{
	private:

		friend class ResourceManager;

		using FramebufferAttachments = Gfl::Array<FrmBfrAttachment>;

	public:

		FramebufferAttachments	ColourAttachments;
		FrmBfrAttachment		DepthAttachment;
		FrmBfrAttachment		DepthStencilAttachment;
		ObjHandle				Handle;
		uint32					Width;
		uint32					Height;

		GrvtFramebuffer();
		~GrvtFramebuffer();

		void Alloc(const FramebufferCreationInfo& Info);
		void Free();

	private:

		GrvtFramebuffer(const GrvtFramebuffer& Rhs)				= delete;
		GrvtFramebuffer& operator= (const GrvtFramebuffer& Rhs) = delete;

		GrvtFramebuffer(GrvtFramebuffer&& Rhs)					= delete;
		GrvtFramebuffer& operator= (GrvtFramebuffer&& Rhs)		= delete;

	};

}

#endif // !GRAVITY_ABSTRACTION_FRAMEBUFFER