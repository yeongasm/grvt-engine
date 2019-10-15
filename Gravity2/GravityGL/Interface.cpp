#include "stdafx.h"


ResourceBuildQueue *buildQueue = nullptr;


MeshPacket::MeshPacket() : MeshPtr(nullptr), BuildData() {}


MeshPacket::MeshPacket(Mesh *Resource, MeshBuildData Data) : 
	MeshPtr(Resource), BuildData(std::move(Data)) {}


MeshPacket::MeshPacket(const MeshPacket &Rhs) { *this = Rhs; }


MeshPacket::MeshPacket(MeshPacket &&Rhs) { *this = std::move(Rhs); }


MeshPacket& MeshPacket::operator= (const MeshPacket &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		MeshPtr		= Rhs.MeshPtr;
		BuildData	= std::move(Rhs.BuildData);
	}

	return *this;
}


MeshPacket& MeshPacket::operator= (MeshPacket &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		new (this) MeshPacket();

		MeshPtr		= Rhs.MeshPtr;
		BuildData	= std::move(Rhs.BuildData);

		new (&Rhs) MeshPacket();
	}

	return *this;
}


MeshPacket::~MeshPacket() { MeshPtr = nullptr; }


TexturePacket::TexturePacket() : TexturePtr(nullptr) {}


TexturePacket::TexturePacket(TextureObj *Resource, TextureBuildData Data) :
	TexturePtr(Resource), BuildData(Data) {}


TexturePacket::TexturePacket(const TexturePacket &Rhs) { *this = Rhs; }


TexturePacket::TexturePacket(TexturePacket &&Rhs) { *this = std::move(Rhs); }


TexturePacket& TexturePacket::operator= (const TexturePacket &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		TexturePtr	= Rhs.TexturePtr;
	}

	return *this;
}


TexturePacket& TexturePacket::operator= (TexturePacket &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		new (this) TexturePacket();

		TexturePtr	= Rhs.TexturePtr;

		new (&Rhs) TexturePacket();
	}

	return *this;
}


TexturePacket::~TexturePacket() { TexturePtr = nullptr; }


RenderbufferPacket::RenderbufferPacket() : RenderBufferPtr(nullptr), BuildData() {}


RenderbufferPacket::RenderbufferPacket(RenderBuffer *Resource, RenderbufferBuildData Data) :
	RenderBufferPtr(Resource), BuildData(Data) {}


RenderbufferPacket::RenderbufferPacket(const RenderbufferPacket &Rhs) { *this = Rhs; }


RenderbufferPacket& RenderbufferPacket::operator= (const RenderbufferPacket &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		RenderBufferPtr = Rhs.RenderBufferPtr;
		BuildData		= Rhs.BuildData;
	}

	return *this;
}


RenderbufferPacket::RenderbufferPacket(RenderbufferPacket &&Rhs) { *this = std::move(Rhs); }


RenderbufferPacket& RenderbufferPacket::operator= (RenderbufferPacket &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		RenderBufferPtr = Rhs.RenderBufferPtr;
		BuildData		= Rhs.BuildData;

		new (&Rhs) RenderbufferPacket();
	}

	return *this;
}


RenderbufferPacket::~RenderbufferPacket() { RenderBufferPtr = nullptr; }


FramebufferPacket::FramebufferPacket() : PostProcessPtr(nullptr), BuildData() {}


FramebufferPacket::FramebufferPacket(PostProcess *Resource, FramebufferBuildData Data) :
	PostProcessPtr(Resource), BuildData(Data) {}


FramebufferPacket::FramebufferPacket(const FramebufferPacket &Rhs) { *this = Rhs; }


FramebufferPacket& FramebufferPacket::operator= (const FramebufferPacket &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		PostProcessPtr	= Rhs.PostProcessPtr;
		BuildData		= Rhs.BuildData;
	}

	return *this;
}


FramebufferPacket::FramebufferPacket(FramebufferPacket &&Rhs) { *this = std::move(Rhs); }


