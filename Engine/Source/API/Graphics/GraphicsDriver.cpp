#include "GrvtPch.h"
#include "API/Graphics/GraphicsDriver.h"


namespace Grvt
{

	VertexAttr::VertexAttr() :
		Name(), Type(Gfx_Type_None), Location(-1), Size(0) {}


	VertexAttr::~VertexAttr() {}


	namespace Driver
	{
		void BufferBuildData::AddData(uint32 Target, size_t Size, void* SrcData)
		{
			this->Target = Target;
			this->Size	 = Size;

			DataPtr = SrcData;
		}

		
		void BufferBuildData::RemoveData()
		{
			Target	= 0;
			Size	= 0;

			DataPtr = nullptr;
		}


		void BufferBuildData::PushSubData(size_t Size, uint32 Offset, void* SubData)
		{
			BufferSubData& Sub = this->SubData.Insert(BufferSubData());
			
			Sub.Size	= Size;
			Sub.Offset	= Offset;
			Sub.DataPtr = SubData;
		}


		void BufferBuildData::PopSubData()
		{
			SubData.Pop();
		}


		void MeshBuildData::PushAttribute(uint32 Index, int32 Size, int32 Stride, size_t Offset, uint32 Divisor)
		{
			MeshVertexAttribPointer& Attribute = Attributes.Insert(MeshVertexAttribPointer());
			
			Attribute.Index		= Index;
			Attribute.Size		= Size;
			Attribute.Stride	= Stride;
			Attribute.Offset	= Offset;
			Attribute.Divisor	= Divisor;
		}


		void MeshBuildData::PopAttribute()
		{
			Attributes.Pop();
		}


		void MeshBuildData::PushBuffer(GfxHandle* Handle, const BufferBuildData& Data)
		{
			BufferDataPair& Pair = Buffers.Insert(BufferDataPair());
			
			Pair.Key	= Handle;
			Pair.Value	= Data;
		}

		
		void MeshBuildData::PopBuffer()
		{
			Buffers.Pop();
		}


		void TextureBuildData::PushTextureParameter(uint32 Parameter, uint32 Value)
		{
			Parameters.Push({Parameter, Value});
		}

		void TextureBuildData::PopTextureParameter()
		{
			Parameters.Pop();
		}

		FrameTexAttachment::FrameTexAttachment(GfxHandle* AttachmentHandle, uint32 AttachmentType, TextureBuildData Data) :
			Handle(AttachmentHandle), Type(AttachmentType), BuildData(Data) {}
	}

	void GraphicsDriver::CreateVertexArray(GfxHandle& Handle)
	{
		glGenVertexArrays(1, &Handle.Id);
	}

	void GraphicsDriver::BindVertexArray(GfxHandle& Handle)
	{
		glBindVertexArray(Handle.Id);
	}

	void GraphicsDriver::UnbindVertexArray(GfxHandle& Handle)
	{
		glBindVertexArray(0);
	}

	void GraphicsDriver::DeleteVertexArray(GfxHandle& Handle)
	{
		glDeleteVertexArrays(1, &Handle.Id);
	}

	void GraphicsDriver::CreateBuffer(GfxHandle& Handle, uint32 Target)
	{
		Handle.Target = Target;
		glGenBuffers(1, &Handle.Id);
	}

	void GraphicsDriver::BindBuffer(GfxHandle& Handle)
	{
		glBindBuffer(Handle.Target, Handle.Id);
	}

	void GraphicsDriver::UnbindBuffer(GfxHandle& Handle)
	{
		glBindBuffer(Handle.Target, 0);
	}

	void GraphicsDriver::DeleteBuffer(GfxHandle& Handle)
	{
		glDeleteBuffers(1, &Handle.Id);
	}

	void GraphicsDriver::CreateShader(GfxHandle& Handle, uint32 Type)
	{
		Handle.Target = Type;
		Handle.Id = glCreateShader(Handle.Target);
	}

	void GraphicsDriver::DeleteShader(GfxHandle& Handle)
	{
		glDeleteShader(Handle.Id);
	}

