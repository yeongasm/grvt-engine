#include "stdafx.h"


static ResourceManager<GrvtModel>		ModelManager;
static ResourceManager<GrvtTexture>		TextureManager;
static ResourceManager<GrvtShader>		ShaderManager;
static ResourceManager<GrvtPostProcess> PostProcessManager;


bool ResourceHandler::CheckIfModelExist(const String& Path) {
	for (auto& [key, value] : ModelManager.Store)
		if (value.Path == Path)
			return true;

	return false;
}


bool ResourceHandler::CheckIfTextureExist(const String& Path) {
	for (auto& [key, value] : TextureManager.Store)
		if (value.Path == Path)
			return true;

	return false;
}


ResourceHandler::ResourceHandler() :
	Resources() {}


ResourceHandler::~ResourceHandler() {}


void ResourceHandler::Alloc(size_t Reserve) {
	// The main handler needs to allocate 5 times more than a manager of a single type of asset.
	Resources.reserve(Reserve * 5);

	ModelManager.Alloc(Reserve);
	TextureManager.Alloc(Reserve);
	ShaderManager.Alloc(Reserve);
	//MaterialManager.Alloc(Reserve);
	PostProcessManager.Alloc(Reserve);
}


void ResourceHandler::Free() {
	ResourceType type = GrvtResource_Type_None;

	for (auto& [key, value] : Resources) {
		type = GetResourceType(key);

		switch (type) {
		case GrvtResource_Type_Model:
			DeleteModel(key, true);
			break;
		case GrvtResource_Type_Texture:
			DeleteTexture(key, true);
			break;
		case GrvtResource_Type_Shader:
			DeleteShader(key, true);
			break;
		//case GrvtResource_Type_Material:
		//	DeleteMaterial(key);
		//	break;
		case GrvtResource_Type_Framebuffer:
			DeletePostProcessing(key, true);
			break;
		default:
			break;
		}
	}
}


ResourceType ResourceHandler::GetResourceType(const String& Identifier) {
	auto it = Resources.find(Identifier);
	if (it == Resources.end())
		return GrvtResource_Type_None;

	size_t type = it->second >> (sizeof(size_t) * 8 - 4);
	return *(ResourceType*)&type;
}


GrvtModel* ResourceHandler::NewImportModel(const ModelImportInfo& Import) {
	if (CheckIfModelExist(Import.Path))
		return GetModel(Import.Name);

	size_t id = GenerateResourceId<GrvtModel>(GrvtResource_Type_Model);
	Resources.insert({Import.Name, id});
	
	GrvtModel* model = ModelManager.NewResource(id, Import.Name);
	model->Alloc(Import);

	ModelManager.Store[id].Path = Import.Path;
	ModelManager.Store[id].Type = GrvtResourceAlloc_Type_Import;
	
	// Implement import model here.
	Util::AssimpImportModelFromPath(Import.Path, model);

	return model;
}


GrvtModel* ResourceHandler::GetModel(const String& Identifier, bool Safe) {
	if (!Safe) {
		size_t id = Resources[Identifier];
		return ModelManager.Store[id].ResourcePtr;
	}

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return ModelManager.Store[it->second].ResourcePtr;

	return nullptr;
}


GrvtModel* ResourceHandler::GetModel(size_t Id, bool Safe) {
	if (!Safe)
		ModelManager.Store[Id].ResourcePtr;

	auto it = ModelManager.Store.find(Id);
	if (it != ModelManager.Store.end())
		return ModelManager.Store[Id].ResourcePtr;

	return nullptr;
}


EngineResource<GrvtModel>* ResourceHandler::GetModelHandle(const String& Identifier, bool Safe) {
	if (!Safe)
		return &ModelManager.Store[Resources[Identifier]];

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return &ModelManager.Store[Resources[Identifier]];

	return nullptr;
}


