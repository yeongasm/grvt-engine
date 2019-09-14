#include "stdafx.h"


MeshNode::MeshNode() : material{}, mesh{} {}


MeshNode::MeshNode(Material *Material, Mesh *Mesh) : material(Material), mesh(Mesh) {}


MeshNode::MeshNode(const MeshNode &Other) { *this = Other; }


MeshNode::MeshNode(MeshNode &&Other) { *this = std::move(Other); }


MeshNode& MeshNode::operator= (const MeshNode &Other) {
	if (this != &Other) {
		material	= Other.material;
		mesh		= Other.mesh;
	}

	return *this;
}


MeshNode& MeshNode::operator= (MeshNode &&Other) {
	if (this != &Other) {
		material	= Other.material;
		mesh		= Other.mesh;

		Other.Free();
	}

	return *this;
}


MeshNode::~MeshNode() { Free(); }


void MeshNode::PushMaterial(Material *Material) {
	material = Material;
	Material->info->references.Push(&material);
}


void MeshNode::PushMesh(Mesh *Mesh) {
	mesh = Mesh;
}


void MeshNode::Alloc(Material *Material, Mesh *Mesh) {
	material	= Material;
	mesh		= Mesh;
}


void MeshNode::Free() {
	mesh		= nullptr;
	material	= nullptr;
}


SceneInstanceCreation::SceneInstanceCreation() : position(0.0f), scale(1.0f), rotation(0.0f), shader{} {}


SceneInstanceCreation::SceneInstanceCreation(const SceneInstanceCreation &Other) { *this = Other; }


SceneInstanceCreation::SceneInstanceCreation(SceneInstanceCreation &&Other) { *this = std::move(Other); }


SceneInstanceCreation& SceneInstanceCreation::operator= (const SceneInstanceCreation &Other) {
	if (this != &Other) {
		position	= Other.position;
		scale		= Other.scale;
		rotation	= Other.rotation;
		shader		= Other.shader;
	}

	return *this;
}


SceneInstanceCreation& SceneInstanceCreation::operator= (SceneInstanceCreation &&Other) {
	if (this != &Other) {
		position	= Other.position;
		scale		= Other.scale;
		rotation	= Other.rotation;
		shader		= Other.shader;

		Other.~SceneInstanceCreation();
	}

	return *this;
}


SceneInstanceCreation::~SceneInstanceCreation() {
	shader = nullptr;
}


RenderState::RenderState() : depthTesting(false), blending(false), cullFace(true), depthFunc(0), 
	blendSrc(0), blendDst(0), frontFace(0), polygonMode(0) {}


RenderState::RenderState(const RenderState &Other) { *this = Other; }


RenderState::RenderState(RenderState &&Other) { *this = std::move(Other); }


RenderState& RenderState::operator= (const RenderState &Other) {
	depthTesting	= Other.depthTesting;
	blending		= Other.blending;
	cullFace		= Other.cullFace;
	depthFunc		= Other.depthFunc;
	blendSrc		= Other.blendSrc;
	blendDst		= Other.blendDst;
	frontFace		= Other.frontFace;
	polygonMode		= Other.polygonMode;

	return *this;
}


RenderState& RenderState::operator= (RenderState &&Other) {
	if (this != &Other) {
		depthTesting	= Other.depthTesting;
		blending		= Other.blending;
		cullFace		= Other.cullFace;
		depthFunc		= Other.depthFunc;
		blendSrc		= Other.blendSrc;
		blendDst		= Other.blendDst;
		frontFace		= Other.frontFace;
		polygonMode		= Other.polygonMode;

		Other.~RenderState();
	}


	return *this;
}


void RenderState::DefaultModelRenderState() {
	depthTesting	= true;
	cullFace		= true;
	depthFunc		= GL_LESS;
	frontFace		= GL_CCW;
	polygonMode		= GL_FILL;
}


RenderState::~RenderState() {}


