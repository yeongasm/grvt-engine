#pragma once


/**
* Although this is not entirely necessary for now. It may be useful in the future for debugging reasons.
* Specify the type of scene that is being created.
*/
enum SceneType {
	SCENE_TYPE_NONE			= 0x00,
	SCENE_TYPE_BASIC_CUBE	= 0x01,
	SCENE_TYPE_BASIC_SPHERE	= 0x02,
	SCENE_TYPE_BASIC_QUAD	= 0x03,
	SCENE_TYPE_BASIC_PLANE	= 0x04,
	SCENE_TYPE_CUSTOM		= 0x05,
	SCENE_TYPE_MAX
};


/**
* Scene creation data structure.
* Specify required information for scene generation.
* A scene is typically a collection of meshes alongside other data required for rendering.
*
* @param [OPTIONAL] (bool)					Default = 0		flipUVs - Flips the UV coordinates along the Y-axis.
* @param [OPTIONAL] (SceneType)								type - Specify type of scene. Does not affect mesh generation in any way.
* @param [REQUIRED] (String)								file - Specific model's filename. Supports .obj and .fbx extensions.
* @param [REQUIRED] (String)								directory - Directory to the file.
* @param [REQUIRED] (String)								name - Name of current scene. Required for identification.
*/
struct SceneCreationInfo {
	bool		flipUVs;
	SceneType	type;
	String		file;
	String		directory;
	String		name;

	SceneCreationInfo();
	SceneCreationInfo(const SceneCreationInfo &Other);
	SceneCreationInfo(SceneCreationInfo &&Other);

	SceneCreationInfo& operator= (const SceneCreationInfo &Other);
	SceneCreationInfo& operator= (SceneCreationInfo &&Other);

	~SceneCreationInfo();
};


/**
* Mesh data structure.
* Current implementation does not include bone data for animation. 
* For the purpose of future proofing, we store every VBO sub data.
*
* TODO(Afiq): Implement CalculateNormals and CalculateTangentAndBiTangent function.
* NOTE(Afiq): Thinking about removing the VBO and EBO and just have the VAO in a mesh. Should it be done?
*/
class Mesh {
public:

	VertArrayObj	vao;
	BufferObject	vbo;
	BufferObject	ebo;
	uint			size;

	Array<glm::vec3>	positions;
	Array<glm::vec2>	uv;
	Array<glm::vec3>	normals;
	Array<glm::vec3>	tangents;
	Array<glm::vec3>	bitangents;
	Array<uint>			indices;

	Mesh();

	Mesh(const Mesh &Other)				= delete;
	Mesh& operator= (const Mesh &Other) = delete;

	Mesh(Mesh &&Other);
	Mesh& operator= (Mesh &&Other);

	~Mesh();

	void	Free();
	//void	CalculateTangentAndBitangent();
	//void	CalculateNormals();
};


// Forward declaration of the struct.
struct SceneData;
struct SceneInstanceCreation;

// Forward declaration of the class.
class SceneInstance;


/**
* Gravity scene data structure.
* A Scene contains all required data for rendering.
*/
class Scene {
public:

	SceneType				type;
	Array<Mesh>				meshes;

private:

	Array<SceneInstance>	instances;

public:

	SceneData				*info;

	Scene();
	Scene(const Scene &Other)				= delete;
	Scene& operator= (const Scene &Other)	= delete;

	Scene(Scene &&Other)					= delete;
	Scene& operator= (Scene &&Other)		= delete;
	~Scene();

	void			Free				();
	SceneInstance*	CreateInstance		(const SceneInstanceCreation &Info);
	SceneInstance*	CreateInstanceFrom	(const SceneInstance &Instance);
	bool			RemoveInstance		(SceneInstance *Instance, bool Move = false);
	void			ReleaseAllInstances	();
};