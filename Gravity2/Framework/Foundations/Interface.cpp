#include "stdafx.h"


namespace Middleware {


	static ResourceBuildQueue* GlobalBuildQueue = nullptr;


	DeletePacket::DeletePacket() : Handle(), Type(GFX_TYPE_NONE) {}


	DeletePacket::DeletePacket(ObjHandle &&Resource, GfxObjectType Type) :
		Handle(Move(Resource)), Type(Type) {}


	DeletePacket::DeletePacket(DeletePacket &&Rhs) { *this = std::move(Rhs); }


	DeletePacket& DeletePacket::operator= (DeletePacket &&Rhs) {
		_ASSERTE(this != &Rhs);

		if (this != &Rhs) {
			Handle		= std::move(Rhs.Handle);
			Type		= Rhs.Type;

			new (&Rhs) DeletePacket();
		}

		return *this;
	}


	DeletePacket::~DeletePacket() { Type = GFX_TYPE_NONE; }


	ResourceBuildQueue::ResourceBuildQueue() 
		: MeshQueue(), TextureQueue(), FramebufferQueue(), DeleteQueue() {}


	ResourceBuildQueue::~ResourceBuildQueue() {}


	void ResourceBuildQueue::QueueMeshForBuild(GrvtMesh* Mesh, BaseAPI::MeshBuildData Data) {
		MeshQueue.push_back(MeshPacket(Mesh, Data));
	}


	void ResourceBuildQueue::QueueTextureForBuild(GrvtTexture* Texture, BaseAPI::TextureBuildData Data) {
		TextureQueue.push_back(TexturePacket(Texture, Data));
	}


	void ResourceBuildQueue::QueueShaderForBuild(GrvtShader* Shader, BaseAPI::ShaderBuildData Data) {
		ShaderQueue.push_back(ShaderPacket(Shader, Data));
	}


	void ResourceBuildQueue::QueuePostProcessForBuild(GrvtPostProcess* Framebuffer, BaseAPI::FramebufferBuildData Data) {
		FramebufferQueue.push_back(FramebufferPacket(Framebuffer, Data));
	}


	void ResourceBuildQueue::QueueHandleForDelete(ObjHandle& Handle, GfxObjectType Type) {
		DeletePacket Packet;
		Packet.Handle	= std::move(Handle);
		Packet.Type		= Type;

		DeleteQueue.push_back(Move(Packet));
	}


	void ResourceBuildQueue::Listen() {
		// Build meshes that are in the queue.
		for (MeshPacket &Packet : MeshQueue) {
			BaseAPI::BuildMesh(Packet.ResourcePtr->Vao, Packet.ResourcePtr->Vbo, Packet.ResourcePtr->Ebo, Packet.BuildData);
			Packet.ResourcePtr->Size = (Packet.ResourcePtr->Ebo.Id) ? (uint)Packet.BuildData.Length : (uint)Packet.BuildData.Size;
			free(Packet.BuildData.Data);
			MeshQueue.pop_front();
		}

		// Build textures that are in the queue.
		for (TexturePacket &Packet : TextureQueue) {
			BaseAPI::BuildTexture(Packet.ResourcePtr->Handle, Packet.BuildData);
			free(Packet.BuildData.DataPtr);
			TextureQueue.pop_front();
		}

		// Build shaders that are in the queue.
		for (ShaderPacket &Packet : ShaderQueue) {
			BaseAPI::BuildShaderProgram(Packet.ResourcePtr->Handle, Packet.BuildData);
			ShaderQueue.pop_front();
		}


		// Build framebuffers that are in the queue.
		for (FramebufferPacket &Packet : FramebufferQueue) {
			BaseAPI::BuildFramebuffer(Packet.ResourcePtr->Handle, Packet.BuildData);
			FramebufferQueue.pop_front();
		}

		// Listen to delete packets.
		for (DeletePacket &Packet : DeleteQueue) {
			switch (Packet.Type) {
			case GFX_TYPE_MESHBUFFER:
				BaseAPI::GrDeleteBufferObject(Packet.Handle);
				break;
			case GFX_TYPE_VERTEXARRAY:
				BaseAPI::GrDeleteVertexArray(Packet.Handle);
				break;
			case GFX_TYPE_TEXTUREID:
				BaseAPI::GrDeleteTexture(Packet.Handle);
				break;
			case GFX_TYPE_SHADERID:
				BaseAPI::GrDeleteShader(Packet.Handle);
				break;
			case GFX_TYPE_POSTPROCESS:
				BaseAPI::GrDeleteFramebuffer(Packet.Handle);
				break;
			case GFX_TYPE_RENDERBUFFER:
				BaseAPI::GrDeleteRenderbuffer(Packet.Handle);
				break;
			}
		}
	}



