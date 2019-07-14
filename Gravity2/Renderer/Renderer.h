#pragma once


/**
* TODO(Afiq): 
* Allow Renderer to only render contents from a Scenery object.
* Renderer has it's own "framebuffer" if you will. 
* When multi-threading is implemented in the future, the main thread would constantly be pushing into the Renderer's buffer deque.
* The Renderer which will be on it's own thread will always render contents from the earliest pushed in RenderBuffer.
*/
class Renderer {
public:

	uint screenWidth;
	uint screenHeight;

private:

	/**
	* Updates the internal rendering state of the renderer. 
	* States are updated based on the inputs given by the RenderCommand.
	*
	* TODO(Afiq):
	* Add support for stencil testing.
	*/
	class RenderFuncs {
	private:

		RenderState state;
		
	public:

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

		bool Blend() const;
		bool CullFace() const;
		bool DepthTest() const;
	};

	RenderFuncs				settings;
	RenderBuffer			*activeBuffer;
	Shader					*activeShader;

	std::deque<RenderBuffer> buffers;

	void UseShader(Shader *Shader);

	void RenderMesh(RenderNode *Node);
	void RenderPushedCommand(RenderCommand *Command);

public:

	Renderer();

	Renderer(const Renderer &Other)				= delete;
	Renderer(Renderer &&Other)					= delete;

	Renderer& operator= (const Renderer &Other) = delete;
	Renderer& operator= (Renderer &&Other)		= delete;

	~Renderer();

	void Render();
	void PreRenderLevel(Scenery *Level);
};