#include "GrvtPch.h"
#include "Framework/Foundation/LowLevelAPI.h"


namespace Grvt
{

	namespace BaseAPI
	{


		VertexAttribPointer::VertexAttribPointer() :
			Index(0), Size(0), Stride(0), PtrOffset(0) {}


		VertexAttribPointer::~VertexAttribPointer() 
		{
			new (this) VertexAttribPointer();
		}


		VertexAttribPointer::VertexAttribPointer(uint32 Index, int32 Size, int32 Stride, size_t Offset) :
			Index(Index), Size(Size), Stride(Stride), PtrOffset(Offset) {}


		VertexAttribPointer::VertexAttribPointer(const VertexAttribPointer& Rhs)
		{
			*this = Rhs;
		}


		VertexAttribPointer& VertexAttribPointer::operator= (const VertexAttribPointer& Rhs) 
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) 
			{
				Index = Rhs.Index;
				Size = Rhs.Size;
				Stride = Rhs.Stride;
				PtrOffset = Rhs.PtrOffset;
			}

			return *this;
		}


		VertexAttribPointer::VertexAttribPointer(VertexAttribPointer&& Rhs)
		{
			*this = Gfl::Move(Rhs);
		}


		VertexAttribPointer& VertexAttribPointer::operator= (VertexAttribPointer&& Rhs) 
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) 
			{
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


		MeshBuildData::~MeshBuildData() 
		{
			Data = nullptr, Indices = nullptr; Size = 0; Length = 0; VertexAttribPointers.Release();
		}


		MeshBuildData::MeshBuildData(const MeshBuildData& Rhs)
		{
			*this = Rhs;
		}


		MeshBuildData::MeshBuildData(MeshBuildData&& Rhs)
		{
			*this = Gfl::Move(Rhs);
		}


		MeshBuildData& MeshBuildData::operator= (const MeshBuildData& Rhs) 
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) 
			{
				Data = Rhs.Data;
				Indices = Rhs.Indices;
				Size = Rhs.Size;
				Length = Rhs.Length;
				VertexAttribPointers = Rhs.VertexAttribPointers;
			}

			return *this;
		}


		MeshBuildData& MeshBuildData::operator= (MeshBuildData&& Rhs) 
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) 
			{
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
			DataPtr(nullptr), CubemapDataPtr{nullptr}, Cubemap(0), Mipmap(1), Flip(1), Width(0), 
			Height(0), Target(0), Type(0), Format(0), InternalFormat(0), Parameters() {}


		TextureBuildData::~TextureBuildData() 
		{
			Parameters.Release();
		}


		TextureBuildData::TextureBuildData(const TextureBuildData& Rhs) 
		{
			*this = Rhs;
		}


		TextureBuildData& TextureBuildData::operator= (const TextureBuildData& Rhs) 
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) 
			{
				DataPtr = Rhs.DataPtr;
				Mipmap = Rhs.Mipmap;
				Flip = Rhs.Flip;
				Width = Rhs.Width;
				Height = Rhs.Height;
				Target = Rhs.Target;
				Type = Rhs.Type;
				Format = Rhs.Format;
				InternalFormat = Rhs.InternalFormat;
				Parameters = Rhs.Parameters;

				if (Rhs.CubemapDataPtr)
				{
					for (size_t i = 0; i < 6; i++)
					{
						CubemapDataPtr[i] = Rhs.CubemapDataPtr[i];
					}
				}
			}

			return *this;
		}


		TextureBuildData::TextureBuildData(TextureBuildData&& Rhs)
		{
			*this = Gfl::Move(Rhs);
		}


		TextureBuildData& TextureBuildData::operator= (TextureBuildData&& Rhs) 
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) 
			{
				new (this) TextureBuildData();

				DataPtr = Rhs.DataPtr;
				Mipmap = Rhs.Mipmap;
				Flip = Rhs.Flip;
				Width = Rhs.Width;
				Height = Rhs.Height;
				Target = Rhs.Target;
				Type = Rhs.Type;
				Format = Rhs.Format;
				InternalFormat = Rhs.InternalFormat;
				Parameters = Rhs.Parameters;

				if (Rhs.CubemapDataPtr)
				{
					for (size_t i = 0; i < 6; i++)
					{
						CubemapDataPtr[i] = Rhs.CubemapDataPtr[i];
					}
				}

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


		ShaderBuildData::ShaderBuildData(const ShaderBuildData& Rhs)
		{
			*this = Rhs;
		}


		ShaderBuildData& ShaderBuildData::operator= (const ShaderBuildData& Rhs) 
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs)
				BuildInfo = Rhs.BuildInfo;

			return *this;
		}


		ShaderBuildData::ShaderBuildData(ShaderBuildData&& Rhs)
		{
			*this = Gfl::Move(Rhs);
		}


		ShaderBuildData& ShaderBuildData::operator= (ShaderBuildData&& Rhs) 
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) 
			{
				BuildInfo = Rhs.BuildInfo;

				new (&Rhs) ShaderBuildData();
			}

			return *this;
		}


		FramebufferAttachment::FramebufferAttachment() : Handle(), Type(0), Component(0) {}


		FramebufferAttachment::~FramebufferAttachment() 
		{
			Type = Component = 0;
		}


		FramebufferAttachment::FramebufferAttachment(ObjHandle* SrcHandle, uint32 SourceAttachment, uint32 Component) :
			Handle(SrcHandle), Type(SourceAttachment), Component(Component) {}


		FramebufferAttachment::FramebufferAttachment(const FramebufferAttachment& Rhs) 
		{
			*this = Rhs;
		}


		FramebufferAttachment& FramebufferAttachment::operator= (const FramebufferAttachment& Rhs) 
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) {
				Handle = Rhs.Handle;
				Type = Rhs.Type;
				Component = Rhs.Component;
			}

			return *this;
		}


		FramebufferAttachment::FramebufferAttachment(FramebufferAttachment&& Rhs)
		{
			*this = Gfl::Move(Rhs);
		}


		FramebufferAttachment& FramebufferAttachment::operator= (FramebufferAttachment&& Rhs) 
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) {
				Handle = Rhs.Handle;
				Type = Rhs.Type;
				Component = Rhs.Component;

				new (&Rhs) FramebufferAttachment();
			}

			return *this;
		}


		FramebufferBuildData::FramebufferBuildData() :
			Attachments(), Width(0), Height(0) {}


		FramebufferBuildData::~FramebufferBuildData()
		{
			Attachments.Release();
			Width = Height = 0;
		}


		FramebufferBuildData::FramebufferBuildData(const FramebufferBuildData& Rhs) 
		{
			*this = Rhs;
		}


		FramebufferBuildData& FramebufferBuildData::operator= (const FramebufferBuildData& Rhs) 
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs)
			{
				Attachments = Rhs.Attachments;
			}

			return *this;
		}


		FramebufferBuildData::FramebufferBuildData(FramebufferBuildData&& Rhs) 
		{
			*this = Gfl::Move(Rhs);
		}


		FramebufferBuildData& FramebufferBuildData::operator= (FramebufferBuildData&& Rhs) 
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs)
			{
				Attachments = Rhs.Attachments;

				new (&Rhs) FramebufferBuildData();
			}

			return *this;
		}


		void BuildMesh(ObjHandle& VAO, ObjHandle& VBO, ObjHandle& EBO, MeshBuildData& Data) 
		{
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


		void BuildTexture(ObjHandle& Handle, TextureBuildData& Data) 
		{
			if (!Handle.Id)
				GrCreateTexture(Handle, Data.Target);

			GrBindTexture(Handle);

			if (Data.Cubemap)
			{
				for (uint32 i = 0; i < 6; i++)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Data.InternalFormat, Data.Width, Data.Height, 0, Data.Format, Data.Type, (uint8*)Data.CubemapDataPtr[i]);
				}
			}
			else
			{
				glTexImage2D(Handle.Target, 0, Data.InternalFormat, Data.Width, Data.Height, 0, Data.Format, Data.Type, (uint8*)Data.DataPtr);
			}

			if (Data.Mipmap)
				glGenerateMipmap(Handle.Target);

			for (Gfl::Pair<uint32, uint32>& Param : Data.Parameters)
				glTexParameteri(Handle.Target, Param.Key, Param.Value);

			GrUnbindTexture(Handle);
		}


		bool CompileShader(ObjHandle& Handle, uint32 Type, const char* SourceCode) 
		{
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


		void BuildShaderProgram(ObjHandle& Handle, ShaderBuildData& Data)
		{
			int32 link = 0;
			size_t index = 0;
			Gfl::Array<ObjHandle> handle(Data.BuildInfo.Length());

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


		void GenerateGenericTextureData(TextureBuildData& Data) 
		{
			Data.Target = GL_TEXTURE_2D;
			Data.Type = GL_UNSIGNED_BYTE;
			Data.InternalFormat = GL_RGBA;
			Data.Format = GL_RGBA;
			Data.Parameters = {{GL_TEXTURE_WRAP_S,		GL_REPEAT},
							   {GL_TEXTURE_WRAP_T,		GL_REPEAT},
							   {GL_TEXTURE_MIN_FILTER,	GL_LINEAR_MIPMAP_LINEAR},
							   {GL_TEXTURE_MAG_FILTER,	GL_LINEAR}};
		}


		void GenerateGenericCubemapData(TextureBuildData& Data)
		{
			Data.Target = GL_TEXTURE_CUBE_MAP;
			Data.Type = GL_UNSIGNED_BYTE;
			Data.InternalFormat = GL_RGB;
			Data.Format = GL_RGB;
			Data.Parameters =  {{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
								{GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
								{GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE},
								{GL_TEXTURE_MAG_FILTER, GL_LINEAR},
								{GL_TEXTURE_MIN_FILTER, GL_LINEAR}};
		}


		void BuildFramebuffer(ObjHandle& Handle, FramebufferBuildData& Data)
		{
			if (!Handle.Id)
				GrCreateFramebuffer(Handle);

			uint32				ColourCount = 0;
			bool				hasImage = false;
			Gfl::Array<uint32>	drawBuffers;

			GrBindFramebuffer(Handle);

			TextureBuildData buildData;
			buildData.DataPtr = 0;
			buildData.Mipmap = false;
			buildData.Width = Data.Width;
			buildData.Height = Data.Height;

			for (FramebufferAttachment& Attachment : Data.Attachments)
			{
				if (Attachment.Component == GrvtFramebuffer_AttachComponent_Cubemap)
				{
					buildData.Cubemap = true;
					buildData.Target = GL_TEXTURE_CUBE_MAP;
					buildData.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_NEAREST});
					buildData.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_NEAREST});
					buildData.Parameters.Push({GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE});
					buildData.Parameters.Push({GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE});
					buildData.Parameters.Push({GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE});
				}
				else
				{
					buildData.Target = GL_TEXTURE_2D;
					buildData.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_LINEAR});
					buildData.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_LINEAR});
					buildData.Parameters.Push({GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE});
					buildData.Parameters.Push({GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE});
				}

				if (Attachment.Type == GL_DEPTH_ATTACHMENT)
				{
					buildData.Type = GL_FLOAT;
					buildData.InternalFormat = GL_DEPTH_COMPONENT;
					buildData.Format = GL_DEPTH_COMPONENT;
				}

				if (Attachment.Type == GL_DEPTH_STENCIL_ATTACHMENT)
				{
					buildData.Type = GL_UNSIGNED_INT_24_8;
					buildData.InternalFormat = GL_DEPTH_STENCIL;
					buildData.Format = GL_DEPTH_STENCIL;
				}

				if (Attachment.Type == GL_COLOR_ATTACHMENT0 + ColourCount++)
				{
					hasImage = true;
					buildData.Type = GL_UNSIGNED_BYTE;
					buildData.InternalFormat = GL_RGBA;
					buildData.Format = GL_RGBA;
					drawBuffers.Insert(Attachment.Type);
				}

				BuildTexture(*Attachment.Handle, buildData);
				glFramebufferTexture2D(Handle.Target, Attachment.Type, Attachment.Handle->Target, Attachment.Handle->Id, 0);
				GrBindTexture(Handle);
				buildData.Parameters.Empty();
			}

			if (!hasImage) {
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
			}

			if (drawBuffers.Length())
				glDrawBuffers((GLsizei)drawBuffers.Length(), drawBuffers.First());

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				_ASSERTE(false);	// Building Framebuffer process failed.

			GrUnbindFramebuffer(Handle);
		}

	}

}