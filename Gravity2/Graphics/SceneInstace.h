#pragma once


/**
* A wrapper to assign materials to meshes.
* For maximum flexibility, the Mesh class is designed to only store data regarding the mesh itself.
* Materials are only assigned to meshes inside of mesh nodes within an instance of a scene.
*/
class MeshNode {
private:
	Mesh		*mesh;
	Material	*material;

	// TODO(Afiq): There should be a better solution than this.
	friend class Renderer;
public:

	MeshNode();
	MeshNode(Material *Material, Mesh *Mesh);

	MeshNode(const MeshNode &Other);
	MeshNode(MeshNode &&Other);

	MeshNode& operator= (const MeshNode &Other);
	MeshNode& operator= (MeshNode &&Other);

	~MeshNode();

	void Alloc			(Material *Material, Mesh *Mesh);
	void PushMaterial	(Material *Material);
	void PushMesh		(Mesh *Mesh);

	void Free			();
};



/**
* Scene Instance Creation data structure.
* Provide information to create an instance of a scene. This is not to be confused with instancing.
* It it required to provide a shader to an instance.
* Not providing a shader will result in Gravity failing to create an instance of the scene.
*
* @param [OPTIONAL] (glm::vec3)		Default = glm::vec3(0.0f)	position	- Initial position of the scene instance.
* @param [OPTIONAL] (glm::vec3)		Default = glm::vec3(1.0f)	scale		- Initial scale of the scene instance.
* @param [OPTIONAL] (glm::vec3)		Default = glm::vec3(0.0f)	rotation	- Initial rotation of the scene instance.
* @param [REQUIRED]	(Shader*)		Default = nullptr			shader		- The shader that the scene instance will rely on.
*/
struct SceneInstanceCreation {
	glm::vec3	position;
	glm::vec3	scale;
	glm::vec3	rotation;
	Shader		*shader;

	SceneInstanceCreation();
	SceneInstanceCreation(const SceneInstanceCreation &Other);
	SceneInstanceCreation(SceneInstanceCreation &&Other);

	SceneInstanceCreation& operator= (const SceneInstanceCreation &Other);
	SceneInstanceCreation& operator= (SceneInstanceCreation &&Other);

	~SceneInstanceCreation();
};


/**
* RenderStates stores depth testing, blending and face culling data about the scene.
* Stencil testing may be introduced in the future.
*/
struct RenderState {
	bool depthTesting;
	bool blending;
	bool cullFace;

	GLenum depthFunc;
	GLenum blendSrc;
	GLenum blendDst;
	GLenum frontFace;
	GLenum polygonMode;

	RenderState();
	RenderState(const RenderState &Other);
	RenderState(RenderState &&Other);

	RenderState& operator= (const RenderState &Other);
	RenderState& operator= (RenderState &&Other);

	/**
	* Sets the values of the render states to a general version.
	* Values are as follow:
	*
	* @param depthTesting	= true.
	* @param blending		= false.
	* @param cullFace		= true.
	* @param depthFunc		= GL_LESS.
	* @param blendSrc		= 0.
	* @param blendDst		= 0.
	* @param frontFace		= GL_CCW.
	* @param polygonMode	= GL_FILL.
	*/
	void DefaultModelRenderState();

	~RenderState();
};


class SceneInstance {
private:
	glm::vec3		position;
	glm::vec3		scale;
	glm::vec3		rotation;
	Scene			*scene;
	Shader			*shader;
	Array<MeshNode> nodes;

	friend class Renderer;
public:
	bool			instanced;
	uint			id;
	RenderState		renderState;

	SceneInstance();
	SceneInstance(const SceneInstanceCreation &Info, Scene *Scene);

	SceneInstance(const SceneInstance &Other);
	SceneInstance(SceneInstance &&Other);

	SceneInstance& operator= (const SceneInstance &Other);
	SceneInstance& operator= (SceneInstance &&Other);

	~SceneInstance();

	bool		Alloc		(const SceneInstanceCreation &Info, Scene *Scene);
	void		Free		();
	
	void		SetPosition	(float X = 0.0f, float Y = 0.0f, float Z = 0.0f);
	void		SetPosition	(const glm::vec3 &Position);
	void		SetScale	(float X = 0.0f, float Y = 0.0f, float Z = 0.0f);
	void		SetScale	(const glm::vec3 &Scale);
	void		SetRotation	(float X = 0.0f, float Y = 0.0f, float Z = 0.0f);
	void		SetRotation	(const glm::vec3 &Rotation);

	glm::vec3&	GetPosition	();
	glm::vec3&	GetScale	();
	glm::vec3&	GetRotation	();

	Shader*		GetShader	();
	Scene*		GetScene	();

	MeshNode*	GetNode		(size_t Index);

};