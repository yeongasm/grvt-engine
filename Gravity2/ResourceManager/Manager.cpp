#include "stdafx.h"

/**
* TODO(Afiq):
* Some of the logging functionalities inside of the functions below need to be properly sentenced.
*/



TextureData::TextureData() : Id(), Name(), Path(), Texture(), References() {}


void TextureData::Alloc(const TextureCreationInfo &Info) {
	Id			= 0;
	Name		= Info.Name;
	Path		= Info.Path[0];
	Texture		= new TextureObj();
	
	Texture->Type = Info.Type;
}


void TextureData::Free() {
	for (size_t i = 0; i < References.Length(); i++)
		*References[i] = nullptr;

	if (Texture) {
		delete Texture;
		Texture = nullptr;
	}

	Id = 0;
	Name.Release();
	Path.Release();
	References.Release();
}


TextureData::~TextureData() {}


ShaderData::ShaderData() : id{}, name{}, vertexFile{}, fragmentFile{}, 
	geometryFile{}, directory{}, shader{}, References{} {}


ShaderData::ShaderData(const ShaderCreationInfo &Info) : id(0), name(Info.name), vertexFile(Info.vertexShader), fragmentFile(Info.fragmentShader),
	geometryFile(Info.geometryShader), directory(Info.directory), shader(new ShaderObj()), References{} {}


ShaderData::~ShaderData() { Free(); }


void ShaderData::Alloc(const ShaderCreationInfo &Info) {
	id				= 0;
	name			= Info.name;
	vertexFile		= Info.vertexShader;
	fragmentFile	= Info.fragmentShader;
	directory		= Info.directory;

	geometryFile	= Info.geometryShader;

	shader			= new Shader();
}


void ShaderData::Free() {
	for (size_t i = 0; i < references.Length(); i++)
		*references[i] = nullptr;

	if (shader) {
		delete shader;
		shader = nullptr;
	}

	id = 0;
	name.Release();
	vertexFile.Release();
	fragmentFile.Release();
	geometryFile.Release();
	directory.Release();
	references.Release();
}


SceneData::SceneData() : id{}, file{}, 
	directory{}, scene{} {}


SceneData::~SceneData() { Free(); }


void SceneData::Alloc(const SceneCreationInfo &Info) {
	name		= Info.name;
	file		= Info.file;
	directory	= Info.directory;
	scene		= new Scene();
}


void SceneData::Free() {
	if (scene)
		delete scene;

	name.Release();
	file.Release();
	directory.Release();
}


MaterialData::MaterialData() : id{}, name{}, material{}, references{} {}


MaterialData::~MaterialData() { Free(); }


void MaterialData::Alloc(const MaterialCreationInfo &Info) {
	name = Info.name;
	material = new Material();
}


void MaterialData::Free() {
	for (size_t i = 0; i < references.Length(); i++)
		*references[i] = nullptr;

	id = 0;
	name.Release();

	if (material) {
		delete material;
		material = nullptr;
	}
}


SceneryData::SceneryData() : id{}, name{}, directory{}, filename{}, level{} {}


SceneryData::~SceneryData() { Free(); }


void SceneryData::Alloc(const LevelCreationInfo &Info) {
	name		= Info.name;
	directory	= Info.directory;
	filename	= Info.filename;

	if (!level)
		level = new Scenery();
}


void SceneryData::Free() {
	if (level) {
		delete level;
		level = nullptr;
	}

	name.Release();
	directory.Release();
	filename.Release();
}


RenderBufferData::RenderBufferData() : id{}, name{}, renderbuffer{}, references{} {}


RenderBufferData::~RenderBufferData() { Free(); }


void RenderBufferData::Alloc(const RenderBufferCreationInfo &Info) {
	name = Info.name;
	renderbuffer = new RenderBuffer();
}


void RenderBufferData::Free() {
	name.Release();

	for (size_t i = 0; i < references.Length(); i++)
		*references[i] = nullptr;

	if (renderbuffer)
		delete renderbuffer;
}


PostProcessData::PostProcessData() : id{}, name{}, framebuffer{}, references{} {}


PostProcessData::~PostProcessData() { Free(); }


void PostProcessData::Alloc(const PostProcessCreationInfo &Info) {
	if (!framebuffer) {
		framebuffer = new PostProcess();
		framebuffer->width		= Info.width;
		framebuffer->height		= Info.height;
		framebuffer->attachment = Info.attachment;
	}

	name = Info.name;
}


