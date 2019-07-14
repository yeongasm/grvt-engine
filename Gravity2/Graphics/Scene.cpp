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


Mesh::Mesh() : vao{}, vbo{}, ebo{}, size{} {}


Mesh::Mesh(Mesh &&Other) { *this = std::move(Other); }


Mesh& Mesh::operator= (Mesh &&Other) {
	_ASSERTE(this != &Other);

	Free();

	vao = Other.vao;
	vbo = Other.vbo;
	ebo = Other.ebo;

	positions	= Other.positions;
	uv			= Other.uv;
	normals		= Other.normals;
	tangents	= Other.tangents;
	bitangents	= Other.bitangents;

	Other.Free();

	return *this;
}


Mesh::~Mesh() {
	Free();
}


void Mesh::Alloc() {
	if (!vao) {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		if (indices.Length())
			glGenBuffers(1, &ebo);
	}

	// NOTE(Afiq): Gravity handles mesh data by always interleaving data between one another.
	Array<float> data;

	data.Reserve(positions.Length() + normals.Length() + uv.Length() + tangents.Length() + bitangents.Length());

	for (uint i = 0; i < positions.Length(); i++) {
		data.Push(positions[i].x);
		data.Push(positions[i].y);
		data.Push(positions[i].z);

		if (normals.Length()) {
			data.Push(normals[i].x);
			data.Push(normals[i].y);
			data.Push(normals[i].z);
		}

		if (uv.Length()) {
			data.Push(uv[i].x);
			data.Push(uv[i].y);
		}

		// Bitangents can only exist when tangents exist. Hence it only made sense to group them up together.
		if (tangents.Length()) {
			data.Push(tangents[i].x);
			data.Push(tangents[i].y);
			data.Push(tangents[i].z);

			data.Push(bitangents[i].x);
			data.Push(bitangents[i].y);
			data.Push(bitangents[i].z);
		}
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.Length() * sizeof(float), &data[0], GL_STATIC_DRAW);

	if (indices.Length()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.Length() * sizeof(uint), &indices[0], GL_STATIC_DRAW);
	}

	int stride							= 3 * sizeof(float);
	if (normals.Length())		stride += 3 * sizeof(float);
	if (uv.Length())			stride += 2 * sizeof(float);
	if (tangents.Length())		stride += 3 * sizeof(float);
	if (bitangents.Length())	stride += 3 * sizeof(float);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(0));
	
	if (normals.Length()) {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(3 * sizeof(float)));
	}

	if (uv.Length()) {
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(6 * sizeof(float)));
	}

	if (tangents.Length()) {
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(8 * sizeof(float)));
	}

	if (bitangents.Length()) {
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(11 * sizeof(float)));
	}

	// NOTE(Afiq):
	// VAO's store the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
	// This also means VAO's store it's unbind call so make sure we don't unbind the element array buffer
	// before unbinding  the VAO. Otherwise it doesn't have an EBO configured.
	glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	size = (indices.Length()) ? (uint)indices.Length() : (uint)data.Length();
}


void Mesh::Free() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	vao = vbo = ebo = 0;

	positions.Release();
	uv.Release();
	normals.Release();
	tangents.Release();
	bitangents.Release();
	indices.Release();
}


void Mesh::CalculateTangentAndBitangent() {
	// TODO(Afiq):
	// Once we have a basic renderer set up, research on methods to make solve calculating tangent and bitangents for mirrored uv coordinates.
}


void Mesh::CalculateNormals() {
	// TODO(Afiq):
	// Find the optimal way to calculate vertex normals of a mesh.
}


Scene::Scene() : instanced(false), type(SCENE_TYPE_NONE), meshes{}, 
	models{}, instances{}, info{} {}


Scene::Scene(Scene &&Other) { *this = std::move(Other); }


Scene& Scene::operator= (Scene &&Other) {
	_ASSERTE(this != &Other);
	
	Free();

	instanced	= Other.instanced;
	meshes		= std::move(Other.meshes);
	models		= Other.models;
	instances	= Other.instances;
	info		= Other.info;

	Other.Free();

	return *this;
}


Scene::~Scene() {
	Free();
}

void Scene::Free() {
	type		= SCENE_TYPE_NONE;
	instanced	= false;
	info		= nullptr;

	instances.Release();
	meshes.Release();
	models.Release();
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


SceneInstance* Scene::CreateInstance(const SceneInstanceCreation &Info) {
	if (!Info.shader)
		return nullptr;

	size_t idx = instances.Push(SceneInstance());
	SceneInstance *instance = &instances[idx];

	instance->id = ((info->id * 1000) + (uint)idx);
	instance->Alloc(Info, this);

	return instance;
}