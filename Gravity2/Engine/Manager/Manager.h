#pragma once


/**
*/
enum ResourceType : uint32 {
	GrvtResource_Type_None			= 0xFF, /** On first init only. */
	GrvtResource_Type_Model			= 0x00,
	GrvtResource_Type_Texture		= 0x01,
	GrvtResource_Type_Shader		= 0x02,
	GrvtResource_Type_Material		= 0x03,
	GrvtResource_Type_Framebuffer	= 0x04
};


/**
*/
struct ResId {
	size_t Id;

	ResId();
	~ResId();

	ResId(size_t Id);

	operator size_t();
};


/**
*/
struct EngineResource {
	Array<void**>	References;
	void*			ResourcePtr;
	ResourceType	Type;
	uint32			RefCount;

	EngineResource();
	~EngineResource();
};


/**
*/
class ResourceManager {
private:
	
	std::unordered_map<ResId, EngineResource> Store;

	template <typename T>
	size_t GenerateResourceId(ResourceType Type) {
		static size_t id = 0;
		return (Type << (sizeof(size_t) * 8 - 2)) | id++;
	}

public:

	ResourceManager();
	~ResourceManager();


};