FramebufferPacket& FramebufferPacket::operator= (FramebufferPacket &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		PostProcessPtr	= Rhs.PostProcessPtr;
		BuildData		= Rhs.BuildData;

		new (&Rhs) FramebufferPacket();
	}

	return *this;
}


FramebufferPacket::~FramebufferPacket() { PostProcessPtr = nullptr; }


DeletePacket::DeletePacket() : Handle(), Type(GFX_TYPE_NONE) {}


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


ResourceBuildQueue::ResourceBuildQueue() : MeshQueue(), TextureQueue(), FramebufferQueue(), DeleteQueue() {}


ResourceBuildQueue::~ResourceBuildQueue() {}


void ResourceBuildQueue::AddMeshForBuild(Mesh *Mesh, MeshBuildData Data) {
	MeshQueue.push_back(MeshPacket(Mesh, Data));
}


void ResourceBuildQueue::AddTextureForBuild(TextureObj *Texture, TextureBuildData Data) {
	TextureQueue.push_back(TexturePacket(Texture, Data));
}


void ResourceBuildQueue::AddPostprocessForBuild(PostProcess *Framebuffer, FramebufferBuildData Data) {
	FramebufferQueue.push_back(FramebufferPacket(Framebuffer, Data));
}


void ResourceBuildQueue::AddRenderBufferForBuild(RenderBuffer *Renderbuffer, RenderbufferBuildData Data) {
	RenderbufferQueue.push_back(RenderbufferPacket(Renderbuffer, Data));
}


void ResourceBuildQueue::AddHandleForDelete(ObjHandle &Handle, GfxObjectType Type) {
	DeletePacket Packet;
	Packet.Handle	= std::move(Handle);
	Packet.Type		= Type;

	DeleteQueue.push_back(std::move(Packet));
}


void ResourceBuildQueue::Listen() {
	// Build meshes that are in the queue.
	for (MeshPacket &Packet : MeshQueue) {
		BaseAPI::BuildMesh(Packet.MeshPtr->vao, Packet.MeshPtr->vbo, Packet.MeshPtr->ebo, Packet.BuildData);
		Packet.MeshPtr->size = (Packet.MeshPtr->ebo.Id) ? (uint)Packet.BuildData.Length : (uint)Packet.BuildData.Size;
		free(Packet.BuildData.Data);
		MeshQueue.pop_front();
	}

	// Build textures that are in the queue.
	for (TexturePacket &Packet : TextureQueue) {
		BaseAPI::BuildTexture(Packet.TexturePtr->Handle, Packet.BuildData);
		free(Packet.BuildData.DataPtr);
		TextureQueue.pop_front();
	}

	// Build framebuffers that are in the queue.
	for (FramebufferPacket &Packet : FramebufferQueue) {
		BaseAPI::BuildFramebuffer(Packet.PostProcessPtr->id, Packet.BuildData);
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
		case GFX_TYPE_POSTPROCESS:
			BaseAPI::GrDeleteFramebuffer(Packet.Handle);
			break;
		case GFX_TYPE_RENDERBUFFER:
			BaseAPI::GrDeleteRenderbuffer(Packet.Handle);
			break;
		}
	}
}


namespace Middleware {


	void SetBuildQueue(ResourceBuildQueue *BuildQueue) {
		if (!buildQueue)
			buildQueue = BuildQueue;
	}


	ResourceBuildQueue* GetBuildQueue() { 
		return buildQueue; 
	}


