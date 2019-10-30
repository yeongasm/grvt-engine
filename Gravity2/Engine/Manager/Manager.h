#pragma once


/**
*/
enum ResourceType : size_t {
	GrvtResource_Type_None			= 0xFF, /** On first init only. */
	GrvtResource_Type_Model			= 0x00,
	GrvtResource_Type_Texture		= 0x01,
	GrvtResource_Type_Shader		= 0x02,
	GrvtResource_Type_Material		= 0x03,
	GrvtResource_Type_Framebuffer	= 0x04
};


/**
* Specifies how a resource has been allocated.
*/
enum GrvtAllocType : uint32 {
	GrvtResourceAlloc_Type_None		= 0xFF,		/** On first init only */
	GrvtResourceAlloc_Type_Import	= 0x00,
	GrvtResourceAlloc_Type_Custom	= 0x01
};


/**
*/
template <class Type>
struct EngineResource {
	Type*			ResourcePtr;
	std::string		Name;
	std::string		Path;
	uint32			RefCount;
	GrvtAllocType	Type;

	EngineResource() :
		ResourcePtr(nullptr), Name(), Path(), RefCount(0), Type(GrvtResourceAlloc_Type_None) {}

	~EngineResource() {}
};


/**
* Manages a single type of resource.
*/
template <class Type>
class ResourceManager {
private:

	friend class ResourceHandler;

	std::unordered_map<size_t, EngineResource<Type>> Store;

public:

	size_t Total;

	ResourceManager() : Store(), Total(0) {}

	~ResourceManager() {}

	/**
	* Reserves the specified of space in memory to be used.
	*/
	void Alloc(size_t Reserve) { Store.reserve(Reserve); }

	/**
	* Releases all resource from the manager.
	* Note, this does not remove the resource from the GPU if it is a graphics object.
	* Only call this function on shutdown.
	*/
	void Free() {
		for (auto& [key, value] : Store)
			DeleteResource(key, true);

		Store.clear();
	}

	/**
	* Allocates a new resource into the manager.
	* Note, this does not create the resource in the GPU if it is a graphics object.
	*/
	Type* NewResource(size_t Id, const std::string& Name) {
		EngineResource<Type> resource;

		resource.Name = Name;
		resource.ResourcePtr = new Type();
		Store.insert({Id, resource});
		Total++;

		return resource.ResourcePtr;
	}

	/**
	* Releases the resource with the specified Id from the manager.
	* Note, this does not remove the resource from the GPU.
	*/
	bool DeleteResource(size_t Id) {
		EngineResource<Type>& resource = Store[Id];

		resource.Name.clear();
		resource.Path.clear();

		delete resource.ResourcePtr;

		Store.erase(Id);
		Total--;

		return true;
	}
};


/**
*/
class ResourceHandler {
private:

	std::unordered_map<std::string, size_t> Resources;

	template <typename T>
	size_t GenerateResourceId(ResourceType Type) {
		static size_t id = 0;
		return (Type << (sizeof(size_t) * 8 - 4)) | id++;
	}

	/**
	* Checks if a model with the specified path already exist in the engine.
	*/
	bool CheckIfModelExist(const std::string& Path);

	/**
	* Checks if a texture with the specified path already exist in the engine.
	*/
	bool CheckIfTextureExist(const std::string& Path);

public:

	ResourceHandler();
	~ResourceHandler();
	
	/**
	* Reserve the amount of spaces for each type of asset.
	*/
	void Alloc(size_t Reserve);

	/**
	* Releases all resources from the handler.
	*/
	void Free();

	/**
	* Determine the resources type from the specified identifier.
	*/
	ResourceType GetResourceType(const std::string& Identifier);

	/**
	* Import a new model into the engine.
	*/
	GrvtModel* NewImportModel(const ModelImportInfo& Import);
	
	/**
	* Retrieves the model specified by the identifier.
	* Safe mode will check if the specified identifier exist and only return if it does.
	*/
	GrvtModel* GetModel(const std::string& Identifier, bool Safe = true);

	/**
	* Retrieves the model specified by the Id.
	* Safe mode will check if the specified Id exist and only return if it does.
	*/
	GrvtModel* GetModel(size_t Id, bool Safe = true);

	/**
	* Retrieves the model's handler.
	* Safe mode will check if the specified identifier provided exists and only return if it does.
	*/
	EngineResource<GrvtModel>* GetModelHandle(const std::string& Identifier, bool Safe = true);

