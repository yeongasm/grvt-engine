#include "stdafx.h"


ResourceBuildQueue *buildQueue = nullptr;


MeshPacket::MeshPacket() : MeshPtr(nullptr), BuildData() {}


MeshPacket::MeshPacket(MeshObj *Resource, MeshBuildData Data) : 
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


ShaderPacket::ShaderPacket() :
	ShaderPtr(nullptr), BuildData() {}


ShaderPacket::ShaderPacket(ShaderObj *Resource, ShaderBuildData Data) :
	ShaderPtr(Resource), BuildData(Data) {}


ShaderPacket::~ShaderPacket() { ShaderPtr = nullptr; }


ShaderPacket::ShaderPacket(const ShaderPacket &Rhs) { *this = Rhs; }


ShaderPacket& ShaderPacket::operator= (const ShaderPacket &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		ShaderPtr = Rhs.ShaderPtr;
		BuildData = Rhs.BuildData;
	}

	return *this;
}


ShaderPacket::ShaderPacket(ShaderPacket &&Rhs) { *this = std::move(Rhs); }


ShaderPacket& ShaderPacket::operator= (ShaderPacket &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		ShaderPtr = Rhs.ShaderPtr;
		BuildData = Rhs.BuildData;

		new (&Rhs) ShaderPacket();
	}

	return *this;
}


FramebufferPacket::FramebufferPacket() : PostProcessPtr(nullptr), BuildData() {}


FramebufferPacket::FramebufferPacket(PostProcessObj *Resource, FramebufferBuildData Data) :
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


void ResourceBuildQueue::AddMeshForBuild(MeshObj *Mesh, MeshBuildData Data) {
	MeshQueue.push_back(MeshPacket(Mesh, Data));
}


void ResourceBuildQueue::AddTextureForBuild(TextureObj *Texture, TextureBuildData Data) {
	TextureQueue.push_back(TexturePacket(Texture, Data));
}


void ResourceBuildQueue::AddShaderForBuild(ShaderObj *Shader, ShaderBuildData Data) {
	ShaderQueue.push_back(ShaderPacket(Shader, Data));
}


