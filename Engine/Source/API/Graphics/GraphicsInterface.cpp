#include "GrvtPch.h"
#include "API/Graphics/GraphicsInterface.h"


namespace Grvt
{

	GraphicsInterface::GraphicsInterface() :
		Driver(nullptr), MeshQueue(), TextureQueue(), ShaderQueue(), FrameQueue(), BufferQueue(), DeleteQueue(), HasResource(false) {}


	GraphicsInterface::~GraphicsInterface() {}


	void GraphicsInterface::Init(GraphicsDriver* Driver)
	{
		this->Driver = Driver;
	}


	void GraphicsInterface::Shutdown()
	{
		Driver = nullptr;

		MeshQueue.clear();
		TextureQueue.clear();
		ShaderQueue.clear();
		FrameQueue.clear();
		BufferQueue.clear();
		DeleteQueue.clear();
	}


	void GraphicsInterface::QueueMeshForBuild(GfxHandle& Handle, const Driver::MeshBuildData& Data)
	{
		Interface::MeshPacket Packet;

		Packet.Handle = &Handle;
		Packet.BuildData = Data;

		MeshQueue.push_back(Packet);

		HasResource = true;
	}


	void GraphicsInterface::QueueTextureForBuild(GfxHandle& Handle, const Driver::TextureBuildData& Data)
	{
		Interface::TexturePacket Packet;
		
		Packet.Handle = &Handle;
		Packet.BuildData = Data;

		TextureQueue.push_back(Packet);
		
		HasResource = true;
	}


	void GraphicsInterface::QueueShaderForBuild(GfxHandle& Handle, const Driver::ShaderBuildData& Data)
	{
		Interface::ShaderPacket Packet;

		Packet.Handle = &Handle;
		Packet.BuildData = Data;

		ShaderQueue.push_back(Packet);
		
		HasResource = true;
	}


	void GraphicsInterface::QueueFramebufferForBuild(GfxHandle& Handle, const Driver::FramebufferBuildData& Data)
	{
		Interface::FramePacket Packet;

		Packet.Handle = &Handle;
		Packet.BuildData = Data;

		FrameQueue.push_back(Packet);
		
		HasResource = true;
	}


	void GraphicsInterface::QueueBufferForBuild(GfxHandle& Handle, const Driver::BufferBuildData& Data)
	{
		Interface::BufferPacket Packet;

		Packet.Handle = &Handle;
		Packet.BuildData = Data;

		BufferQueue.push_back(Packet);
		
		HasResource = true;
	}


	void GraphicsInterface::QueueMeshPacketForBuild(const Interface::MeshPacket& Packet)
	{
		MeshQueue.push_back(Packet);
	}


	void GraphicsInterface::QueueTexturePacketForBuild(const Interface::TexturePacket& Packet)
	{
		TextureQueue.push_back(Packet);
	}


	void GraphicsInterface::QueueShaderPacketForBuild(const Interface::ShaderPacket& Packet)
	{
		ShaderQueue.push_back(Packet);
	}


	void GraphicsInterface::QueueFramePacketForBuild(const Interface::FramePacket& Packet)
	{
		FrameQueue.push_back(Packet);
	}


	void GraphicsInterface::QueueBufferPacketForBuild(const Interface::BufferPacket& Packet)
	{
		BufferQueue.push_back(Packet);
	}

	void GraphicsInterface::QueueHandleForDelete(GfxHandle&& Handle, HandleType Type)
	{
		Interface::DeletePacket Packet;

		Packet.Handle = Gfl::Move(Handle);
		Packet.Type = Type;

		DeleteQueue.push_back(Packet);
	}


	/*
	glGetProgramiv(Packet.ResourcePtr->Handle.Id, GL_ACTIVE_UNIFORMS, &total);
	Packet.ResourcePtr->Uniforms.Reserve((size_t)total);
	for (uint32 i = 0; i < (uint32)total; i++)
	{
		glGetActiveUniform(Packet.ResourcePtr->Handle.Id, i, sizeof(buffer), 0, &size, &type, buffer);
		GetUniformType(type, uniform.Type, uniform.SubType);
		uniform.Name = buffer;
		uniform.Location = glGetUniformLocation(Packet.ResourcePtr->Handle.Id, buffer);
		uniform.Size = size;

		Packet.ResourcePtr->Uniforms.Insert(uniform);
	}
	*/


