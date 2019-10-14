#include "stdafx.h"


VertexAttribPointer::VertexAttribPointer() : Index(0), Size(0), Stride(0), PtrOffset(0) {}


VertexAttribPointer::VertexAttribPointer(uint32 Index, int32 Size, int32 Stride, size_t Offset) :
	Index(Index), Size(Size), Stride(Stride), PtrOffset(Offset) {}


VertexAttribPointer::VertexAttribPointer(const VertexAttribPointer &Rhs) { *this = Rhs; }


VertexAttribPointer& VertexAttribPointer::operator= (const VertexAttribPointer &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Index		= Rhs.Index;
		Size		= Rhs.Size;
		Stride		= Rhs.Stride;
		PtrOffset	= Rhs.PtrOffset;
	}

	return *this;
}


VertexAttribPointer::VertexAttribPointer(VertexAttribPointer &&Rhs) { *this = std::move(Rhs); }


VertexAttribPointer& VertexAttribPointer::operator= (VertexAttribPointer &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		new (this) VertexAttribPointer();

		Index		= Rhs.Index;
		Size		= Rhs.Size;
		Stride		= Rhs.Stride;
		PtrOffset	= Rhs.PtrOffset;

		new (&Rhs) VertexAttribPointer();
	}

	return *this;
}


VertexAttribPointer::~VertexAttribPointer() { new (this) VertexAttribPointer(); }


MeshBuildData::MeshBuildData() : Data(nullptr), Indices(nullptr), Size(0), Length(0), VertexAttribPointers() {}


MeshBuildData::MeshBuildData(const MeshBuildData &Rhs) { *this = Rhs; }


MeshBuildData::MeshBuildData(MeshBuildData &&Rhs) { *this = std::move(Rhs); }


MeshBuildData& MeshBuildData::operator= (const MeshBuildData &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Data	= Rhs.Data;
		Indices = Rhs.Indices;
		Size	= Rhs.Size;
		Length	= Rhs.Length;
		VertexAttribPointers = Rhs.VertexAttribPointers;
	}

	return *this;
}


MeshBuildData& MeshBuildData::operator= (MeshBuildData &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		new (this) MeshBuildData();

		Data	= Rhs.Data;
		Indices = Rhs.Indices;
		Size	= Rhs.Size;
		Length	= Rhs.Length;
		VertexAttribPointers = Rhs.VertexAttribPointers;

		new (&Rhs) MeshBuildData();
	}

	return *this;
}


MeshBuildData::~MeshBuildData() { Data = nullptr, Indices = nullptr; Size = 0; Length = 0; VertexAttribPointers.Release(); }


TextureBuildData::TextureBuildData() : DataPtr(nullptr), Mipmap(1), Cubemap(0), Flip(1), 
	Width(0), Height(0), Target(0), Type(0), Format(0), Parameters() {}


TextureBuildData::TextureBuildData(const TextureBuildData &Rhs) { *this = Rhs; }


TextureBuildData::TextureBuildData(TextureBuildData &&Rhs) { *this = std::move(Rhs); }


TextureBuildData& TextureBuildData::operator= (const TextureBuildData &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		DataPtr = Rhs.DataPtr;
		Mipmap	= Rhs.Mipmap;
		Cubemap = Rhs.Cubemap;
		Flip	= Rhs.Flip;
		Width	= Rhs.Width;
		Height	= Rhs.Height;
		Target	= Rhs.Target;
		Type	= Rhs.Type;
		Format	= Rhs.Format;
		Parameters = Rhs.Parameters;
	}

	return *this;
}


TextureBuildData& TextureBuildData::operator= (TextureBuildData &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		new (this) TextureBuildData();

		DataPtr	= Rhs.DataPtr;
		Mipmap	= Rhs.Mipmap;
		Cubemap = Rhs.Cubemap;
		Flip	= Rhs.Flip;
		Width	= Rhs.Width;
		Height	= Rhs.Height;
		Target	= Rhs.Target;
		Type	= Rhs.Type;
		Format	= Rhs.Format;
		Parameters = Rhs.Parameters;

		new (&Rhs) TextureBuildData();
	}

	return *this;
}


