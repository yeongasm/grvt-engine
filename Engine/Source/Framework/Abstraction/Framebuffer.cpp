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


	GrvtFramebuffer::Attachment::Attachment() :
		Handle(), Component(GrvtFramebuffer_AttachComponent_None), Type(GrvtFramebuffer_Attachment_None), Count(0) {}


	GrvtFramebuffer::Attachment::~Attachment() {}


	GrvtFramebuffer::Attachment::Attachment(GrvtFramebuffer::Attachment&& Rhs)
	{
		*this = Gfl::Move(Rhs);
	}


	GrvtFramebuffer::Attachment& GrvtFramebuffer::Attachment::operator= (GrvtFramebuffer::Attachment&& Rhs)
	{
		_ASSERTE(this != &Rhs);

		if (this != &Rhs)
		{
			Handle = Gfl::Move(Rhs.Handle);
			Component = Rhs.Component;
			Type = Rhs.Type;
			Count = Rhs.Count;

			new (&Rhs) GrvtFramebuffer::Attachment();
		}

		return *this;
	}


	GrvtFramebuffer::GrvtFramebuffer() :
		Attachments(), Handle(), Width(0), Height(0) {}


	GrvtFramebuffer::~GrvtFramebuffer() {}


	void GrvtFramebuffer::Alloc(const FramebufferCreationInfo& Info)
	{
		Width = Info.Width;
		Height = Info.Height;
		size_t Count = 0;

		for (Gfl::Pair<AttachComponent, AttachmentType>& Attach : Info.Attachments)
		{
			Attachment& AttachPoint = Attachments.Insert(Attachment());

			AttachPoint.Component = Attach.Key;
			AttachPoint.Type = Attach.Value;
			AttachPoint.Count = (Attach.Value == GrvtFramebuffer_Attachment_Colour) ? (uint32)Count++ : 0;
		}
	}


	void GrvtFramebuffer::Free()
	{
		Attachments.Release();
	}

}