	/**
	* Deletes a model with the specified identifier from the engine.
	* Deletes the object from the GPU as well.
	* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
	*/
	bool DeleteModel(const std::string& Identifier, bool Force = false);

	/**
	* Deletes a model with the specified Id from the engine.
	* Deletes the object from the GPU as well.
	* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
	*/
	bool DeleteModel(size_t Id, bool Force = false);

	/**
	* Import a new texture into the engine.
	*/
	GrvtTexture* NewImportTexture(const TextureImportInfo& Import);

	/**
	* Retrieves the texture specified by the identifier.
	* Safe mode will check if the specified identifier exist and only return if it does.
	*/
	GrvtTexture* GetTexture(const std::string& Identifier, bool Safe = true);

	/**
	* Retrieves the texture specified by the Id.
	* Safe mode will check if the specified identifier exist and only return if it does.
	*/
	GrvtTexture* GetTexture(size_t Id, bool Safe = true);

	/**
	* Retrieve's the texture's handler.
	* Safe mode will check if the specified identifier provided exists and only return if it does.
	*/
	EngineResource<GrvtTexture>* GetTextureHandle(const std::string& Identifier, bool Safe = true);

	/**
	* Deletes a texture with the specified identifier.
	* Deletes the object from the GPU as well.
	* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
	*/
	bool DeleteTexture(const std::string& Identifier, bool Force = false);

	/**
	* Deletes a texture with the specified Id.
	* Deletes the object from the GPU as well.
	* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
	*/
	bool DeleteTexture(size_t Id, bool Force = false);

	/**
	* Creates a new shader and stores it into the engine.
	*/
	GrvtShader* NewShaderProgram(const ShaderImportInfo& Import);

	/**
	* Retrieves the shader specified by the identifier from the engine.
	* Safe mode will check if the specified identifier exist and only return if it does.
	*/
	GrvtShader* GetShader(const std::string& Identifier, bool Safe = true);
	
	/**
	* Retrieves the shader specified by the Id from the engine.
	* Safe mode will check if the specified identifier exist and only return if it does.
	*/
	GrvtShader* GetShader(size_t Id, bool Safe = true);

	/**
	* Retrieve's the shader's handler.
	* Safe mode will check if the specified identifier provided exists and only return if it does.
	*/
	EngineResource<GrvtShader>* GetShaderHandle(const std::string& Name, bool Safe = true);

	/**
	* Delete a shader program with the specified identifier.
	* Deletes the object from the GPU as well.
	* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
	*/
	bool DeleteShader(const std::string& Identifier, bool Force = false);
	
	/**
	* Deletes a shader program with the specified Id.
	* Deletes the object from the GPU as well.
	* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
	*/
	bool DeleteShader(size_t Id, bool Force = false);


	//GrvtMaterial* NewMaterial(const MaterialCreationInfo& Info);

	//GrvtMaterial* GetMaterial(const String& Identifier, bool Safe = true);
	//GrvtMaterial* GetMaterial(size_t Id, bool Safe = true);

	//bool DeleteMaterial(const String& Identifier);
	//bool DeleteMaterial(size_t Id);

	/**
	* Creates a new framebuffer and stores it in the engine.
	* Resource is also allocated on the GPU.
	*/
	GrvtPostProcess* NewPostProcessing(const PostProcessCreationInfo& Info);

	/**
	* Retrieves the framebuffer specified by the identifier from the engine.
	* Safe mode will check if the resource with such identifier exist and only return if it does.
	*/
	GrvtPostProcess* GetPostProcessing(const std::string& Identifier, bool Safe = true);

	/**
	* Retrieves the framebuffer specified by the id from the engine.
	* Safe mode will check if the resource with such identifier exist and only return if it does.
	*/
	GrvtPostProcess* GetPostProcessing(size_t Id, bool Safe = true);

	/**
	* Retrieve's the post processing's handler.
	* Safe mode will check if the specified identifier provided exists and only return if it does.
	*/
	EngineResource<GrvtPostProcess>* GetPostProcessingHandle(const std::string& Identifier, bool Safe = true);

	/**
	* Deletes a framebuffer with the specified identifier.
	* Deletes the object from the GPU as well.
	* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
	*/
	bool DeletePostProcessing(const std::string& Identifier, bool Force = false);
	
	/**
	* Deletes a framebuffer with the specified id.
	* Deletes the object from the GPU as well.
	* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
	*/
	bool DeletePostProcessing(size_t Id, bool Force = false);
};