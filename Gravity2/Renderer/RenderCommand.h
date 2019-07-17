#pragma once


/**
* RenderCommand data structure.
*
* RenderCommands are objects that are pushed into the renderer for every frame.
* Scenes that are pushed into the renderer are stored as a RenderCommand object.
* Contains all necessary details for rendering.
*/
struct RenderCommand {
private:

	using Node = RenderNode;

public:

	Scene				*scene;
	Shader				*shader;
	RenderState			renderSetting;
	Array<Node>			nodes;
	Array<glm::mat4>	instances;
	glm::mat4			model;
	

	RenderCommand();
	RenderCommand(const RenderCommand &Other);
	RenderCommand(RenderCommand &&Other);

	RenderCommand& operator= (const RenderCommand &Other);
	RenderCommand& operator= (RenderCommand &&Other);

	~RenderCommand();
};