bool ResourceHandler::DeleteModel(const String& Identifier, bool Force) {
	EngineResource<GrvtModel>* handle = GetModelHandle(Identifier);
	if (!handle)
		return false;

	if (!Force && handle->RefCount)
		return false;

	for (GrvtMesh& mesh : handle->ResourcePtr->Meshes) {
		Middleware::GetBuildQueue()->QueueHandleForDelete(mesh.Vao, Middleware::GrvtGfx_Type_VertexArray);
		Middleware::GetBuildQueue()->QueueHandleForDelete(mesh.Vbo, Middleware::GrvtGfx_Type_MeshBuffer);
		Middleware::GetBuildQueue()->QueueHandleForDelete(mesh.Ebo, Middleware::GrvtGfx_Type_MeshBuffer);
	}

	handle->ResourcePtr->Free();
	ModelManager.DeleteResource(Resources[Identifier]);
	Resources.erase(Identifier);

	return true;
}


bool ResourceHandler::DeleteModel(size_t Id, bool Force) {
	GrvtModel* model = GetModel(Id);
	if (!model)
		return false;

	return DeleteModel(ModelManager.Store[Id].Name, Force);
}


GrvtTexture* ResourceHandler::NewImportTexture(const TextureImportInfo& Import) {
	if (Import.Path.Length() != 1)
		return nullptr;

	if (CheckIfTextureExist(Import.Path[0]))
		return GetTexture(Import.Name);

	size_t id = GenerateResourceId<GrvtTexture>(GrvtResource_Type_Texture);
	Resources.insert({Import.Name, id});

	GrvtTexture* texture = TextureManager.NewResource(id, Import.Name);
	texture->Alloc(Import);

	stbi_set_flip_vertically_on_load(true);
	texture->Properties[0].DataPtr = (uint8*)stbi_load(Import.Path[0].c_str(), &texture->Properties[0].Width, &texture->Properties[0].Height, &texture->Properties[0].Channel, 0);
	TextureManager.Store[id].Type = GrvtResourceAlloc_Type_Import;

	Middleware::PackageTextureForBuild(texture);

	return texture;
}


GrvtTexture* ResourceHandler::GetTexture(const String& Identifier, bool Safe) {
	if (!Safe) {
		size_t id = Resources[Identifier];
		return TextureManager.Store[id].ResourcePtr;
	}

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return TextureManager.Store[Resources[Identifier]].ResourcePtr;

	return nullptr;
}


GrvtTexture* ResourceHandler::GetTexture(size_t Id, bool Safe) {
	if (!Safe)
		return TextureManager.Store[Id].ResourcePtr;

	auto it = TextureManager.Store.find(Id);
	if (it != TextureManager.Store.end())
		return TextureManager.Store[Id].ResourcePtr;

	return nullptr;
}


EngineResource<GrvtTexture>* ResourceHandler::GetTextureHandle(const String& Identifier, bool Safe) {
	if (!Safe)
		return &TextureManager.Store[Resources[Identifier]];

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return &TextureManager.Store[Resources[Identifier]];

	return nullptr;
}


bool ResourceHandler::DeleteTexture(const String& Identifier, bool Force) {
	EngineResource<GrvtTexture>* handle = GetTextureHandle(Identifier);
	if (!handle)
		return false;

	if (!Force && handle->RefCount)
		return false;

	Middleware::GetBuildQueue()->QueueHandleForDelete(handle->ResourcePtr->Handle, Middleware::GrvtGfx_Type_Texture);
	handle->ResourcePtr->Free();

	TextureManager.DeleteResource(Resources[Identifier]);
	Resources.erase(Identifier);

	return true;
}


bool ResourceHandler::DeleteTexture(size_t Id, bool Force) {
	GrvtTexture* texture = GetTexture(Id);
	if (!texture)
		return false;

	return DeleteTexture(TextureManager.Store[Id].Name, Force);
}


