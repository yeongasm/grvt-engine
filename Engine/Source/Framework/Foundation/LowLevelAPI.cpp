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
			CubemapDataPtr{nullptr}, Mipmap(1), Flip(1), Width(0),
			Height(0), Target(GL_TEXTURE_2D), Type(GL_UNSIGNED_BYTE), Format(0), InternalFormat(0), Parameters() {}


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

				for (size_t i = 0; i < 4; i++)
				{
					BorderColour[i] = Rhs.BorderColour[i];
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

				for (size_t i = 0; i < 4; i++)
				{
					BorderColour[i] = Rhs.BorderColour[i];
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


		TextureAttachment::TextureAttachment() : Handle(nullptr), Type(0) {}


		TextureAttachment::~TextureAttachment()
		{
			Handle		= nullptr;
			Type		= 0;
		}


		TextureAttachment::TextureAttachment(ObjHandle* SrcHandle, uint32 Type, TextureBuildData BuildData) :
			Handle(SrcHandle), Type(Type), BuildData(BuildData) {}


		TextureAttachment::TextureAttachment(const TextureAttachment& Rhs)
		{
			*this = Rhs;
		}


		TextureAttachment& TextureAttachment::operator= (const TextureAttachment& Rhs)
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) {
				BuildData	= Rhs.BuildData;
				Handle		= Rhs.Handle;
				Type		= Rhs.Type;
			}

			return *this;
		}


		TextureAttachment::TextureAttachment(TextureAttachment&& Rhs)
		{
			*this = Gfl::Move(Rhs);
		}


		TextureAttachment& TextureAttachment::operator= (TextureAttachment&& Rhs)
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs) {
				BuildData	= Gfl::Move(Rhs.BuildData);
				Handle		= Gfl::Move(Rhs.Handle);
				Type		= Gfl::Move(Rhs.Type);

				new (&Rhs) TextureAttachment();
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
				Width = Rhs.Width;
				Height = Rhs.Height;
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
				Width = Rhs.Width;
				Height = Rhs.Height;

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
			
			if (Data.Target == GL_TEXTURE_CUBE_MAP)
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
			{
				glGenerateMipmap(Handle.Target);

				float32 AnisotropicF = 0.0f;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &AnisotropicF);
				glTexParameterf(Handle.Target, GL_TEXTURE_MAX_ANISOTROPY, min(4.0f, AnisotropicF));
			}

			for (Gfl::Pair<uint32, uint32>& Param : Data.Parameters)
			{
				glTexParameteri(Handle.Target, Param.Key, Param.Value);
			}
			
			if (Data.BorderColour[0] || Data.BorderColour[1] || Data.BorderColour[2] || Data.BorderColour[3])
			{
				glTexParameterfv(Handle.Target, GL_TEXTURE_BORDER_COLOR, Data.BorderColour);
			}

			GrUnbindTexture(Handle);
		}


		// NOTE(Afiq):
		// This method is flawed! If the shader fails to compile, the results would not be reflected outside of the the low level API.
		// What really needs to be done is a job based system. The result would send a job that updates the state of this shader.
		// An even better approach would be to not allow shader compilation failure to not happen at all.
		bool CompileShader(ObjHandle& Handle, uint32 Type, const char* SourceCode) 
		{
			int32 compiled = 0;

			GrCreateShader(Handle, Type);
			glShaderSource(Handle.Id, 1, &SourceCode, nullptr);
			glCompileShader(Handle.Id);
			glGetShaderiv(Handle.Id, GL_COMPILE_STATUS, &compiled);

			if (!compiled) {
				char Buf[1024];
				glGetShaderInfoLog(Handle.Id, 1024, nullptr, Buf);
				printf(Buf);
				glDeleteShader(Handle.Id);
				return false;
			}

			return true;
		}


		void BuildShaderProgram(ObjHandle& Handle, ShaderBuildData& Data)
		{
			int32 Link = 0;
			size_t Total = 0;
			Gfl::Array<ObjHandle> Handles(Data.BuildInfo.Length());

			GrCreateShaderProgram(Handle);

			for (ShaderInfo& Info : Data.BuildInfo) 
			{
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

				if (!CompileShader(Handles[Total], type, Info.SourceCode))
					_ASSERTE(false); // Failed to compile shader.

				glAttachShader(Handle.Id, Handles[Total].Id);
				Total++;
			}

			// Unlike other graphic objects, shaders are not allowed to be linked twice.
			// Hence there is really no point use the same shader.
			glLinkProgram(Handle.Id);
			glGetProgramiv(Handle.Id, GL_LINK_STATUS, &Link);
			
			if (!Link)
			{
				char Buf[1024];
				glGetProgramInfoLog(Handle.Id, 1024, nullptr, Buf);
				printf(Buf);
				_ASSERTE(false); // Failed to create a shader.
			}

			while (Total > 0) 
			{
				glDeleteShader(Handles[Total - 1].Id);
				Total--;
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
			Gfl::Array<uint32>	DrawBuffers;

			GrBindFramebuffer(Handle);

			for (TextureAttachment& Texture : Data.Attachments)
			{
				if (Texture.Type == GL_COLOR_ATTACHMENT0 + ColourCount)
				{
					DrawBuffers.Push(Texture.Type);
					ColourCount++;
				}

				Texture.BuildData.Width  = Data.Width;
				Texture.BuildData.Height = Data.Height;
				BuildTexture(*Texture.Handle, Texture.BuildData);

				if (Texture.BuildData.Target == GL_TEXTURE_CUBE_MAP)
				{
					glFramebufferTexture(Handle.Target, Texture.Type, Texture.Handle->Id, 0);
				}
				else
				{
					glFramebufferTexture2D(Handle.Target, Texture.Type, Texture.Handle->Target, Texture.Handle->Id, 0);
				}
			}

			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

			if (DrawBuffers.Length())
			{
				glDrawBuffers((int32)DrawBuffers.Length(), DrawBuffers.First());
			}

			if (glCheckFramebufferStatus(Handle.Target) != GL_FRAMEBUFFER_COMPLETE)
			{
				printf("Framebuffer is incomplete because it's size is not a power of 2 most likely ...\n");
				//_ASSERTE(false);	// Building Framebuffer process failed.
			}

			GrUnbindFramebuffer(Handle);
		}

	}

}