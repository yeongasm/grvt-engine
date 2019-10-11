#include "stdafx.h"


ObjHandle::ObjHandle() : Id(0), Target(0) {}


ObjHandle::~ObjHandle() {}


ObjHandle::ObjHandle(ObjHandle &&Rhs) { *this = std::move(Rhs); }


ObjHandle& ObjHandle::operator= (ObjHandle &&Rhs) {
	_ASSERTE(this != &Rhs);

	if (this != &Rhs) {
		Id		= Rhs.Id;
		Target	= Rhs.Target;

		new (&Rhs) ObjHandle();
	}

	return *this;
}


void BaseAPI::GrCreateVertexArray(ObjHandle &Handle) {
	glGenVertexArrays(1, &Handle.Id);
}


bool BaseAPI::GrBindVertexArray(ObjHandle &Handle) {
	if (!Handle.Id)
		return false;

	glBindVertexArray(Handle.Id);

	return true;
}


void BaseAPI::GrUnbindVertexArray() {
	glBindVertexArray(0);
}


void BaseAPI::GrDeleteVertexArray(ObjHandle &Handle) {
	glDeleteVertexArrays(1, &Handle.Id);
}


void BaseAPI::GrCreateBufferObject(ObjHandle &Handle, uint32 Target) {
	Handle.Target = Target;
	glGenBuffers(1, &Handle.Id);
}


bool BaseAPI::GrBindBufferObject(ObjHandle &Handle) {
	if (!Handle.Id || !Handle.Target)
		return false;

	glBindBuffer(Handle.Target, Handle.Id);

	return true;
}


void BaseAPI::GrUnbindBufferObject(ObjHandle &Handle) {
	glBindBuffer(Handle.Target, 0);
}


void BaseAPI::GrDeleteBufferObject(ObjHandle &Handle) {
	glDeleteBuffers(1, &Handle.Id);
}


void BaseAPI::GrCreateTexture(ObjHandle &Handle, uint32 Target) {
	Handle.Target = Target;
	glGenTextures(1, &Handle.Id);
}


bool BaseAPI::GrBindTexture(ObjHandle &Handle) {
	if (!Handle.Id || !Handle.Target)
		return false;

	glBindTexture(Handle.Target, Handle.Id);

	return true;
}


void BaseAPI::GrUnbindTexture(ObjHandle &Handle) {
	glBindTexture(Handle.Target, 0);
}


void BaseAPI::GrDeleteTexture(ObjHandle &Handle) {
	glDeleteTextures(1, &Handle.Id);
}


void BaseAPI::GrCreateFramebuffer(ObjHandle &Handle) {
	Handle.Target = GL_FRAMEBUFFER;
	glGenFramebuffers(1, &Handle.Id);
}


bool BaseAPI::GrBindFramebuffer(ObjHandle &Handle) {
	if (!Handle.Id || !Handle.Target)
		return false;

	glBindFramebuffer(Handle.Target, Handle.Id);

	return true;
}


void BaseAPI::GrUnbindFramebuffer(ObjHandle &Handle) {
	glBindFramebuffer(Handle.Target, Handle.Id);
}


void BaseAPI::GrDeleteFramebuffer(ObjHandle &Handle) {
	glDeleteFramebuffers(1, &Handle.Id);
}


void BaseAPI::GrCreateRenderbuffer(ObjHandle &Handle) {
	Handle.Target = GL_RENDERBUFFER;
	glGenRenderbuffers(1, &Handle.Id);
}


bool BaseAPI::GrBindRenderbuffer(ObjHandle &Handle) {
	if (!Handle.Id || !Handle.Target)
		return false;

	glBindRenderbuffer(Handle.Target, Handle.Id);

	return true;
}


void BaseAPI::GrUnbindRenderbuffer(ObjHandle &Handle) {
	glBindRenderbuffer(Handle.Target, 0);
}


void BaseAPI::GrDeleteRenderbuffer(ObjHandle &Handle) {
	glDeleteRenderbuffers(1, &Handle.Id);
}