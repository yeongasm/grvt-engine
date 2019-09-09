#include "stdafx.h"


RenderCommand::RenderCommand() : shader{}, renderSetting{},
nodes{}, model(1.0f) {}


RenderCommand::RenderCommand(const RenderCommand &Other) { *this = Other; }


RenderCommand::RenderCommand(RenderCommand &&Other) { *this = std::move(Other); }


RenderCommand& RenderCommand::operator= (const RenderCommand &Other) {
	if (this != &Other) {
		shader = Other.shader;
		renderSetting = Other.renderSetting;
		nodes = Other.nodes;
		model = Other.model;
	}

	return *this;
}


RenderCommand& RenderCommand::operator= (RenderCommand &&Other) {
	if (this != &Other) {
		shader = Other.shader;
		renderSetting = Other.renderSetting;
		nodes = Other.nodes;
		model = Other.model;

		Other.~RenderCommand();
	}


	return *this;
}


RenderCommand::~RenderCommand() {
	nodes.Release();
}