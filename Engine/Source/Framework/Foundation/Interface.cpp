#include "GrvtPch.h"
#include "Framework/Foundation/Interface.h"
#include "Framework/Abstraction/Model.h"
#include "Framework/Abstraction/Shader.h"
#include "Framework/Abstraction/Texture.h"
#include "Framework/Abstraction/Framebuffer.h"

namespace Grvt
{

	namespace Middleware
	{


		static ResourceBuildQueue* GlobalBuildQueue = nullptr;


		DeletePacket::DeletePacket() : Handle(), Type(GrvtGfx_Type_None) {}


		DeletePacket::DeletePacket(ObjHandle&& Resource, GfxObjectType Type) :
			Handle(Gfl::Move(Resource)), Type(Type) {}


		DeletePacket::DeletePacket(DeletePacket&& Rhs)
		{
			*this = Gfl::Move(Rhs);
		}


		DeletePacket& DeletePacket::operator= (DeletePacket&& Rhs)
		{
			_ASSERTE(this != &Rhs);

			if (this != &Rhs)
			{
				Handle = Gfl::Move(Rhs.Handle);
				Type = Rhs.Type;

				new (&Rhs) DeletePacket();
			}

			return *this;
		}


		DeletePacket::~DeletePacket() {
			Type = GrvtGfx_Type_None;
		}


		ResourceBuildQueue::ResourceBuildQueue()
			: MeshQueue(), TextureQueue(), ShaderQueue(), FramebufferQueue(), DeleteQueue() {}


		ResourceBuildQueue::~ResourceBuildQueue() {}


		void ResourceBuildQueue::QueueMeshForBuild(GrvtMesh* Mesh, BaseAPI::MeshBuildData Data)
		{
			MeshQueue.push_back(MeshPacket(Mesh, Data));
		}


		void ResourceBuildQueue::QueueTextureForBuild(GrvtTexture* Texture, BaseAPI::TextureBuildData Data)
		{
			TextureQueue.push_back(TexturePacket(Texture, Data));
		}


		void ResourceBuildQueue::QueueShaderForBuild(GrvtShader* Shader, BaseAPI::ShaderBuildData Data)
		{
			ShaderQueue.push_back(ShaderPacket(Shader, Data));
		}


		void ResourceBuildQueue::QueueFramebufferForBuild(GrvtFramebuffer* Framebuffer, BaseAPI::FramebufferBuildData Data)
		{
			FramebufferQueue.push_back(FramebufferPacket(Framebuffer, Data));
		}


		void ResourceBuildQueue::QueueHandleForDelete(ObjHandle&& Handle, GfxObjectType Type)
		{
			DeleteQueue.emplace_back(Gfl::Move(Handle), Type);
		}


