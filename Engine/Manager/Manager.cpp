#include "stdafx.h"

static ResourceManager<GrvtModel>		ModelManager;
static ResourceManager<GrvtTexture>		TextureManager;
static ResourceManager<GrvtShader>		ShaderManager;
static ResourceManager<GrvtFramebuffer> FramebufferManager;
static ResourceManager<GrvtMaterial>	MaterialManager;


bool ResourceHandler::CheckIfModelExist(const String& Path) 
{
	for (auto& It : ModelManager.Store)
	{
		if (It.second.Path == Path)
		{
			return true;
		}
	}

	return false;
}


bool ResourceHandler::CheckIfTextureExist(const String& Path) 
{
	for (auto& It : TextureManager.Store) 
	{
		if (It.second.Path == Path) 
		{
			return true;
		}
	}

	return false;
}


ResourceHandler::ResourceHandler() :
	Resources() {}


ResourceHandler::~ResourceHandler() {}


void ResourceHandler::Alloc(size_t Reserve) 
{
	// The main handler needs to allocate 5 times more than a manager of a single type of asset.
	Resources.reserve(Reserve * 5);

	ModelManager.Alloc(Reserve);
	TextureManager.Alloc(Reserve);
	ShaderManager.Alloc(Reserve);
	MaterialManager.Alloc(Reserve);
	FramebufferManager.Alloc(Reserve);
}


void ResourceHandler::Free() 
{
	ResourceType type = GrvtResource_Type_None;

	for (auto& It : Resources) 
	{
		type = GetResourceType(It.first);

		switch (type) 
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
}


ResourceType ResourceHandler::GetResourceType(const String& Identifier) 
{
	auto it = Resources.find(Identifier);
	if (it == Resources.end())
		return GrvtResource_Type_None;

	size_t type = it->second >> (sizeof(size_t) * 8 - 4);
	return *(ResourceType*)&type;
}


GrvtModel* ResourceHandler::NewImportModel(const ModelImportInfo& Import) 
{
	if (CheckIfModelExist(Import.Path))
		return GetModel(Import.Name.C_Str());

	size_t id = GenerateResourceId<GrvtModel>(GrvtResource_Type_Model);
	Resources.emplace(Import.Name, id);
	
	GrvtModel* model = ModelManager.NewResource(id, Import.Name);
	model->Alloc(Import);

	ModelManager.Store[id].Path = Import.Path;
	ModelManager.Store[id].Type = GrvtResourceAlloc_Type_Import;
	
	Util::AssimpImportModelFromPath(Import.Path, model);

	return model;
}


GrvtModel* ResourceHandler::GetModel(const String& Identifier, bool Safe) 
{
	if (!Safe) 
	{
		size_t id = Resources[Identifier];
		return ModelManager.Store[id].ResourcePtr;
	}

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return ModelManager.Store[it->second].ResourcePtr;

	return nullptr;
}


GrvtModel* ResourceHandler::GetModel(size_t Id, bool Safe) 
{
	if (!Safe)
		ModelManager.Store[Id].ResourcePtr;

	auto it = ModelManager.Store.find(Id);
	if (it != ModelManager.Store.end())
		return ModelManager.Store[Id].ResourcePtr;

	return nullptr;
}


EngineResource<GrvtModel>* ResourceHandler::GetModelHandle(const String& Identifier, bool Safe) 
{
	if (!Safe)
		return &ModelManager.Store[Resources[Identifier]];

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return &ModelManager.Store[Resources[Identifier]];

	return nullptr;
}


bool ResourceHandler::DeleteModel(const String& Identifier, bool Force) 
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
	ModelManager.DeleteResource(Resources[Identifier]);
	Resources.erase(Identifier);

	return true;
}


bool ResourceHandler::DeleteModel(size_t Id, bool Force) 
{
	GrvtModel* model = GetModel(Id);
	if (!model)
		return false;

	return DeleteModel(ModelManager.Store[Id].Name, Force);
}


GrvtTexture* ResourceHandler::NewImportTexture(const TextureImportInfo& Import) 
{
	if (Import.Path.Length() != 1)
		return nullptr;

	if (CheckIfTextureExist(Import.Path[0]))
		return GetTexture(Import.Name);

	size_t id = GenerateResourceId<GrvtTexture>(GrvtResource_Type_Texture);
	Resources.emplace(Import.Name, id);

	GrvtTexture* texture = TextureManager.NewResource(id, Import.Name);
	texture->Alloc(Import);

	stbi_set_flip_vertically_on_load(true);
	texture->DataPtr = (uint8*)stbi_load(Import.Path[0].C_Str(), &texture->Width, &texture->Height, &texture->Channel, 0);
	TextureManager.Store[id].Type = GrvtResourceAlloc_Type_Import;

	Middleware::PackageTextureForBuild(texture);

	return texture;
}


