#include "GrvtPch.h"
#include "Manager/Manager.h"
#include "Thirdparty/stb/stb_image.h"

namespace Grvt
{

	ResourceManager* g_ResourceManager = nullptr;


	ResourceManager::ResourceManager() :
		Resources() {}


	ResourceManager::~ResourceManager() {}


	void ResourceManager::Alloc(size_t Reserve)
	{
		// The main handler needs to allocate 5 times more than a manager of a single type of asset.
		Resources.reserve(Reserve * GrvtResource_Max);

		g_ModelManager.Alloc(Reserve);
		g_TextureManager.Alloc(Reserve);
		g_ShaderManager.Alloc(Reserve);
		g_MaterialManager.Alloc(Reserve);
		g_SceneManager.Alloc(Reserve);
	}


	void ResourceManager::Free()
	{
		ResourceType Type = GrvtResource_Type_None;

		for (auto& It : Resources)
		{
			Type = GetResourceType(It.first);

			switch (Type)
			{
			case GrvtResource_Type_Model:
				DeleteModel(It.first, true, false);
				break;
			case GrvtResource_Type_Texture:
				DeleteTexture(It.first, true, false);
				break;
			case GrvtResource_Type_Shader:
				DeleteShader(It.first, true, false);
				break;
			case GrvtResource_Type_Material:
				DeleteMaterial(It.first, true, false);
				break;
			case GrvtResource_Type_Scene:
				DeleteScene(It.first, true, false);
				break;
			default:
				break;
			}
		}

		Resources.clear();

		g_ModelManager.Free();
		g_TextureManager.Free();
		g_ShaderManager.Free();
		g_MaterialManager.Free();
	}


	ResourceType ResourceManager::GetResourceType(const Gfl::HashString& Identifier)
	{
		auto it = Resources.find(Identifier);
		if (it == Resources.end())
			return GrvtResource_Type_None;

		size_t type = it->second >> (sizeof(size_t) * 8 - 4);
		return *(ResourceType*)&type;
	}


	GrvtModel* ResourceManager::NewImportModel(const ModelImportInfo& Import)
	{
		size_t Id = GenerateResourceId<GrvtModel>(GrvtResource_Type_Model);
		Resources.emplace(Import.Name, Id);

		GrvtModel* Model = g_ModelManager.NewResource(Id, Import.Name);
		Model->Alloc(Import);

		
		if (Import.Path.Length())
		{
			AssimpImportModelFromPath(Import.Path, Model);
		}

		for (GrvtMesh& Mesh : Model->Meshes)
		{
			GfxInterface->PackageMeshForBuild(Mesh);
		}

		return Model;
	}


	GrvtModel* ResourceManager::NewModel(const ModelCreationInfo& Info)
	{
		size_t Id = GenerateResourceId<GrvtModel>(GrvtResource_Type_Model);
		Resources.emplace(Info.Name, Id);

		GrvtModel* Model = g_ModelManager.NewResource(Id, Info.Name);
		Model->Alloc(Info);

		for (GrvtMesh& Mesh : Model->Meshes)
		{
			GfxInterface->PackageMeshForBuild(Mesh);
		}

		return Model;
	}


	GrvtModel* ResourceManager::GetModel(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
		{
			size_t Id = Resources[Identifier];
			return g_ModelManager.Store[Id].ResourcePtr;
		}

		auto It = Resources.find(Identifier);
		
		if (It != Resources.end())
		{
			return g_ModelManager.Store[It->second].ResourcePtr;
		}

		return nullptr;
	}


	GrvtModel* ResourceManager::GetModel(size_t Id, bool Safe)
	{
		if (!Safe)
		{
			g_ModelManager.Store[Id].ResourcePtr;
		}

		auto It = g_ModelManager.Store.find(Id);
		
		if (It != g_ModelManager.Store.end())
		{
			return g_ModelManager.Store[Id].ResourcePtr;
		}

		return nullptr;
	}


	EngineResource<GrvtModel>* ResourceManager::GetModelHandle(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
		{
			return &g_ModelManager.Store[Resources[Identifier]];
		}

		auto It = Resources.find(Identifier);
		
		if (It != Resources.end())
		{
			return &g_ModelManager.Store[Resources[Identifier]];
		}

		return nullptr;
	}


