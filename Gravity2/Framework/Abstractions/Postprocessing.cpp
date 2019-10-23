#include "stdafx.h"


PostProcessAttachment::PostProcessAttachment() :
	Component(GrvtFramebuffer_AttachComponent_None), Type(GrvtFramebuffer_Attachment_None), Draw(false) {}


PostProcessAttachment::~PostProcessAttachment() {}


PostProcessAttachment::PostProcessAttachment(AttachComponent Component, AttachmentType Type, bool Draw) :
	Component(Component), Type(Type), Draw(Draw) {}


PostProcessAttachment& PostProcessCreationInfo::AddAttachment(AttachComponent Component, AttachmentType Type, bool Draw) {
	return Attachments.Insert(PostProcessAttachment(Component, Type, Draw));
}


void PostProcessCreationInfo::PopAttachment(PostProcessAttachment& Attachment) {
	size_t idx = Attachments.IndexOf(Attachment);
	Attachments.PopAt(idx);
}


GrvtPostProcess::GrvtPostProcess() :
	Attachments(), Handle(), Width(), Height() {}


GrvtPostProcess::~GrvtPostProcess() {}