#include "stdafx.h"


RenderBufferCreationInfo::RenderBufferCreationInfo() :
	Name(), Type(RENDERBUFFER_TYPE_NONE) {}


RenderBufferCreationInfo::~RenderBufferCreationInfo() {
	Name.Release();
	Type = RENDERBUFFER_TYPE_NONE;
}


RenderBufferCreationInfo::RenderBufferCreationInfo(const RenderBufferCreationInfo &Other) { *this = Other; }


RenderBufferCreationInfo& RenderBufferCreationInfo::operator= (const RenderBufferCreationInfo &Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Name = Other.Name;
		Type = Other.Type;
	}

	return *this;
}


RenderBufferCreationInfo::RenderBufferCreationInfo(RenderBufferCreationInfo &&Other) { *this = std::move(Other); }


RenderBufferCreationInfo& RenderBufferCreationInfo::operator= (RenderBufferCreationInfo &&Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Name = Other.Name;
		Type = Other.Type;

		new (&Other) RenderBufferCreationInfo();
	}

	return *this;
}


RenderBufferObj::RenderBufferObj() : Handle(), Type(RENDERBUFFER_TYPE_NONE), Info(nullptr) {}


RenderBufferObj::~RenderBufferObj() {}


RenderBufferObj::RenderBufferObj(RenderBufferObj &&Other) { *this = std::move(Other); }


RenderBufferObj& RenderBufferObj::operator= (RenderBufferObj &&Other) {
	_ASSERTE(this != &Other);
	
	if (this != &Other) {
		Handle	= std::move(Other.Handle);
		Type	= Other.Type;
		Info	= Other.Info;

		new (&Other) RenderBufferObj();
	}

	return *this;
}


PostProcessAttachment::PostProcessAttachment() : Texture(nullptr), Type(FRAMEBUFFER_ATTACHMENT_NONE), SubType(FRAMEBUFFER_SUBATTACH_NONE), draw(false) {}


PostProcessAttachment::PostProcessAttachment(const PostProcessAttachment &Other) { *this = Other; }


PostProcessAttachment& PostProcessAttachment::operator= (const PostProcessAttachment &Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		if (Other.Type == FRAMEBUFFER_ATTACHMENT_TEXTURE)
			Texture = Other.Texture;

		if (Other.Type == FRAMEBUFFER_ATTACHMENT_RENDERBUFFER)
			Renderbuffer = Other.Renderbuffer;

		Type	= Other.type;
		SubType = Other.subType;
		Draw	= Other.draw;
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

		type	= Other.type;
		subType = Other.subType;
		draw	= Other.draw;

		new (&Other) PostProcessAttachment();
	}

	return *this;
}


PostProcessAttachment::~PostProcessAttachment() {
	texture = nullptr;
	type	= FRAMEBUFFER_ATTACHMENT_NONE;
	subType = FRAMEBUFFER_SUBATTACH_NONE;
	draw	= false;
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