	bool ResourceManager::DeleteModel(const Gfl::String& Identifier, bool Force, bool Remove)
	{
		EngineResource<GrvtModel>* Handle = GetModelHandle(Identifier);

		if (!Handle)
		{
			return false;
		}

		if (!Force && Handle->RefCount)
		{
			return false;
		}

		for (GrvtMesh& Mesh : Handle->ResourcePtr->Meshes)
		{
			GfxInterface->QueueHandleForDelete(Gfl::Move(Mesh.Vao), HandleType::Handle_VertexArray);
			GfxInterface->QueueHandleForDelete(Gfl::Move(Mesh.Vbo), HandleType::Handle_Buffer);
			GfxInterface->QueueHandleForDelete(Gfl::Move(Mesh.Ebo), HandleType::Handle_Buffer);

			if (Mesh.Ibo.Id)
			{
				GfxInterface->QueueHandleForDelete(Gfl::Move(Mesh.Ibo), HandleType::Handle_Buffer);
			}
		}

		Handle->ResourcePtr->Free();
		g_ModelManager.DeleteResource(Resources[Identifier]);

		if (Remove)
		{
			Resources.erase(Identifier);
		}

		return true;
	}


	bool ResourceManager::DeleteModel(size_t Id, bool Force, bool Remove)
	{
		GrvtModel* Model = GetModel(Id);
		
		if (!Model)
		{
			return false;
		}

		return DeleteModel(g_ModelManager.Store[Id].Name, Force);
	}


	GrvtTexture* ResourceManager::NewImportTexture(const TextureCreationInfo& Import)
	{
		if (Import.Path.Length() != 1)
		{
			return nullptr;
		}

		size_t Id = GenerateResourceId<GrvtTexture>(GrvtResource_Type_Texture);
		Resources.emplace(Import.Name, Id);

		GrvtTexture* Texture = g_TextureManager.NewResource(Id, Import.Name);
		
		uint8* Data = nullptr;

		stbi_set_flip_vertically_on_load(true);
		Data = (uint8*)stbi_load(Import.Path[0].C_Str(), &Texture->Width, &Texture->Height, &Texture->Channel, 0);
		
		Texture->Data.Push(Gfl::Move(Data));

		GfxInterface->PackageTextureForBuild(*Texture);

		return Texture;
	}


	GrvtTexture* ResourceManager::GetTexture(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
		{
			size_t Id = Resources[Identifier];
			return g_TextureManager.Store[Id].ResourcePtr;
		}

		auto It = Resources.find(Identifier);

		if (It != Resources.end())
		{
			return g_TextureManager.Store[Resources[Identifier]].ResourcePtr;
		}

		return nullptr;
	}


	GrvtTexture* ResourceManager::GetTexture(size_t Id, bool Safe)
	{
		if (!Safe)
		{
			return g_TextureManager.Store[Id].ResourcePtr;
		}

		auto It = g_TextureManager.Store.find(Id);

		if (It != g_TextureManager.Store.end())
		{
			return g_TextureManager.Store[Id].ResourcePtr;
		}

		return nullptr;
	}


	EngineResource<GrvtTexture>* ResourceManager::GetTextureHandle(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
		{
			return &g_TextureManager.Store[Resources[Identifier]];
		}

		auto It = Resources.find(Identifier);

		if (It != Resources.end())
		{
			return &g_TextureManager.Store[Resources[Identifier]];
		}

		return nullptr;
	}


	bool ResourceManager::DeleteTexture(const Gfl::String& Identifier, bool Force, bool Remove)
	{
		EngineResource<GrvtTexture>* Handle = GetTextureHandle(Identifier);

		if (!Handle)
		{
			return false;
		}

		if (!Force && Handle->RefCount)
		{
			return false;
		}

		GfxInterface->QueueHandleForDelete(Gfl::Move(Handle->ResourcePtr->Handle), HandleType::Handle_Texture);
		
		g_TextureManager.DeleteResource(Resources[Identifier]);

		if (Remove)
		{
			Resources.erase(Identifier);
		}

		return true;
	}


	bool ResourceManager::DeleteTexture(size_t Id, bool Force, bool Remove)
	{
		GrvtTexture* Texture = GetTexture(Id);

		if (!Texture)
		{
			return false;
		}

		return DeleteTexture(g_TextureManager.Store[Id].Name, Force);
	}