SceneInstance::SceneInstance() : position(0.0f), scale(1.0f), rotation(0.0f),
	scene{}, /*shader{},*/ nodes{}, instanced{}, id{}, renderState{} {}


SceneInstance::SceneInstance(const SceneInstanceCreation &Info, Scene *Scene) { Alloc(Info, Scene); }


SceneInstance::SceneInstance(const SceneInstance &Other) { *this = Other; }


SceneInstance::SceneInstance(SceneInstance &&Other) { *this = std::move(Other); }


SceneInstance& SceneInstance::operator= (const SceneInstance &Other) {
	if (this != &Other) {
		position	= Other.position;
		scale		= Other.scale;
		rotation	= Other.rotation;
		scene		= Other.scene;
		//shader		= Other.shader;
		nodes		= Other.nodes;
		instanced	= Other.instanced;
		id			= Other.id;
		renderState = Other.renderState;
	}

	return *this;
}


SceneInstance& SceneInstance::operator= (SceneInstance &&Other) {
	if (this != &Other) {
		position	= Other.position;
		scale		= Other.scale;
		rotation	= Other.rotation;
		scene		= Other.scene;
		//shader		= Other.shader;
		nodes		= Other.nodes;
		instanced	= Other.instanced;
		id			= Other.id;
		renderState = Other.renderState;

		Other.Free();
	}

	return *this;
}


SceneInstance::~SceneInstance() { Free(); }


bool SceneInstance::Alloc(const SceneInstanceCreation &Info, Scene *Scene) {
	scene		= Scene;
	//shader		= Info.shader;
	rotation	= Info.rotation;
	position	= Info.position;
	scale		= Info.scale;
	
	//Info.shader->info->references.Push(&shader);
	nodes.Reserve(Scene->meshes.Length());

	MeshNode *node = nullptr;

	for (Mesh &mesh : scene->meshes) {
		node = &nodes.Insert(MeshNode());
		node->PushMesh(&mesh);
	}

	//nodes.ShrinkToFit();

	return true;
}


void SceneInstance::Free() {
	position	= glm::vec3(0.0f);
	scale		= glm::vec3(1.0f);
	rotation	= glm::vec3(0.0f);

	//shader = nullptr;
	scene  = nullptr;

	nodes.Release();
}


void SceneInstance::SetPosition(float X, float Y, float Z) {
	if (position.x != X)
		position.x = X;

	if (position.y != Y)
		position.y = Y;

	if (position.z != Z)
		position.z = Z;
}


void SceneInstance::SetPosition(const glm::vec3 &Position) {
	if (position != Position)
		position = Position;
}


void SceneInstance::SetScale(float X, float Y, float Z) {
	if (scale.x != X)
		scale.x = X;

	if (scale.y != Y)
		scale.y = Y;

	if (scale.z != Z)
		scale.z = Z;
}


void SceneInstance::SetScale(const glm::vec3 &Scale) {
	if (scale != Scale)
		scale = Scale;
}


void SceneInstance::SetRotation(float X, float Y, float Z) {
	if (rotation.x != X)
		rotation.x = X;

	if (rotation.y != Y)
		rotation.y = Y;

	if (rotation.z != Z)
		rotation.z = Z;
}


void SceneInstance::SetRotation(const glm::vec3 &Rotation) {
	if (rotation != Rotation)
		rotation = Rotation;
}


glm::vec3& SceneInstance::GetPosition() {
	return position;
}


glm::vec3& SceneInstance::GetScale() {
	return scale;
}


glm::vec3& SceneInstance::GetRotation() {
	return rotation;
}


Scene* SceneInstance::GetScene() {
	return scene;
}


MeshNode* SceneInstance::GetNode(size_t Index) {
	if (Index >= nodes.Length())
		return nullptr;

	return &nodes[Index];
}


size_t SceneInstance::TotalNodes() {
	// TODO(Afiq):
	// A function for a single line of code ... Really? 
	// Need to think of an alternative.
	return nodes.Length();
}