		void ResourceBuildQueue::Listen()
		{
			// Build meshes that are in the queue.
			for (MeshPacket& Packet : MeshQueue)
			{
				BaseAPI::BuildMesh(Packet.ResourcePtr->Vao, Packet.ResourcePtr->Vbo, Packet.ResourcePtr->Ebo, Packet.BuildData);
				Packet.ResourcePtr->Size = (Packet.ResourcePtr->Ebo.Id) ? (uint32)Packet.BuildData.Length : (uint32)Packet.BuildData.Size;
				free(Packet.BuildData.Data);
				MeshQueue.pop_front();
			}

			// Build textures that are in the queue.
			for (TexturePacket& Packet : TextureQueue)
			{
				BaseAPI::BuildTexture(Packet.ResourcePtr->Handle, Packet.BuildData);
				//if (Packet.BuildData.CubemapDataPtr)
				//{
				//	free(Packet.BuildData.CubemapDataPtr);
				//}
				//else
				//{
				//	free(Packet.BuildData.DataPtr);
				//}
				TextureQueue.pop_front();
			}

			// Build shaders that are in the queue.
			for (ShaderPacket& Packet : ShaderQueue)
			{
				BaseAPI::BuildShaderProgram(Packet.ResourcePtr->Handle, Packet.BuildData);
				char buffer[128];
				uint32	type = 0;
				int32	total = 0;
				int32	size = 0;
				UniformAttr uniform;

				glGetProgramiv(Packet.ResourcePtr->Handle.Id, GL_ACTIVE_UNIFORMS, &total);
				Packet.ResourcePtr->Uniforms.Reserve(*(size_t*)&total);
				for (uint32 i = 0; i < *(uint32*)&total; i++)
				{
					glGetActiveUniform(Packet.ResourcePtr->Handle.Id, i, sizeof(buffer), 0, &size, &type, buffer);
					GetUniformType(type, uniform.Type, uniform.SubType);
					uniform.Name = buffer;
					uniform.Location = glGetUniformLocation(Packet.ResourcePtr->Handle.Id, buffer);
					uniform.Size = size;

					Packet.ResourcePtr->Uniforms.Insert(uniform);
				}

				ShaderQueue.pop_front();
			}


			// Build framebuffers that are in the queue.
			for (FramebufferPacket& Packet : FramebufferQueue)
			{
				BaseAPI::BuildFramebuffer(Packet.ResourcePtr->Handle, Packet.BuildData);
				FramebufferQueue.pop_front();
			}

			// Listen to delete packets.
			for (DeletePacket& Packet : DeleteQueue)
			{
				switch (Packet.Type) {
				case GrvtGfx_Type_MeshBuffer:
					BaseAPI::GrDeleteBufferObject(Packet.Handle);
					break;
				case GrvtGfx_Type_VertexArray:
					BaseAPI::GrDeleteVertexArray(Packet.Handle);
					break;
				case GrvtGfx_Type_Texture:
					BaseAPI::GrDeleteTexture(Packet.Handle);
					break;
				case GrvtGfx_Type_Shader:
					BaseAPI::GrDeleteShader(Packet.Handle);
					break;
				case GrvtGfx_Type_Framebuffer:
					BaseAPI::GrDeleteFramebuffer(Packet.Handle);
					break;
				case GrvtGfx_Type_RenderBuffer:
					BaseAPI::GrDeleteRenderbuffer(Packet.Handle);
					break;
				}
			}
		}



		ResourceBuildQueue* InitialiseBuildQueue()
		{
			if (!GlobalBuildQueue)
				GlobalBuildQueue = new ResourceBuildQueue();

			return GlobalBuildQueue;
		}


		ResourceBuildQueue* GetBuildQueue()
		{
			return GlobalBuildQueue;
		}


		void ReleaseBuildQueue() {
			if (GlobalBuildQueue)
				delete GlobalBuildQueue;

			GlobalBuildQueue = nullptr;
		}


