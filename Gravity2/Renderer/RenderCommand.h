#pragma once


/**
* RenderCommand data structure.
*
* RenderCommands are objects that are pushed into the renderer for every frame.
* Only contain low level information required for rendering.
*/
struct RenderCommand {
public:

	Shader				*shader;
	RenderState			renderSetting;
	Array<RenderNode>	nodes;
	Array<glm::mat4>	instances;
	glm::mat4			model;
	

	RenderCommand();
	RenderCommand(const RenderCommand &Other);
	RenderCommand(RenderCommand &&Other);

	RenderCommand& operator= (const RenderCommand &Other);
	RenderCommand& operator= (RenderCommand &&Other);

	~RenderCommand();
};