void PostProcessData::Free() {
	if (framebuffer)
		delete framebuffer;

	for (size_t i = 0; i < references.Length(); i++)
		*references[i] = nullptr;

	name.Release();
}


Scene* ResourceManager::NewScene(const SceneCreationInfo &Info) {
	String path("%s/%s", ~Info.directory, ~Info.file);
	String msg;

	// Check for Scene with existing filename. We do not check the directory where the file is placed at.
	// NOTE(Afiq): This should really start becoming a function. Maybe in the future.
	for (auto &pair : scenes) {
		if (pair->file != Info.file)
			continue;

		msg.Write("Scene with file named [%s] already exist!. Returning existing scene instead.", ~Info.name);
		Logger::Log(LOG_WARN, LOG_SCENE, msg);

		return pair->scene;
	}

	if (GetScene(Info.name)) {
		msg.Write("Scene with name [%s] already exist! Name must be unique. Returning NULL.", ~Info.name);
		Logger::Log(LOG_ERR, LOG_SCENE, msg);

		return nullptr;
	}

	msg.Write("Attempting to load mesh at path: [%s].", ~path);
	Logger::Log(LOG_INFO, LOG_SCENE, msg);

	if (!DoesFileExist(path)) {
		msg.Write("Path [%s] is invalid. Returning NULL.", ~path);
		Logger::Log(LOG_ERR, LOG_SCENE, msg);

		return nullptr;
	}

	SceneData *data = scenes.Insert(new SceneData());

	data->Alloc(Info);
	data->id = GenerateID<Scene>();
	data->scene->info = data;
	data->scene->type = Info.type;

	msg.Write("Building [%s] ... ", ~Info.name);
	Logger::Log(LOG_INFO, LOG_SCENE, msg);

	/**
	* TODO(Afiq):
	* Replace std::thread with Window's API thread wrapper class.
	* There needs to be a tracker to track the state of a thread.
	* Hence, we should create a global thread handler to track the lifetime of the threads.
	*/
	// Now we make pass the process of parsing in the mesh data by a mid layer API. 
	//std::thread thread(Middleware::ParseMeshFromAssimp, path, Info.flipUVs, data->scene);
	//thread.detach();
	Middleware::ParseMeshFromAssimp(path, Info.flipUVs, data->scene);

	return data->scene;
}


Shader* ResourceManager::NewShader(const ShaderCreationInfo &Info) {
	String msg;

	// 1. Check if vertex shader path and fragment shader path are specified.
	if (!Info.vertexShader.Length() || !Info.fragmentShader.Length()) {
		msg.SetString("Must specify vertex or fragment shader.");
		Logger::Log(LOG_WARN, LOG_SHADER, msg);
		return nullptr;
	}

	// 2. Check if the combination of files specified inside of the shader are already present inside Gravity.
	uint count = 0;
	for (ShaderData *data : shaders) {
		if (data->vertexFile == Info.vertexShader)
			count++;

		if (data->fragmentFile == Info.fragmentShader)
			count++;

		if (Info.geometryShader.Length()) {
			if (data->geometryFile == Info.geometryShader)
				count++;

			if (count == 3) {
				Logger::Log(LOG_WARN, LOG_SHADER, "Shader with specified file combination already exist. Cancelling operation");
				return nullptr;
			}
		} else {
			if (count == 2) {
				Logger::Log(LOG_WARN, LOG_SHADER, "Shader with specified file combination already exist. Cancelling operation");
				return nullptr;
			}
		}

		count = 0;
	}

	// 3. Check if specified name already exist.
	for (ShaderData *data : shaders) {
		if (data->name == Info.name) {
			Logger::Log(LOG_WARN, LOG_SHADER, "Shader with the specified name already exist. Shader name must be unique.");
			return nullptr;
		}
	}

	// 4. Create the shader.
	size_t idx = shaders.Push(new ShaderData());
	ShaderData *data = shaders[idx];
	
	data->Alloc(Info);
	data->id = GenerateID<Shader>();
	data->shader->info = data;

	if (!data->shader->Alloc(Info)) {
		Logger::Log(LOG_ERR, LOG_SHADER, "Unable to create shader.");
		
		delete data;
		shaders[idx] = nullptr;
		shaders.PopAt(idx);

		return nullptr;
	}

	return data->shader;
}