		void PackageMeshForBuild(GrvtMesh* MeshSrc)
		{
			int32	stride = 0;
			size_t	idx = 0;
			size_t	total = 0;
			size_t	pointer = 0;
			GrvtMesh& mesh = *MeshSrc;

			BaseAPI::MeshBuildData buildData;

			total = mesh.Positions.Length() * 3;
			if (mesh.Normals.Length())		total += mesh.Normals.Length() * 3;
			if (mesh.Uv.Length())			total += mesh.Uv.Length() * 2;
			if (mesh.Tangents.Length())		total += mesh.Tangents.Length() * 3;
			if (mesh.Bitangents.Length())	total += mesh.Bitangents.Length() * 3;

			stride = 3 * sizeof(float);
			if (mesh.Normals.Length())		stride += 3 * sizeof(float);
			if (mesh.Uv.Length())			stride += 2 * sizeof(float);
			if (mesh.Tangents.Length())		stride += 3 * sizeof(float);
			if (mesh.Bitangents.Length())	stride += 3 * sizeof(float);

			buildData.Data = (float*)malloc(total * sizeof(float));
			buildData.Size = total;
			buildData.Indices = &mesh.Indices.Front();
			buildData.Length = mesh.Indices.Length();

			buildData.VertexAttribPointers.Push(BaseAPI::VertexAttribPointer(0, 3, stride, pointer));
			pointer += 3 * sizeof(float);

			if (mesh.Normals.Length())
			{
				buildData.VertexAttribPointers.Push(BaseAPI::VertexAttribPointer(1, 3, stride, pointer));
				pointer += 3 * sizeof(float);
			}

			if (mesh.Uv.Length())
			{
				buildData.VertexAttribPointers.Push(BaseAPI::VertexAttribPointer(2, 2, stride, pointer));
				pointer += 2 * sizeof(float);
			}

			if (mesh.Tangents.Length())
			{
				buildData.VertexAttribPointers.Push(BaseAPI::VertexAttribPointer(3, 3, stride, pointer));
				pointer += 3 * sizeof(float);

				buildData.VertexAttribPointers.Push(BaseAPI::VertexAttribPointer(4, 3, stride, pointer));
			}

			for (uint32 i = 0; i < mesh.Positions.Length(); i++)
			{
				buildData.Data[idx++] = mesh.Positions[i].x;
				buildData.Data[idx++] = mesh.Positions[i].y;
				buildData.Data[idx++] = mesh.Positions[i].z;

				if (mesh.Normals.Length())
				{
					buildData.Data[idx++] = mesh.Normals[i].x;
					buildData.Data[idx++] = mesh.Normals[i].y;
					buildData.Data[idx++] = mesh.Normals[i].z;
				}

				if (mesh.Uv.Length())
				{
					buildData.Data[idx++] = mesh.Uv[i].x;
					buildData.Data[idx++] = mesh.Uv[i].y;
				}

				if (mesh.Tangents.Length())
				{
					buildData.Data[idx++] = mesh.Tangents[i].x;
					buildData.Data[idx++] = mesh.Tangents[i].y;
					buildData.Data[idx++] = mesh.Tangents[i].z;

					buildData.Data[idx++] = mesh.Bitangents[i].x;
					buildData.Data[idx++] = mesh.Bitangents[i].y;
					buildData.Data[idx++] = mesh.Bitangents[i].z;

				}
			}

			GetBuildQueue()->QueueMeshForBuild(&mesh, buildData);
			idx = total = pointer = stride = 0;
		}


		void PackageTextureForBuild(GrvtTexture* TextureSrc)
		{
			BaseAPI::TextureBuildData buildData;
			BaseAPI::GenerateGenericTextureData(buildData);
			buildData.Flip = true;
			buildData.DataPtr = TextureSrc->DataPtr;
			buildData.Width = TextureSrc->Width;
			buildData.Height = TextureSrc->Height;

			switch (TextureSrc->Channel)
			{
			case 1:
				buildData.Format = GL_RED;
				break;
			case 3:
				buildData.Format = GL_RGB;
				break;
			case 4:
				buildData.Format = GL_RGBA;
				break;
			}

			GetBuildQueue()->QueueTextureForBuild(TextureSrc, buildData);
		}

		
		void PackageCubemapForBuild(GrvtTexture* CubemapSrc)
		{
			BaseAPI::TextureBuildData BuildData;
			BaseAPI::GenerateGenericCubemapData(BuildData);
			for (size_t i = 0; i < 6; i++)
			{
				BuildData.CubemapDataPtr[i] = CubemapSrc->CubemapPtr[i];

			}
			//memcpy(BuildData.CubemapDataPtr, CubemapSrc->CubemapPtr, 6);
			BuildData.Width = CubemapSrc->Width;
			BuildData.Height = CubemapSrc->Height;

			switch (CubemapSrc->Channel)
			{
			case 1:
				BuildData.Format = GL_RED;
				break;
			case 3:
				BuildData.Format = GL_RGB;
				break;
			case 4:
				BuildData.Format = GL_RGBA;
				break;
			}

			GetBuildQueue()->QueueTextureForBuild(CubemapSrc, BuildData);
		}


		void PackageCustomTextureForBuild(GrvtTexture* TextureSrc, BaseAPI::TextureBuildData& BuildData)
		{
			GetBuildQueue()->QueueTextureForBuild(TextureSrc, BuildData);
		}


