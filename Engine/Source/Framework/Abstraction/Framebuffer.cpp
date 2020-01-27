#include "GrvtPch.h"
#include "Framework/Abstraction/Framebuffer.h"
#include "Minimal.h"

namespace Grvt
{

	bool FramebufferCreationInfo::AddAttachment(AttachComponent Component, AttachmentType Type)
	{
		_ASSERTE(Component != GrvtFramebuffer_AttachComponent_RenderBuffer); // Renderbuffers are currently not being supported!

		for (Gfl::Pair<AttachComponent, AttachmentType>& Attachment : Attachments)
		{
			if (Attachment.Value != Type && Attachment.Value != GrvtFramebuffer_Attachment_Colour)
				continue;

			return false;
		}

		Attachments.Push({Component, Type});

		return true;
	}


	void FramebufferCreationInfo::PopAttachment(AttachmentType Type, size_t Count)
	{
		uint32 Index = 0;
		uint32 Popped = 0;

		if (Type != GrvtFramebuffer_Attachment_Colour && Count != 1)
			Count = 1;

		while (Popped != Count)
		{
			if (Attachments[Index].Value == Type)
			{
				Attachments.PopAt(Index);
				Popped++;
			}

			Index++;
		}
	}


	FrmBfrAttachment::FrmBfrAttachment() :
		Handle(), Component(GrvtFramebuffer_AttachComponent_None), Type(GrvtFramebuffer_Attachment_None), Floats(false) {}


	FrmBfrAttachment::~FrmBfrAttachment() {}


	FrmBfrAttachment::FrmBfrAttachment(FrmBfrAttachment&& Other)
	{
		*this = Gfl::Move(Other);
	}


	FrmBfrAttachment& FrmBfrAttachment::operator= (FrmBfrAttachment&& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Handle		= Gfl::Move(Other.Handle);
			Component	= Gfl::Move(Other.Component);
			Type		= Gfl::Move(Other.Type);
			Floats		= Gfl::Move(Other.Floats);

			new (&Other) FrmBfrAttachment();
		}

		return *this;
	}


	GrvtFramebuffer::GrvtFramebuffer() :
		ColourAttachments(), DepthAttachment(), DepthStencilAttachment(), Handle(), Width(0), Height(0) {}


	GrvtFramebuffer::~GrvtFramebuffer() {}


	void GrvtFramebuffer::Alloc(const FramebufferCreationInfo& Info)
	{
		Width  = Info.Width;
		Height = Info.Height;

		for (Gfl::Pair<AttachComponent, AttachmentType>& Attachment : Info.Attachments)
		{
			if (Attachment.Value == GrvtFramebuffer_Attachment_Colour)
			{
				FrmBfrAttachment& Colour = ColourAttachments.Insert(FrmBfrAttachment());
				Colour.Component = Attachment.Key;
				Colour.Type = Attachment.Value;

				continue;
			}

			if (Attachment.Value == GrvtFramebuffer_Attachment_Depth)
			{
				DepthAttachment.Component = Attachment.Key;
				DepthAttachment.Type = Attachment.Value;

				continue;
			}

			if (Attachment.Value == GrvtFramebuffer_Attachment_DepthStencil)
			{
				DepthStencilAttachment.Component = Attachment.Key;
				DepthStencilAttachment.Type = Attachment.Value;

				continue;
			}
		}
	}


	void GrvtFramebuffer::Free()
	{
		ColourAttachments.Release();
	}

}