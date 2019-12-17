#pragma once

#ifndef GRAVITY_RESOURCE_MANAGER
#define GRAVITY_RESOURCE_MANAGER

#include <unordered_map>
#include "Framework/Abstraction/Scene.h"
#include "Framework/Abstraction/Model.h"
#include "Framework/Abstraction/Shader.h"
#include "Framework/Abstraction/Texture.h"
#include "Framework/Abstraction/Material.h"
#include "Framework/Abstraction/Framebuffer.h"
#include "Framework/Foundation/LowLevelAPI.h"

namespace Grvt
{

	/**
	*/
	enum ResourceType : size_t
	{
		GrvtResource_Type_None			= 0xFF, /** On first init only. */
		GrvtResource_Type_Model			= 0x00,
		GrvtResource_Type_Texture		= 0x01,
		GrvtResource_Type_Shader		= 0x02,
		GrvtResource_Type_Material		= 0x03,
		GrvtResource_Type_Framebuffer	= 0x04,
		GrvtResource_Type_Scene			= 0x05,
		GrvtResource_Max
	};


	/**
	* Specifies how a resource has been allocated.
	*/
	enum GrvtAllocType : uint32
	{
		GrvtResourceAlloc_Type_None		= 0xFF,		/** On first init only */
		GrvtResourceAlloc_Type_Import	= 0x00,
		GrvtResourceAlloc_Type_Custom	= 0x01
	};


	/**
	*/
	template <class Type>
	struct EngineResource
	{
		Type*			ResourcePtr;
		Gfl::String		Name;
		Gfl::String		Path;
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
	class ResourceHandle
	{
	private:

		friend class ResourceManager;

		std::unordered_map<size_t, EngineResource<Type>> Store;

	public:

		size_t Total;

		ResourceHandle() : Store(), Total(0) {}

		~ResourceHandle() {}

		/**
		* Reserves the specified of space in memory to be used.
		*/
		void Alloc(size_t Reserve) {
			Store.reserve(Reserve);
		}

		/**
		* Releases all resource from the manager.
		* Note, this does not remove the resource from the GPU if it is a graphics object.
		* Only call this function on shutdown.
		*/
		void Free()
		{
			//for (auto& It : Store)
			//	DeleteResource(It.first, false);

			Store.clear();
		}

		/**
		* Allocates a new resource into the manager.
		* Note, this does not create the resource in the GPU if it is a graphics object.
		*/
		Type* NewResource(size_t Id, const Gfl::String& Name)
		{
			EngineResource<Type> resource;

			resource.Name = Name;
			resource.ResourcePtr = new Type();
			Store.emplace(Id, resource);
			Total++;

			return resource.ResourcePtr;
		}

		/**
		* Releases the resource with the specified Id from the manager.
		* Note, this does not remove the resource from the GPU.
		*/
		bool DeleteResource(size_t Id)
		{
			EngineResource<Type>& resource = Store[Id];

			resource.Name.Release();
			resource.Path.Release();

			delete resource.ResourcePtr;
			resource.ResourcePtr = nullptr;
			
			Store.erase(Id);

			Total--;

			return true;
		}
	};


	/**
	* NOTE(Afiq):
	* Should deleting a material delete the shader and material as well?
	*/
	class ResourceManager
	{
	private:

		using ResourceManagerMap = std::unordered_map<Gfl::HashString, size_t, Gfl::MurmurHash<Gfl::String>>;

		ResourceManagerMap Resources;

		template <typename T>
		size_t GenerateResourceId(ResourceType Type)
		{
			static size_t id = 0;
			return (Type << (sizeof(size_t) * 8 - 4)) | id++;
		}

		/**
		* Checks if a model with the specified path already exist in the engine.
		*/
		bool CheckIfModelExist(const Gfl::String& Path);

		/**
		* Checks if a texture with the specified path already exist in the engine.
		*/
		bool CheckIfTextureExist(const Gfl::String& Path);

	public:

		ResourceManager();
		~ResourceManager();

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
		ENGINE_API ResourceType GetResourceType(const Gfl::HashString& Identifier);

		/**
		* Import a new model into the engine.
		*/
		ENGINE_API GrvtModel* NewImportModel(const ModelImportInfo& Import);

		/**
		* Creates a new model into the engine.
		* This method however will only create a single mesh within the model.
		*/
		ENGINE_API GrvtModel* NewModel(const ModelCreationInfo& Info);

