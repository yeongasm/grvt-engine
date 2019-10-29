#include "stdafx.h"


namespace BaseAPI {


	VertexAttribPointer::VertexAttribPointer() :
		Index(0), Size(0), Stride(0), PtrOffset(0) {}


	VertexAttribPointer::~VertexAttribPointer() { new (this) VertexAttribPointer(); }


	VertexAttribPointer::VertexAttribPointer(uint32 Index, int32 Size, int32 Stride, size_t Offset) :
		Index(Index), Size(Size), Stride(Stride), PtrOffset(Offset) {}


	VertexAttribPointer::VertexAttribPointer(const VertexAttribPointer& Rhs) { *this = Rhs; }


	VertexAttribPointer& VertexAttribPointer::operator= (const VertexAttribPointer& Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs) {
			Index = Rhs.Index;
			Size = Rhs.Size;
			Stride = Rhs.Stride;
			PtrOffset = Rhs.PtrOffset;
		}

		return *this;
	}


	VertexAttribPointer::VertexAttribPointer(VertexAttribPointer&& Rhs) { *this = Move(Rhs); }


	VertexAttribPointer& VertexAttribPointer::operator= (VertexAttribPointer&& Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs) {
			new (this) VertexAttribPointer();

			Index = Rhs.Index;
			Size = Rhs.Size;
			Stride = Rhs.Stride;
			PtrOffset = Rhs.PtrOffset;

			new (&Rhs) VertexAttribPointer();
		}

		return *this;
	}


	MeshBuildData::MeshBuildData() :
		Data(nullptr), Indices(nullptr), Size(0), Length(0), VertexAttribPointers() {}


	MeshBuildData::~MeshBuildData() { Data = nullptr, Indices = nullptr; Size = 0; Length = 0; VertexAttribPointers.Release(); }


	MeshBuildData::MeshBuildData(const MeshBuildData& Rhs) { *this = Rhs; }


	MeshBuildData::MeshBuildData(MeshBuildData&& Rhs) { *this = Move(Rhs); }


	MeshBuildData& MeshBuildData::operator= (const MeshBuildData& Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs) {
			Data = Rhs.Data;
			Indices = Rhs.Indices;
			Size = Rhs.Size;
			Length = Rhs.Length;
			VertexAttribPointers = Rhs.VertexAttribPointers;
		}

		return *this;
	}


	MeshBuildData& MeshBuildData::operator= (MeshBuildData&& Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs) {
			new (this) MeshBuildData();

			Data = Rhs.Data;
			Indices = Rhs.Indices;
			Size = Rhs.Size;
			Length = Rhs.Length;
			VertexAttribPointers = Rhs.VertexAttribPointers;

			new (&Rhs) MeshBuildData();
		}

		return *this;
	}


	TextureBuildData::TextureBuildData() :
		DataPtr(nullptr), Mipmap(1), Cubemap(0), Flip(1), Width(0),
		Height(0), Target(0), Type(0), Format(0), Parameters() {}


	TextureBuildData::~TextureBuildData() { Parameters.Release(); }


	TextureBuildData::TextureBuildData(const TextureBuildData& Rhs) { *this = Rhs; }


	TextureBuildData& TextureBuildData::operator= (const TextureBuildData& Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs) {
			DataPtr = Rhs.DataPtr;
			Mipmap = Rhs.Mipmap;
			Cubemap = Rhs.Cubemap;
			Flip = Rhs.Flip;
			Width = Rhs.Width;
			Height = Rhs.Height;
			Target = Rhs.Target;
			Type = Rhs.Type;
			Format = Rhs.Format;
			Parameters = Rhs.Parameters;
		}

		return *this;
	}


	TextureBuildData::TextureBuildData(TextureBuildData&& Rhs) { *this = Move(Rhs); }


	TextureBuildData& TextureBuildData::operator= (TextureBuildData&& Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs) {
			new (this) TextureBuildData();

			DataPtr = Rhs.DataPtr;
			Mipmap = Rhs.Mipmap;
			Cubemap = Rhs.Cubemap;
			Flip = Rhs.Flip;
			Width = Rhs.Width;
			Height = Rhs.Height;
			Target = Rhs.Target;
			Type = Rhs.Type;
			Format = Rhs.Format;
			Parameters = Rhs.Parameters;

			new (&Rhs) TextureBuildData();
		}

		return *this;
	}


	ShaderInfo::ShaderInfo() :
		SourceCode(nullptr), Type(GrvtFoundations_ShaderType_None) {}


	ShaderInfo::~ShaderInfo() {}


	ShaderBuildData::ShaderBuildData() :
		BuildInfo() {}


	ShaderBuildData::~ShaderBuildData() {}


	ShaderBuildData::ShaderBuildData(const ShaderBuildData& Rhs) { *this = Rhs; }


	ShaderBuildData& ShaderBuildData::operator= (const ShaderBuildData& Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs)
			BuildInfo = Rhs.BuildInfo;

		return *this;
	}


	ShaderBuildData::ShaderBuildData(ShaderBuildData&& Rhs) { *this = Move(Rhs); }


	ShaderBuildData& ShaderBuildData::operator= (ShaderBuildData&& Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs) {
			BuildInfo = Rhs.BuildInfo;

			new (&Rhs) ShaderBuildData();
		}

		return *this;
	}


	FramebufferAttachment::FramebufferAttachment() : Target(0), Attachment(0), Draw(false) {}


	FramebufferAttachment::~FramebufferAttachment() { HandleId = Target = Attachment = Draw = 0; }


	FramebufferAttachment::FramebufferAttachment(uint32 SrcHandle, uint32 SrcTarget, uint32 SourceAttachment, bool DrawBuffer) :
		HandleId(SrcHandle), Target(SrcTarget), Attachment(SourceAttachment), Draw(DrawBuffer) {}


	FramebufferAttachment::FramebufferAttachment(const FramebufferAttachment& Rhs) { *this = Rhs; }


	FramebufferAttachment& FramebufferAttachment::operator= (const FramebufferAttachment& Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs) {
			HandleId = Rhs.HandleId;
			Target = Rhs.Target;
			Attachment = Rhs.Attachment;
			Draw = Rhs.Draw;
		}

		return *this;
	}


	FramebufferAttachment::FramebufferAttachment(FramebufferAttachment&& Rhs) { *this = Move(Rhs); }


	FramebufferAttachment& FramebufferAttachment::operator= (FramebufferAttachment&& Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs) {
			HandleId = Rhs.HandleId;
			Target = Rhs.Target;
			Attachment = Rhs.Attachment;
			Draw = Rhs.Draw;

			new (&Rhs) FramebufferAttachment();
		}

		return *this;
	}


	FramebufferBuildData::FramebufferBuildData() : Attachment() {}


	FramebufferBuildData::~FramebufferBuildData() { Attachment.Release(); }


	FramebufferBuildData::FramebufferBuildData(const FramebufferBuildData& Rhs) { *this = Rhs; }


	FramebufferBuildData& FramebufferBuildData::operator= (const FramebufferBuildData& Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs)
			Attachment = Rhs.Attachment;

		return *this;
	}


	FramebufferBuildData::FramebufferBuildData(FramebufferBuildData&& Rhs) { *this = std::move(Rhs); }


	FramebufferBuildData& FramebufferBuildData::operator= (FramebufferBuildData&& Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs) {
			Attachment = Rhs.Attachment;

			new (&Rhs) FramebufferBuildData();
		}

		return *this;
	}


	void BuildMesh(ObjHandle& VAO, ObjHandle& VBO, ObjHandle& EBO, MeshBuildData& Data) {
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

		for (VertexAttribPointer& Ptr : Data.VertexAttribPointers) {
			glEnableVertexAttribArray(Ptr.Index);
			glVertexAttribPointer(Ptr.Index, Ptr.Size, GL_FLOAT, GL_FALSE, Ptr.Stride, (GLvoid*)(Ptr.PtrOffset));
		}


		GrUnbindVertexArray();
	}


	void BuildTexture(ObjHandle& Handle, TextureBuildData& Data) {
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

		for (Pair<uint32, uint32>& Param : Data.Parameters)
			glTexParameteri(Handle.Target, Param.Key, Param.Value);

		GrUnbindTexture(Handle);
	}


	bool CompileShader(ObjHandle& Handle, uint32 Type, const char* SourceCode) {
		int32 compiled = 0;

		GrCreateShader(Handle, Type);
		glShaderSource(Handle.Id, 1, &SourceCode, nullptr);
		glCompileShader(Handle.Id);
		glGetShaderiv(Handle.Id, GL_COMPILE_STATUS, &compiled);

		if (!compiled) {
			glDeleteShader(Handle.Id);
			return false;
		}

		return true;
	}


	void BuildShaderProgram(ObjHandle& Handle, ShaderBuildData& Data) {
		int32 link = 0;
		size_t index = 0;
		Array<ObjHandle> handle(Data.BuildInfo.Length());

		GrCreateShaderProgram(Handle);

		for (ShaderInfo& Info : Data.BuildInfo) {
			uint32 type = 0;
			switch (Info.Type) {
			case GrvtFoundations_ShaderType_Vertex:
				type = GL_VERTEX_SHADER;
				break;
			case GrvtFoundations_ShaderType_Fragment:
				type = GL_FRAGMENT_SHADER;
				break;
			case GrvtFoundations_ShaderType_Geometry:
				type = GL_GEOMETRY_SHADER;
				break;
			}

			if (!CompileShader(handle[index], GL_VERTEX_SHADER, Info.SourceCode))
				_ASSERTE(false); // Failed to compile vertex shader.

			glAttachShader(Handle.Id, handle[index].Id);
			index++;
		}

		// Unlike other graphic objects, shaders are not allowed to be linked twice.
		// Hence there is really no point use the same shader.
		glLinkProgram(Handle.Id);
		glGetProgramiv(Handle.Id, GL_LINK_STATUS, &link);

		if (!link)
			_ASSERTE(false); // Failed to create a shader.

		while (index >= 0) {
			glDeleteShader(handle[index].Id);
			index--;
		}
	}


	void GenerateGenericTextureData(TextureBuildData& Data) {
		Data.Target = GL_TEXTURE_2D;
		Data.Type = GL_UNSIGNED_BYTE;
		Data.Format = GL_RGBA;
		Data.Parameters = {{GL_TEXTURE_WRAP_S,		GL_REPEAT},
						   {GL_TEXTURE_WRAP_T,		GL_REPEAT},
						   {GL_TEXTURE_MIN_FILTER,	GL_LINEAR_MIPMAP_LINEAR},
						   {GL_TEXTURE_MAG_FILTER,	GL_LINEAR}};
	}


	void BuildFramebuffer(ObjHandle& Handle, FramebufferBuildData& Data) {
		if (!Handle.Id)
			GrCreateFramebuffer(Handle);

		bool			hasImage = false;
		Array<uint32>	DrawBuffers;

		GrBindFramebuffer(Handle);

		for (FramebufferAttachment& Attachment : Data.Attachment) {
			if (Attachment.Target != GL_RENDERBUFFER) {
				hasImage = true;
				glFramebufferTexture2D(Handle.Target, Attachment.Attachment, Attachment.Target, Attachment.HandleId, 0);

				if (Attachment.Draw)
					DrawBuffers.Push(Attachment.Attachment);

				continue;
			}

			glFramebufferRenderbuffer(Handle.Target, Attachment.Attachment, Attachment.Target, Attachment.HandleId);
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

}