	GrvtShader* ResourceManager::NewShaderProgram(const ShaderImportInfo& Import)
	{
		size_t Id = GenerateResourceId<GrvtShader>(GrvtResource_Type_Shader);
		Resources.emplace(Import.Name, Id);

		GrvtShader* Shader = g_ShaderManager.NewResource(Id, Import.Name);

		// TODO(Afiq):
		// We'll need to load the shaders from text.
		OpenFile(Shader->VertexShader,	 Import.PathToVertexShader.C_Str());
		OpenFile(Shader->FragmentShader, Import.PathToFragmentShader.C_Str());

		if (Import.PathToGeometryShader.Length())
		{
			OpenFile(Shader->GeometryShader, Import.PathToGeometryShader.C_Str());
		}

		GfxInterface->PackageShaderForBuild(*Shader);

		return Shader;
	}


	GrvtShader* ResourceManager::GetShader(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
		{
			return g_ShaderManager.Store[Resources[Identifier]].ResourcePtr;
		}

		auto It = Resources.find(Identifier);

		if (It != Resources.end())
		{
			return g_ShaderManager.Store[Resources[Identifier]].ResourcePtr;
		}

		return nullptr;
	}


	GrvtShader* ResourceManager::GetShader(size_t Id, bool Safe)
	{
		if (!Safe)
		{
			return g_ShaderManager.Store[Id].ResourcePtr;
		}

		auto It = g_ShaderManager.Store.find(Id);

		if (It != g_ShaderManager.Store.end())
		{
			return g_ShaderManager.Store[Id].ResourcePtr;
		}

		return nullptr;
	}


	EngineResource<GrvtShader>* ResourceManager::GetShaderHandle(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
		{
			return &g_ShaderManager.Store[Resources[Identifier]];
		}

		auto It = Resources.find(Identifier);

		if (It != Resources.end())
		{
			return &g_ShaderManager.Store[Resources[Identifier]];
		}

		return nullptr;
	}


	bool ResourceManager::DeleteShader(const Gfl::String& Identifier, bool Force, bool Remove)
	{
		EngineResource<GrvtShader>* Handle = GetShaderHandle(Identifier);
		if (!Handle)
		{
			return false;
		}

		if (!Force && Handle->RefCount)
		{
			return false;
		}

		GfxInterface->QueueHandleForDelete(Gfl::Move(Handle->ResourcePtr->Handle), HandleType::Handle_Shader);
		g_ShaderManager.DeleteResource(Resources[Identifier]);

		if (Remove)
		{
			Resources.erase(Identifier);
		}

		return true;
	}


	bool ResourceManager::DeleteShader(size_t Id, bool Force, bool Remove)
	{
		GrvtShader* Shader = GetShader(Id);
		
		if (!Shader)
		{
			return false;
		}

		return DeleteShader(g_ShaderManager.Store[Id].Name.C_Str(), Force);
	}


	GrvtMaterial* ResourceManager::NewMaterial(const MaterialCreationInfo& Info)
	{
		size_t Id = GenerateResourceId<GrvtMaterial>(GrvtResource_Type_Material);
		Resources.insert({Info.Name, Id});

		GrvtMaterial* Material = g_MaterialManager.NewResource(Id, Info.Name);
		Material->Alloc(Info);

		// Increase the shader ref count by 1.
		for (auto& It : g_ShaderManager.Store)
		{
			if (Info.Shader != It.second.ResourcePtr)
			{
				continue;
			}

			It.second.RefCount++;
			break;
		}

		// Increase the textures used ref counts by 1.
		for (auto& It : g_TextureManager.Store)
		{
			for (TexturePair& Pair : Info.Textures)
			{
				if (Pair.Value == &It.second.ResourcePtr->Handle)
				{
					It.second.RefCount++;
				}
			}
		}

		return Material;
	}


	GrvtMaterial* ResourceManager::GetMaterial(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
			return g_MaterialManager.Store[Resources[Identifier]].ResourcePtr;

		auto it = Resources.find(Identifier);
		if (it != Resources.end())
			return g_MaterialManager.Store[Resources[Identifier]].ResourcePtr;

		return nullptr;
	}


