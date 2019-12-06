#include "Framework/Abstraction/Model.h"


namespace Grvt
{

	GrvtMesh::GrvtMesh() :
		Vao(), Vbo(), Ebo(), Size(0), Positions(), Uv(),
		Normals(), Tangents(), Bitangents(), Indices() {}


	GrvtMesh::GrvtMesh(GrvtMesh&& Rhs)
	{
		*this = Gfl::Move(Rhs);
	}


	GrvtMesh& GrvtMesh::operator= (GrvtMesh&& Rhs)
	{
		_ASSERTE(this != &Rhs);

		if (this != &Rhs)
		{
			Vao = Gfl::Move(Rhs.Vao);
			Vbo = Gfl::Move(Rhs.Vbo);
			Ebo = Gfl::Move(Rhs.Ebo);

			Size = Rhs.Size;

			Positions = Rhs.Positions;
			Uv = Rhs.Uv;
			Normals = Rhs.Normals;
			Tangents = Rhs.Tangents;
			Bitangents = Rhs.Bitangents;
			Indices = Rhs.Indices;

			new (&Rhs) GrvtMesh();
		}

		return *this;
	}


	GrvtMesh::~GrvtMesh() 
	{
		Free();
	}


	void GrvtMesh::Free()
	{
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
	//void Mesh::UpdateInstanceBufferData(uint32 Amount, const glm::mat4 *MatrixArray) {
	//	glBindVertexArray(vao);
	//	glBindBuffer(GL_ARRAY_BUFFER, ibo);
	//	glBufferData(GL_ARRAY_BUFFER, Amount * sizeof(glm::mat4), MatrixArray, GL_STATIC_DRAW);
	//	glBindVertexArray(0);
	//}


	GrvtModel::GrvtModel() :
		Meshes() {}


	GrvtModel::~GrvtModel() {}


	GrvtModel* GrvtModel::Alloc(const ModelImportInfo& Info) 
	{
		return this;
	}


	void GrvtModel::Free() 
	{
		Meshes.Release();
	}

}