#pragma once


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

	String		Name;
	String		Path;
	bool		FlipUVs;

	SceneCreationInfo();
	~SceneCreationInfo();

	SceneCreationInfo(const SceneCreationInfo &Other);
	SceneCreationInfo& operator= (const SceneCreationInfo &Other);

	SceneCreationInfo(SceneCreationInfo &&Other);
	SceneCreationInfo& operator= (SceneCreationInfo &&Other);

};


/**
* Mesh data structure.
* Current implementation does not include bone data for animation. 
* For the purpose of future proofing, we store every VBO sub data.
*
* TODO(Afiq): Implement CalculateNormals and CalculateTangentAndBiTangent function.
* NOTE(Afiq): Thinking about removing the VBO and EBO and just have the VAO in a mesh. Should it be done?
*/
class MeshObj {
public:

	ObjHandle			Vao;
	ObjHandle			Vbo;
	ObjHandle			Ebo;
	uint				Size;
	Array<glm::vec3>	Positions;
	Array<glm::vec2>	Uv;
	Array<glm::vec3>	Normals;
	Array<glm::vec3>	Tangents;
	Array<glm::vec3>	Bitangents;
	Array<uint>			Indices;

	MeshObj();
	~MeshObj();

	MeshObj(const MeshObj &Other)				= delete;
	MeshObj& operator= (const MeshObj &Other)	= delete;

	MeshObj(MeshObj &&Other);
	MeshObj& operator= (MeshObj &&Other);


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
*
* TODO(Afiq):
* The concept of SceneInstance should be changed.
*/
class SceneObj {
private:

	Array<SceneInstance>	Instances;

public:

	Array<MeshObj>			Meshes;
	SceneData				*Info;

	SceneObj();
	~SceneObj();

	SceneObj(const SceneObj &Other)				= delete;
	SceneObj& operator= (const SceneObj &Other)	= delete;

	SceneObj(SceneObj &&Other)					= delete;
	SceneObj& operator= (SceneObj &&Other)		= delete;

	void			Free				();
	SceneInstance*	CreateInstance		(const SceneInstanceCreation &Info);
	SceneInstance*	CreateInstanceFrom	(const SceneInstance &Instance);
	bool			RemoveInstance		(SceneInstance *Instance, bool Move = false);
	void			ReleaseAllInstances	();

};