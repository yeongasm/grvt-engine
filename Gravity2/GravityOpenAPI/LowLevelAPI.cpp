#include "stdafx.h"


BufferObject::BufferObject() : Id(0), Target(0) {}


BufferObject::~BufferObject() { Delete(); }


BufferObject::BufferObject(BufferObject &&Rhs) { *this = std::move(Rhs); }


BufferObject& BufferObject::operator= (BufferObject &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Reset();

		Id		= Rhs.Id;
		Target	= Rhs.Target;

		Rhs.Reset();
	}

	return *this;
}


BufferObject::operator uint32() { return Id; }


bool BufferObject::Alloc(uint32 Target) {
	if (Id || !Target)
		return false;

	this->Target = Target;
	glGenBuffers(1, &Id);

	return true;
}


bool BufferObject::Delete() {
	if (!Id)
		return false;

	glDeleteBuffers(1, &Id);
	new (this) BufferObject();

	return true;
}


bool BufferObject::Bind() {
	if (!Target)
		return false;

	glBindBuffer(Target, Id);

	return true;
}


bool BufferObject::UnBind() {
	if (!Target)
		return false;

	glBindBuffer(Target, 0);

	return true;
}


void BufferObject::Reset() {
	Id = Target = 0;
}


VertArrayObj::VertArrayObj() : Id(0) {}


VertArrayObj::~VertArrayObj() { Delete(); }


VertArrayObj::VertArrayObj(VertArrayObj &&Rhs) { *this = std::move(Rhs); }


VertArrayObj& VertArrayObj::operator= (VertArrayObj &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Reset();

		Id = Rhs.Id;

		Rhs.Reset();
	}

	return *this;
}


VertArrayObj::operator uint32() { return Id; }


bool VertArrayObj::Alloc() {
	if (Id)
		return false;

	glGenVertexArrays(1, &Id);

	return true;
}


bool VertArrayObj::Delete() {
	if (!Id)
		return false;

	glDeleteVertexArrays(1, &Id);
	new (this) VertArrayObj();

	return true;
}


bool VertArrayObj::Bind() {
	if (!Id)
		return false;

	glBindVertexArray(Id);

	return true;
}


void VertArrayObj::UnBind() {
	glBindVertexArray(0);
}


void VertArrayObj::Reset() {
	Id = 0;
}


VertexAttribPointer::VertexAttribPointer() : Index(0), Size(0), Stride(0), Pointer(0) {}


VertexAttribPointer::VertexAttribPointer(uint32 Index, int32 Size, int32 Stride, size_t Pointer) :
	Index(Index), Size(Size), Stride(Stride), Pointer(Pointer) {}


VertexAttribPointer::VertexAttribPointer(const VertexAttribPointer &Rhs) { *this = Rhs; }


VertexAttribPointer& VertexAttribPointer::operator= (const VertexAttribPointer &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Index	= Rhs.Index;
		Size	= Rhs.Size;
		Stride	= Rhs.Stride;
		Pointer	= Rhs.Pointer;
	}

	return *this;
}


VertexAttribPointer::VertexAttribPointer(VertexAttribPointer &&Rhs) { *this = std::move(Rhs); }


VertexAttribPointer& VertexAttribPointer::operator= (VertexAttribPointer &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		new (this) VertexAttribPointer();

		Index	= Rhs.Index;
		Size	= Rhs.Size;
		Stride	= Rhs.Stride;
		Pointer = Rhs.Pointer;

		new (&Rhs) VertexAttribPointer();
	}

	return *this;
}


VertexAttribPointer::~VertexAttribPointer() { new (this) VertexAttribPointer(); }


MeshBuildData::MeshBuildData() : Data(nullptr), Indices(nullptr), Size(0), Length(0), VertexAttribPointers() {}


MeshBuildData::MeshBuildData(const MeshBuildData &Rhs) { *this = Rhs; }


MeshBuildData::MeshBuildData(MeshBuildData &&Rhs) { *this = std::move(Rhs); }


MeshBuildData& MeshBuildData::operator= (const MeshBuildData &Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Data	= Rhs.Data;
		Indices = Rhs.Indices;
		Size	= Rhs.Size;
		Length	= Rhs.Length;
		VertexAttribPointers = Rhs.VertexAttribPointers;
	}

	return *this;
}


MeshBuildData& MeshBuildData::operator= (MeshBuildData &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		new (this) MeshBuildData();

		Data	= Rhs.Data;
		Indices = Rhs.Indices;
		Size	= Rhs.Size;
		Length	= Rhs.Length;
		VertexAttribPointers = Rhs.VertexAttribPointers;

		new (&Rhs) MeshBuildData();
	}

	return *this;
}


MeshBuildData::~MeshBuildData() { Data = nullptr, Indices = nullptr; Size = 0; Length = 0; VertexAttribPointers.Release(); }


TextureID::TextureID() : Id(0), Target(0) {}


TextureID::~TextureID() { Delete(); }


TextureID::TextureID(TextureID &&Rhs) { *this = std::move(Rhs); }


TextureID& TextureID::operator= (TextureID &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Reset();

		Id		= Rhs.Id;
		Target	= Rhs.Target;

		Rhs.Reset();
	}

	return *this;
}


TextureID::operator uint32() { return Id; }


bool TextureID::Alloc(uint32 Target) {
	if (Id || !Target)
		return false;

	this->Target = Target;
	glGenTextures(1, &Id);

	return true;
}


bool TextureID::Delete() {
	if (!Id)
		return false;

	glDeleteTextures(1, &Id);
	new (this) TextureID();

	return true;
}


bool TextureID::Bind() {
	if (!Target)
		return false;

	glBindTexture(Target, Id);

	return true;
}


bool TextureID::UnBind() {
	if (!Target)
		return false;

	glBindTexture(Target, 0);

	return true;
}


void TextureID::Reset() {
	Id = Target = 0;
}


//TextureBuildData::TextureBuildData() : DataPtr(nullptr), TextureInfo() {}


void OpenWrapBuildMesh(VertArrayObj &VAO, BufferObject &VBO, BufferObject &EBO, MeshBuildData &Data) {
	VAO.Alloc();
	VBO.Alloc(GL_ARRAY_BUFFER);

	if (Data.Length)
		EBO.Alloc(GL_ELEMENT_ARRAY_BUFFER);

	VAO.Bind();
	VBO.Bind();
	glBufferData(VBO.Target, Data.Size * sizeof(float), Data.Data, GL_STATIC_DRAW);

	if (Data.Length) {
		EBO.Bind();
		glBufferData(EBO.Target, Data.Length * sizeof(uint32), Data.Indices, GL_STATIC_DRAW);
	}

	for (VertexAttribPointer &Ptr : Data.VertexAttribPointers) {
		glEnableVertexAttribArray(Ptr.Index);
		glVertexAttribPointer(Ptr.Index, Ptr.Size, GL_FLOAT, GL_FALSE, Ptr.Stride, (GLvoid*)(Ptr.Pointer));
	}

	// NOTE(Afiq): Should we make the mesh only hold a VAO and not the VBO and EBO since they're not really required.
	VAO.UnBind();
}


void OpenWrapDeleteMesh(VertArrayObj &VAO, BufferObject &VBO, BufferObject &EBO) {
	VBO.Delete();
	EBO.Delete();
	VAO.Delete();
}