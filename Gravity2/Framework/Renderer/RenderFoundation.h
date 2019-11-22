#pragma once


/**
* GrvtRenderNode data structure.
*
* A render node is the most basic unit required to render an object onto the screen.
* 
* @param Material	- Material to be rendered in this node.
* @param Mesh		- Mesh to be rendered in this node.
* @param Amount		- Amount of nodes to be rendered. Only works for instanced rendering.
* @param Mode		- The mode that this node will be rendered with. E.g: GL_TRIANGLES, GL_LINES, etc. 
*/
struct GrvtRenderNode {

	GrvtMaterial*	Material;
	GrvtMesh*		Mesh;
	size_t			Amount;
	uint32			Mode;

	GrvtRenderNode();

	GrvtRenderNode(const GrvtRenderNode& Other);
	GrvtRenderNode& operator= (const GrvtRenderNode& Other);

	GrvtRenderNode(GrvtRenderNode&& Other);
	GrvtRenderNode& operator= (GrvtRenderNode&& Other);

	~GrvtRenderNode();

};


/**
* GrvtRenderCommand data structure.
*
* A render command represent a "model" that will be rendered onto the screen.
*
* @param Nodes		- An array of GrvtRenderNode.
* @param Instances	- An array of 4x4 matrices that represents the model matrix of each instance.
* @param Model		- For render commands that are not instanced, this would represent the object's model matrix.
*/
struct GrvtRenderCommand {

	Array<GrvtRenderNode>	Nodes;
	Array<glm::mat4>		Instances;
	glm::mat4				Model;

	GrvtRenderCommand();

	GrvtRenderCommand(const GrvtRenderCommand& Other);
	GrvtRenderCommand& operator= (const GrvtRenderCommand& Other);

	GrvtRenderCommand(GrvtRenderCommand&& Other);
	GrvtRenderCommand& operator= (GrvtRenderCommand&& Other);

	~GrvtRenderCommand();

};


/**
* GrvtRenderTarget data structure.
*/
struct GrvtRenderTarget {



};


/**
* GrvtRenderBuffer data structure.
* 
*/
struct GrvtCommandBuffer {

	glm::mat4					CompositeMatrix;
	Array<glm::mat4>			Lights;
	Array<GrvtRenderCommand>	RenderCommands;
	Array<GrvtRenderCommand>	InstanceCommands;

	GrvtCommandBuffer();

	GrvtCommandBuffer(const GrvtCommandBuffer& Other);
	GrvtCommandBuffer& operator= (const GrvtCommandBuffer& Other);

	GrvtCommandBuffer(GrvtCommandBuffer&& Other);
	GrvtCommandBuffer& operator= (GrvtCommandBuffer&& Other);

	~GrvtCommandBuffer();

};