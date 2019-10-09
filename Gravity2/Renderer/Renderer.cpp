#include "stdafx.h"


Renderer::RenderFuncs::RenderFuncs() : state{} {
	state.frontFace = GL_FRONT;
	state.polygonMode = GL_FILL;
}


Renderer::RenderFuncs::~RenderFuncs() {}


void Renderer::RenderFuncs::SetDepthTest(bool Enable) {
	if (state.depthTesting != Enable) {
		state.depthTesting = Enable;

		if (state.depthTesting)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}
}


void Renderer::RenderFuncs::SetDepthFunc(GLenum Func) {
	if (state.depthFunc != Func) {
		state.depthFunc = Func;

		glDepthFunc(state.depthFunc);
	}
}


void Renderer::RenderFuncs::SetBlend(bool Enable) {
	if (state.blending != Enable) {
		state.blending = Enable;

		if (state.blending)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
	}
}


void Renderer::RenderFuncs::SetBlendFunc(GLenum Src, GLenum Dst) {
	if (state.blendSrc != Src || state.blendDst != Dst) {
		state.blendSrc = Src;
		state.blendDst = Dst;

		glBlendFunc(state.blendSrc, state.blendDst);
	}
}


void Renderer::RenderFuncs::SetCull(bool Enable) {
	if (state.cullFace != Enable) {
		state.cullFace = Enable;

		if (state.cullFace)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
	}
}


void Renderer::RenderFuncs::SetCullFace(GLenum Face) {
	if (state.frontFace != Face) {
		state.frontFace = Face;

		glFrontFace(state.frontFace);
	}
}


void Renderer::RenderFuncs::SetCullDirection(GLenum Direction) {
	if (state.cullDirection != Direction) {
		state.cullDirection = Direction;

		glCullFace(state.cullDirection);
	}
}


void Renderer::RenderFuncs::SetPolygonMode(GLenum Mode) {
	if (state.polygonMode != Mode) {
		state.polygonMode = Mode;

		glPolygonMode(GL_FRONT_AND_BACK, state.polygonMode);
	}
}


bool Renderer::RenderFuncs::Blend() const {
	return state.blending;
}


bool Renderer::RenderFuncs::CullFace() const {
	return state.cullFace;
}


bool Renderer::RenderFuncs::DepthTest() const {
	return state.depthTesting;
}


void Renderer::UseShader(Shader *Shader) {
	if (activeShader != Shader)
		activeShader = Shader;
		
	activeShader->Use();
}


void Renderer::RenderMesh(RenderNode *Node) {
	Mesh *mesh = Node->mesh;

	glBindVertexArray(mesh->vao.Id);

	if (Node->mode == GL_POINTS)
		glPointSize(5.0f);

	if (mesh->ebo.Id)
		glDrawElements(Node->mode, mesh->size, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(Node->mode, 0, mesh->size);
	
	glBindVertexArray(0);
}


void Renderer::RenderPushedCommand(RenderCommand *Command) {


	for (RenderNode &node : Command->nodes) {
		// Update shader uniforms here and bind texture units to it's respective slots.
		UniformAttr *pUniform = nullptr;

		if (node.material) {
			// 1. Update the current shader when a material exists.
			UseShader(node.material->shader);
			activeShader->SetMatrix("model", Command->model);
			
			// 2. Update shader uniform values.
			for (auto &pair : node.material->uniforms) {
				pUniform = &pair.second;
				activeShader->SetUniform(*pUniform);
			}

			// 3. Update texture sample units.
			for (Texture *texture : node.material->textures) {
				glActiveTexture(GL_TEXTURE0 + texture->type);
				glBindTexture(texture->handle.Target, texture->handle.Id);
			}
			glActiveTexture(GL_TEXTURE0);
		}


		RenderMesh(&node);
	}
}


Renderer::Renderer() :
	renderWidth(0),
	renderHeight(0),
	lightBufferObj(0),
	pvBufferObject(0),
	activeShader(nullptr),
	settings(),
	renderInstance() {}


Renderer::~Renderer() {}


void Renderer::Init() {
	// Init function should initialize the light's UBO and projection * view UBO.
	// UBO for projection and view matrices.
	glGenBuffers(1, &pvBufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, pvBufferObject);

	// The shader UBO woud hold 2 mat4's for this buffer.
	size_t size = sizeof(glm::mat4);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, pvBufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// UBO for lighting information.
	glGenBuffers(1, &lightBufferObj);
	glBindBuffer(GL_UNIFORM_BUFFER, lightBufferObj);

	// The size needs to fit the size of the object inside of the UBO object which is an int + 1000 mat4's.
	size = sizeof(int) + (1000 * sizeof(glm::mat4));
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightBufferObj);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// NOTE(Afiq):
	// Perhaps we need to do something with the render buffer once multi-threading is implemented.
}


