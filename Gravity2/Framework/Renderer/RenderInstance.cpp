#include "stdafx.h"


RenderInstance::RenderInstance() :
	compProjView{},
	lights{},
	commands{},
	//instancingCommands{},
	camera{} {}


RenderInstance::RenderInstance(const RenderInstance &Other) { *this = Other; }


RenderInstance::RenderInstance(RenderInstance &&Other) { *this = std::move(Other); }


RenderInstance& RenderInstance::operator= (const RenderInstance &Other) {
	// Prevent the object from copying to itself in debug.
	_ASSERTE(this != &Other);

	// Runtime check.
	if (this != &Other) {
		commands			= Other.commands;
		//instancingCommands	= Other.instancingCommands;
		camera				= Other.camera;
	}

	return *this;
}


RenderInstance& RenderInstance::operator= (RenderInstance &&Other) {
	// Prevent the object from copying to itself in debug.
	_ASSERTE(this != &Other);

	// Runtime check.
	if (this != &Other) {
		Free();

		commands			= Other.commands;
		//instancingCommands	= Other.instancingCommands;
		camera				= Other.camera;

		new (&Other) RenderInstance();
	}

	return *this;
}


RenderInstance::~RenderInstance() {
	Free();
}


void RenderInstance::Free() {
	camera = nullptr;
	commands.Empty();
	lights.Empty();
	//instancingCommands.Release();
}