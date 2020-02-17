#pragma once

#ifndef GRAVITY_RESOURCE_MANAGER
#define GRAVITY_RESOURCE_MANAGER

#include "Minimal.h"
#include "API/Graphics/GraphicsDriver.h"
#include "API/Graphics/GraphicsInterface.h"

#include "Framework/Scene.h"
#include "Framework/Model.h"
#include "Framework/Shader.h"
#include "Framework/Texture.h"
#include "Framework/Material.h"

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
		GrvtResource_Type_Scene			= 0x04,
		GrvtResource_Max
	};


	/**
	*/
	template <class Type>
	struct EngineResource
	{
		Type*			ResourcePtr;
		Gfl::String		Name;
		uint32			RefCount;

		EngineResource() :
			ResourcePtr(nullptr), Name(), RefCount(0) {}

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
			Store.clear();
		}

		/**
		* Allocates a new resource into the manager.
		* Note, this does not create the resource in the GPU if it is a graphics object.
		*/
		Type* NewResource(size_t Id, const Gfl::String& Name)
		{
			EngineResource<Type> Resource;

			Resource.Name = Name;
			Resource.ResourcePtr = new Type();
			Store.emplace(Id, Resource);
			
			Total++;

			return Resource.ResourcePtr;
		}

		/**
		* Releases the resource with the specified Id from the manager.
		* Note, this does not remove the resource from the GPU.
		*/
		bool DeleteResource(size_t Id)
		{
			EngineResource<Type>& Resource = Store[Id];

			Resource.Name.Release();

			delete Resource.ResourcePtr;
			Resource.ResourcePtr = nullptr;
			
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
		GraphicsInterface* GfxInterface;

		template <typename T>
		size_t GenerateResourceId(ResourceType Type)
		{
			static size_t id = 0;
			return (Type << (sizeof(size_t) * 8 - 4)) | id++;
		}

		ResourceHandle<GrvtModel>		g_ModelManager;
		ResourceHandle<GrvtTexture>		g_TextureManager;
		ResourceHandle<GrvtShader>		g_ShaderManager;
		ResourceHandle<GrvtMaterial>	g_MaterialManager;
		ResourceHandle<GrvtScene>		g_SceneManager;

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
		ENGINE_API GrvtModel* NewImportModel(const ModelImportInfo& Info);

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
		* Uses generic texture generation parameters.
		*/
		ENGINE_API GrvtTexture* NewImportTexture(const TextureCreationInfo& Info);

		/**
		* Creates a new texture into the engine and manually specifying it's build data.
		*/
		ENGINE_API GrvtTexture* NewCustomTexture(const TextureCreationInfo& Info, const Driver::TextureBuildData& BuildData);

		/**
		* Creates a new custom texture into the engine and manually specifying it's build data.
		*/
		ENGINE_API GrvtTexture* NewCustomTexture(const Gfl::String Name, void* SrcData, const Driver::TextureBuildData& BuildData);

		/**
		* Import a new cubemap into the engine.
		* Uses generic cubemap generation parameters.
		*/
		ENGINE_API GrvtTexture* NewImportCubemap(const TextureCreationInfo& Info);

		/*
		* Creates a new cubemap into the engine and manually specifying it's build data. 
		*/
		ENGINE_API GrvtTexture* NewCustomCubemap(const TextureCreationInfo& Info, const Driver::TextureBuildData& BuildData);

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
		ENGINE_API GrvtShader* NewShaderProgram(const ShaderImportInfo& Info);

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

}

#endif // !GRAVITY_RESOURCE_MANAGER