#include "stdafx.h"


BufferObject::BufferObject() : Id(0), Target(EMPTY_BUFFER_OBJECT) {}


BufferObject::~BufferObject() { Release(); }


BufferObject::BufferObject(BufferObject &&Rhs) { *this = std::move(Rhs); }


BufferObject& BufferObject::operator= (BufferObject &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Release();

		Id		= Rhs.Id;
		Target	= Rhs.Target;

		Rhs.Release();
	}

	return *this;
}


BufferObject::operator uint32() { return Id; }


bool BufferObject::Alloc() {
	if (Id)
		return false;

	glGenBuffers(1, &Id);

	return true;
}


bool BufferObject::Release() {
	if (!Id)
		return false;

	glDeleteBuffers(1, &Id);

	return true;
}


void BufferObject::Bind() {
	glBindBuffer(Target, Id);
}


void BufferObject::UnBind() {
	glBindBuffer(Target, 0);
}