TextureObj* ResourceManager::NewTexture(const TextureCreationInfo &Info) {
	TextureData *pData = Textures.Insert(new TextureData());

	pData->Alloc(Info);
	pData->Id = GenerateID<TextureObj>();
	pData->Texture->Info = pData;

	Middleware::ParseTextureFromFile(pData->Path, pData->Texture);

	return pData->Texture;
}


Material* ResourceManager::NewMaterial(const MaterialCreationInfo &Info) {
	String msg;

	// 1. Material must contain a name.
	if (!Info.name.Length()) {
		msg.SetString("A name must be specified for the material. Aborting operation!");
		Logger::Log(LOG_WARN, LOG_MATERIAL, msg);

		return nullptr;
	}

	// 2. Check if material with the existing name exist.
	for (MaterialData *data : materials) {
		if (data->name == Info.name) {
			msg.SetString("Material name must be unique. Material with such name already exist in engine.");
			Logger::Log(LOG_WARN, LOG_MATERIAL, msg);

			return nullptr;
		}
	}

	size_t idx = materials.Push(new MaterialData());
	MaterialData *data = materials[idx];

	data->id = GenerateID<Material>();
	data->Alloc(Info);
	data->material->Alloc(Info);
	data->material->info = data;

	return data->material;
}


Scenery* ResourceManager::NewLevel(const LevelCreationInfo &Info) {
	String msg;

	// 1. Material must contain a name.
	if (!Info.name.Length()) {
		msg.SetString("A name must be specified for the level. Aborting operation!");
		Logger::Log(LOG_WARN, LOG_LEVEL, msg);

		return nullptr;
	}

	// 2. Check if material with the existing name exist.
	for (SceneryData *data : levels) {
		if (data->name == Info.name) {
			msg.SetString("Level name must be unique. Level with such name already exist in engine.");
			Logger::Log(LOG_WARN, LOG_LEVEL, msg);

			return nullptr;
		}
	}

	size_t idx = levels.Push(new SceneryData());
	SceneryData *data = levels[idx];

	data->id = GenerateID<Scenery>();
	data->Alloc(Info);
	data->level->info = data;

	return data->level;
}


PostProcess* ResourceManager::NewPostProcess(const PostProcessCreationInfo &Info) {
	String msg;

	// 1. PostProcess must contain a name.
	if (!Info.name.Length()) {
		msg.SetString("A name must be specified for the framebuffer. Aborting operation!");
		Logger::Log(LOG_WARN, LOG_POSTPROCESS, msg);

		return nullptr;
	}

	// 2. Check if postprocess with the specified name exists.
	for (PostProcessData *data : framebuffers) {
		if (data->name == Info.name) {
			msg.SetString("Framebuffer name must be unique. Framebuffer with such name already exist in engine.");
			Logger::Log(LOG_WARN, LOG_POSTPROCESS, msg);

			return nullptr;
		}
	}
	
	PostProcessData *data = framebuffers.Insert(new PostProcessData());
	data->id = GenerateID<PostProcess>();
	data->Alloc(Info);
	data->framebuffer->info = data;

	// 3. Sends the framebuffer into the Middleware to be built by OpenGL.
	// This phase does not need to multi-threaded as we're only doing a simple operation.
	Middleware::PackageFramebufferForBuild(data->framebuffer);

	return data->framebuffer;
}


RenderBuffer* ResourceManager::NewRenderBuffer(const RenderBufferCreationInfo &Info) {
	String msg;

	// 1. RenderBuffer must contain a name.
	if (!Info.name.Length()) {
		msg.SetString("A name must be specified for the renderbuffer. Aborting operation!");
		Logger::Log(LOG_WARN, LOG_RENDERBUFFER, msg);

		return nullptr;
	}

	// 2. Check if renderbuffer with the specified name exists.
	for (RenderBufferData *data : renderbuffers) {
		if (data->name == Info.name) {
			msg.SetString("Renderbuffer name must be unique. Renderbuffer with such name already exist in engine.");
			Logger::Log(LOG_WARN, LOG_RENDERBUFFER, msg);

			return nullptr;
		}
	}

	RenderBufferData *data = renderbuffers.Insert(new RenderBufferData());
	data->id = GenerateID<RenderBuffer>();
	data->Alloc(Info);
	data->renderbuffer->info = data;

	// 3. Sends the renderbuffer into the Middleware to be built by OpenGL.
	// This phase does not need to be multi-threaded as we are only doing a simple operation.
	Middleware::PackageRenderbufferForBuild(data->renderbuffer);

	return data->renderbuffer;
}


