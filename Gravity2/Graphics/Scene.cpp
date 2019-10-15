#include "stdafx.h"


SceneCreationInfo::SceneCreationInfo() : Name(), Path(), FlipUVs() {}


SceneCreationInfo::SceneCreationInfo(const SceneCreationInfo &Other) { *this = Other; }


SceneCreationInfo::SceneCreationInfo(SceneCreationInfo &&Other) { *this = std::move(Other); }


SceneCreationInfo& SceneCreationInfo::operator= (const SceneCreationInfo &Other) {
	Name		= Other.Name;
	Path		= Other.Path;
	FlipUVs		= Other.FlipUVs;

	return *this;
}


SceneCreationInfo& SceneCreationInfo::operator= (SceneCreationInfo &&Other) {
	if (this != &Other) {
		Name	= Other.Name;
		Path	= Other.Path;
		FlipUVs = Other.FlipUVs;

		Other.~SceneCreationInfo();
	}

	return *this;
}


SceneCreationInfo::~SceneCreationInfo() {
	Name.Release();
	Path.Release();
}


MeshObj::MeshObj() : Vao(), Vbo(), Ebo(), Size(0) {}


MeshObj::MeshObj(MeshObj &&Other) { *this = std::move(Other); }


MeshObj& MeshObj::operator= (MeshObj &&Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Vao = std::move(Other.Vao);
		Vbo = std::move(Other.Vbo);
		Ebo = std::move(Other.Ebo);

		Size = Other.Size;

		Positions	= Other.Positions;
		Uv			= Other.Uv;
		Normals		= Other.Normals;
		Tangents	= Other.Tangents;
		Bitangents	= Other.Bitangents;
		Indices		= Other.Indices;

		Other.Free();
	}

	return *this;
}


MeshObj::~MeshObj() {}


void MeshObj::Free() {
	Size = 0;
	Positions.Release();
	Uv.Release();
	Normals.Release();
	Tangents.Release();
	Bitangents.Release();
	Indices.Release();
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


SceneObj::SceneObj() : Meshes(), Instances(), Info(nullptr) {}


SceneObj::~SceneObj() {}

void SceneObj::Free() {
	Info = nullptr;

	Instances.Release();
	Meshes.Release();
}


SceneInstance* SceneObj::CreateInstance(const SceneInstanceCreation &Info) {
	SceneInstance *instance = &Instances.Insert(SceneInstance());
	instance->Alloc(Info, this);

	return instance;
}


SceneInstance* SceneObj::CreateInstanceFrom(const SceneInstance &Instance) {
	return &Instances.Insert(Instance);
}


bool SceneObj::RemoveInstance(SceneInstance *Instance, bool Move) {
	size_t idx = -1;

	for (SceneInstance &instance : Instances) {
		if (Instance != &instance)
			continue;

		idx = Instances.IndexOf(instance);
		break;
	}

	Instances.PopAt(idx, Move);

	return (idx == -1) ? false : true;
}


void SceneObj::ReleaseAllInstances() {
	Instances.Empty();
}