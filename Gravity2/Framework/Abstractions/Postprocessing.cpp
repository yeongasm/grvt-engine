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


GrvtPostProcess::AttachmentProperty::AttachmentProperty() :
	Handle(), Component(GrvtFramebuffer_AttachComponent_None), Type(GrvtFramebuffer_Attachment_None), Count(0), Draw(false) {}


GrvtPostProcess::AttachmentProperty::~AttachmentProperty() {}


GrvtPostProcess::GrvtPostProcess() :
	Attachments(), Handle(), Width(), Height() {}


GrvtPostProcess::~GrvtPostProcess() {}


GrvtPostProcess* GrvtPostProcess::Alloc(const PostProcessCreationInfo& Info) {
	Width = Info.Width;
	Height = Info.Height;

	AttachmentProperty* attachProp = nullptr;
	for (PostProcessAttachment& attachment : Info.Attachments) {
		attachProp = &Attachments.Insert(AttachmentProperty());
		attachProp->Type = attachment.Type;
		attachProp->Component = attachment.Component;
		attachProp->Draw = attachment.Draw;
	}
}


void GrvtPostProcess::Free() {
	Attachments.Release();
}