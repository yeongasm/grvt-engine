#include "stdafx.h"


RenderBufferCreationInfo::RenderBufferCreationInfo() : name() {}


RenderBufferCreationInfo::RenderBufferCreationInfo(const RenderBufferCreationInfo &Other) { *this = Other; }


RenderBufferCreationInfo& RenderBufferCreationInfo::operator= (const RenderBufferCreationInfo &Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		name = Other.name;
		type = Other.type;
	}

	return *this;
}


RenderBufferCreationInfo::RenderBufferCreationInfo(RenderBufferCreationInfo &&Other) { *this = std::move(Other); }


RenderBufferCreationInfo& RenderBufferCreationInfo::operator= (RenderBufferCreationInfo &&Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		name = Other.name;
		type = Other.type;

		new (&Other) RenderBufferCreationInfo();
	}

	return *this;
}


RenderBufferCreationInfo::~RenderBufferCreationInfo() {
	name.Release();
	type = RENDERBUFFER_TYPE_NONE;
}


RenderBuffer::RenderBuffer() : id(), type(RENDERBUFFER_TYPE_NONE), info(nullptr) {}


RenderBuffer::RenderBuffer(RenderBuffer &&Other) { *this = std::move(Other); }


RenderBuffer& RenderBuffer::operator= (RenderBuffer &&Other) {
	_ASSERTE(this != &Other);
	
	if (this != &Other) {
		id		= std::move(Other.id);
		type	= Other.type;
		info	= Other.info;

		new (&Other) RenderBuffer();
	}

	return *this;
}


RenderBuffer::~RenderBuffer() {}


PostProcessAttachment::PostProcessAttachment() : texture(nullptr), type(FRAMEBUFFER_ATTACHMENT_NONE) {}


PostProcessAttachment::PostProcessAttachment(const PostProcessAttachment &Other) { *this = Other; }


PostProcessAttachment& PostProcessAttachment::operator= (const PostProcessAttachment &Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		if (Other.type == FRAMEBUFFER_ATTACHMENT_TEXTURE)
			texture = Other.texture;

		if (Other.type == FRAMEBUFFER_ATTACHMENT_RENDERBUFFER)
			renderbuffer = Other.renderbuffer;

		type = Other.type;
	}

	return *this;
}


PostProcessAttachment::PostProcessAttachment(PostProcessAttachment &&Other) { *this = std::move(Other); }


PostProcessAttachment& PostProcessAttachment::operator= (PostProcessAttachment &&Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		if (Other.type == FRAMEBUFFER_ATTACHMENT_TEXTURE)
			texture = Other.texture;

		if (Other.type == FRAMEBUFFER_ATTACHMENT_RENDERBUFFER)
			renderbuffer = Other.renderbuffer;

		type = Other.type;

		new (&Other) PostProcessAttachment();
	}

	return *this;
}


PostProcessAttachment::~PostProcessAttachment() {
	texture = nullptr;
	type	= FRAMEBUFFER_ATTACHMENT_NONE;
}


PostProcessCreationInfo::PostProcessCreationInfo() : name(), attachment(), width(0), height(0) {}


PostProcessCreationInfo::PostProcessCreationInfo(const PostProcessCreationInfo &Other) { *this = Other; }


PostProcessCreationInfo& PostProcessCreationInfo::operator= (const PostProcessCreationInfo &Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		name		= Other.name;
		attachment	= Other.attachment;
		width		= Other.width;
		height		= Other.height;
	}

	return *this;
}


PostProcessCreationInfo::PostProcessCreationInfo(PostProcessCreationInfo &&Other) { *this = std::move(Other); }


PostProcessCreationInfo& PostProcessCreationInfo::operator= (PostProcessCreationInfo &&Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		name		= Other.name;
		attachment	= Other.attachment;
		width		= Other.width;
		height		= Other.height;

		new (&Other) PostProcessCreationInfo();
	}

	return *this;
}


PostProcessCreationInfo::~PostProcessCreationInfo() {
	name.Release();
	attachment.Release();
}


PostProcess::PostProcess() : id(), width(0), height(0), info(nullptr), attachment() {}


PostProcess::PostProcess(PostProcess &&Other) { *this = std::move(Other); }


PostProcess& PostProcess::operator= (PostProcess &&Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		id			= std::move(Other.id);
		width		= Other.width;
		height		= Other.height;
		info		= Other.info;
		attachment	= Other.attachment;

		new (&Other) PostProcess();
	}

	return *this;
}


PostProcess::~PostProcess() {
	width = height = 0;
	info = nullptr;
	attachment.Release();
}