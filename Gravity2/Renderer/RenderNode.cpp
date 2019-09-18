#include "stdafx.h"


RenderNode::RenderNode() :
	size{},
	amount{},
	material{},
	mesh{} {}


RenderNode::RenderNode(const RenderNode &Other) { *this = Other; }


RenderNode::RenderNode(RenderNode &&Other) { *this = std::move(Other); }


RenderNode& RenderNode::operator= (const RenderNode &Other) {
	// Prevent it from copying to itself in debug.
	_ASSERTE(this != &Other);

	// Prevention check in runtime.
	if (this != &Other) {
		size		= Other.size;
		mode		= Other.mode;
		amount		= Other.amount;
		material	= Other.material;
		mesh		= Other.mesh;
	}

	return *this;
}


RenderNode& RenderNode::operator= (RenderNode &&Other) {
	// Prevent it from executing move semantics to itself in debug.
	_ASSERTE(this != &Other);

	// Prevention check in runtime.
	if (this != &Other) {
		size		= Other.size;
		mode		= Other.mode;
		amount		= Other.amount;
		material	= Other.material;
		mesh		= Other.mesh;

		new (&Other) RenderNode();
	}

	return *this;
}


RenderNode::~RenderNode() {
	size		= 0;
	mode		= 0;
	amount		= 0;
	material	= nullptr;
	mesh		= nullptr;
}