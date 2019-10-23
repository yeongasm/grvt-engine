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
*/
template <class Type>
struct EngineResource {
	Array<Type**>	References;
	Type*			ResourcePtr;
	String			Name;
	uint32			RefCount;

	EngineResource() :
		References(), ResourcePtr(nullptr), Name(), RefCount(0) {}

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
	ResourceType Type;

public:

	ResourceManager() :
		Store(), Type(GrvtResource_Type_None) {}

	~ResourceManager() {}

	void Alloc(ResourceType Type, size_t Reserve);
	void Free();
};


/**
*/
class ResourceHandler {
private:

	std::unordered_map<String, size_t> Resources;

	template <typename T>
	size_t GenerateResourceId(ResourceType Type) {
		static size_t id = 0;
		return (Type << (sizeof(size_t) * 8 - 4)) | id++;
	}

	ResourceType GetResourceType(const String& Identifier);

public:

	ResourceHandler();
	~ResourceHandler();

	GrvtModel* NewImportModel(const ModelImportInfo& Import);
	
	GrvtModel* GetModel(const String& Model);

	void DeleteModel(const String& Identifier);
	void DeleteModel(GrvtModel* Model);
	void DeleteModel(size_t Id);

};