#include "GrvtPch.h"
#include "Manager/Manager.h"
#include "Framework/Foundation/Interface.h"
#include "Thirdparty/stb/stb_image.h"

namespace Grvt
{

	ResourceHandle<GrvtModel>		g_ModelManager;
	ResourceHandle<GrvtTexture>		g_TextureManager;
	ResourceHandle<GrvtShader>		g_ShaderManager;
	ResourceHandle<GrvtFramebuffer>	g_FramebufferManager;
	ResourceHandle<GrvtMaterial>	g_MaterialManager;
	ResourceHandle<GrvtScene>		g_SceneManager;

	ResourceManager* g_ResourceManager = nullptr;


	bool ResourceManager::CheckIfModelExist(const Gfl::String& Path)
	{
		for (auto& It : g_ModelManager.Store)
		{
			if (It.second.Path == Path)
			{
				return true;
			}
		}

		return false;
	}


	bool ResourceManager::CheckIfTextureExist(const Gfl::String& Path)
	{
		for (auto& It : g_TextureManager.Store)
		{
			if (It.second.Path == Path)
			{
				return true;
			}
		}

		return false;
	}


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
		g_FramebufferManager.Alloc(Reserve);
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
			case GrvtResource_Type_Framebuffer:
				DeleteFramebuffer(It.first, true, false);
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
		g_FramebufferManager.Free();
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
		if (CheckIfModelExist(Import.Path))
		{
			return GetModel(Import.Name.C_Str());
		}

		size_t Id = GenerateResourceId<GrvtModel>(GrvtResource_Type_Model);
		Resources.emplace(Import.Name, Id);

		GrvtModel* Model = g_ModelManager.NewResource(Id, Import.Name);
		Model->Alloc(Import);

		g_ModelManager.Store[Id].Type = GrvtResourceAlloc_Type_Import;

		if (Import.Path.Length())
		{
			g_ModelManager.Store[Id].Path = Import.Path;
			AssimpImportModelFromPath(Import.Path, Model);
		}

		for (GrvtMesh& Mesh : Model->Meshes)
		{
			Middleware::PackageMeshForBuild(&Mesh);
		}