	bool GraphicsDriver::SetUniformBool(uint32 Location, bool Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniform1i(Location, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformInt(uint32 Location, int32 Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniform1i(Location, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformFloat(uint32 Location, float32 Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniform1f(Location, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformDouble(uint32 Location, float64 Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniform1d(Location, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformVec2F(uint32 Location, float32* Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniform2fv(Location, 1, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformVec2D(uint32 Location, float64* Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniform2dv(Location, 1, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformVec3F(uint32 Location, float32* Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniform3fv(Location, 1, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformVec3D(uint32 Location, float64* Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniform3dv(Location, 1, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformVec4F(uint32 Location, float32* Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniform4fv(Location, 1, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformVec4D(uint32 Location, float64* Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniform3dv(Location, 1, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformMat2F(uint32 Location, float32* Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniformMatrix2fv(Location, 1, GL_FALSE, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformMat2D(uint32 Location, float64* Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniformMatrix2dv(Location, 1, GL_FALSE, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformMat3F(uint32 Location, float32* Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniformMatrix3fv(Location, 1, GL_FALSE, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformMat3D(uint32 Location, float64* Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniformMatrix3dv(Location, 1, GL_FALSE, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformMat4F(uint32 Location, float32* Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniformMatrix4fv(Location, 1, GL_FALSE, Value);

		return true;
	}

	bool GraphicsDriver::SetUniformMat4D(uint32 Location, float64* Value)
	{
		if (Location == -1)
		{
			return false;
		}

		glUniformMatrix4dv(Location, 1, GL_FALSE, Value);

		return true;
	}

	void GraphicsDriver::CreateProgram(GfxHandle& Handle)
	{
		Handle.Id = glCreateProgram();
	}

	void GraphicsDriver::BindProgram(GfxHandle& Handle)
	{
		glUseProgram(Handle.Id);
	}

	void GraphicsDriver::UnbindProgram(GfxHandle& Handle)
	{
		glUseProgram(0);
	}

	void GraphicsDriver::DeleteProgram(GfxHandle& Handle)
	{
		glDeleteProgram(Handle.Id);
	}

	void GraphicsDriver::CreateTexture(GfxHandle& Handle, uint32 Target)
	{
		Handle.Target = Target;
		glGenTextures(1, &Handle.Id);
	}

	void GraphicsDriver::BindTexture(GfxHandle& Handle)
	{
		glBindTexture(Handle.Target, Handle.Id);
	}

	void GraphicsDriver::UnbindTexture(GfxHandle& Handle)
	{
		glBindTexture(Handle.Target, 0);
	}

	void GraphicsDriver::DeleteTexture(GfxHandle& Handle)
	{
		glDeleteTextures(1, &Handle.Id);
	}

	void GraphicsDriver::ActiveTexture(uint32 Unit)
	{
		glActiveTexture(GL_TEXTURE0 + Unit);
	}

	void GraphicsDriver::CreateFramebuffer(GfxHandle& Handle)
	{
		Handle.Target = GL_FRAMEBUFFER;
		glGenFramebuffers(1, &Handle.Id);
	}

	void GraphicsDriver::BindFramebuffer(GfxHandle& Handle)
	{
		glBindFramebuffer(Handle.Target, Handle.Id);
	}

	void GraphicsDriver::UnbindFramebuffer(GfxHandle& Handle)
	{
		glBindFramebuffer(Handle.Target, 0);
	}

	void GraphicsDriver::DeleteFramebuffer(GfxHandle& Handle)
	{
		glDeleteFramebuffers(1, &Handle.Id);
	}

	void GraphicsDriver::BuildMesh(GfxHandle& Handle, const Driver::MeshBuildData& Data)
	{
		if (!Handle.Id)
		{
			CreateVertexArray(Handle);
		}

		BindVertexArray(Handle);

		for (Driver::BufferDataPair& Buffer : Data.Buffers)
		{
			if (!Buffer.Key->Id)
			{
				CreateBuffer(*Buffer.Key, Buffer.Value.Target);
			}

			BindBuffer(*Buffer.Key);
			BuildBuffer(*Buffer.Key, Buffer.Value);

			// Because BuildBuffer() unbinds the buffer automatically, we'll have to bind it back.
			BindBuffer(*Buffer.Key);
		}

		for (Driver::MeshVertexAttribPointer& Attrib : Data.Attributes)
		{
			glEnableVertexAttribArray(Attrib.Index);
			glVertexAttribPointer(Attrib.Index, Attrib.Size, GL_FLOAT, GL_FALSE, Attrib.Stride, (GLvoid*)Attrib.Offset);
		}

		UnbindVertexArray(Handle);
	}

	void GraphicsDriver::BuildTexture(GfxHandle& Handle, const Driver::TextureBuildData& Data)
	{
		if (!Handle.Id)
		{
			CreateTexture(Handle, Data.Target);
		}

		BindTexture(Handle);

		if (Data.Target == GL_TEXTURE_CUBE_MAP)
		{
			for (uint32 i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Data.InternalFormat, Data.Width, Data.Height, 0, Data.Format, Data.Type, (uint8*)Data.CubeMapDataPtr[i]);
			}
		}
		else
		{
			glTexImage2D(Handle.Target, 0, Data.InternalFormat, Data.Width, Data.Height, 0, Data.Format, Data.Type, (uint8*)Data.DataPtr);
		}

		if (Data.Mipmap)
		{
			glGenerateMipmap(Handle.Target);
		}

		if (Data.FilterLevel)
		{
			float32 Filter = Data.FilterLevel;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &Filter);
			glTexParameterf(Handle.Target, GL_TEXTURE_MAX_ANISOTROPY, min(4.0f, Filter));
		}

		for (Driver::TextureParameter& Param : Data.Parameters)
		{
			glTexParameteri(Handle.Target, Param.Key, Param.Value);
		}

		if (Data.BorderColour[0] || Data.BorderColour[1] || Data.BorderColour[2] || Data.BorderColour[3])
		{
			glTexParameterfv(Handle.Target, GL_TEXTURE_BORDER_COLOR, Data.BorderColour);
		}

		UnbindTexture(Handle);
	}

	bool GraphicsDriver::CompileShader(GfxHandle& Handle, uint32 Type, const char* SrcCode)
	{
		int32 Compiled = 0;

		CreateShader(Handle, Type);
		glShaderSource(Handle.Id, 1, &SrcCode, nullptr);
		glCompileShader(Handle.Id);
		glGetShaderiv(Handle.Id, GL_COMPILE_STATUS, &Compiled);

		if (!Compiled) {
			char Buf[1024];
			glGetShaderInfoLog(Handle.Id, 1024, nullptr, Buf);
			printf(Buf);
			glDeleteShader(Handle.Id);

			return false;
		}

		return true;
	}

	void GraphicsDriver::BuildShaderProgram(GfxHandle& Handle, const Driver::ShaderBuildData& Data)
	{
		int32	Link  = 0;
		size_t	Total = 0;

		GfxHandle TempHandles[3];

		if (!Handle.Id)
		{
			CreateProgram(Handle);
		}

		if (Data.VertexShader.Length())
		{
			if (CompileShader(TempHandles[0], GL_VERTEX_SHADER, Data.VertexShader.C_Str()))
			{
				_ASSERTE(false); // Failed to compile shader.
			}

			glAttachShader(Handle.Id, TempHandles[Total++].Id);
		}

		if (Data.GeometryShader.Length())
		{
			if (CompileShader(TempHandles[1], GL_GEOMETRY_SHADER, Data.GeometryShader.C_Str()))
			{
				_ASSERTE(false); // Failed to compile shader.
			}

			glAttachShader(Handle.Id, TempHandles[Total++].Id);
		}

		if (Data.FragmentShader.Length())
		{
			if (CompileShader(TempHandles[2], GL_FRAGMENT_SHADER, Data.FragmentShader.C_Str()))
			{
				_ASSERTE(false); // Failed to compile shader.
			}

			glAttachShader(Handle.Id, TempHandles[Total++].Id);
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
			if (!TempHandles[Total - 1].Id)
			{
				continue;
			}

			glDeleteShader(TempHandles[Total - 1].Id);
			Total--;
		}
	}

	void GraphicsDriver::BuildFramebuffer(GfxHandle& Handle, const Driver::FramebufferBuildData& Data)
	{
		if (!Handle.Id)
		{
			CreateFramebuffer(Handle);
		}

		uint32				ColourCount = 0;
		bool				hasImage = false;
		Gfl::Array<uint32>	DrawBuffers;

		BindFramebuffer(Handle);

		for (Driver::FrameTexAttachment& Texture : Data.Attachments)
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

		if (!DrawBuffers.Length())
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		if (DrawBuffers.Length())
		{
			glDrawBuffers((int32)DrawBuffers.Length(), DrawBuffers.First());
		}

		if (glCheckFramebufferStatus(Handle.Target) != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("Framebuffer is incomplete because it's size is not a power of 2 most likely ...\n");
			//_ASSERTE(false);	// Building Framebuffer process failed.
		}

		UnbindFramebuffer(Handle);
	}

	void GraphicsDriver::BuildBuffer(GfxHandle& Handle, const Driver::BufferBuildData& Data)
	{
		if (!Handle.Id)
		{
			CreateBuffer(Handle, Data.Target);
		}

		BindBuffer(Handle);

		glBufferData(Handle.Target, Data.Size, Data.DataPtr, GL_STATIC_DRAW);

		if (Data.SubData.Length())
		{
			for (Driver::BufferSubData& SubData : Data.SubData)
			{
				glBufferSubData(Handle.Target, SubData.Offset, SubData.Size, SubData.DataPtr);
			}
		}

		UnbindBuffer(Handle);
	}

	void GraphicsDriver::SetViewport(int32 X, int32 Y, int32 Width, int32 Height)
	{
		glViewport(X, Y, Width, Height);
	}

	int32 GraphicsDriver::GetProgramUniforms(GfxHandle& Handle)
	{
		int32 Total = 0;

		glGetProgramiv(Handle.Id, GL_ACTIVE_UNIFORMS, &Total);

		return Total;
	}

	int32 GraphicsDriver::GetProgramAttributes(GfxHandle& Handle)
	{
		int32 Total = 0;

		glGetProgramiv(Handle.Id, GL_ACTIVE_ATTRIBUTES, &Total);

		return Total;
	}

	UniformAttr GraphicsDriver::GetActiveUniform(GfxHandle& Handle, uint32 Index)
	{
		UniformAttr Uniform;

		Uniform.Name.Reserve(128);

		glGetActiveUniform(Handle.Id, Index, 128, 0, &Uniform.Size, (uint32*)&Uniform.Type, Uniform.Name.First());

		return Uniform;
	}

	VertexAttr GraphicsDriver::GetActiveAttribute(GfxHandle& Handle, uint32 Index)
	{
		VertexAttr Attribute;

		Attribute.Name.Reserve(128);

		glGetActiveAttrib(Handle.Id, Index, 128, 0, &Attribute.Size, (uint32*)&Attribute.Type, Attribute.Name.First());

		return Attribute;
	}



}