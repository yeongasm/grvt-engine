#pragma once


struct TextureData {
	uint		id;
	String		name;
	String		file;
	String		directory;
	Texture		*texture;

	TextureData();
	TextureData(const TextureCreationInfo &Info);
	TextureData(const TextureData &Data)				= delete;
	TextureData(TextureData &&Data)						= delete;
	
	TextureData& operator= (const TextureData &Data)	= delete;
	TextureData& operator= (TextureData &&Data)			= delete;

	~TextureData();

	void Alloc(const TextureCreationInfo &Info);
	void Free();
};


struct ShaderData {
	uint	id;
	String	name;
	String	vertexFile;
	String	fragmentFile;
	String	geometryFile;
	String	directory;
	Shader	*shader;

	ShaderData();
	ShaderData(const ShaderCreationInfo &Info);
	ShaderData(const ShaderData &Data)				= delete;
	ShaderData(ShaderData &&Data)					= delete;

	ShaderData& operator= (const ShaderData &Data)	= delete;
	ShaderData& operator= (ShaderData &&Data)		= delete;

	~ShaderData();

	void Alloc(const ShaderCreationInfo &Info);
	void Free();
};


struct SceneData {
	uint	id;
	String	name;
	String	file;
	String	directory;
	Scene	*scene;

	SceneData();
	SceneData(const SceneCreationInfo &Info);
	SceneData(const SceneData &Data)				= delete;
	SceneData(SceneData &&Data)						= delete;

	SceneData& operator= (const SceneData &Data)	= delete;
	SceneData& operator= (SceneData &&Data)			= delete;

	~SceneData();

	void Alloc(const SceneCreationInfo &Info);
	void Free();
};


struct MaterialData {
	uint		id;
	String		name;
	Material	*material;

	MaterialData();
	MaterialData(const MaterialData &Data)				= delete;
	MaterialData(MaterialData &&Data)					= delete;

	MaterialData& operator= (const MaterialData &Data)	= delete;
	MaterialData& operator= (MaterialData &&Data)		= delete;

	~MaterialData();

	void Alloc(const MaterialCreationInfo &Info);
	void Free();
};


struct SceneryData {
	uint		id;
	String		name;
	String		directory;
	String		filename;
	Scenery		*level;

	SceneryData();
	SceneryData(const SceneryData &Data)				= delete;
	SceneryData(SceneryData &&Data)						= delete;

	SceneryData& operator= (const SceneryData &Data)	= delete;
	SceneryData& operator= (SceneryData &&Data)			= delete;

	~SceneryData();

	void Alloc(const LevelCreationInfo &Info);
	void Free();
};


/**
* The resource manager class acts as a database for all Gravity objects.
* Every object has a database of it's own with a unique id assigned to it.
*
* TODO(Afiq):
* Separate resource loading from OpenGL calls.
* Doing this would enable us to parse data into the engine in a separate thread.
*/
class ResourceManager {
public:

	/**
	* Never call this to add new textures into the engine. Call NewTexture() instead.
	*/
	Array<TextureData* > textures;

	/**
	* Never call this to add new shaders into the engine. Call NewShader() instead.
	*/
	Array<ShaderData*  > shaders;

	/**
	* Never call this to add new scenes into the engine. Call NewScene() instead.
	*/
	Array<SceneData*   > scenes;

	/**
	* Never call this to add new materials into the engine. Call NewMaterial() instead.
	*/
	Array<MaterialData*> materials;

	/**
	* Never call this to add new levels into the engine. Call NewLevel() instead.
	*/
	Array<SceneryData* > levels;
	
private:

	/**
	* A step up from the previous version where we use templates instead of a long list of enums.
	* This function would return a unique id from every single resource type.
	*/
	uint GenerateResourceID();

	/**
	* We supply the base resource type into the template specifier to generate unique ids.
	* 
	* @template <Resource> - Gravity resource types such as a scene, shader, textures and etc.
	*/
	template <typename Resource>
	uint GenerateID() {
		static uint id = GenerateResourceID();
		return id;
	}

	void		ProcessNode			(Scene *Scene, aiNode *AiNode, const aiScene *AiScene, const SceneCreationInfo &Info);
	Mesh*		BuildMesh			(Scene *Scene, aiMesh *AiMesh, const aiScene *AiScene);

public:

	Scene*		NewScene			(const SceneCreationInfo &Info);
	Shader*		NewShader			(const ShaderCreationInfo &Info);
	Texture*	NewTexture			(const TextureCreationInfo &Info);
	Material*	NewMaterial			(const MaterialCreationInfo &Info);
	Scenery*	NewLevel			(const LevelCreationInfo &Info);

	Scene*		GetScene			(const String &Name);
	Shader*		GetShader			(const String &Name);
	Texture*	GetTexture			(const String &Name);
	Material*	GetMaterial			(const String &Name);
	Scenery*	GetLevel			(const String &Name);



	bool		DeleteScene			(const String &Name);
	bool		DeleteShader		(const String &Name);
	bool		DeleteTexture		(const String &Name);
	bool		DeleteMaterial		(const String &Name);
	bool		DeleteLevel			(const String &Name);

	void		CleanResource		();

	ResourceManager();
	~ResourceManager();
};