Scene* ResourceManager::GetScene(const String &Name) {
	Scene *pScene = nullptr;
	
	String msg("Attempting to retrieve [%s] scene from engine.", ~Name);
	Logger::Log(LOG_INFO, LOG_SCENE, msg);

	for (size_t i = 0; i < scenes.Length(); i++) {
		if (scenes[i]->name != Name)
			continue;

		pScene = scenes[i]->scene;
		break;
	}

	if (!pScene) {
		msg.Write("Unable to retrieve [%s] scene from engine.", ~Name);
		Logger::Log(LOG_ERR, LOG_SCENE, msg);
	} else {
		msg.SetString("Scene retrieved from engine!");
		Logger::Log(LOG_INFO, LOG_SCENE, msg);
	}
	
	return pScene;
}


Shader* ResourceManager::GetShader(const String &Name) {
	Shader *pShader = nullptr;

	String msg("Attempting to retrieve [%s] shader from engine.", ~Name);
	Logger::Log(LOG_INFO, LOG_SHADER, msg);

	for (size_t i = 0; i < shaders.Length(); i++) {
		if (shaders[i]->name != Name)
			continue;

		pShader = shaders[i]->shader;
		break;
	}

	if (!pShader) {
		msg.Write("Unable to retrieve [%s] shader from engine", ~Name);
		Logger::Log(LOG_ERR, LOG_SHADER, msg);
	} else {
		msg.SetString("Shader retrieved from engine!");
		Logger::Log(LOG_INFO, LOG_SHADER, msg);
	}
	
	return pShader;
}


TextureObj* ResourceManager::GetTexture(const String &Name) {
	TextureObj *pTexture = nullptr;

	for (TextureData *pData : Textures) {
		if (pData->Name != Name)
			continue;

		pTexture = pData->Texture;
		break;
	}

	return pTexture;
}


Material* ResourceManager::GetMaterial(const String &Name) {
	Material *pMaterial = nullptr;

	String msg("Attempting to retrieve [%s] material from engine.", ~Name);
	Logger::Log(LOG_INFO, LOG_MATERIAL, msg);

	for (size_t i = 0; i < materials.Length(); i++) {
		if (materials[i]->name != Name)
			continue;

		pMaterial = materials[i]->material;
		break;
	}

	if (!pMaterial) {
		msg.Write("Unable to retrieve [%s] material from engine", ~Name);
		Logger::Log(LOG_ERR, LOG_MATERIAL, msg);
	} else {
		msg.SetString("Material retrieved from engine!");
		Logger::Log(LOG_INFO, LOG_MATERIAL, msg);
	}

	return pMaterial;
}


Scenery* ResourceManager::GetLevel(const String &Name) {
	Scenery *pLevel = nullptr;

	String msg("Attempting to retrieve [%s] level from engine", ~Name);
	Logger::Log(LOG_INFO, LOG_MATERIAL, msg);

	for (size_t i = 0; i < levels.Length(); i++) {
		if (levels[i]->name != Name)
			continue;

		pLevel = levels[i]->level;
		break;
	}

	if (!pLevel) {
		msg.Write("Unable to retrieve [%s] level from engine", ~Name);
		Logger::Log(LOG_INFO, LOG_LEVEL, msg);
	} else {
		msg.SetString("Level retrieved from engine!");
		Logger::Log(LOG_INFO, LOG_LEVEL, msg);
	}

	return pLevel;
}


bool ResourceManager::DeleteScene(const String &Name) {
	String msg("Attempting to delete [%s] scene.", ~Name);
	Logger::Log(LOG_INFO, LOG_SCENE, msg);

	size_t idx = -1;

	for (size_t i = 0; i < scenes.Length(); i++) {
		if (scenes[i]->name != Name)
			continue;

		idx = i;
		break;
	}

	if (idx == -1) {
		msg.SetString("Unable to find scene with such name. Probably does not exist in engine's DB.");
		Logger::Log(LOG_ERR, LOG_SCENE, msg);
		return false;
	}

	// Remove all SceneInstance that refers to this Scene from all Levels.
	for (SceneryData *data : levels)
		data->level->PopAllInstanceWithScene(scenes[idx]->scene);

	delete scenes[idx];
	scenes.PopAt(idx);

	msg.SetString("Scene deleted!");
	Logger::Log(LOG_INFO, LOG_SCENE, msg);

	return true;
}