void ResourceBuildQueue::AddPostprocessForBuild(PostProcessObj *Framebuffer, FramebufferBuildData Data) {
	FramebufferQueue.push_back(FramebufferPacket(Framebuffer, Data));
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
		BaseAPI::BuildMesh(Packet.MeshPtr->Vao, Packet.MeshPtr->Vbo, Packet.MeshPtr->Ebo, Packet.BuildData);
		Packet.MeshPtr->Size = (Packet.MeshPtr->Ebo.Id) ? (uint)Packet.BuildData.Length : (uint)Packet.BuildData.Size;
		free(Packet.BuildData.Data);
		MeshQueue.pop_front();
	}

	// Build textures that are in the queue.
	for (TexturePacket &Packet : TextureQueue) {
		BaseAPI::BuildTexture(Packet.TexturePtr->Handle, Packet.BuildData);
		free(Packet.BuildData.DataPtr);
		TextureQueue.pop_front();
	}

	// Build shaders that are in the queue.
	for (ShaderPacket &Packet : ShaderQueue) {
		BaseAPI::BuildShaderProgram(Packet.ShaderPtr->Handle, Packet.BuildData);
		ShaderQueue.pop_front();
	}


	// Build framebuffers that are in the queue.
	for (FramebufferPacket &Packet : FramebufferQueue) {
		BaseAPI::BuildFramebuffer(Packet.PostProcessPtr->Handle, Packet.BuildData);
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


namespace Middleware {


	ResourceBuildQueue* InitialiseBuildQueue() {
		if (!buildQueue)
			buildQueue = new ResourceBuildQueue();

		return buildQueue;
	}


	ResourceBuildQueue* GetBuildQueue() { 
		return buildQueue; 
	}


	void ParseMeshFromAssimp(const String Path, bool FlipUV, ModelObj *Model) {
		Assimp::Importer	importFile;

		float	*data		= nullptr;
		MeshObj	*mesh		= nullptr;
		aiMesh	*assimpMesh = nullptr;

		uint flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
		if (FlipUV)
			flags |= aiProcess_FlipUVs;

		const aiScene *assimpScene = importFile.ReadFile(~Path, flags);

		for (uint i = 0; i < assimpScene->mNumMeshes; i++) {
			assimpMesh = assimpScene->mMeshes[i];
			mesh = &Model->Meshes.Insert(MeshObj());

			mesh->Positions.Reserve(assimpMesh->mNumVertices, false);
			mesh->Normals.Reserve(assimpMesh->mNumVertices, false);
			
			if (assimpMesh->mNumUVComponents) {
				mesh->Uv.Reserve(assimpMesh->mNumVertices,         false);
				mesh->Tangents.Reserve(assimpMesh->mNumVertices,   false);
				mesh->Bitangents.Reserve(assimpMesh->mNumVertices, false);
			}

			glm::vec3	vector;
			glm::vec2	uv;

			for (uint i = 0; i < assimpMesh->mNumVertices; i++) {
				vector.x = assimpMesh->mVertices[i].x;
				vector.y = assimpMesh->mVertices[i].y;
				vector.z = assimpMesh->mVertices[i].z;
				
				mesh->Positions.Push(vector);

				if (assimpMesh->mNormals) {
					vector.x = assimpMesh->mNormals[i].x;
					vector.y = assimpMesh->mNormals[i].y;
					vector.z = assimpMesh->mNormals[i].z;

					mesh->Normals.Push(vector);
				}

				if (assimpMesh->mTextureCoords[0]) {
					uv.x = assimpMesh->mTextureCoords[0][i].x;
					uv.y = assimpMesh->mTextureCoords[0][i].y;

					mesh->Uv.Push(uv);
				}

				if (assimpMesh->mTangents) {
					vector.x = assimpMesh->mTangents[i].x;
					vector.y = assimpMesh->mTangents[i].y;
					vector.z = assimpMesh->mTangents[i].z;

					mesh->Tangents.Push(vector);

					vector.x = assimpMesh->mBitangents[i].x;
					vector.y = assimpMesh->mBitangents[i].y;
					vector.z = assimpMesh->mBitangents[i].z;

					mesh->Bitangents.Push(vector);
				}
			}

			for (uint i = 0; i < assimpMesh->mNumFaces; i++) {
				aiFace assimpFace = assimpMesh->mFaces[i];

				for (uint j = 0; j < assimpFace.mNumIndices; j++)
					mesh->Indices.Push(assimpFace.mIndices[j]);
			}
		}

		for (MeshObj &mesh : Model->Meshes) {
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


	void PackageMeshForBuild(MeshObj *MeshSrc) {
		int32	stride	= 0;
		size_t	idx		= 0;
		size_t	total	= 0;
		size_t	pointer = 0;
		MeshObj	&mesh	= *MeshSrc;

		MeshBuildData buildData;

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

		buildData.VertexAttribPointers.Push(VertexAttribPointer(0, 3, stride, pointer));
		pointer += 3 * sizeof(float);

		if (mesh.Normals.Length()) {
			buildData.VertexAttribPointers.Push(VertexAttribPointer(1, 3, stride, pointer));
			pointer += 3 * sizeof(float);
		}

		if (mesh.Uv.Length()) {
			buildData.VertexAttribPointers.Push(VertexAttribPointer(2, 2, stride, pointer));
			pointer += 2 * sizeof(float);
		}

		if (mesh.Tangents.Length()) {
			buildData.VertexAttribPointers.Push(VertexAttribPointer(3, 3, stride, pointer));
			pointer += 3 * sizeof(float);

			buildData.VertexAttribPointers.Push(VertexAttribPointer(4, 3, stride, pointer));
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

		GetBuildQueue()->AddMeshForBuild(&mesh, buildData);
		idx = total = pointer = stride = 0;
	}


	void PackageFramebufferForBuild(PostProcessObj *FramebufferSrc) {
		uint32 imgCount = 0;
		uint32 rbCount = 0;
		FramebufferBuildData buildData;

		for (PostProcessAttachment &attachment : FramebufferSrc->Attachment) {
			if (attachment.Type == FRAMEBUFFER_ATTACHMENT_TEXTURE) {
				switch (attachment.SubType) {
				case FRAMEBUFFER_SUBATTACH_COLOUR:
					buildData.Attachment.Push(FramebufferAttachment(attachment.Texture->Handle.Id, attachment.Texture->Handle.Target, GL_COLOR_ATTACHMENT0 + imgCount++, attachment.Draw));
					break;
				case FRAMEBUFFER_SUBATTACH_DEPTH:
					buildData.Attachment.Push(FramebufferAttachment(attachment.Texture->Handle.Id, attachment.Texture->Handle.Target, GL_DEPTH_ATTACHMENT, false));
					break;
				case FRAMEBUFFER_SUBATTACH_STENCIL:
					buildData.Attachment.Push(FramebufferAttachment(attachment.Texture->Handle.Id, attachment.Texture->Handle.Target, GL_DEPTH_STENCIL_ATTACHMENT, false));
					break;
				}
			}

			if (attachment.Type == FRAMEBUFFER_ATTACHMENT_RENDERBUFFER) {
				switch (attachment.SubType) {
				case FRAMEBUFFER_SUBATTACH_COLOUR:
					buildData.Attachment.Push(FramebufferAttachment(attachment.Renderbuffer->Handle.Id, attachment.Renderbuffer->Handle.Target, GL_COLOR_ATTACHMENT0 + rbCount++, false));
					break;
				case FRAMEBUFFER_SUBATTACH_DEPTH:
					buildData.Attachment.Push(FramebufferAttachment(attachment.Renderbuffer->Handle.Id, attachment.Renderbuffer->Handle.Target, GL_DEPTH_ATTACHMENT, false));
					break;
				case FRAMEBUFFER_SUBATTACH_STENCIL:
					buildData.Attachment.Push(FramebufferAttachment(attachment.Renderbuffer->Handle.Id, attachment.Renderbuffer->Handle.Target, GL_DEPTH_STENCIL_ATTACHMENT, false));
					break;
				}
			}
		}

		GetBuildQueue()->AddPostprocessForBuild(FramebufferSrc, buildData);
	}


}