	void ParseMeshFromAssimp(const String Path, bool FlipUV, Scene *Scene) {
		Assimp::Importer	importFile;

		float	*data		= nullptr;
		Mesh	*mesh		= nullptr;
		aiMesh	*assimpMesh = nullptr;

		uint flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
		if (FlipUV)
			flags |= aiProcess_FlipUVs;

		const aiScene *assimpScene = importFile.ReadFile(~Path, flags);

		for (uint i = 0; i < assimpScene->mNumMeshes; i++) {
			assimpMesh = assimpScene->mMeshes[i];
			mesh = &Scene->meshes.Insert(Mesh());

			mesh->positions.Reserve(assimpMesh->mNumVertices, false);
			mesh->normals.Reserve(assimpMesh->mNumVertices, false);
			
			if (assimpMesh->mNumUVComponents) {
				mesh->uv.Reserve(assimpMesh->mNumVertices,         false);
				mesh->tangents.Reserve(assimpMesh->mNumVertices,   false);
				mesh->bitangents.Reserve(assimpMesh->mNumVertices, false);
			}

			glm::vec3	vector;
			glm::vec2	uv;

			for (uint i = 0; i < assimpMesh->mNumVertices; i++) {
				vector.x = assimpMesh->mVertices[i].x;
				vector.y = assimpMesh->mVertices[i].y;
				vector.z = assimpMesh->mVertices[i].z;
				
				mesh->positions.Push(vector);

				if (assimpMesh->mNormals) {
					vector.x = assimpMesh->mNormals[i].x;
					vector.y = assimpMesh->mNormals[i].y;
					vector.z = assimpMesh->mNormals[i].z;

					mesh->normals.Push(vector);
				}

				if (assimpMesh->mTextureCoords[0]) {
					uv.x = assimpMesh->mTextureCoords[0][i].x;
					uv.y = assimpMesh->mTextureCoords[0][i].y;

					mesh->uv.Push(uv);
				}

				if (assimpMesh->mTangents) {
					vector.x = assimpMesh->mTangents[i].x;
					vector.y = assimpMesh->mTangents[i].y;
					vector.z = assimpMesh->mTangents[i].z;

					mesh->tangents.Push(vector);

					vector.x = assimpMesh->mBitangents[i].x;
					vector.y = assimpMesh->mBitangents[i].y;
					vector.z = assimpMesh->mBitangents[i].z;

					mesh->bitangents.Push(vector);
				}
			}

			for (uint i = 0; i < assimpMesh->mNumFaces; i++) {
				aiFace assimpFace = assimpMesh->mFaces[i];

				for (uint j = 0; j < assimpFace.mNumIndices; j++)
					mesh->indices.Push(assimpFace.mIndices[j]);
			}
		}

		for (Mesh &mesh : Scene->meshes) {
			PackageMeshForBuild(&mesh);
		}
	}


	void Middleware::ParseTextureFromFile(const String Path, TextureObj *Texture) {
		int32 channels = 0;
		TextureBuildData buildData;

		BaseAPI::GenerateGenericTextureData(buildData);
		stbi_set_flip_vertically_on_load(buildData.Flip);

		buildData.DataPtr = (uint8*)stbi_load(Path.c_str(), (int32*)&buildData.Width, (int32*)&buildData.Height, &channels, 0);

		if (channels == 1)
			buildData.Format = GL_RED;
		else if (channels == 3)
			buildData.Format = GL_RGB;
		else if (channels == 4)
			buildData.Format = GL_RGBA;

		GetBuildQueue()->AddTextureForBuild(Texture, buildData);
	}