		void PackageShaderForBuild(GrvtShader* ShaderSrc)
		{
			BaseAPI::ShaderBuildData buildData;
			BaseAPI::ShaderInfo info;

			for (size_t i = 0; i < ShaderSrc->Properties.Length(); i++)
			{
				BaseAPI::FoundationsShaderType type;
				switch (ShaderSrc->Properties[i].Component)
				{
				case GrvtShader_SourceType_Vertex:
					type = BaseAPI::GrvtFoundations_ShaderType_Vertex;
					break;
				case GrvtShader_SourceType_Fragment:
					type = BaseAPI::GrvtFoundations_ShaderType_Fragment;
					break;
				case GrvtShader_SourceType_Geometry:
					type = BaseAPI::GrvtFoundations_ShaderType_Geometry;
					break;
				}

				info.SourceCode = ShaderSrc->Properties[i].Code.First();
				info.Type = type;
				buildData.BuildInfo.Push(info);
			}

			GetBuildQueue()->QueueShaderForBuild(ShaderSrc, buildData);
		}


		void PackageFramebufferForBuild(GrvtFramebuffer* FramebufferSrc)
		{
			uint32 Type = GL_NONE;
			BaseAPI::FramebufferBuildData buildData;
			buildData.Width = FramebufferSrc->Width;
			buildData.Height = FramebufferSrc->Height;

			for (auto& Attachment : FramebufferSrc->Attachments)
			{
				switch (Attachment.Type)
				{
				case GrvtFramebuffer_Attachment_Depth:
					Type = GL_DEPTH_ATTACHMENT;
					break;
				case GrvtFramebuffer_Attachment_DepthStencil:
					Type = GL_DEPTH_STENCIL_ATTACHMENT;
					break;
				case GrvtFramebuffer_Attachment_Colour:
					Type = GL_COLOR_ATTACHMENT0 + Attachment.Count;
					break;
				}

				buildData.Attachments.Push(BaseAPI::FramebufferAttachment(&Attachment.Handle, Type, Attachment.Component));
			}

			GetBuildQueue()->QueueFramebufferForBuild(FramebufferSrc, buildData);
		}


		void GetUniformType(uint32 Type, uint32& AttributeType, uint32& AttributeSubType)
		{
			switch (Type)
			{
			case GL_BOOL:
				AttributeType = GrvtShader_AttrType_Boolean;
				AttributeSubType = GrvtShader_AttrSubType_None;
				break;
			case GL_INT:
				AttributeType = GrvtShader_AttrType_Integer;
				AttributeSubType = GrvtShader_AttrSubType_None;
				break;
			case GL_FLOAT:
				AttributeType = GrvtShader_AttrType_Float;
				AttributeSubType = GrvtShader_AttrSubType_None;
				break;
			case GL_SAMPLER_1D:
				AttributeType = GrvtShader_AttrType_Sampler;
				AttributeSubType = GrvtShader_AttrSubType_Sampler1D;
				break;
			case GL_SAMPLER_2D:
				AttributeType = GrvtShader_AttrType_Sampler;
				AttributeSubType = GrvtShader_AttrSubType_Sampler2D;
				break;
			case GL_SAMPLER_3D:
				AttributeType = GrvtShader_AttrType_Sampler;
				AttributeSubType = GrvtShader_AttrSubType_Sampler3D;
				break;
			case GL_SAMPLER_CUBE:
				AttributeType = GrvtShader_AttrType_Sampler;
				AttributeSubType = GrvtShader_AttrSubType_SamplerCube;
				break;
			case GL_FLOAT_VEC2:
				AttributeType = GrvtShader_AttrType_Vector;
				AttributeSubType = GrvtShader_AttrSubType_Vector2;
				break;
			case GL_FLOAT_VEC3:
				AttributeType = GrvtShader_AttrType_Vector;
				AttributeSubType = GrvtShader_AttrSubType_Vector3;
				break;
			case GL_FLOAT_VEC4:
				AttributeType = GrvtShader_AttrType_Vector;
				AttributeSubType = GrvtShader_AttrSubType_Vector4;
				break;
			case GL_FLOAT_MAT2:
				AttributeType = GrvtShader_AttrType_Matrix;
				AttributeSubType = GrvtShader_AttrSubType_Matrix2;
				break;
			case GL_FLOAT_MAT3:
				AttributeType = GrvtShader_AttrType_Matrix;
				AttributeSubType = GrvtShader_AttrSubType_Matrix3;
				break;
			case GL_FLOAT_MAT4:
				AttributeType = GrvtShader_AttrType_Matrix;
				AttributeSubType = GrvtShader_AttrSubType_Matrix4;
				break;
			default:
				break;
			}
		}

	}

}