TextureBuildData::~TextureBuildData() { Parameters.Release(); }


RenderbufferBuildData::RenderbufferBuildData() : Width(0), Height(0), InternalFormat() {}


RenderbufferBuildData::RenderbufferBuildData(const RenderbufferBuildData &Rhs) { *this = Rhs; }


RenderbufferBuildData& RenderbufferBuildData::operator= (const RenderbufferBuildData &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Width			= Rhs.Width;
		Height			= Rhs.Height;
		InternalFormat	= Rhs.InternalFormat;
	}

	return *this;
}


RenderbufferBuildData::RenderbufferBuildData(RenderbufferBuildData &&Rhs) { *this = std::move(Rhs); }


RenderbufferBuildData& RenderbufferBuildData::operator= (RenderbufferBuildData &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Width			= Rhs.Width;
		Height			= Rhs.Height;
		InternalFormat	= Rhs.InternalFormat;

		new (&Rhs) RenderbufferBuildData();
	}

	return *this;
}


RenderbufferBuildData::~RenderbufferBuildData() {
	Width = Height = InternalFormat = 0;
}


FramebufferAttachment::FramebufferAttachment() : Handle(nullptr), Attachment(0), Draw(false) {}


FramebufferAttachment::FramebufferAttachment(ObjHandle *SrcHandle, uint32 SourceAttachment, bool DrawBuffer) :
	Handle(SrcHandle), Attachment(SourceAttachment), Draw(DrawBuffer) {}


FramebufferAttachment::FramebufferAttachment(const FramebufferAttachment &Rhs) { *this = Rhs; }


FramebufferAttachment& FramebufferAttachment::operator= (const FramebufferAttachment &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Handle		= Rhs.Handle;
		Attachment	= Rhs.Attachment;
		Draw		= Rhs.Draw;
	}

	return *this;
}


FramebufferAttachment::FramebufferAttachment(FramebufferAttachment &&Rhs) { *this = std::move(Rhs); }


FramebufferAttachment& FramebufferAttachment::operator= (FramebufferAttachment &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Handle		= Rhs.Handle;
		Attachment	= Rhs.Attachment;
		Draw		= Rhs.Draw;

		new (&Rhs) FramebufferAttachment();
	}

	return *this;
}


FramebufferAttachment::~FramebufferAttachment() {
	Handle		= nullptr;
	Attachment	= 0;
	Draw		= false;
}


FramebufferBuildData::FramebufferBuildData() : Attachment() {}


FramebufferBuildData::FramebufferBuildData(const FramebufferBuildData &Rhs) { *this = Rhs; }


FramebufferBuildData& FramebufferBuildData::operator= (const FramebufferBuildData &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs)
		Attachment	= Rhs.Attachment;

	return *this;
}


FramebufferBuildData::FramebufferBuildData(FramebufferBuildData &&Rhs) { *this = std::move(Rhs); }


FramebufferBuildData& FramebufferBuildData::operator= (FramebufferBuildData &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Attachment	= Rhs.Attachment;

		new (&Rhs) FramebufferBuildData();
	}

	return *this;
}


FramebufferBuildData::~FramebufferBuildData() { Attachment.Release(); }


void BaseAPI::BuildMesh(ObjHandle &VAO, ObjHandle &VBO, ObjHandle &EBO, MeshBuildData &Data) {
	if (!VAO.Id) {
		GrCreateVertexArray(VAO);
		GrCreateBufferObject(VBO, GL_ARRAY_BUFFER);

		if (Data.Length)
			GrCreateBufferObject(EBO, GL_ELEMENT_ARRAY_BUFFER);

	}

	GrBindVertexArray(VAO);
	GrBindBufferObject(VBO);

	glBufferData(VBO.Target, Data.Size * sizeof(float), Data.Data, GL_STATIC_DRAW);

	if (Data.Length) {
		GrBindBufferObject(EBO);
		glBufferData(EBO.Target, Data.Length * sizeof(uint32), Data.Indices, GL_STATIC_DRAW);
	}

	for (VertexAttribPointer &Ptr : Data.VertexAttribPointers) {
		glEnableVertexAttribArray(Ptr.Index);
		glVertexAttribPointer(Ptr.Index, Ptr.Size, GL_FLOAT, GL_FALSE, Ptr.Stride, (GLvoid*)(Ptr.PtrOffset));
	}


	GrUnbindVertexArray();
}