GrvtTexture* ResourceHandler::GetTexture(const String& Identifier, bool Safe) 
{
	if (!Safe) 
	{
		size_t id = Resources[Identifier];
		return TextureManager.Store[id].ResourcePtr;
	}

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return TextureManager.Store[Resources[Identifier]].ResourcePtr;

	return nullptr;
}


GrvtTexture* ResourceHandler::GetTexture(size_t Id, bool Safe) 
{
	if (!Safe)
		return TextureManager.Store[Id].ResourcePtr;

	auto it = TextureManager.Store.find(Id);
	if (it != TextureManager.Store.end())
		return TextureManager.Store[Id].ResourcePtr;

	return nullptr;
}


EngineResource<GrvtTexture>* ResourceHandler::GetTextureHandle(const String& Identifier, bool Safe) 
{
	if (!Safe)
		return &TextureManager.Store[Resources[Identifier]];

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return &TextureManager.Store[Resources[Identifier]];

	return nullptr;
}


bool ResourceHandler::DeleteTexture(const String& Identifier, bool Force) 
{
	EngineResource<GrvtTexture>* Handle = GetTextureHandle(Identifier);
	if (!Handle)
		return false;

	if (!Force && Handle->RefCount)
		return false;

	Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Handle->ResourcePtr->Handle), Middleware::GrvtGfx_Type_Texture);
	Handle->ResourcePtr->Free();

	TextureManager.DeleteResource(Resources[Identifier]);
	Resources.erase(Identifier);

	return true;
}


bool ResourceHandler::DeleteTexture(size_t Id, bool Force) 
{
	GrvtTexture* Texture = GetTexture(Id);
	if (!Texture)
		return false;

	return DeleteTexture(TextureManager.Store[Id].Name, Force);
}


GrvtShader* ResourceHandler::NewShaderProgram(const ShaderImportInfo& Import) 
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

	GrvtShader* shader = ShaderManager.NewResource(id, Import.Name);
	shader->Alloc(Import);

	ShaderManager.Store[id].Type = (imported) ? GrvtResourceAlloc_Type_Import : GrvtResourceAlloc_Type_Custom;
	Middleware::PackageShaderForBuild(shader);

	return shader;
}


GrvtShader* ResourceHandler::GetShader(const String& Identifier, bool Safe) 
{
	if (!Safe)
		return ShaderManager.Store[Resources[Identifier]].ResourcePtr;

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return ShaderManager.Store[Resources[Identifier]].ResourcePtr;

	return nullptr;
}


GrvtShader* ResourceHandler::GetShader(size_t Id, bool Safe) 
{
	if (!Safe)
		return ShaderManager.Store[Id].ResourcePtr;

	auto it = ShaderManager.Store.find(Id);
	if (it != ShaderManager.Store.end())
		return ShaderManager.Store[Id].ResourcePtr;

	return nullptr;
}


EngineResource<GrvtShader>* ResourceHandler::GetShaderHandle(const String& Identifier, bool Safe) 
{
	if (!Safe)
		return &ShaderManager.Store[Resources[Identifier]];

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return &ShaderManager.Store[Resources[Identifier]];

	return nullptr;
}


bool ResourceHandler::DeleteShader(const String& Identifier, bool Force) 
{
	EngineResource<GrvtShader>* Handle = GetShaderHandle(Identifier);
	if (!Handle)
		return false;

	if (!Force && Handle->RefCount)
		return false;

	Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Handle->ResourcePtr->Handle), Middleware::GrvtGfx_Type_Shader);
	Handle->ResourcePtr->Free();

	ShaderManager.DeleteResource(Resources[Identifier]);
	Resources.erase(Identifier);

	return true;
}


bool ResourceHandler::DeleteShader(size_t Id, bool Force) 
{
	GrvtShader* shader = GetShader(Id);
	if (!shader)
		return false;

	return DeleteShader(ShaderManager.Store[Id].Name.C_Str(), Force);
}