		return Model;
	}


	GrvtModel* ResourceManager::NewModel(const ModelCreationInfo& Info)
	{
		size_t Id = GenerateResourceId<GrvtModel>(GrvtResource_Type_Model);
		Resources.emplace(Info.Name, Id);

		GrvtModel* Model = g_ModelManager.NewResource(Id, Info.Name);
		Model->Alloc(Info);

		g_ModelManager.Store[Id].Type = GrvtResourceAlloc_Type_Custom;

		// Future proofing to allow a more robust custom model creation.
		for (GrvtMesh& Mesh : Model->Meshes)
		{
			Middleware::PackageMeshForBuild(&Mesh);
		}

		return Model;
	}


	GrvtModel* ResourceManager::GetModel(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
		{
			size_t id = Resources[Identifier];
			return g_ModelManager.Store[id].ResourcePtr;
		}

		auto it = Resources.find(Identifier);
		if (it != Resources.end())
			return g_ModelManager.Store[it->second].ResourcePtr;

		return nullptr;
	}


	GrvtModel* ResourceManager::GetModel(size_t Id, bool Safe)
	{
		if (!Safe)
			g_ModelManager.Store[Id].ResourcePtr;

		auto it = g_ModelManager.Store.find(Id);
		if (it != g_ModelManager.Store.end())
			return g_ModelManager.Store[Id].ResourcePtr;

		return nullptr;
	}


	EngineResource<GrvtModel>* ResourceManager::GetModelHandle(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
			return &g_ModelManager.Store[Resources[Identifier]];

		auto it = Resources.find(Identifier);
		if (it != Resources.end())
			return &g_ModelManager.Store[Resources[Identifier]];

		return nullptr;
	}


	bool ResourceManager::DeleteModel(const Gfl::String& Identifier, bool Force, bool Remove)
	{
		EngineResource<GrvtModel>* Handle = GetModelHandle(Identifier);
		if (!Handle)
			return false;

		if (!Force && Handle->RefCount)
			return false;

		for (GrvtMesh& Mesh : Handle->ResourcePtr->Meshes)
		{
			Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Mesh.Vao), Middleware::GrvtGfx_Type_VertexArray);
			Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Mesh.Vbo), Middleware::GrvtGfx_Type_MeshBuffer);
			Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Mesh.Ebo), Middleware::GrvtGfx_Type_MeshBuffer);
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
		GrvtModel* model = GetModel(Id);
		if (!model)
			return false;

		return DeleteModel(g_ModelManager.Store[Id].Name, Force);
	}


	GrvtTexture* ResourceManager::NewImportTexture(const TextureImportInfo& Import)
	{
		if (Import.Path.Length() != 1)
			return nullptr;

		if (CheckIfTextureExist(Import.Path[0]))
			return nullptr;

		size_t id = GenerateResourceId<GrvtTexture>(GrvtResource_Type_Texture);
		Resources.emplace(Import.Name, id);

		GrvtTexture* texture = g_TextureManager.NewResource(id, Import.Name);
		texture->Alloc(Import);

		stbi_set_flip_vertically_on_load(true);
		texture->DataPtr = (uint8*)stbi_load(Import.Path[0].C_Str(), &texture->Width, &texture->Height, &texture->Channel, 0);
		g_TextureManager.Store[id].Type = GrvtResourceAlloc_Type_Import;

		Middleware::PackageTextureForBuild(texture);

		return texture;
	}


	GrvtTexture* ResourceManager::NewImportTexture(const TextureImportInfo& Import, BaseAPI::TextureBuildData& BuildData)
	{
		if (Import.Path.Length() != 1)
			return nullptr;

		if (CheckIfTextureExist(Import.Path[0]))
			return nullptr;

		size_t id = GenerateResourceId<GrvtTexture>(GrvtResource_Type_Texture);
		Resources.emplace(Import.Name, id);

		GrvtTexture* texture = g_TextureManager.NewResource(id, Import.Name);
		texture->Alloc(Import);

		stbi_set_flip_vertically_on_load(true);
		texture->DataPtr = (uint8*)stbi_load(Import.Path[0].C_Str(), &texture->Width, &texture->Height, &texture->Channel, 0);
		g_TextureManager.Store[id].Type = GrvtResourceAlloc_Type_Import;

		BuildData.DataPtr	= texture->DataPtr;
		BuildData.Height	= texture->Height;
		BuildData.Width		= texture->Width;

		Middleware::PackageCustomTextureForBuild(texture, BuildData);

		return texture;
	}


	GrvtTexture* ResourceManager::NewCustomTexture(const TextureImportInfo& Info, void* TextureData, BaseAPI::TextureBuildData& BuildData)
	{
		size_t id = GenerateResourceId<GrvtTexture>(GrvtResource_Type_Texture);
		Resources.emplace(Info.Name, id);

		GrvtTexture* texture = g_TextureManager.NewResource(id, Info.Name);
		texture->Alloc(Info);
		texture->DataPtr = (uint8*)TextureData;
		g_TextureManager.Store[id].Type = GrvtResourceAlloc_Type_Custom;

		Middleware::PackageCustomTextureForBuild(texture, BuildData);

		return texture;
	}


	GrvtTexture* ResourceManager::NewImportCubemap(const TextureImportInfo& Import)
	{
		if (Import.Path.Length() < 6)
		{
			return nullptr;
		}

		size_t Id = GenerateResourceId<GrvtTexture>(GrvtResource_Type_Texture);
		Resources.emplace(Import.Name, Id);

		GrvtTexture* Texture = g_TextureManager.NewResource(Id, Import.Name);
		Texture->Alloc(Import);

		for (size_t i = 0; i < Import.Path.Length(); i++)
		{
			Texture->CubemapPtr[i] = (uint8*)stbi_load(Import.Path[i].C_Str(), &Texture->Width, &Texture->Height, &Texture->Channel, 0);
		}
		g_TextureManager.Store[Id].Type = GrvtResourceAlloc_Type_Import;

		Middleware::PackageCubemapForBuild(Texture);

		return Texture;
	}


	GrvtTexture* ResourceManager::GetTexture(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
		{
			size_t id = Resources[Identifier];
			return g_TextureManager.Store[id].ResourcePtr;
		}

		auto it = Resources.find(Identifier);
		if (it != Resources.end())
			return g_TextureManager.Store[Resources[Identifier]].ResourcePtr;

		return nullptr;
	}


	GrvtTexture* ResourceManager::GetTexture(size_t Id, bool Safe)
	{
		if (!Safe)
			return g_TextureManager.Store[Id].ResourcePtr;

		auto it = g_TextureManager.Store.find(Id);
		if (it != g_TextureManager.Store.end())
			return g_TextureManager.Store[Id].ResourcePtr;

		return nullptr;
	}


	EngineResource<GrvtTexture>* ResourceManager::GetTextureHandle(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
			return &g_TextureManager.Store[Resources[Identifier]];

		auto it = Resources.find(Identifier);
		if (it != Resources.end())
			return &g_TextureManager.Store[Resources[Identifier]];

		return nullptr;
	}


	bool ResourceManager::DeleteTexture(const Gfl::String& Identifier, bool Force, bool Remove)
	{
		EngineResource<GrvtTexture>* Handle = GetTextureHandle(Identifier);
		if (!Handle)
			return false;

		if (!Force && Handle->RefCount)
			return false;

		Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Handle->ResourcePtr->Handle), Middleware::GrvtGfx_Type_Texture);
		Handle->ResourcePtr->Free();

		if (Handle->ResourcePtr->Type == GrvtTexture_Type_Cubemap)
		{
			for (size_t i = 0; i < 6; i++)
			{
				free(Handle->ResourcePtr->CubemapPtr[i]);
			}
		}
		else
		{
			free(Handle->ResourcePtr->DataPtr);
		}

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
			return false;

		return DeleteTexture(g_TextureManager.Store[Id].Name, Force);
	}


	GrvtShader* ResourceManager::NewShaderProgram(const ShaderImportInfo& Import)
	{
		// To accommodate shader graphs in the future, there is no method to check if the files used are the same.

		// We do however need to check if more than one type of shader component is provided.
		bool imported = false;
		ShaderComponent comp = GrvtShader_SourceType_None;
		for (ShaderProps& prop : Import.Properties)
		{
			if (comp == prop.Component)
				return nullptr;

			comp = prop.Component;
			imported = (prop.Path.Length()) ? true : false;
		}

		// Create the resource id.
		size_t id = GenerateResourceId<GrvtShader>(GrvtResource_Type_Shader);
		Resources.emplace(Import.Name, id);

		GrvtShader* shader = g_ShaderManager.NewResource(id, Import.Name);
		shader->Alloc(Import);

		g_ShaderManager.Store[id].Type = (imported) ? GrvtResourceAlloc_Type_Import : GrvtResourceAlloc_Type_Custom;
		Middleware::PackageShaderForBuild(shader);

		return shader;
	}


	GrvtShader* ResourceManager::GetShader(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
			return g_ShaderManager.Store[Resources[Identifier]].ResourcePtr;

		auto it = Resources.find(Identifier);
		if (it != Resources.end())
			return g_ShaderManager.Store[Resources[Identifier]].ResourcePtr;

		return nullptr;
	}


	GrvtShader* ResourceManager::GetShader(size_t Id, bool Safe)
	{
		if (!Safe)
			return g_ShaderManager.Store[Id].ResourcePtr;

		auto it = g_ShaderManager.Store.find(Id);
		if (it != g_ShaderManager.Store.end())
			return g_ShaderManager.Store[Id].ResourcePtr;

		return nullptr;
	}


	EngineResource<GrvtShader>* ResourceManager::GetShaderHandle(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
			return &g_ShaderManager.Store[Resources[Identifier]];

		auto it = Resources.find(Identifier);
		if (it != Resources.end())
			return &g_ShaderManager.Store[Resources[Identifier]];

		return nullptr;
	}


	bool ResourceManager::DeleteShader(const Gfl::String& Identifier, bool Force, bool Remove)
	{
		EngineResource<GrvtShader>* Handle = GetShaderHandle(Identifier);
		if (!Handle)
			return false;

		if (!Force && Handle->RefCount)
			return false;

		Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Handle->ResourcePtr->Handle), Middleware::GrvtGfx_Type_Shader);
		Handle->ResourcePtr->Free();

		g_ShaderManager.DeleteResource(Resources[Identifier]);

		if (Remove)
		{
			Resources.erase(Identifier);
		}

		return true;
	}


	bool ResourceManager::DeleteShader(size_t Id, bool Force, bool Remove)
	{
		GrvtShader* shader = GetShader(Id);
		if (!shader)
			return false;

		return DeleteShader(g_ShaderManager.Store[Id].Name.C_Str(), Force);
	}


	GrvtMaterial* ResourceManager::NewMaterial(const MaterialCreationInfo& Info)
	{
		size_t id = GenerateResourceId<GrvtMaterial>(GrvtResource_Type_Material);
		Resources.insert({Info.Name, id});

		GrvtMaterial* material = g_MaterialManager.NewResource(id, Info.Name);
		material->Alloc(Info);
		g_MaterialManager.Store[id].Type = GrvtResourceAlloc_Type_Custom;

		// Increase the shader  ref count by 1.
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
			for (TexturePair& pair : Info.Textures)
			{
				if (pair.Value == &It.second.ResourcePtr->Handle)
				{
					It.second.RefCount++;
				}
			}
		}

		return material;
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


	GrvtFramebuffer* ResourceManager::NewFramebuffer(const FramebufferCreationInfo& Info)
	{
		size_t id = GenerateResourceId<GrvtFramebuffer>(GrvtResource_Type_Framebuffer);
		Resources.emplace(Info.Name, id);

		GrvtFramebuffer* framebuffer = g_FramebufferManager.NewResource(id, Info.Name);
		framebuffer->Alloc(Info);

		g_FramebufferManager.Store[id].Type = GrvtResourceAlloc_Type_Custom;

		// Send the framebuffer to the GPU for it to be created in the context.
		Middleware::PackageFramebufferForBuild(framebuffer);

		return framebuffer;
	}


	GrvtFramebuffer* ResourceManager::GetFramebuffer(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
			return g_FramebufferManager.Store[Resources[Identifier]].ResourcePtr;

		auto it = Resources.find(Identifier);
		if (it != Resources.end())
			return g_FramebufferManager.Store[Resources[Identifier]].ResourcePtr;

		return nullptr;
	}


	GrvtFramebuffer* ResourceManager::GetFramebuffer(size_t Id, bool Safe)
	{
		if (!Safe)
			return g_FramebufferManager.Store[Id].ResourcePtr;

		auto it = g_FramebufferManager.Store.find(Id);
		if (it != g_FramebufferManager.Store.end())
			return g_FramebufferManager.Store[Id].ResourcePtr;

		return nullptr;
	}


	EngineResource<GrvtFramebuffer>* ResourceManager::GetFramebufferHandle(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
			return &g_FramebufferManager.Store[Resources[Identifier]];

		auto it = Resources.find(Identifier);
		if (it != Resources.end())
			return &g_FramebufferManager.Store[Resources[Identifier]];

		return nullptr;
	}


	bool ResourceManager::DeleteFramebuffer(const Gfl::String& Identifier, bool Force, bool Remove)
	{
		EngineResource<GrvtFramebuffer>* Handle = GetFramebufferHandle(Identifier);
		if (!Handle)
			return false;

		if (!Force && g_FramebufferManager.Store[Resources[Identifier]].RefCount)
			return false;

		for (GrvtFramebuffer::Attachment& Attachment : Handle->ResourcePtr->Attachments)
		{
			Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Attachment.Handle), Middleware::GrvtGfx_Type_Texture);
		}

		Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Handle->ResourcePtr->Handle), Middleware::GrvtGfx_Type_Framebuffer);

		Handle->ResourcePtr->Free();
		g_FramebufferManager.DeleteResource(Resources[Identifier]);

		if (Remove)
		{
			Resources.erase(Identifier);
		}

		return true;
	}


	bool ResourceManager::DeleteFramebuffer(size_t Id, bool Force, bool Remove)
	{
		GrvtFramebuffer* Framebuffer = GetFramebuffer(Id);
		if (!Framebuffer)
			return false;

		return DeleteFramebuffer(g_FramebufferManager.Store[Id].Name, Force);
	}


	GrvtScene* ResourceManager::NewScene(const SceneCreationInfo& Info)
	{
		size_t Id = GenerateResourceId<GrvtScene>(GrvtResource_Type_Scene);
		Resources.emplace(Info.Name, Id);

		GrvtScene* Scene = g_SceneManager.NewResource(Id, Info.Name);
		Scene->Alloc(Info);

		g_SceneManager.Store[Id].Type = GrvtResourceAlloc_Type_Custom;

		return Scene;
	}


	GrvtScene* ResourceManager::GetScene(const Gfl::String& Identifier, bool Safe)
	{
		if (!Safe)
		{
			return g_SceneManager.Store[Resources[Identifier]].ResourcePtr;
		}

		auto it = Resources.find(Identifier);

		if (it != Resources.end())
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

		auto it = g_SceneManager.Store.find(Id);

		if (it != g_SceneManager.Store.end())
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

		auto it = Resources.find(Identifier);

		if (it != Resources.end())
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
			return;

		g_ResourceManager->Free();

		delete g_ResourceManager;
		g_ResourceManager = nullptr;
	}

}