		/**
		* Retrieves the model specified by the identifier.
		* Safe mode will check if the specified identifier exist and only return if it does.
		*/
		ENGINE_API GrvtModel* GetModel(const Gfl::String& Identifier, bool Safe = true);

		/**
		* Retrieves the model specified by the Id.
		* Safe mode will check if the specified Id exist and only return if it does.
		*/
		ENGINE_API GrvtModel* GetModel(size_t Id, bool Safe = true);

		/**
		* Retrieves the model's handler.
		* Safe mode will check if the specified identifier provided exists and only return if it does.
		*/
		ENGINE_API EngineResource<GrvtModel>* GetModelHandle(const Gfl::String& Identifier, bool Safe = true);

		/**
		* Deletes a model with the specified identifier from the engine.
		* Deletes the object from the GPU as well.
		* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
		*/
		ENGINE_API bool DeleteModel(const Gfl::String& Identifier, bool Force = false, bool Remove = true);

		/**
		* Deletes a model with the specified Id from the engine.
		* Deletes the object from the GPU as well.
		* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
		*/
		ENGINE_API bool DeleteModel(size_t Id, bool Force = false, bool Remove = true);

		/**
		* Import a new texture into the engine.
		*/
		ENGINE_API GrvtTexture* NewImportTexture(const TextureImportInfo& Import);

		/**
		* Create a custom texture and store it into the engine.
		*/
		ENGINE_API GrvtTexture* NewCustomTexture(const TextureImportInfo& Info, void* TextureData, BaseAPI::TextureBuildData& BuildData);

		/**
		* Import a new cubemap into the engine.
		*/
		ENGINE_API GrvtTexture* NewImportCubemap(const TextureImportInfo& Import);

		/**
		* Retrieves the texture specified by the identifier.
		* Safe mode will check if the specified identifier exist and only return if it does.
		*/
		ENGINE_API GrvtTexture* GetTexture(const Gfl::String& Identifier, bool Safe = true);

		/**
		* Retrieves the texture specified by the Id.
		* Safe mode will check if the specified identifier exist and only return if it does.
		*/
		ENGINE_API GrvtTexture* GetTexture(size_t Id, bool Safe = true);

		/**
		* Retrieve's the texture's handler.
		* Safe mode will check if the specified identifier provided exists and only return if it does.
		*/
		ENGINE_API EngineResource<GrvtTexture>* GetTextureHandle(const Gfl::String& Identifier, bool Safe = true);

		/**
		* Deletes a texture with the specified identifier.
		* Deletes the object from the GPU as well.
		* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
		*/
		ENGINE_API bool DeleteTexture(const Gfl::String& Identifier, bool Force = false, bool Remove = true);

		/**
		* Deletes a texture with the specified Id.
		* Deletes the object from the GPU as well.
		* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
		*/
		ENGINE_API bool DeleteTexture(size_t Id, bool Force = false, bool Remove = true);

		/**
		* Creates a new shader and stores it into the engine.
		*/
		ENGINE_API GrvtShader* NewShaderProgram(const ShaderImportInfo& Import);

		/**
		* Retrieves the shader specified by the identifier from the engine.
		* Safe mode will check if the specified identifier exist and only return if it does.
		*/
		ENGINE_API GrvtShader* GetShader(const Gfl::String& Identifier, bool Safe = true);

		/**
		* Retrieves the shader specified by the Id from the engine.
		* Safe mode will check if the specified identifier exist and only return if it does.
		*/
		ENGINE_API GrvtShader* GetShader(size_t Id, bool Safe = true);

		/**
		* Retrieve's the shader's handler.
		* Safe mode will check if the specified identifier provided exists and only return if it does.
		*/
		ENGINE_API EngineResource<GrvtShader>* GetShaderHandle(const Gfl::String& Name, bool Safe = true);

		/**
		* Delete a shader program with the specified identifier.
		* Deletes the object from the GPU as well.
		* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
		*/
		ENGINE_API bool DeleteShader(const Gfl::String& Identifier, bool Force = false, bool Remove = true);

		/**
		* Deletes a shader program with the specified Id.
		* Deletes the object from the GPU as well.
		* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
		*/
		ENGINE_API bool DeleteShader(size_t Id, bool Force = false, bool Remove = true);