	GrvtMaterial* ResourceManager::GetMaterial(size_t Id, bool Safe)
	{
		if (!Safe)
			return g_MaterialManager.Store[Id].ResourcePtr;

		auto it = g_MaterialManager.Store.find(Id);
		if (it != g_MaterialManager.Store.end())
			return g_MaterialManager.Store[Id].ResourcePtr;

		return nullptr;
	}


	EngineResource<GrvtMaterial>* ResourceManager::GetMaterialHandle(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
			return &g_MaterialManager.Store[Resources[Identifier]];

		auto it = Resources.find(Identifier);
		if (it != Resources.end())
			return &g_MaterialManager.Store[Resources[Identifier]];

		return nullptr;
	}


	bool ResourceManager::DeleteMaterial(const Gfl::String& Identifier, bool Force, bool Remove)
	{
		EngineResource<GrvtMaterial>* handle = GetMaterialHandle(Identifier);
		if (!handle)
			return false;

		if (!Force && handle->RefCount)
			return false;

		g_MaterialManager.DeleteResource(Resources[Identifier]);

		if (Remove)
		{
			Resources.erase(Identifier);
		}

		return true;
	}


	bool ResourceManager::DeleteMaterial(size_t Id, bool Force, bool Remove)
	{
		GrvtMaterial* material = GetMaterial(Id);
		if (!material)
			return false;

		return DeleteMaterial(g_MaterialManager.Store[Id].Name.C_Str(), Force);
	}


	GrvtScene* ResourceManager::NewScene(const SceneCreationInfo& Info)
	{
		size_t Id = GenerateResourceId<GrvtScene>(GrvtResource_Type_Scene);
		Resources.emplace(Info.Name, Id);

		GrvtScene* Scene = g_SceneManager.NewResource(Id, Info.Name);
		Scene->Alloc(Info);

		return Scene;
	}


	GrvtScene* ResourceManager::GetScene(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
		{
			return g_SceneManager.Store[Resources[Identifier]].ResourcePtr;
		}

		auto It = Resources.find(Identifier);

		if (It != Resources.end())
		{
			return g_SceneManager.Store[Resources[Identifier]].ResourcePtr;
		}

		return nullptr;
	}


	GrvtScene* ResourceManager::GetScene(size_t Id, bool Safe)
	{
		if (!Safe)
		{
			return g_SceneManager.Store[Id].ResourcePtr;
		}

		auto It = g_SceneManager.Store.find(Id);

		if (It != g_SceneManager.Store.end())
		{
			return g_SceneManager.Store[Id].ResourcePtr;
		}

		return nullptr;
	}


	EngineResource<GrvtScene>* ResourceManager::GetSceneHandle(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
		{
			return &g_SceneManager.Store[Resources[Identifier]];
		}

		auto It = Resources.find(Identifier);

		if (It != Resources.end())
		{
			return &g_SceneManager.Store[Resources[Identifier]];
		}

		return nullptr;
	}


	bool ResourceManager::DeleteScene(const Gfl::String& Identifier, bool Force, bool Remove)
	{
		EngineResource<GrvtScene>* Handle = GetSceneHandle(Identifier);

		if (!Handle)
		{
			return false;
		}

		if (!Force && g_SceneManager.Store[Resources[Identifier]].RefCount)
		{
			return false;
		}

		Handle->ResourcePtr->Free();
		g_SceneManager.DeleteResource(Resources[Identifier]);

		if (Remove)
		{
			Resources.erase(Identifier);
		}

		return true;
	}


	bool ResourceManager::DeleteScene(size_t Id, bool Force, bool Remove)
	{
		GrvtScene* Scene = GetScene(Id);

		if (!Scene)
		{
			return false;
		}

		return DeleteScene(g_SceneManager.Store[Id].Name, Force);
	}


	ResourceManager* InitialiseResourceManager()
	{
		if (g_ResourceManager)
		{
			return g_ResourceManager;
		}

		g_ResourceManager = new ResourceManager();
		g_ResourceManager->Alloc(128);

		return g_ResourceManager;
	}


	ResourceManager* GetResourceManager()
	{
		return g_ResourceManager;
	}


	void FreeResourceManager()
	{
		if (!g_ResourceManager)
		{
			return;
		}

		g_ResourceManager->Free();

		delete g_ResourceManager;
		g_ResourceManager = nullptr;
	}

}