	void GraphicsInterface::Tick()
	{
		if (!HasResource)
		{
			return;
		}

		for (Interface::MeshPacket& Packet : MeshQueue)
		{
			Driver->BuildMesh(*Packet.Handle, Packet.BuildData);
			MeshQueue.pop_front();
		}

		for (Interface::TexturePacket& Packet : TextureQueue)
		{
			Driver->BuildTexture(*Packet.Handle, Packet.BuildData);
			TextureQueue.pop_front();
		}

		for (Interface::ShaderPacket& Packet : ShaderQueue)
		{
			Driver->BuildShaderProgram(*Packet.Handle, Packet.BuildData);
			ShaderQueue.pop_front();
		}

		for (Interface::FramePacket& Packet : FrameQueue)
		{
			Driver->BuildFramebuffer(*Packet.Handle, Packet.BuildData);
			FrameQueue.pop_front();
		}

		for (Interface::BufferPacket& Packet : BufferQueue)
		{
			Driver->BuildBuffer(*Packet.Handle, Packet.BuildData);
			BufferQueue.pop_front();
		}

		for (Interface::DeletePacket& Packet : DeleteQueue)
		{
			switch (Packet.Type) {
			case HandleType::Handle_Buffer:
				Driver->DeleteBuffer(Packet.Handle);
				break;
			case HandleType::Handle_VertexArray:
				Driver->DeleteVertexArray(Packet.Handle);
				break;
			case HandleType::Handle_Texture:
				Driver->DeleteTexture(Packet.Handle);
				break;
			case HandleType::Handle_Shader:
				Driver->DeleteShader(Packet.Handle);
				break;
			case HandleType::Handle_Framebuffer:
				Driver->DeleteFramebuffer(Packet.Handle);
				break;
			}
		}

		HasResource = false;
	}


	void GraphicsInterface::PackageMeshForBuild(GrvtMesh& MeshSrc)
	{
		int32	Stride = 0;
		size_t	Index = 0;
		size_t	Total = 0;
		size_t	Offset = 0;

		GrvtMesh* Mesh = &MeshSrc;

		Total = Mesh->Positions.Length() * 3;
		if (Mesh->Normals.Length())		Total += Mesh->Normals.Length() * 3;
		if (Mesh->Uv.Length())			Total += Mesh->Uv.Length() * 2;
		if (Mesh->Tangents.Length())	Total += Mesh->Tangents.Length() * 3;
		if (Mesh->Bitangents.Length())	Total += Mesh->Bitangents.Length() * 3;

		Stride = 3 * sizeof(float32);
		if (Mesh->Normals.Length())		Stride += 3 * sizeof(float32);
		if (Mesh->Uv.Length())			Stride += 2 * sizeof(float32);
		if (Mesh->Tangents.Length())	Stride += 3 * sizeof(float32);
		if (Mesh->Bitangents.Length())	Stride += 3 * sizeof(float32);

		Mesh->Size = (Mesh->Indices.Length()) ? (uint32)Mesh->Indices.Length() : (uint32)Total;

		Interface::MeshPacket Packet;

		Packet.Handle = &Mesh->Vao;
		Packet.BuildData.PushAttribute(0, 3, Stride, Offset);
		Offset += 3 * sizeof(float32);

		if (Mesh->Normals.Length())
		{
			Packet.BuildData.PushAttribute(1, 3, Stride, Offset);
			Offset += 3 * sizeof(float32);
		}

		if (Mesh->Uv.Length())
		{
			Packet.BuildData.PushAttribute(2, 2, Stride, Offset);
			Offset += 2 * sizeof(float32);
		}

		if (Mesh->Tangents.Length())
		{
			Packet.BuildData.PushAttribute(3, 3, Stride, Offset);
			Offset += 3 * sizeof(float32);
		}

		if (Mesh->Bitangents.Length())
		{
			Packet.BuildData.PushAttribute(4, 3, Stride, Offset);
			Offset += 3 * sizeof(float32);
		}

		Packet.Interleaved.Reserve(Total);

		for (size_t i = 0; i < Mesh->Positions.Length(); i++)
		{
			if (Mesh->Normals.Length())
			{
				Packet.Interleaved[Index++] = Mesh->Normals[i].x;
				Packet.Interleaved[Index++] = Mesh->Normals[i].y;
				Packet.Interleaved[Index++] = Mesh->Normals[i].z;
			}

			if (Mesh->Uv.Length())
			{
				Packet.Interleaved[Index++] = Mesh->Uv[i].x;
				Packet.Interleaved[Index++] = Mesh->Uv[i].y;
			}

			if (Mesh->Tangents.Length())
			{
				Packet.Interleaved[Index++] = Mesh->Tangents[i].x;
				Packet.Interleaved[Index++] = Mesh->Tangents[i].y;
				Packet.Interleaved[Index++] = Mesh->Tangents[i].z;

				Packet.Interleaved[Index++] = Mesh->Bitangents[i].x;
				Packet.Interleaved[Index++] = Mesh->Bitangents[i].y;
				Packet.Interleaved[Index++] = Mesh->Bitangents[i].z;
			}
		}

		Driver::BufferBuildData VboData;

		VboData.Size = Total;
		VboData.Target = GL_ARRAY_BUFFER;
		VboData.DataPtr = Packet.Interleaved.First();

		Packet.BuildData.PushBuffer(&Mesh->Vbo, VboData);

		Driver::BufferBuildData EboData;

		EboData.Size = Mesh->Indices.Length();
		EboData.Target = GL_ELEMENT_ARRAY_BUFFER;
		EboData.DataPtr = Mesh->Indices.First();

		Packet.BuildData.PushBuffer(&Mesh->Ebo, EboData);

		QueueMeshPacketForBuild(Packet);
	}


