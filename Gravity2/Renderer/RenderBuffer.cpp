#include "stdafx.h"


RenderBuffer::RenderBuffer() :
	compProjView{},
	lights{},
	commands{},
	//alphaCommands{},
	//instancingCommands{},
	camera{} {}


RenderBuffer::RenderBuffer(const RenderBuffer &Other) { *this = Other; }


RenderBuffer::RenderBuffer(RenderBuffer &&Other) { *this = std::move(Other); }


RenderBuffer& RenderBuffer::operator= (const RenderBuffer &Other) {
	// Prevent the object from copying to itself in debug.
	_ASSERTE(this != &Other);

	// Runtime check.
	if (this != &Other) {
		commands			= Other.commands;
		//alphaCommands		= Other.alphaCommands;
		//instancingCommands	= Other.instancingCommands;
		camera				= Other.camera;
	}

	return *this;
}


RenderBuffer& RenderBuffer::operator= (RenderBuffer &&Other) {
	// Prevent the object from copying to itself in debug.
	_ASSERTE(this != &Other);

	// Runtime check.
	if (this != &Other) {
		Free();

		commands			= Other.commands;
		//alphaCommands		= Other.alphaCommands;
		//instancingCommands	= Other.instancingCommands;
		camera				= Other.camera;

		new (&Other) RenderBuffer();
	}

	return *this;
}


RenderBuffer::~RenderBuffer() {
	Free();
}


void RenderBuffer::Free() {
	camera = nullptr;
	commands.Empty();
	lights.Empty();
	//alphaCommands.Release();
	//instancingCommands.Release();
}