bool ResourceManager::DeleteShader(const String &Name) {
	String msg("Attempting to delete [%s] shader.", ~Name);
	Logger::Log(LOG_INFO, LOG_SHADER, msg);

	size_t idx = -1;

	for (size_t i = 0; i < shaders.Length(); i++) {
		if (shaders[i]->name != Name)
			continue;

		idx = i;
		break;
	}


	if (idx == -1) {
		msg.SetString("Unable to find shader with such name. Does not exist within engine.");
		Logger::Log(LOG_ERR, LOG_SHADER, msg);
		return false;
	}

	//// Remove all reference to this Shader from SceneInstances.
	//for (SceneData *data : scenes) {
	//	for (SceneInstance &instance : data->scene->instances) {
	//		if (instance.shader != shaders[idx]->shader)
	//			continue;

	//		instance.shader = nullptr;
	//	}
	//}

	//// Remove all reference to this Shader from Materials.
	//for (MaterialData *data : materials) {
	//	data->material->shader = nullptr;
	//	data->material->uniforms.clear();
	//}

	delete shaders[idx];
	shaders.PopAt(idx);

	msg.SetString("Shader deleted!");
	Logger::Log(LOG_INFO, LOG_SHADER, msg);

	return true;
}


void ResourceManager::DeleteTexture(TextureObj *&Texture) {
	// Remove the texture object from the GPU.
	Middleware::GetBuildQueue()->AddHandleForDelete(Texture->Handle, GFX_TYPE_TEXTUREID);

	// Get the data handle and remove the texture from memory.
	TextureData *pData = Texture->Info;
	size_t index = Textures.Find(pData);
	pData->Free();

	// Delete the data handle and remove it from the array.
	delete pData;
	Textures.PopAt(index);

	Texture = nullptr;
}


bool ResourceManager::DeleteMaterial(const String &Name) {
	String msg("Attempting to delete [%s] material", ~Name);
	Logger::Log(LOG_INFO, LOG_MATERIAL, msg);

	size_t idx = -1;

	for (size_t i = 0; i < materials.Length(); i++) {
		if (materials[i]->name != Name)
			continue;

		idx = i;
		break;
	}


	if (idx == -1) {
		msg.SetString("Unable to find material with such name. Does not exist within engine.");
		Logger::Log(LOG_ERR, LOG_MATERIAL, msg);
		return false;
	}

	delete materials[idx];
	materials.PopAt(idx);

	msg.SetString("Material deleted!");
	Logger::Log(LOG_INFO, LOG_MATERIAL, msg);

	return true;
}


bool ResourceManager::DeleteLevel(const String &Name) {
	String msg("Attenmpting to delete [%s] level", ~Name);
	Logger::Log(LOG_INFO, LOG_LEVEL, msg);

	size_t idx = -1;

	for (size_t i = 0; i < levels.Length(); i++) {
		if (levels[i]->name != Name)
			continue;

		idx = i;
		break;
	}


	if (idx == -1) {
		msg.SetString("Unable to find level with such name. Does not exist within engine.");
		Logger::Log(LOG_ERR, LOG_LEVEL, msg);
		return false;
	}

	delete levels[idx];
	levels.PopAt(idx);

	msg.SetString("Level deleted!");
	Logger::Log(LOG_INFO, LOG_LEVEL, msg);

	return true;
}


void ResourceManager::CleanResource() {

	for (TextureData *pData : Textures)
		pData->Free();

	for (size_t i = 0; i < shaders.Length(); i++) {
		delete shaders[i];
		shaders[i] = nullptr;
	}

	for (size_t i = 0; i < scenes.Length(); i++) {
		delete scenes[i];
		scenes[i] = nullptr;
	}

	for (size_t i = 0; i < materials.Length(); i++) {
		delete materials[i];
		materials[i] = nullptr;
	}

	for (size_t i = 0; i < levels.Length(); i++) {
		delete levels[i];
		levels[i] = nullptr;
	}

	Textures.Empty(); shaders.Empty(); scenes.Empty(); materials.Empty(); levels.Empty();
}

ResourceManager::ResourceManager() {
	Textures.Reserve(256);
	shaders.Reserve(256);
	scenes.Reserve(256);
	materials.Reserve(256);
	levels.Reserve(256);
}

ResourceManager::~ResourceManager() {
	CleanResource();
}