void BaseAPI::BuildTexture(ObjHandle &Handle, TextureBuildData &Data) {
	if (!Handle.Id)
		GrCreateTexture(Handle, Data.Target);

	GrBindTexture(Handle);

	switch (Data.Type) {
	case GL_UNSIGNED_BYTE:
		glTexImage2D(Handle.Target, 0, Data.Format, Data.Width, Data.Height, 0, Data.Format, Data.Type, (uint8*)Data.DataPtr);
		break;
	case GL_UNSIGNED_SHORT:
		glTexImage2D(Handle.Target, 0, Data.Format, Data.Width, Data.Height, 0, Data.Format, Data.Type, (uint16*)Data.DataPtr);
		break;
	case GL_UNSIGNED_INT:
		glTexImage2D(Handle.Target, 0, Data.Format, Data.Width, Data.Height, 0, Data.Format, Data.Type, (uint32*)Data.DataPtr);
		break;
	default:
		break;
	}
	
	if (Data.Mipmap)
		glGenerateMipmap(Handle.Target);

	for (Pair<uint32, uint32> &Param : Data.Parameters)
		glTexParameteri(Handle.Target, Param.Key, Param.Value);

	GrUnbindTexture(Handle);
}


void BaseAPI::GenerateGenericTextureData(TextureBuildData &Data) {
	Data.Target = GL_TEXTURE_2D;
	Data.Type	= GL_UNSIGNED_BYTE;
	Data.Format = GL_RGBA;
	Data.Parameters = {{GL_TEXTURE_WRAP_S,		GL_REPEAT},
					   {GL_TEXTURE_WRAP_T,		GL_REPEAT},
					   {GL_TEXTURE_MIN_FILTER,	GL_LINEAR_MIPMAP_LINEAR},
					   {GL_TEXTURE_MAG_FILTER,	GL_LINEAR}};
}


void BaseAPI::BuildFramebuffer(ObjHandle &Handle, FramebufferBuildData &Data) {
	if (!Handle.Id)
		GrCreateFramebuffer(Handle);

	bool			hasImage = false;
	Array<uint32>	DrawBuffers;

	GrBindFramebuffer(Handle);

	for (FramebufferAttachment &Attachment : Data.Attachment) {
		if (Attachment.Handle->Target != GL_RENDERBUFFER) {
			hasImage = true;
			glFramebufferTexture2D(Handle.Target, Attachment.Attachment, Attachment.Handle->Target, Attachment.Handle->Id, 0);

			if (Attachment.Draw)
				DrawBuffers.Push(Attachment.Attachment);

			continue;
		}

		glFramebufferRenderbuffer(Handle.Target, Attachment.Attachment, Attachment.Handle->Target, Attachment.Handle->Id);
	}

	if (!hasImage) {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	if (DrawBuffers.Length())
		glDrawBuffers((GLsizei)DrawBuffers.Length(), DrawBuffers.First());

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		_ASSERTE(false);	// Building Framebuffer process failed.

	GrUnbindFramebuffer(Handle);
}


void BaseAPI::BuildRenderbuffer(ObjHandle &Handle, RenderbufferBuildData &Data) {
	if (!Handle.Id)
		GrCreateRenderbuffer(Handle);

	GrBindRenderbuffer(Handle);
	glRenderbufferStorage(Handle.Target, Data.InternalFormat, Data.Width, Data.Height);
	GrUnbindRenderbuffer(Handle);
}