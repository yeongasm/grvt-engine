#pragma once


/**
* RenderNode data structure.
*
* A RenderNode is simply a fancy name for a mesh that will be rendered onto the screen.
* This was done simply to get required information from a scene.
*
* The reason why we didn't use a MeshNode object and manually specified the Material and Mesh 
  object was because the MeshNode object was designed to not give users access to those data.
*/
struct RenderNode {
	uint		size;
	size_t		amount;
	Material	*material;
	Mesh		*mesh;

	RenderNode();

	RenderNode(const RenderNode &Other);
	RenderNode(RenderNode &&Other);

	RenderNode& operator= (const RenderNode &Other);
	RenderNode& operator= (RenderNode &&Other);

	~RenderNode();
};