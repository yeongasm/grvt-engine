#pragma once


/**
* RenderCommands are objects that are pushed into the renderer for every frame.
* Scenes that are pushed into the renderer are stored as a RenderCommand object.
* Contains all necessary details for rendering.
*/
struct RenderCommand {
	/**
	* A RenderNode is simply a fancy name for a mesh that will be rendered onto the screen.
	* This was done simply to get required information from a scene.
	*/
	struct RenderNode {
		bool		instanced;
		uint		size;
		size_t		amount;
		Material	*material;
		Mesh		*mesh;

		RenderNode();

		//RenderNode(const RenderNode &Other)				= delete;
		//RenderNode(RenderNode &&Other)					= delete;

		//RenderNode& operator= (const RenderNode &Other)	= delete;
		//RenderNode& operator= (RenderNode &&Other)		= delete;

		~RenderNode();
	};

	using RenderNodes = Array<RenderNode>;

	Scene			*scene;
	Shader			*shader;
	RenderState		*renderSetting;
	RenderNodes		nodes;
	glm::mat4		model;

	RenderCommand();
	RenderCommand(const RenderCommand &Other);
	RenderCommand(RenderCommand &&Other);

	RenderCommand& operator= (const RenderCommand &Other);
	RenderCommand& operator= (RenderCommand &&Other);

	~RenderCommand();
};


/**
* TODO(Afiq): 
* Allow Renderer to only render contents from a Scenery object.
*/
class Renderer {
private:
	// This class is mainly for inserting commands like blending, face culling or depth testing.
	// The scenes should hold the values for the settings and this class would only need to update them.
	// Stencil testing will be done later on.
	struct RenderFuncs {
		bool depthTest;
		bool blend;
		bool cullFace;

		GLenum depthFunc;
		GLenum blendSrc;
		GLenum blendDst;
		GLenum frontFace;
		GLenum polygonMode;

		RenderFuncs();

		RenderFuncs(const RenderFuncs &Other)				= delete;
		RenderFuncs(RenderFuncs &&Other)					= delete;

		RenderFuncs& operator= (const RenderFuncs &Other)	= delete;
		RenderFuncs& operator= (RenderFuncs &&Other)		= delete;

		~RenderFuncs();

		void SetDepthTest(bool Enable);
		void SetDepthFunc(GLenum Func);
		void SetBlend(bool Enable);
		void SetBlendFunc(GLenum Src, GLenum Dst);
		void SetCull(bool Enable);
		void SetCullFace(GLenum Face);
		void SetPolygonMode(GLenum Mode);
	};

	Array<RenderCommand>	renderCommands;
	Array<RenderCommand>	instancedCommands;
	//Array<RenderCommand>	alphaCommands;
	RenderFuncs				settings;
	Shader					*activeShader;
	EulerCamera				*camera;

	void UseShader(Shader *Shader);

	void RenderMesh(RenderCommand::RenderNode *Node);
	void RenderPushedCommand(RenderCommand *Command);

public:
	// Make the renderer just point to the current Camera. Futureproofing for future cameras.
	// TODO(Afiq): This needs to take in the framebuffer's height and width instead.
	uint SCR_WIDTH, SCR_HEIGHT;

	Renderer();

	Renderer(const Renderer &Other)				= delete;
	Renderer(Renderer &&Other)					= delete;

	Renderer& operator= (const Renderer &Other) = delete;
	Renderer& operator= (Renderer &&Other)		= delete;

	~Renderer();

	void PushSceneForRender(SceneInstance *Instance);
	void Render();
	void AttachCamera(EulerCamera *Camera);
};