	void PackageMeshForBuild(Mesh *MeshSrc) {
		int32	stride	= 0;
		size_t	idx		= 0;
		size_t	total	= 0;
		size_t	pointer = 0;
		Mesh	&mesh	= *MeshSrc;

		MeshBuildData buildData;

		total = mesh.positions.Length() * 3;
		if (mesh.normals.Length())		total += mesh.normals.Length() * 3;
		if (mesh.uv.Length())			total += mesh.uv.Length() * 2;
		if (mesh.tangents.Length())		total += mesh.tangents.Length() * 3;
		if (mesh.bitangents.Length())	total += mesh.bitangents.Length() * 3;

		stride = 3 * sizeof(float);
		if (mesh.normals.Length())		stride += 3 * sizeof(float);
		if (mesh.uv.Length())			stride += 2 * sizeof(float);
		if (mesh.tangents.Length())		stride += 3 * sizeof(float);
		if (mesh.bitangents.Length())	stride += 3 * sizeof(float);

		buildData.Data = (float*)malloc(total * sizeof(float));
		buildData.Size = total;
		buildData.Indices = &mesh.indices.Front();
		buildData.Length = mesh.indices.Length();

		buildData.VertexAttribPointers.Push(VertexAttribPointer(0, 3, stride, pointer));
		pointer += 3 * sizeof(float);

		if (mesh.normals.Length()) {
			buildData.VertexAttribPointers.Push(VertexAttribPointer(1, 3, stride, pointer));
			pointer += 3 * sizeof(float);
		}

		if (mesh.uv.Length()) {
			buildData.VertexAttribPointers.Push(VertexAttribPointer(2, 2, stride, pointer));
			pointer += 2 * sizeof(float);
		}

		if (mesh.tangents.Length()) {
			buildData.VertexAttribPointers.Push(VertexAttribPointer(3, 3, stride, pointer));
			pointer += 3 * sizeof(float);

			buildData.VertexAttribPointers.Push(VertexAttribPointer(4, 3, stride, pointer));
		}

		for (uint i = 0; i < mesh.positions.Length(); i++) {
			buildData.Data[idx++] = mesh.positions[i].x;
			buildData.Data[idx++] = mesh.positions[i].y;
			buildData.Data[idx++] = mesh.positions[i].z;

			if (mesh.normals.Length()) {
				buildData.Data[idx++] = mesh.normals[i].x;
				buildData.Data[idx++] = mesh.normals[i].y;
				buildData.Data[idx++] = mesh.normals[i].z;
			}

			if (mesh.uv.Length()) {
				buildData.Data[idx++] = mesh.uv[i].x;
				buildData.Data[idx++] = mesh.uv[i].y;
			}

			if (mesh.tangents.Length()) {
				buildData.Data[idx++] = mesh.tangents[i].x;
				buildData.Data[idx++] = mesh.tangents[i].y;
				buildData.Data[idx++] = mesh.tangents[i].z;

				buildData.Data[idx++] = mesh.bitangents[i].x;
				buildData.Data[idx++] = mesh.bitangents[i].y;
				buildData.Data[idx++] = mesh.bitangents[i].z;

			}
		}

		GetBuildQueue()->AddMeshForBuild(&mesh, buildData);
		idx = total = pointer = stride = 0;
	}


	void PackageFramebufferForBuild(PostProcess *FramebufferSrc) {
		uint32 imgCount = 0;
		uint32 rbCount = 0;
		FramebufferBuildData buildData;

		for (PostProcessAttachment &attachment : FramebufferSrc->attachment) {
			if (attachment.type == FRAMEBUFFER_ATTACHMENT_TEXTURE) {
				switch (attachment.subType) {
				case FRAMEBUFFER_SUBATTACH_COLOUR:
					buildData.Attachment.Push(FramebufferAttachment(&attachment.texture->handle, GL_COLOR_ATTACHMENT0 + imgCount++, attachment.draw));
					break;
				case FRAMEBUFFER_SUBATTACH_DEPTH:
					buildData.Attachment.Push(FramebufferAttachment(&attachment.texture->handle, GL_DEPTH_ATTACHMENT, false));
					break;
				case FRAMEBUFFER_SUBATTACH_STENCIL:
					buildData.Attachment.Push(FramebufferAttachment(&attachment.texture->handle, GL_DEPTH_STENCIL_ATTACHMENT, false));
					break;
				}
			}

			if (attachment.type == FRAMEBUFFER_ATTACHMENT_RENDERBUFFER) {
				switch (attachment.subType) {
				case FRAMEBUFFER_SUBATTACH_COLOUR:
					buildData.Attachment.Push(FramebufferAttachment(&attachment.renderbuffer->handle, GL_COLOR_ATTACHMENT0 + rbCount++, false));
					break;
				case FRAMEBUFFER_SUBATTACH_DEPTH:
					buildData.Attachment.Push(FramebufferAttachment(&attachment.renderbuffer->handle, GL_DEPTH_ATTACHMENT, false));
					break;
				case FRAMEBUFFER_SUBATTACH_STENCIL:
					buildData.Attachment.Push(FramebufferAttachment(&attachment.renderbuffer->handle, GL_DEPTH_STENCIL_ATTACHMENT, false));
					break;
				}
			}
		}

		GetBuildQueue()->AddPostprocessForBuild(FramebufferSrc, buildData);
	}


}