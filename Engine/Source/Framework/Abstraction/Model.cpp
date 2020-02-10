#include "GrvtPch.h"
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


	void CalculateTangentAndBitangent(ModelCreationInfo& Info)
	{
		Info.TangentData.Reserve(Info.PositionData.Length());
		Info.BitangentData.Reserve(Info.PositionData.Length());

		for (size_t i = 0; i < Info.IndexData.Length() - 2; i++)
		{
			size_t Index1 = Info.IndexData[i];
			size_t Index2 = Info.IndexData[i + 1];
			size_t Index3 = Info.IndexData[i + 2];

			glm::vec3& Pos0 = Info.PositionData[Index1];
			glm::vec3& Pos1 = Info.PositionData[Index2];
			glm::vec3& Pos2 = Info.PositionData[Index3];

			glm::vec2& Uv0 = Info.TexCoordData[Index1];
			glm::vec2& Uv1 = Info.TexCoordData[Index2];
			glm::vec2& Uv2 = Info.TexCoordData[Index3];


			glm::vec3 Edge1 = Pos1 - Pos0;
			glm::vec3 Edge2 = Pos2 - Pos0;

			glm::vec2 DeltaUV1 = Uv1 - Uv0;
			glm::vec2 DeltaUV2 = Uv2 - Uv0;

			float f = 1.0f / (DeltaUV1.x * DeltaUV2.y - DeltaUV2.x * DeltaUV1.y);

			glm::vec3 Tangent, Bitangent;

			Tangent.x = f * (DeltaUV2.y * Edge1.x - DeltaUV1.y * Edge2.x);
			Tangent.y = f * (DeltaUV2.y * Edge1.y - DeltaUV1.y * Edge2.y);
			Tangent.z = f * (DeltaUV2.y * Edge1.z - DeltaUV1.y * Edge2.z);

			if (i % 2 == 0)
			{
				Bitangent.x = f * (-DeltaUV2.x * Edge1.x - DeltaUV1.x * Edge2.x);
				Bitangent.y = f * (-DeltaUV2.x * Edge1.y - DeltaUV1.x * Edge2.y);
				Bitangent.z = f * (-DeltaUV2.x * Edge1.z - DeltaUV1.x * Edge2.z);
			}
			else
			{
				Bitangent.x = f * (-DeltaUV2.x * Edge2.x - DeltaUV1.x * Edge1.x);
				Bitangent.y = f * (-DeltaUV2.x * Edge2.y - DeltaUV1.x * Edge1.y);
				Bitangent.z = f * (-DeltaUV2.x * Edge2.z - DeltaUV1.x * Edge1.z);
			}

			Info.TangentData[Index1] += Tangent;
			Info.TangentData[Index2] += Tangent;
			Info.TangentData[Index3] += Tangent;
			Info.BitangentData[Index1] += Bitangent;
			Info.BitangentData[Index2] += Bitangent;
			Info.BitangentData[Index3] += Bitangent;
		}

		for (size_t i = 0; i < Info.TangentData.Length(); i++)
		{
			Info.TangentData[i] = glm::normalize(Info.TangentData[i]);
			Info.BitangentData[i] = glm::normalize(Info.BitangentData[i]);
		}
	}


	GrvtModel::GrvtModel() :
		Meshes() {}


	GrvtModel::~GrvtModel() {}


	GrvtModel* GrvtModel::Alloc(const ModelImportInfo& Info) 
	{
		return this;
	}


	GrvtModel* GrvtModel::Alloc(const ModelCreationInfo& Info)
	{
		GrvtMesh* Mesh = &Meshes.Insert(GrvtMesh());
		Mesh->Positions = Info.PositionData;
		Mesh->Uv		= Info.TexCoordData;
		Mesh->Normals	= Info.NormalsData;
		Mesh->Tangents	= Info.TangentData;
		Mesh->Bitangents = Info.BitangentData;

		return this;
	}


	void GrvtModel::Free() 
	{
		Meshes.Release();
	}

}