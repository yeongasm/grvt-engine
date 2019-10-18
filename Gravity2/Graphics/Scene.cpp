#include "stdafx.h"


SceneCreationInfo::SceneCreationInfo() :
	Name(), Path(), FlipUVs() {}


SceneCreationInfo::~SceneCreationInfo() {
	Name.Release();
	Path.Release();
}


MeshObj::MeshObj() : 
	Vao(), Vbo(), Ebo(), Size(0), Positions(), Uv(), 
	Normals(), Tangents(), Bitangents(), Indices() {}


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

		Other.~MeshObj();
	}

	return *this;
}


MeshObj::~MeshObj() {
	Size = 0;
	Positions.Release();
	Uv.Release();
	Normals.Release();
	Tangents.Release();
	Bitangents.Release();
	Indices.Release();
}


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


SceneObj::SceneObj() : Meshes(), Info(nullptr) {}


SceneObj::~SceneObj() {
	Info = nullptr;
	Meshes.Release();
}