		/**
		* Creates a new material and stores it in the engine.
		*/
		ENGINE_API GrvtMaterial* NewMaterial(const MaterialCreationInfo& Info);

		/**
		* Retrieves the material specified by the identifier from the engine.
		* Safe mode will check if the resource with such identifier exist and only return if it does.
		*/
		ENGINE_API GrvtMaterial* GetMaterial(const Gfl::String& Identifier, bool Safe = true);

		/**
		* Retrieves the material specified by the id from the engine.
		* Safe mode will check if the resource with such identifier exist and only return if it does.
		*/
		ENGINE_API GrvtMaterial* GetMaterial(size_t Id, bool Safe = true);

		/**
		* Retrieve's the material's handler.
		* Safe mode will check if the specified identifier provided exists and only return if it does.
		*/
		ENGINE_API EngineResource<GrvtMaterial>* GetMaterialHandle(const Gfl::String& Identifier, bool Safe = true);

		/**
		* Deletes a material with the specified identifier.
		* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
		*/
		ENGINE_API bool DeleteMaterial(const Gfl::String& Identifier, bool Force = false, bool Remove = true);

		/**
		* Deletes a material with the specified id.
		* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
		*/
		ENGINE_API bool DeleteMaterial(size_t Id, bool Force = false, bool Remove = true);

		/**
		* Creates a new framebuffer and stores it in the engine.
		* Resource is also allocated on the GPU.
		*/
		ENGINE_API GrvtFramebuffer* NewFramebuffer(const FramebufferCreationInfo& Info);

		/**
		* Retrieves the framebuffer specified by the identifier from the engine.
		* Safe mode will check if the resource with such identifier exist and only return if it does.
		*/
		ENGINE_API GrvtFramebuffer* GetFramebuffer(const Gfl::String& Identifier, bool Safe = true);

		/**
		* Retrieves the framebuffer specified by the id from the engine.
		* Safe mode will check if the resource with such identifier exist and only return if it does.
		*/
		ENGINE_API GrvtFramebuffer* GetFramebuffer(size_t Id, bool Safe = true);

		/**
		* Retrieve's the post processing's handler.
		* Safe mode will check if the specified identifier provided exists and only return if it does.
		*/
		ENGINE_API EngineResource<GrvtFramebuffer>* GetFramebufferHandle(const Gfl::String& Identifier, bool Safe = true);

		/**
		* Deletes a framebuffer with the specified identifier.
		* Deletes the object from the GPU as well.
		* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
		*/
		ENGINE_API bool DeleteFramebuffer(const Gfl::String& Identifier, bool Force = false, bool Remove = true);

		/**
		* Deletes a framebuffer with the specified id.
		* Deletes the object from the GPU as well.
		* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
		*/
		ENGINE_API bool DeleteFramebuffer(size_t Id, bool Force = false, bool Remove = true);

		/**
		* Creates a new scene into the engine.
		*/
		ENGINE_API GrvtScene* NewScene(const SceneCreationInfo& Info);

		/**
		* Retrieves the scene specified by the identifier.
		* Safe mode will check if the specified identifier exist and only return if it does.
		*/
		ENGINE_API GrvtScene* GetScene(const Gfl::String& Identifier, bool Safe = true);

		/**
		* Retrieves the scene specified by the Id.
		* Safe mode will check if the specified Id exist and only return if it does.
		*/
		ENGINE_API GrvtScene* GetScene(size_t Id, bool Safe = true);

		/**
		* Retrieves the scene's handler.
		* Safe mode will check if the specified identifier provided exists and only return if it does.
		*/
		ENGINE_API EngineResource<GrvtScene>* GetSceneHandle(const Gfl::String& Identifier, bool Safe = true);

		/**
		* Deletes a scene with the specified identifier from the engine.
		* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
		*/
		ENGINE_API bool DeleteScene(const Gfl::String& Identifier, bool Force = false, bool Remove = true);

		/**
		* Deletes a scene with the specified Id from the engine.
		* Force when enabled will ignore all resources referencing this one and proceeds to delete the object.
		*/
		ENGINE_API bool DeleteScene(size_t Id, bool Force = false, bool Remove = true);
	};


	ResourceManager* InitialiseResourceManager();
	void			 FreeResourceManager();

	ENGINE_API ResourceManager* GetResourceManager();

}

#endif // !GRAVITY_RESOURCE_MANAGER