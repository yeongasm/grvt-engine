#include "stdafx.h"


SceneCreationInfo::SceneCreationInfo() : flipUVs(0), type{}, file{}, 
	directory{}, name{} {}


SceneCreationInfo::SceneCreationInfo(const SceneCreationInfo &Other) { *this = Other; }


SceneCreationInfo::SceneCreationInfo(SceneCreationInfo &&Other) { *this = std::move(Other); }


SceneCreationInfo& SceneCreationInfo::operator= (const SceneCreationInfo &Other) {
	type		= Other.type;
	file		= Other.file;
	directory	= Other.directory;
	name		= Other.name;

	return *this;
}


SceneCreationInfo& SceneCreationInfo::operator= (SceneCreationInfo &&Other) {
	if (this != &Other) {

		type		= Other.type;
		file		= Other.file;
		directory	= Other.directory;
		name		= Other.name;
	}

	Other.~SceneCreationInfo();

	return *this;
}


SceneCreationInfo::~SceneCreationInfo() {
	type = SCENE_TYPE_NONE;
	file.Release();
	directory.Release();
	name.Release();
}


Mesh::Mesh() : vao(), vbo(), ebo(), size(0) {}


Mesh::Mesh(Mesh &&Other) { *this = std::move(Other); }


Mesh& Mesh::operator= (Mesh &&Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		vao = std::move(Other.vao);
		vbo = std::move(Other.vbo);
		ebo = std::move(Other.ebo);

		size = Other.size;

		positions	= Other.positions;
		uv			= Other.uv;
		normals		= Other.normals;
		tangents	= Other.tangents;
		bitangents	= Other.bitangents;
		indices		= Other.indices;

		Other.Free();
	}

	return *this;
}


Mesh::~Mesh() {
	Free();
}


void Mesh::Free() {
	size = 0;
	positions.Release();
	uv.Release();
	normals.Release();
	tangents.Release();
	bitangents.Release();
	indices.Release();
}


//void Mesh::CalculateTangentAndBitangent() {
//	// TODO(Afiq):
//	// Once we have a basic renderer set up, research on methods to make solve calculating tangent and bitangents for mirrored uv coordinates.
//}


//void Mesh::CalculateNormals() {
//	// TODO(Afiq):
//	// Find the optimal way to calculate vertex normals of a mesh.
//}


//void Mesh::CreateInstanceBuffer() {
//	// Just to make sure no one tries to remake an existing ibo;
//	if (ibo)
//		return;
//
//	glGenBuffers(1, &ibo);
//	glBindVertexArray(vao);
//	glBindBuffer(GL_ARRAY_BUFFER, ibo);
//
//	glEnableVertexAttribArray(5);
//	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
//	glEnableVertexAttribArray(6);
//	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(sizeof(glm::vec4)));
//	glEnableVertexAttribArray(7);
//	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
//	glEnableVertexAttribArray(8);
//	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
//
//	glVertexAttribDivisor(5, 1);
//	glVertexAttribDivisor(6, 1);
//	glVertexAttribDivisor(7, 1);
//	glVertexAttribDivisor(8, 1);
//
//	glBindVertexArray(0);
//}
//
//
//void Mesh::UpdateInstanceBufferData(uint Amount, const glm::mat4 *MatrixArray) {
//	glBindVertexArray(vao);
//	glBindBuffer(GL_ARRAY_BUFFER, ibo);
//	glBufferData(GL_ARRAY_BUFFER, Amount * sizeof(glm::mat4), MatrixArray, GL_STATIC_DRAW);
//	glBindVertexArray(0);
//}


Scene::Scene() : type(SCENE_TYPE_NONE), meshes(), instances(), info(nullptr) {}


Scene::~Scene() {
	Free();
}

void Scene::Free() {
	type		= SCENE_TYPE_NONE;
	info		= nullptr;

	instances.Release();
	meshes.Release();
}


SceneInstance* Scene::CreateInstance(const SceneInstanceCreation &Info) { 
	SceneInstance *instance = &instances.Insert(SceneInstance());
	instance->Alloc(Info, this);

	return instance;
}


SceneInstance* Scene::CreateInstanceFrom(const SceneInstance &Instance) {
	return &instances.Insert(Instance);
}


bool Scene::RemoveInstance(SceneInstance *Instance, bool Move) {
	size_t idx = -1;

	for (SceneInstance &instance : instances) {
		if (Instance != &instance)
			continue;

		idx = instances.IndexOf(instance);
		break;
	}

	instances.PopAt(idx, Move);

	return (idx == -1) ? false : true;
}


void Scene::ReleaseAllInstances() {
	instances.Empty();
}