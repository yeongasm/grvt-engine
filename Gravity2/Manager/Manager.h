#pragma once


/**
* TextureData object.
*
* Holds the actual texture object and stores additional information for the texture.
*/
struct TextureData {

	Array<TextureObj**>	References;
	String				Name;
	String				Path;
	TextureObj			*Texture;
	uint32				Id;
	int32				Width;
	int32				Height;

	TextureData();
	~TextureData();
;
	TextureData(const TextureData &Data)				= delete;
	TextureData& operator= (const TextureData &Data)	= delete;
	
	TextureData(TextureData &&Data)						= delete;
	TextureData& operator= (TextureData &&Data)			= delete;

	/**
	* Only allocates memory for a TextureObj and does construct the object in the GPU.
	*/
	void Alloc(const TextureCreationInfo &Info);

	/**
	* Only de-allocates the memory and does not destroy the object in the GPU.
	* Do not call this function before removing the object from the GPU.
	*/
	void Free();
};


/**
* TODO(Afiq):
* The shader will be revamped into a more real time approach.
*/
struct ShaderData {

	Array<ShaderObj**>	References;
	String				Name;
	String				VertexPath;
	String				FragmentPath;
	String				GeometryPath;
	ShaderObj			*Shader;
	uint				Id;

	ShaderData();
	~ShaderData();

	ShaderData(const ShaderData &Data)				= delete;
	ShaderData& operator= (const ShaderData &Data)	= delete;

	ShaderData(ShaderData &&Data)					= delete;
	ShaderData& operator= (ShaderData &&Data)		= delete;

	/**
	* Only allocates memory for a ShaderObj and does the construct a shader program in the GPU.
	*/
	void Alloc(const ShaderCreationInfo &Info);

	/**
	* Only de-allocates the memory and does not destroy the shader program in the GPU.
	* Do not call this function before removing the object from the GPU.
	*/
	void Free();
};


struct ModelData {

	uint		Id;
	String		Name;
	String		Path;
	ModelObj	*Model;

	ModelData();
	~ModelData();

	ModelData(const ModelData &Data)				= delete;
	ModelData& operator= (const ModelData &Data)	= delete;

	ModelData(ModelData &&Data)						= delete;
	ModelData& operator= (ModelData &&Data)			= delete;

	/**
	* Only allocates memory for a ModelObj and does not construct the mesh in the GPU.
	*/
	void Alloc(const ModelCreationInfo &Info);

	/**
	* Only de-allocate the memory and does not destroy the model in the GPU.
	* Do not call this function before removing the object from the GPU.
	*/
	void Free();

};


struct MaterialData {

	Array<MaterialObj**> References;
	String		Name;
	MaterialObj	*Material;
	uint		Id;

	MaterialData();
	~MaterialData();

	MaterialData(const MaterialData &Data)				= delete;
	MaterialData& operator= (const MaterialData &Data)	= delete;

	MaterialData(MaterialData &&Data)					= delete;
	MaterialData& operator= (MaterialData &&Data)		= delete;

	/**
	* Allocates memory for a MaterialObj.
	*/
	void Alloc(const MaterialCreationInfo &Info);

	/**
	* De-allocates memory for a material.
	*/
	void Free();

};


struct PostProcessData {

	Array<PostProcessObj**>	References;
	String			Name;
	PostProcessObj	*Framebuffer;
	uint32			Id;

	PostProcessData();
	~PostProcessData();

	PostProcessData(const PostProcessData &Data)				= delete;
	PostProcessData& operator= (const PostProcessData &Data)	= delete;

	PostProcessData(PostProcessData &&Data)						= delete;
	PostProcessData& operator= (PostProcessData &&Data)			= delete;

	/**
	* Only allocates memory for a PostProcessObj and does not construct a framebuffer in the GPU.
	*/
	void Alloc(const PostProcessCreationInfo &Info);

	/**
	* Only de-allocate the memory and does not destroy the framebuffer in the GPU.
	* Do not call this function before removing the framebuffer from the GPU.
	*/
	void Free();

};


/**
* The resource manager class acts as a database for all Gravity objects.
* Every object has a database of it's own with a unique id assigned to it.
*/
class ResourceManager {
public:

	/**
	* Never call this to add new textures into the engine. Call NewTexture() instead.
	*/
	Array<TextureData*>			TextureStore;

	/**
	* Never call this to add new shaders into the engine. Call NewShader() instead.
	*/
	Array<ShaderData*>			ShaderStore;

	/**
	* Never call this to add new materials into the engine. Call NewMaterial() instead.
	*/
	Array<MaterialData*>		MaterialStore;

	/**
	* Never call this to add new levels into the engine. Call NewLevel() instead.
	*/
	Array<SceneryData*>			LevelStore;

	/**
	* Never call this to add new framebuffers into the engine. Call NewPostProcess() instead.
	*/
	Array<PostProcessData*>		FramebufferStore;

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
	ShaderObj*		NewShader			(const ShaderCreationInfo &Info);
	TextureObj*		NewTexture			(const TextureCreationInfo &Info);
	MaterialObj*	NewMaterial			(const MaterialCreationInfo &Info);
	Scenery*		NewLevel			(const LevelCreationInfo &Info);
	PostProcess*	NewPostProcess		(const PostProcessCreationInfo &Info);
	RenderBuffer*	NewRenderBuffer		(const RenderBufferCreationInfo &Info);

	Scene*			GetScene			(const String &Name);
	ShaderObj*		GetShader			(const String &Name);
	TextureObj*		GetTexture			(const String &Name);
	MaterialObj*	GetMaterial			(const String &Name);
	Scenery*		GetLevel			(const String &Name);
	PostProcess*	GetPostProcess		(const String &Name);
	RenderBuffer*	GetRenderBuffer		(const String &Name);

	bool			DeleteScene			(const String &Name);
	bool			DeleteShader		(const String &Name);
	void			DeleteTexture		(TextureObj *&Texture);
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