	ResourceBuildQueue* InitialiseBuildQueue() {
		if (!GlobalBuildQueue)
			GlobalBuildQueue = new ResourceBuildQueue();

		return GlobalBuildQueue;
	}


	ResourceBuildQueue* GetBuildQueue() { 
		return GlobalBuildQueue;
	}


	void ReleaseBuildQueue() {
		if (GlobalBuildQueue)
			delete GlobalBuildQueue;

		GlobalBuildQueue = nullptr;
	}


	//void ParseMeshFromAssimp(const String Path, bool FlipUV, ModelObj *Model) {
	//	Assimp::Importer	importFile;

	//	float	*data		= nullptr;
	//	MeshObj	*mesh		= nullptr;
	//	aiMesh	*assimpMesh = nullptr;

	//	uint flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
	//	if (FlipUV)
	//		flags |= aiProcess_FlipUVs;

	//	const aiScene *assimpScene = importFile.ReadFile(~Path, flags);

	//	for (uint i = 0; i < assimpScene->mNumMeshes; i++) {
	//		assimpMesh = assimpScene->mMeshes[i];
	//		mesh = &Model->Meshes.Insert(MeshObj());

	//		mesh->Positions.Reserve(assimpMesh->mNumVertices, false);
	//		mesh->Normals.Reserve(assimpMesh->mNumVertices, false);
	//		
	//		if (assimpMesh->mNumUVComponents) {
	//			mesh->Uv.Reserve(assimpMesh->mNumVertices,         false);
	//			mesh->Tangents.Reserve(assimpMesh->mNumVertices,   false);
	//			mesh->Bitangents.Reserve(assimpMesh->mNumVertices, false);
	//		}

	//		glm::vec3	vector;
	//		glm::vec2	uv;

	//		for (uint i = 0; i < assimpMesh->mNumVertices; i++) {
	//			vector.x = assimpMesh->mVertices[i].x;
	//			vector.y = assimpMesh->mVertices[i].y;
	//			vector.z = assimpMesh->mVertices[i].z;
	//			
	//			mesh->Positions.Push(vector);

	//			if (assimpMesh->mNormals) {
	//				vector.x = assimpMesh->mNormals[i].x;
	//				vector.y = assimpMesh->mNormals[i].y;
	//				vector.z = assimpMesh->mNormals[i].z;

	//				mesh->Normals.Push(vector);
	//			}

	//			if (assimpMesh->mTextureCoords[0]) {
	//				uv.x = assimpMesh->mTextureCoords[0][i].x;
	//				uv.y = assimpMesh->mTextureCoords[0][i].y;

	//				mesh->Uv.Push(uv);
	//			}

	//			if (assimpMesh->mTangents) {
	//				vector.x = assimpMesh->mTangents[i].x;
	//				vector.y = assimpMesh->mTangents[i].y;
	//				vector.z = assimpMesh->mTangents[i].z;

	//				mesh->Tangents.Push(vector);

	//				vector.x = assimpMesh->mBitangents[i].x;
	//				vector.y = assimpMesh->mBitangents[i].y;
	//				vector.z = assimpMesh->mBitangents[i].z;

	//				mesh->Bitangents.Push(vector);
	//			}
	//		}

	//		for (uint i = 0; i < assimpMesh->mNumFaces; i++) {
	//			aiFace assimpFace = assimpMesh->mFaces[i];

	//			for (uint j = 0; j < assimpFace.mNumIndices; j++)
	//				mesh->Indices.Push(assimpFace.mIndices[j]);
	//		}
	//	}

	//	for (MeshObj &mesh : Model->Meshes) {
	//		PackageMeshForBuild(&mesh);
	//	}
	//}


