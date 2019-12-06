#include "Manager/Manager.h"
#include "Framework/Foundation/Interface.h"
#include "Utility/Utility.h"
#include "Thirdparty/stb/stb_image.h"

namespace Grvt
{

	ResourceHandle<GrvtModel>		g_ModelManager;
	ResourceHandle<GrvtTexture>		g_TextureManager;
	ResourceHandle<GrvtShader>		g_ShaderManager;
	ResourceHandle<GrvtFramebuffer>	g_FramebufferManager;
	ResourceHandle<GrvtMaterial>	g_MaterialManager;

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
		Resources.reserve(Reserve * 5);

		g_ModelManager.Alloc(Reserve);
		g_TextureManager.Alloc(Reserve);
		g_ShaderManager.Alloc(Reserve);
		g_MaterialManager.Alloc(Reserve);
		g_FramebufferManager.Alloc(Reserve);
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
				DeleteModel(It.first, true);
				break;
			case GrvtResource_Type_Texture:
				DeleteTexture(It.first, true);
				break;
			case GrvtResource_Type_Shader:
				DeleteShader(It.first, true);
				break;
			case GrvtResource_Type_Material:
				DeleteMaterial(It.first);
				break;
			case GrvtResource_Type_Framebuffer:
				DeleteFramebuffer(It.first, true);
				break;
			default:
				break;
			}
		}

		g_ModelManager.Free();
		g_TextureManager.Free();
		g_ShaderManager.Free();
		g_MaterialManager.Free();
		g_FramebufferManager.Free();
	}


	ResourceType ResourceManager::GetResourceType(const Gfl::String& Identifier)
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
			return GetModel(Import.Name.C_Str());

		size_t id = GenerateResourceId<GrvtModel>(GrvtResource_Type_Model);
		Resources.emplace(Import.Name, id);

		GrvtModel* model = g_ModelManager.NewResource(id, Import.Name);
		model->Alloc(Import);

		g_ModelManager.Store[id].Path = Import.Path;
		g_ModelManager.Store[id].Type = GrvtResourceAlloc_Type_Import;

		Util::AssimpImportModelFromPath(Import.Path, model);

		return model;
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


	bool ResourceManager::DeleteModel(const Gfl::String& Identifier, bool Force)
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
		Resources.erase(Identifier);

		return true;
	}


	bool ResourceManager::DeleteModel(size_t Id, bool Force)
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
			return GetTexture(Import.Name);

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


	bool ResourceManager::DeleteTexture(const Gfl::String& Identifier, bool Force)
	{
		EngineResource<GrvtTexture>* Handle = GetTextureHandle(Identifier);
		if (!Handle)
			return false;

		if (!Force && Handle->RefCount)
			return false;

		Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Handle->ResourcePtr->Handle), Middleware::GrvtGfx_Type_Texture);
		Handle->ResourcePtr->Free();

		g_TextureManager.DeleteResource(Resources[Identifier]);
		Resources.erase(Identifier);

		return true;
	}


	bool ResourceManager::DeleteTexture(size_t Id, bool Force)
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


	bool ResourceManager::DeleteShader(const Gfl::String& Identifier, bool Force)
	{
		EngineResource<GrvtShader>* Handle = GetShaderHandle(Identifier);
		if (!Handle)
			return false;

		if (!Force && Handle->RefCount)
			return false;

		Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Handle->ResourcePtr->Handle), Middleware::GrvtGfx_Type_Shader);
		Handle->ResourcePtr->Free();

		g_ShaderManager.DeleteResource(Resources[Identifier]);
		Resources.erase(Identifier);

		return true;
	}


	bool ResourceManager::DeleteShader(size_t Id, bool Force)
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


	bool ResourceManager::DeleteMaterial(const Gfl::String& Identifier, bool Force)
	{
		EngineResource<GrvtMaterial>* handle = GetMaterialHandle(Identifier);
		if (!handle)
			return false;

		if (!Force && handle->RefCount)
			return false;

		g_MaterialManager.DeleteResource(Resources[Identifier]);
		Resources.erase(Identifier);

		return true;
	}


	bool ResourceManager::DeleteMaterial(size_t Id, bool Force)
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
			g_FramebufferManager.Store[Resources[Identifier]].ResourcePtr;

		auto it = Resources.find(Identifier);
		if (it != Resources.end())
			return g_FramebufferManager.Store[Resources[Identifier]].ResourcePtr;

		return nullptr;
	}


	GrvtFramebuffer* ResourceManager::GetFramebuffer(size_t Id, bool Safe)
	{
		if (!Safe)
			g_FramebufferManager.Store[Id].ResourcePtr;

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


	bool ResourceManager::DeleteFramebuffer(const Gfl::String& Identifier, bool Force)
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
		Resources.erase(Identifier);

		return true;
	}


	bool ResourceManager::DeleteFramebuffer(size_t Id, bool Force)
	{
		GrvtFramebuffer* Framebuffer = GetFramebuffer(Id);
		if (!Framebuffer)
			return false;

		return DeleteFramebuffer(g_FramebufferManager.Store[Id].Name, Force);
	}


	ResourceManager* InitialiseResourceManager()
	{
		if (g_ResourceManager)
			return g_ResourceManager;

		g_ResourceManager = new ResourceManager();

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