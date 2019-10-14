#pragma once


struct TextureData {
private:

	using References = Array<Texture**>;

public:

	uint		id;
	String		name;
	String		file;
	String		directory;
	Texture		*texture;
	References	references;

	TextureData();
	~TextureData();
;
	TextureData(const TextureData &Data)				= delete;
	TextureData& operator= (const TextureData &Data)	= delete;
	
	TextureData(TextureData &&Data)						= delete;
	TextureData& operator= (TextureData &&Data)			= delete;

	void Alloc(const TextureCreationInfo &Info);
	void Free();
};


struct ShaderData {
private:

	using References = Array<Shader**>;

public:

	uint		id;
	String		name;
	String		vertexFile;
	String		fragmentFile;
	String		geometryFile;
	String		directory;
	Shader		*shader;
	References	references;

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
	~SceneData();

	SceneData(const SceneData &Data)				= delete;
	SceneData(SceneData &&Data)						= delete;

	SceneData& operator= (const SceneData &Data)	= delete;
	SceneData& operator= (SceneData &&Data)			= delete;

	void Alloc(const SceneCreationInfo &Info);
	void Free();

};


struct MaterialData {
private:

	using References = Array<Material**>;

public:

	uint		id;
	String		name;
	Material	*material;
	References	references;

	MaterialData();
	~MaterialData();

	MaterialData(const MaterialData &Data)				= delete;
	MaterialData& operator= (const MaterialData &Data)	= delete;

	MaterialData(MaterialData &&Data)					= delete;
	MaterialData& operator= (MaterialData &&Data)		= delete;

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
	~SceneryData();

	SceneryData(const SceneryData &Data)				= delete;
	SceneryData& operator= (const SceneryData &Data)	= delete;

	SceneryData(SceneryData &&Data)						= delete;
	SceneryData& operator= (SceneryData &&Data)			= delete;

	void Alloc(const LevelCreationInfo &Info);
	void Free();

};


struct RenderBufferData {
private:

	using References = Array<RenderBuffer**>;

public:

	uint			id;
	String			name;
	RenderBuffer	*renderbuffer;
	References		references;

	RenderBufferData();
	~RenderBufferData();

	RenderBufferData(const RenderBufferData &Data)				= delete;
	RenderBufferData& operator= (const RenderBufferData &Data)	= delete;

	RenderBufferData& operator= (RenderBufferData &&Data)		= delete;
	RenderBufferData(RenderBufferData &&Data)					= delete;

	void Alloc(const RenderBufferCreationInfo &Info);
	void Free();
};


struct PostProcessData {
private:

	using References = Array<PostProcess**>;

public:

	uint		id;
	String		name;
	PostProcess *framebuffer;
	References	references;

	PostProcessData();
	~PostProcessData();

	PostProcessData(const PostProcessData &Data)				= delete;
	PostProcessData& operator= (const PostProcessData &Data)	= delete;

	PostProcessData(PostProcessData &&Data)						= delete;
	PostProcessData& operator= (PostProcessData &&Data)			= delete;


	void Alloc(const PostProcessCreationInfo &Info);
	void Free();

};


/**
* The resource manager class acts as a database for all Gravity objects.
* Every object has a database of it's own with a unique id assigned to it.
*
* TODO(Afiq):
* Deleting a resource that's tied to an OpenGL object can no longer be done in the resource manager.
* It needs to send the OpenGL object to the ResourceBuildQueue, when the build queue has finished deleting the object from the GPU,
* only then do we delete the object from memory.
*
* TODO(Afiq):
* Remove all logs from the ResourceManager. Make the UI handle the logic for the logs instead.
* Replace logs with comments instead.
*/
class ResourceManager {
public:

	/**
	* Never call this to add new textures into the engine. Call NewTexture() instead.
	*/
	Array<TextureData*>			textures;

	/**
	* Never call this to add new shaders into the engine. Call NewShader() instead.
	*/
	Array<ShaderData*>			shaders;

	/**
	* Never call this to add new scenes into the engine. Call NewScene() instead.
	*/
	Array<SceneData*>			scenes;

	/**
	* Never call this to add new materials into the engine. Call NewMaterial() instead.
	*/
	Array<MaterialData*>		materials;

	/**
	* Never call this to add new levels into the engine. Call NewLevel() instead.
	*/
	Array<SceneryData*>			levels;

	/**
	* Never call this to add new framebuffers into the engine. Call NewPostProcess() instead.
	*/
	Array<PostProcessData*>		framebuffers;

	/**
	* Never call this to add new renderbuffers into the engine. Call NewRenderBuffer() instead.
	*/
	Array<RenderBufferData*>	renderbuffers;

private:

	/**
	* We supply the base resource type into the template specifier to generate unique ids.
	* 
	* @template <Resource> - Gravity resource types such as a scene, shader, textures and etc.
	*/
	template <typename Resource>
	uint GenerateID() { static uint id = 0; return id++; }

public:

	Scene*			NewScene			(const SceneCreationInfo &Info);
	Shader*			NewShader			(const ShaderCreationInfo &Info);
	Texture*		NewTexture			(const TextureCreationInfo &Info);
	Material*		NewMaterial			(const MaterialCreationInfo &Info);
	Scenery*		NewLevel			(const LevelCreationInfo &Info);
	PostProcess*	NewPostProcess		(const PostProcessCreationInfo &Info);
	RenderBuffer*	NewRenderBuffer		(const RenderBufferCreationInfo &Info);

	Scene*			GetScene			(const String &Name);
	Shader*			GetShader			(const String &Name);
	Texture*		GetTexture			(const String &Name);
	Material*		GetMaterial			(const String &Name);
	Scenery*		GetLevel			(const String &Name);
	PostProcess*	GetPostProcess		(const String &Name);
	RenderBuffer*	GetRenderBuffer		(const String &Name);

	bool			DeleteScene			(const String &Name);
	bool			DeleteShader		(const String &Name);
	bool			DeleteTexture		(const String &Name);
	bool			DeleteMaterial		(const String &Name);
	bool			DeleteLevel			(const String &Name);
	bool			DeletePostProcess	(const String &Name);
	bool			DeleteRenderBuffer	(const String &Name);

	/**
	* TODO(Afiq):
	* We need to let the BuildQueue delete the OpenGL objects first before releasing them from memory.
	* One method that can be thought of right now is to push all resource into the build queue and wait for an OK message only then do we release it from memory.
	*/
	void		CleanResource		();

	ResourceManager();
	~ResourceManager();
};