	void PackageMeshForBuild(GrvtMesh* MeshSrc) {
		int32	stride	= 0;
		size_t	idx		= 0;
		size_t	total	= 0;
		size_t	pointer = 0;
		GrvtMesh &mesh	= *MeshSrc;

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

		if (mesh.Normals.Length()) {
			buildData.VertexAttribPointers.Push(BaseAPI::VertexAttribPointer(1, 3, stride, pointer));
			pointer += 3 * sizeof(float);
		}

		if (mesh.Uv.Length()) {
			buildData.VertexAttribPointers.Push(BaseAPI::VertexAttribPointer(2, 2, stride, pointer));
			pointer += 2 * sizeof(float);
		}

		if (mesh.Tangents.Length()) {
			buildData.VertexAttribPointers.Push(BaseAPI::VertexAttribPointer(3, 3, stride, pointer));
			pointer += 3 * sizeof(float);

			buildData.VertexAttribPointers.Push(BaseAPI::VertexAttribPointer(4, 3, stride, pointer));
		}

		for (uint i = 0; i < mesh.Positions.Length(); i++) {
			buildData.Data[idx++] = mesh.Positions[i].x;
			buildData.Data[idx++] = mesh.Positions[i].y;
			buildData.Data[idx++] = mesh.Positions[i].z;

			if (mesh.Normals.Length()) {
				buildData.Data[idx++] = mesh.Normals[i].x;
				buildData.Data[idx++] = mesh.Normals[i].y;
				buildData.Data[idx++] = mesh.Normals[i].z;
			}

			if (mesh.Uv.Length()) {
				buildData.Data[idx++] = mesh.Uv[i].x;
				buildData.Data[idx++] = mesh.Uv[i].y;
			}

			if (mesh.Tangents.Length()) {
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


	void PackageTextureForBuild(GrvtTexture* TextureSrc) {
		BaseAPI::TextureBuildData buildData;
		BaseAPI::GenerateGenericTextureData(buildData);
		buildData.Flip = true;
		buildData.DataPtr	= TextureSrc->Properties[0].DataPtr.First();
		buildData.Width		= TextureSrc->Properties[0].Width;
		buildData.Height	= TextureSrc->Properties[0].Height;

		switch (TextureSrc->Properties[0].Channel) {
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


	void PackageShaderForBuild(GrvtShader* ShaderSrc) {
		BaseAPI::ShaderBuildData buildData;
		BaseAPI::ShaderInfo info;
		
		for (size_t i = 0; i < ShaderSrc->Properties.Length(); i++) {
			BaseAPI::FoundationsShaderType type;
			switch (ShaderSrc->Properties[i].Component) {
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


	void PackageFramebufferForBuild(GrvtPostProcess* FramebufferSrc) {
		uint32 imgCount = 0;
		uint32 rbCount = 0;
		BaseAPI::FramebufferBuildData buildData;

		for (size_t i = 0; i < FramebufferSrc->Attachments.Length(); i++) {
			uint32* count = nullptr;
			if (FramebufferSrc->Attachments[i].Component == GrvtFramebuffer_AttachComponent_Texture)
				count = &imgCount;

			if (FramebufferSrc->Attachments[i].Component == GrvtFramebuffer_AttachComponent_RenderBuffer)
				count = &rbCount;

			switch (FramebufferSrc->Attachments[i].Type) {
			case GrvtFramebuffer_Attachment_Colour:
				buildData.Attachment.Push(BaseAPI::FramebufferAttachment(FramebufferSrc->Attachments[i].Handle.Id, FramebufferSrc->Attachments[i].Handle.Target, GL_COLOR_ATTACHMENT0 + *count++, FramebufferSrc->Attachments[i].Draw));
				break;
			case GrvtFramebuffer_Attachment_Depth:
				buildData.Attachment.Push(BaseAPI::FramebufferAttachment(FramebufferSrc->Attachments[i].Handle.Id, FramebufferSrc->Attachments[i].Handle.Target, GL_DEPTH_ATTACHMENT, false));
				break;
			case GrvtFramebuffer_Attachment_DepthStencil:
				buildData.Attachment.Push(BaseAPI::FramebufferAttachment(FramebufferSrc->Attachments[i].Handle.Id, FramebufferSrc->Attachments[i].Handle.Target, GL_DEPTH_STENCIL_ATTACHMENT, false));
				break;
			}

			FramebufferSrc->Attachments[i].Count = *count;
		}

		GetBuildQueue()->QueuePostProcessForBuild(FramebufferSrc, buildData);
	}


}