GrvtShader* ResourceHandler::NewShaderProgram(const ShaderImportInfo& Import) {
	// To accommodate shader graphs in the future, there is no method to check if the files used are the same.

	// We do however need to check if more than one type of shader component is provided.
	bool imported = false;
	ShaderComponent comp = GrvtShader_SourceType_None;
	for (ShaderProps& prop : Import.Properties) {
		if (comp == prop.Component)
			return nullptr;

		comp = prop.Component;
		imported = (prop.Path.Length()) ? true : false;
	}

	// Create the resource id.
	size_t id = GenerateResourceId<GrvtShader>(GrvtResource_Type_Shader);
	Resources.insert({Import.Name, id});

	GrvtShader* shader = ShaderManager.NewResource(id, Import.Name);
	shader->Alloc(Import);

	ShaderManager.Store[id].Type = (imported) ? GrvtResourceAlloc_Type_Import : GrvtResourceAlloc_Type_Custom;
	Middleware::PackageShaderForBuild(shader);

	return shader;
}


GrvtShader* ResourceHandler::GetShader(const String& Identifier, bool Safe) {
	if (!Safe)
		return ShaderManager.Store[Resources[Identifier]].ResourcePtr;

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return ShaderManager.Store[Resources[Identifier]].ResourcePtr;

	return nullptr;
}


GrvtShader* ResourceHandler::GetShader(size_t Id, bool Safe) {
	if (!Safe)
		return ShaderManager.Store[Id].ResourcePtr;

	auto it = ShaderManager.Store.find(Id);
	if (it != ShaderManager.Store.end())
		return ShaderManager.Store[Id].ResourcePtr;

	return nullptr;
}


EngineResource<GrvtShader>* ResourceHandler::GetShaderHandle(const String& Identifier, bool Safe) {
	if (!Safe)
		return &ShaderManager.Store[Resources[Identifier]];

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return &ShaderManager.Store[Resources[Identifier]];

	return nullptr;
}


bool ResourceHandler::DeleteShader(const String& Identifier, bool Force) {
	EngineResource<GrvtShader>* handle = GetShaderHandle(Identifier);
	if (!handle)
		return false;

	if (!Force && handle->RefCount)
		return false;

	Middleware::GetBuildQueue()->QueueHandleForDelete(handle->ResourcePtr->Handle, Middleware::GrvtGfx_Type_Shader);
	handle->ResourcePtr->Free();

	ShaderManager.DeleteResource(Resources[Identifier]);
	Resources.erase(Identifier);

	return true;
}


bool ResourceHandler::DeleteShader(size_t Id, bool Force) {
	GrvtShader* shader = GetShader(Id);
	if (!shader)
		return false;

	return DeleteShader(ShaderManager.Store[Id].Name, Force);
}


//GrvtMaterial* ResourceHandler::NewMaterial(const MaterialCreationInfo& Info) {
//	// To accommodate shader graphs in the future, there is no method we can check if the same material exist in the system.
//
//	size_t id = GenerateResourceId<GrvtMaterial>(GrvtResource_Type_Material);
//	Resources.insert({Info.Name, id});
//	
//	GrvtMaterial* material = MaterialManager.NewResource(id, Info.Name);
//	material->Alloc(Info);
//	MaterialManager.Store[id].Type = GrvtResourceAlloc_Type_Custom;
//
//	// Increase the shader  ref count by 1.
//	for (auto& [key, value] : ShaderManager.Store) {
//		if (Info.Shader != value.ResourcePtr)
//			continue;
//
//		value.RefCount++;
//		break;
//	}
//
//	// Increase the textures used ref counts by 1.
//	for (auto& [key, value] : TextureManager.Store) {
//		for (TexturePair& pair : Info.Textures) {
//			if (pair.Value == &value.ResourcePtr->Handle)
//				value.RefCount++;
//		}
//	}
//
//
//	return material;
//}
//
//
//GrvtMaterial* ResourceHandler::GetMaterial(const String& Identifier, bool Safe) {
//	if (!Safe)
//		return MaterialManager.Store[Resources[Identifier]].ResourcePtr;
//
//	auto it = Resources.find(Identifier);
//	if (it != Resources.end())
//		return MaterialManager.Store[Resources[Identifier]].ResourcePtr;
//
//	return nullptr;
//}
//
//
//GrvtMaterial* ResourceHandler::GetMaterial(size_t Id, bool Safe) {
//	if (!Safe)
//		return MaterialManager.Store[Id].ResourcePtr;
//
//	auto it = MaterialManager.Store.find(Id);
//	if (it != MaterialManager.Store.end())
//		return MaterialManager.Store[Id].ResourcePtr;
//
//	return nullptr;
//}


