#include "stdafx.h"

RenderCommand::RenderNode::RenderNode() : instanced(false), amount(0),
	size(0), mesh(nullptr) {}


RenderCommand::RenderNode::~RenderNode() {
	instanced	= false;
	amount		= 0;
	size		= 0; 
	mesh		= nullptr;
}


RenderCommand::RenderCommand() : scene{}, shader{}, renderSetting{}, 
	nodes{}, model(1.0f) {}


RenderCommand::RenderCommand(const RenderCommand &Other) { *this = Other; }


RenderCommand::RenderCommand(RenderCommand &&Other) { *this = std::move(Other); }


RenderCommand& RenderCommand::operator= (const RenderCommand &Other) {
	if (this != &Other) {
		scene			= Other.scene;
		shader			= Other.shader;
		renderSetting	= Other.renderSetting;
		nodes			= Other.nodes;
		model			= Other.model;
	}

	return *this;
}


RenderCommand& RenderCommand::operator= (RenderCommand &&Other) {
	if (this != &Other) {
		scene			= Other.scene;
		shader			= Other.shader;
		renderSetting	= Other.renderSetting;
		nodes			= Other.nodes;
		model			= Other.model;	

		Other.~RenderCommand();
	}


	return *this;
}


RenderCommand::~RenderCommand() {
	scene = nullptr;
	nodes.Release();
}


Renderer::RenderFuncs::RenderFuncs() : depthTest(false), blend(false), cullFace(true), depthFunc{},
	blendSrc{}, blendDst{}, frontFace(GL_CCW), polygonMode(GL_FILL) {}


Renderer::RenderFuncs::~RenderFuncs() {}


void Renderer::RenderFuncs::SetDepthTest(bool Enable) {
	if (depthTest != Enable) {
		depthTest = Enable;

		if (depthTest)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}
}


void Renderer::RenderFuncs::SetDepthFunc(GLenum Func) {
	if (depthFunc != Func) {
		depthFunc = Func;

		glDepthFunc(depthFunc);
	}
}


void Renderer::RenderFuncs::SetBlend(bool Enable) {
	if (blend != Enable) {
		blend = Enable;

		if (blend)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
	}
}


void Renderer::RenderFuncs::SetBlendFunc(GLenum Src, GLenum Dst) {
	if (blendSrc != Src || blendDst != Dst) {
		blendSrc = Src;
		blendDst = Dst;

		glBlendFunc(blendSrc, blendDst);
	}
}


void Renderer::RenderFuncs::SetCull(bool Enable) {
	if (cullFace != Enable) {
		cullFace = Enable;

		if (cullFace)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
	}
}


void Renderer::RenderFuncs::SetCullFace(GLenum Face) {
	if (frontFace != Face) {
		frontFace = Face;

		glCullFace(frontFace);
	}
}


void Renderer::RenderFuncs::SetPolygonMode(GLenum Mode) {
	if (polygonMode != Mode) {
		polygonMode = Mode;

		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	}
}


void Renderer::UseShader(Shader *Shader) {
	if (activeShader != Shader)
		activeShader = Shader;
		
	activeShader->Use();
}


void Renderer::RenderMesh(RenderCommand::RenderNode *Node) {
	Mesh *mesh = Node->mesh;

	glBindVertexArray(mesh->vao);

	if (Node->instanced)
		glDrawElementsInstanced(GL_TRIANGLES, Node->size, GL_UNSIGNED_INT, 0, (uint)Node->amount);
	else if (mesh->ebo)
		glDrawElements(GL_TRIANGLES, mesh->size, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, mesh->size);
	
	glBindVertexArray(0);
}


void Renderer::RenderPushedCommand(RenderCommand *Command) {
	// TODO(Afiq):
	// There has got to be a more elegant way to doing this.
	// Find other options.

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 1000.0f);
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 composite = projection * view * Command->model;

	activeShader->SetMatrix("composite", composite);

	//activeShader->SetMatrix("projection", projection);
	//activeShader->SetMatrix("view", view);
	//activeShader->SetMatrix("model", Command->model);

	for (RenderCommand::RenderNode &node : Command->nodes) {
		// Update shader uniforms here and bind texture units to it's respective slots.
		UniformAttr *pUniform = nullptr;

		if (node.material) {
			// 1. Update shader uniform values.
			for (auto &pair : node.material->uniforms) {
				pUniform = &pair.second;
				activeShader->SetUniform(*pUniform);
			}

			// 2. Update texture sample units.
			for (Texture *texture : node.material->textures) {
				glActiveTexture(GL_TEXTURE0 + texture->type);
				glBindTexture(texture->target, texture->id);
			}
			glActiveTexture(GL_TEXTURE0);
		}


		RenderMesh(&node);
	}
}


Renderer::Renderer() : 
	renderCommands{},
	settings {}, 
	activeShader{}, 
	camera{} {}


Renderer::~Renderer() {
	renderCommands.Release();
}


void Renderer::PushSceneForRender(SceneInstance *Instance) {
	RenderCommand::RenderNode rnode;
	RenderCommand command;

	command.scene			= Instance->scene;
	command.shader			= Instance->shader;
	command.renderSetting	= &Instance->renderState;
	
	glm::mat4 &model = command.model;

	model = glm::translate(model, Instance->GetPosition());
	model = glm::scale(model, Instance->GetScale());

	if (Instance->GetRotation().x)
		model = glm::rotate(model, glm::radians(Instance->GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));

	if (Instance->GetRotation().y)
		model = glm::rotate(model, glm::radians(Instance->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));

	if (Instance->GetRotation().z)
		model = glm::rotate(model, glm::radians(Instance->GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

	for (MeshNode &node : Instance->nodes) {
		rnode.material = node.material;
		rnode.mesh = node.mesh;
		rnode.size = node.mesh->size;

		command.nodes.Push(rnode);
	}

	// Pre-allocate render command buffer on first push.
	if (!renderCommands.Size())
		renderCommands.Reserve(512, false);

	renderCommands.Push(command);
}


void Renderer::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Currently we only use the default frambuffer to render our objects into.
	// In the future, we should probably have a RenderTarget class and iterate all over that instead.

	// 1. Set the default values for the rendering states.
	settings.SetBlend(false);
	settings.SetCull(true);
	settings.SetCullFace(GL_BACK);
	settings.SetDepthTest(true);
	settings.SetDepthFunc(GL_LESS);

	RenderCommand *command = nullptr;
	RenderState	*state = nullptr;

	// 2. Render all pushed data onto the screen.
	for (size_t i = 0; i < renderCommands.Length(); i++) {
		command	= &renderCommands[i];

		state = command->renderSetting;
		
		// 2.1 Update rendering states per command. Currently being tested to see if it works.
		settings.SetBlend(state->blending);
		settings.SetCull(state->cullFace);
		settings.SetDepthTest(state->cullFace);

		if (settings.blend)
			settings.SetBlendFunc(state->blendSrc, state->blendDst);

		if (settings.cullFace)
			settings.SetCullFace(state->frontFace);

		if (settings.depthTest)
			settings.SetDepthFunc(state->depthFunc);

		settings.SetPolygonMode(state->polygonMode);

		UseShader(command->shader);

		// Updating shader uniforms and texture units will be done in this function.
		RenderPushedCommand(command);
	}

	settings.SetPolygonMode(GL_FILL);

	// 3. Clear command buffer when finished.
	renderCommands.Empty();
}


void Renderer::AttachCamera(EulerCamera *Camera) {
	if (camera != Camera)
		camera = Camera;
}