	void GraphicsInterface::PackageTextureForBuild(GrvtTexture& TextureSrc)
	{
		Driver::TextureBuildData BuildData;

		BuildData.Target  = GL_TEXTURE_2D;
		BuildData.Type = GL_UNSIGNED_BYTE;

		BuildData.Width  = TextureSrc.Width;
		BuildData.Height = TextureSrc.Height;

		BuildData.FilterLevel = 4.0f;
		BuildData.Mipmap = true;

		BuildData.PushTextureParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		BuildData.PushTextureParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
		BuildData.PushTextureParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		BuildData.PushTextureParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		BuildData.DataPtr = TextureSrc.Data.First();

		if (TextureSrc.Data.Length())
		{
			BuildData.Target = GL_TEXTURE_CUBE_MAP;

			BuildData.Parameters.Empty();

			BuildData.PushTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			BuildData.PushTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			BuildData.PushTextureParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			BuildData.PushTextureParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			BuildData.PushTextureParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			BuildData.DataPtr = nullptr;

			for (size_t i = 0; i < TextureSrc.Data.Length(); i++)
			{
				BuildData.CubeMapDataPtr[i] = TextureSrc.Data[i];
			}
		}
		
		switch (TextureSrc.Channel)
		{
		case 1:
			BuildData.Format = GL_RED;
			BuildData.InternalFormat = GL_RED;
			break;
		case 3:
			BuildData.Format = GL_RGB;
			BuildData.InternalFormat = GL_RGB;
			break;
		case 4:
			BuildData.Format = GL_RGBA;
			BuildData.InternalFormat = GL_RGBA;
			break;
		}

		QueueTextureForBuild(TextureSrc.Handle, BuildData);
	}


	void GraphicsInterface::PackageShaderForBuild(GrvtShader& ShaderSrc)
	{
		Driver::ShaderBuildData BuildData;

		BuildData.VertexShader = ShaderSrc.VertexShader;
		BuildData.FragmentShader = ShaderSrc.FragmentShader;

		if (ShaderSrc.GeometryShader.Length())
		{
			BuildData.GeometryShader = ShaderSrc.GeometryShader;
		}

		QueueShaderForBuild(ShaderSrc.Handle, BuildData);
	}

}