GrvtMaterial* ResourceHandler::NewMaterial(const MaterialCreationInfo& Info) 
{
	size_t id = GenerateResourceId<GrvtMaterial>(GrvtResource_Type_Material);
	Resources.insert({Info.Name, id});
	
	GrvtMaterial* material = MaterialManager.NewResource(id, Info.Name);
	material->Alloc(Info);
	MaterialManager.Store[id].Type = GrvtResourceAlloc_Type_Custom;

	// Increase the shader  ref count by 1.
	for (auto& It : ShaderManager.Store) 
	{
		if (Info.Shader != It.second.ResourcePtr)
		{
			continue;
		}

		It.second.RefCount++;
		break;
	}

	// Increase the textures used ref counts by 1.
	for (auto& It : TextureManager.Store) 
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


GrvtMaterial* ResourceHandler::GetMaterial(const String& Identifier, bool Safe) 
{
	if (!Safe)
		return MaterialManager.Store[Resources[Identifier]].ResourcePtr;

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return MaterialManager.Store[Resources[Identifier]].ResourcePtr;

	return nullptr;
}


GrvtMaterial* ResourceHandler::GetMaterial(size_t Id, bool Safe) 
{
	if (!Safe)
		return MaterialManager.Store[Id].ResourcePtr;

	auto it = MaterialManager.Store.find(Id);
	if (it != MaterialManager.Store.end())
		return MaterialManager.Store[Id].ResourcePtr;

	return nullptr;
}


EngineResource<GrvtMaterial>* ResourceHandler::GetMaterialHandle(const String& Identifier, bool Safe) 
{
	if (!Safe)
		return &MaterialManager.Store[Resources[Identifier]];

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return &MaterialManager.Store[Resources[Identifier]];

	return nullptr;
}


bool ResourceHandler::DeleteMaterial(const String& Identifier, bool Force) 
{
	EngineResource<GrvtMaterial>* handle = GetMaterialHandle(Identifier);
	if (!handle)
		return false;

	if (!Force && handle->RefCount)
		return false;

	MaterialManager.DeleteResource(Resources[Identifier]);
	Resources.erase(Identifier);

	return true;
}


bool ResourceHandler::DeleteMaterial(size_t Id, bool Force) 
{
	GrvtMaterial* material = GetMaterial(Id);
	if (!material)
		return false;

	return DeleteMaterial(MaterialManager.Store[Id].Name.C_Str(), Force);
}


GrvtFramebuffer* ResourceHandler::NewFramebuffer(const FramebufferCreationInfo& Info) 
{
	size_t id = GenerateResourceId<GrvtFramebuffer>(GrvtResource_Type_Framebuffer);
	Resources.emplace(Info.Name, id);

	GrvtFramebuffer* framebuffer = FramebufferManager.NewResource(id, Info.Name);
	framebuffer->Alloc(Info);

	FramebufferManager.Store[id].Type = GrvtResourceAlloc_Type_Custom;
	
	// Send the framebuffer to the GPU for it to be created in the context.
	Middleware::PackageFramebufferForBuild(framebuffer);

	return framebuffer;
}


GrvtFramebuffer* ResourceHandler::GetFramebuffer(const String& Identifier, bool Safe) 
{
	if (!Safe)
		FramebufferManager.Store[Resources[Identifier]].ResourcePtr;

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return FramebufferManager.Store[Resources[Identifier]].ResourcePtr;

	return nullptr;
}


GrvtFramebuffer* ResourceHandler::GetFramebuffer(size_t Id, bool Safe) 
{
	if (!Safe)
		FramebufferManager.Store[Id].ResourcePtr;

	auto it = FramebufferManager.Store.find(Id);
	if (it != FramebufferManager.Store.end())
		return FramebufferManager.Store[Id].ResourcePtr;

	return nullptr;
}


EngineResource<GrvtFramebuffer>* ResourceHandler::GetFramebufferHandle(const String& Identifier, bool Safe) 
{
	if (!Safe)
		return &FramebufferManager.Store[Resources[Identifier]];

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return &FramebufferManager.Store[Resources[Identifier]];

	return nullptr;
}


bool ResourceHandler::DeleteFramebuffer(const String& Identifier, bool Force) 
{
	EngineResource<GrvtFramebuffer>* handle = GetFramebufferHandle(Identifier);
	if (!handle)
		return false;

	if (!Force && FramebufferManager.Store[Resources[Identifier]].RefCount)
		return false;

	// TODO(Afiq):
	// Rewrite this portion of the code.
	//for (GrvtFramebuffer::AttachmentProperty& attachment : handle->ResourcePtr->Attachments) 
	//{
	//	if (attachment.Component == GrvtFramebuffer_AttachComponent_Texture) 
	//	{
	//		Middleware::GetBuildQueue()->QueueHandleForDelete(attachment.Handle, Middleware::GrvtGfx_Type_Texture);
	//		continue;
	//	}
	//	Middleware::GetBuildQueue()->QueueHandleForDelete(attachment.Handle, Middleware::GrvtGfx_Type_RenderBuffer);
	//}
	//Middleware::GetBuildQueue()->QueueHandleForDelete(handle->ResourcePtr->Handle, Middleware::GrvtGfx_Type_Framebuffer);

	handle->ResourcePtr->Free();
	FramebufferManager.DeleteResource(Resources[Identifier]);
	Resources.erase(Identifier);

	return true;
}


bool ResourceHandler::DeleteFramebuffer(size_t Id, bool Force) 
{
	GrvtFramebuffer* framebuffer = GetFramebuffer(Id);
	if (!framebuffer)
		return false;

	return DeleteFramebuffer(FramebufferManager.Store[Id].Name, Force);
}