void Renderer::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Short fix for the jaggies.
	// TODO(Afiq):
	// Implement a proper anti anliasing algorithm.
	// Currently we only use the default frambuffer to render our objects into.
	// In the future, we should probably have a RenderTarget class and iterate all over that instead.
	glEnable(GL_MULTISAMPLE);

	// 1. Set the default values for the rendering states.
	settings.SetBlend(false);
	settings.SetCull(true);
	settings.SetCullFace(GL_CCW);
	settings.SetCullDirection(GL_BACK);
	settings.SetDepthTest(true);
	settings.SetDepthFunc(GL_LESS);

	RenderState		state;
	RenderCommand	*command = nullptr;

	/**
	* TODO(Afiq):
	* 1. Once shadow mapping is introduced into the engine, we'll need to configure the Renderer to cater for that process.
	* 2. Lighting information should be passed into the shader for every object that is being rendered.
	*/

	// Update the projection and view composite matrix inside of the UBO.
	glBindBuffer(GL_UNIFORM_BUFFER, pvBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(renderInstance.compProjView));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Update the light data inside of the UBO.
	size_t totalLights = renderInstance.lights.Length();
	glBindBuffer(GL_UNIFORM_BUFFER, lightBufferObj);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &totalLights);
	glBufferSubData(GL_UNIFORM_BUFFER, 16, totalLights * sizeof(glm::mat4), renderInstance.lights.First());


	// 2. Render all pushed data onto the screen.
	for (size_t i = 0; i < renderInstance.commands.Length(); i++) {
		command	= &renderInstance.commands[i];

		state = command->renderSetting;
		
		// 2.1 Update rendering states per command. Currently being tested to see if it works.
		settings.SetBlend(state.blending);
		settings.SetCull(state.cullFace);
		settings.SetDepthTest(state.depthFunc);

		if (settings.Blend())
			settings.SetBlendFunc(state.blendSrc, state.blendDst);

		if (settings.CullFace()) {
			settings.SetCullFace(state.frontFace);
			settings.SetCullDirection(state.cullDirection);
		}

		if (settings.DepthTest())
			settings.SetDepthFunc(state.depthFunc);

		settings.SetPolygonMode(state.polygonMode);

		// Updating shader uniforms and texture units will be done in this function.
		RenderPushedCommand(command);
	}

	settings.SetPolygonMode(GL_FILL);

	// 3. Remove the instance from the queue.
	renderInstance.Free();
}


void Renderer::PreRenderLevel(Scenery *Level) {

	// Add a new buffer object into the buffer container.
	RenderInstance	*instance = &renderInstance;
	RenderCommand	command;
	RenderNode		rnode;

	// Right now we only cater for scene instances that are not pushed for instanced rendering.
	for (SceneInstance *scene : Level->renderInstances) {
		command.renderSetting	= scene->renderState;

		glm::mat4 &model = command.model;

		model = glm::translate(model, scene->GetPosition());
		model = glm::scale(model, scene->GetScale());

		if (scene->GetRotation().x)
			model = glm::rotate(model, glm::radians(scene->GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));

		if (scene->GetRotation().y)
			model = glm::rotate(model, glm::radians(scene->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));

		if (scene->GetRotation().z)
			model = glm::rotate(model, glm::radians(scene->GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

		for (MeshNode &node : scene->nodes) {
			rnode.material	= node.material;
			rnode.mesh		= node.mesh;
			rnode.size		= node.mesh->size;
			rnode.mode		= node.mode;

			//rnode.amount = instance->scene->meshes.Length();
			command.nodes.Push(rnode);
		}

		instance->commands.Push(command);
		command.nodes.Empty();
	}

	// TODO(Afiq):
	// Update RenderBuffer to store instanced rendering scenes.
	

	glm::mat4 light;

	// Pack lighting data.
	// We send lighting data into the shader by using a 4x4 matrice and let the shader translate the data into what it needs by using this scheme.
	// mat[0][0]				= Type.
	// mat[0][1]				= Brightness;
	// mat[0][2]				= Cutoff (For Spotlight).
	// mat[1][0] - mat[1][2]	= Position.
	// mat[2][0] - mat[2][2]	= Colour.
	// mat[3][0]				= Constant (For Pointlight).
	// mat[3][1]				= Linear (For Spotlight).
	// mat[3][2]				= Quadratic (For  Spotlight).
	for (Light *plight : Level->lights) {
		plight->Compute(light);
		instance->lights.Push(light);
	}

	// TODO(Afiq):
	// Implement state sorting of the commands here. 
	// Sort by FBO (When implemented) -> impleShader -> RenderStates (Mainly depth and then face culling) -> Models.
	// This is to reduce OpenGL's internal state switching in order to enhance performance.

	instance->camera = Level->camera;
	

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (*(float*)&renderWidth / *(float*)&renderHeight), 0.1f, 1000.0f);
	glm::mat4 view = instance->camera->GetViewMatrix();

	instance->compProjView = projection * view;
}