GrvtPostProcess* ResourceHandler::NewPostProcessing(const PostProcessCreationInfo& Info) {
	size_t id = GenerateResourceId<GrvtPostProcess>(GrvtResource_Type_Framebuffer);
	Resources.insert({Info.Name, id});

	GrvtPostProcess* framebuffer = PostProcessManager.NewResource(id, Info.Name);
	framebuffer->Alloc(Info);

	PostProcessManager.Store[id].Type = GrvtResourceAlloc_Type_Custom;
	
	// Send the framebuffer to the GPU for it to be created in the context.
	Middleware::PackageFramebufferForBuild(framebuffer);

	return framebuffer;
}


GrvtPostProcess* ResourceHandler::GetPostProcessing(const String& Identifier, bool Safe) {
	if (!Safe)
		PostProcessManager.Store[Resources[Identifier]].ResourcePtr;

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return PostProcessManager.Store[Resources[Identifier]].ResourcePtr;

	return nullptr;
}


GrvtPostProcess* ResourceHandler::GetPostProcessing(size_t Id, bool Safe) {
	if (!Safe)
		PostProcessManager.Store[Id].ResourcePtr;

	auto it = PostProcessManager.Store.find(Id);
	if (it != PostProcessManager.Store.end())
		return PostProcessManager.Store[Id].ResourcePtr;

	return nullptr;
}


EngineResource<GrvtPostProcess>* ResourceHandler::GetPostProcessingHandle(const String& Identifier, bool Safe) {
	if (!Safe)
		return &PostProcessManager.Store[Resources[Identifier]];

	auto it = Resources.find(Identifier);
	if (it != Resources.end())
		return &PostProcessManager.Store[Resources[Identifier]];

	return nullptr;
}


bool ResourceHandler::DeletePostProcessing(const String& Identifier, bool Force) {
	EngineResource<GrvtPostProcess>* handle = GetPostProcessingHandle(Identifier);
	if (!handle)
		return false;

	if (!Force && PostProcessManager.Store[Resources[Identifier]].RefCount)
		return false;

	for (GrvtPostProcess::AttachmentProperty& attachment : handle->ResourcePtr->Attachments) {
		if (attachment.Component == GrvtFramebuffer_AttachComponent_Texture) {
			Middleware::GetBuildQueue()->QueueHandleForDelete(attachment.Handle, Middleware::GrvtGfx_Type_Texture);
			continue;
		}
		Middleware::GetBuildQueue()->QueueHandleForDelete(attachment.Handle, Middleware::GrvtGfx_Type_RenderBuffer);
	}
	Middleware::GetBuildQueue()->QueueHandleForDelete(handle->ResourcePtr->Handle, Middleware::GrvtGfx_Type_PostProcess);

	handle->ResourcePtr->Free();
	PostProcessManager.DeleteResource(Resources[Identifier]);
	Resources.erase(Identifier);

	return true;
}


bool ResourceHandler::DeletePostProcessing(size_t Id, bool Force) {
	GrvtPostProcess* framebuffer = GetPostProcessing(Id);
	if (!framebuffer)
		return false;

	return DeletePostProcessing(PostProcessManager.Store[Id].Name, Force);
}