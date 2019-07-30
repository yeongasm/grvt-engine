#include "stdafx.h"

/**
* TODO(Afiq):
* Some of the logging functionalities inside of the functions below need to be properly sentenced.
*/



TextureData::TextureData() : id{}, name{}, file{},
	directory{}, texture{} {}


TextureData::TextureData(const TextureCreationInfo &Info) : id(0), name(Info.name),
	file(Info.files[0]), directory(Info.directory), texture(new Texture()) {}


void TextureData::Alloc(const TextureCreationInfo &Info) {
	id			= 0;
	name		= Info.name;
	file		= Info.files[0];
	directory	= Info.directory;
	texture		= new Texture();
}


void TextureData::Free() {
	if (texture)
		delete texture;

	id = 0;
	name.Release();
	file.Release();
	directory.Release();
}


TextureData::~TextureData() { Free(); }


ShaderData::ShaderData() : id{}, name{}, vertexFile{}, fragmentFile{}, 
	geometryFile{}, directory{}, shader{} {}


ShaderData::ShaderData(const ShaderCreationInfo &Info) : id(0), name(Info.name), vertexFile(Info.vertexShader),
	fragmentFile(Info.fragmentShader), geometryFile(Info.geometryShader), directory(Info.directory), shader(new Shader()) {}


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
	if (shader)
		delete shader;

	// new (this) ShaderData();
}


SceneData::SceneData() : id{}, file{}, 
	directory{}, scene{} {}


SceneData::SceneData(const SceneCreationInfo &Info) : id(0), name(Info.name),
file(Info.file), directory(Info.directory), scene(new Scene()) {}


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


MaterialData::MaterialData() : id{}, name{}, material{} {}


MaterialData::~MaterialData() { Free(); }


void MaterialData::Alloc(const MaterialCreationInfo &Info) {
	name = Info.name;
	material = new Material();
}


void MaterialData::Free() {
	id = 0;
	name.Release();
	delete material;
}


SceneryData::SceneryData() : id{}, name{}, directory{}, filename{}, level{} {}


SceneryData::~SceneryData() { Free(); }


void SceneryData::Alloc(const LevelCreationInfo &Info) {
	name		= Info.name;
	directory	= Info.directory;
	filename	= Info.filename;

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


uint ResourceManager::GenerateResourceID() {
	static uint lastID = 0;
	return lastID++;
}


void ResourceManager::ProcessNode(Scene *Scene, aiNode *AiNode, const aiScene *AiScene, const SceneCreationInfo &Info) {
	aiMesh	*assimpMesh = nullptr;
	

	for (uint i = 0; i < AiScene->mNumMeshes; i++) {
		 assimpMesh = AiScene->mMeshes[i];

		 BuildMesh(Scene, assimpMesh, AiScene);
	}

	//for (uint i = 0; i < AiNode->mNumChildren; i++)
	//	ProcessNode(Scene, AiNode->mChildren[i], AiScene, Info);
}


Mesh* ResourceManager::BuildMesh(Scene *Scene, aiMesh *AiMesh, const aiScene *AiScene) {
	String msg;

	msg.SetString("Creating a new mesh into the scene.");
	Logger::Log(LOG_INFO, LOG_SCENE, msg);
	Mesh *mesh = &Scene->meshes.Insert(Mesh());
	
	msg.SetString("Parsing mesh data from Assimp into scene.");
	Logger::Log(LOG_INFO, LOG_SCENE, msg);

	mesh->positions.Reserve(AiMesh->mNumVertices, false);
	mesh->normals.Reserve(AiMesh->mNumVertices, false);

	if (AiMesh->mNumUVComponents) {
		mesh->uv.Reserve(AiMesh->mNumVertices, false);
		mesh->tangents.Reserve(AiMesh->mNumVertices, false);
		mesh->bitangents.Reserve(AiMesh->mNumVertices, false);
	}

	mesh->indices.Reserve((size_t)(AiMesh->mNumFaces * 3));

	glm::vec3 vector;
	glm::vec2 uv;

	for (uint i = 0; i < AiMesh->mNumVertices; i++) {
		vector.x = AiMesh->mVertices[i].x;
		vector.y = AiMesh->mVertices[i].y;
		vector.z = AiMesh->mVertices[i].z;

		mesh->positions.Push(vector);

		if (AiMesh->mNormals) {
			vector.x = AiMesh->mNormals[i].x;
			vector.y = AiMesh->mNormals[i].y;
			vector.z = AiMesh->mNormals[i].z;

			mesh->normals.Push(vector);
		}

		if (AiMesh->mTextureCoords[0]) {
			uv.x = AiMesh->mTextureCoords[0][i].x;
			uv.y = AiMesh->mTextureCoords[0][i].y;

			mesh->uv.Push(uv);
		}

		if (AiMesh->mTangents) {
			vector.x = AiMesh->mTangents[i].x;
			vector.y = AiMesh->mTangents[i].y;
			vector.z = AiMesh->mTangents[i].z;

			mesh->tangents.Push(vector);

			vector.x = AiMesh->mBitangents[i].x;
			vector.y = AiMesh->mBitangents[i].y;
			vector.z = AiMesh->mBitangents[i].z;

			mesh->bitangents.Push(vector);
		}
	}
	
	for (uint i = 0; i < AiMesh->mNumFaces; i++) {
		aiFace assimpFace = AiMesh->mFaces[i];

		for (uint j = 0; j < assimpFace.mNumIndices; j++)
			mesh->indices.Push(assimpFace.mIndices[j]);
	}

	msg.SetString("Building mesh into Gravity.");
	Logger::Log(LOG_INFO, LOG_SCENE, msg);

	msg.SetString("Build complete!");
	Logger::Log(LOG_INFO, LOG_SCENE, msg);

	return mesh;
}


Scene* ResourceManager::NewScene(const SceneCreationInfo &Info) {
	String path("%s/%s", ~Info.directory, ~Info.file);
	String msg;

	// Check for Scene with existing filename. We do not check the directory where the file is placed at.
	// NOTE(Afiq): This should really start becoming a function. Maybe in the future.
	for (auto &pair : scenes) {
		if (pair.second->file != Info.file)
			continue;

		msg.Write("Scene with file named [%s] already exist!. Returning existing scene instead.", ~Info.name);
		Logger::Log(LOG_WARN, LOG_SCENE, msg);

		return pair.second->scene;
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

	auto it = scenes.emplace(Info.name, new SceneData()).first;

	SceneData *data = it->second;
	data->id = GenerateID<Scene>();
	data->Alloc(Info);
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
	for (auto &pair : shaders) {
		if (pair.second->vertexFile == Info.vertexShader)
			count++;

		if (pair.second->fragmentFile == Info.fragmentShader)
			count++;

		if (Info.geometryShader.Length()) {
			if (pair.second->geometryFile == Info.geometryShader)
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
	for (auto &pair : shaders) {
		if (pair.second->name == Info.name) {
			Logger::Log(LOG_WARN, LOG_SHADER, "Shader with the specified name already exist. Shader name must be unique.");
			return nullptr;
		}
	}

	// 4. Create the shader.
	auto it = shaders.emplace(Info.name, new ShaderData()).first;
	ShaderData *data = it->second;
	
	data->Alloc(Info);
	data->id = GenerateID<Shader>();
	data->shader->info = data;

	if (!data->shader->Alloc(Info)) {
		Logger::Log(LOG_ERR, LOG_SHADER, "Unable to create shader.");
		
		delete data;
		shaders[Info.name] = nullptr;
		shaders.erase(it);

		return nullptr;
	}

	return data->shader;
}


Texture* ResourceManager::NewTexture(const TextureCreationInfo &Info) {
	String msg;
	// 1. Check if specified files already exist inside Gravity as a texture.
	// NOTE: We are generating a texture and not a cubemap. Best to seggregate different features.
	for (auto &pair : textures) {
		if (pair.second->file == Info.files[0]) {
			msg.Write("Texture with file [%s] already exist!", ~Info.files[0]);
			Logger::Log(LOG_WARN, LOG_TEXTURE, ~msg);
			return nullptr;
		}
	}

	// 2. Check if name exist in TextureCreationInfo struct.
	if (!Info.name.Length()) {
		msg.SetString("A name must be specified for the texture. Aborting operation.");
		Logger::Log(LOG_WARN, LOG_TEXTURE, msg);
		return nullptr;
	}

	// 3. Check for textures with similar name.
	for (auto &pair : textures) {
		if (pair.second->name == Info.name) {
			msg.Write("Texture with name [%s] already exist! Texture name must be unique", ~Info.name);
			Logger::Log(LOG_WARN, LOG_TEXTURE, msg);
			return pair.second->texture;
		}
	}

	// 4. Ensure that only 1 file is specified for a texture.
	if (Info.files.Length() > 1) {
		msg.SetString("More than 1 texture file is specified! Unable to create a texture with multiple texture files.");
		Logger::Log(LOG_WARN, LOG_TEXTURE, msg);
		return nullptr;
	}

	auto it = textures.emplace(Info.name, new TextureData()).first;
	TextureData *data = it->second;

	data->id = GenerateID<Texture>();
	data->Alloc(Info);
	data->texture->info = data;

	//db->texture = new Texture();
	if (!data->texture->Alloc(Info)) {
		msg.SetString("Unable to allocate new texture. Exiting operation");
		Logger::Log(LOG_ERR, LOG_TEXTURE, msg);
		//delete db->texture;
		delete data;
		textures[Info.name] = nullptr;

		textures.erase(it);

		return nullptr;
	}

	return data->texture;
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
	for (auto &pair : materials) {
		if (pair.second->name == Info.name) {
			msg.SetString("Material name must be unique. Material with such name already exist in engine.");
			Logger::Log(LOG_WARN, LOG_MATERIAL, msg);

			return nullptr;
		}
	}

	auto it = materials.emplace(Info.name, new MaterialData()).first;
	MaterialData *data = it->second;

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
	for (auto &pair : levels) {
		if (pair.second->name == Info.name) {
			msg.SetString("Level name must be unique. Level with such name already exist in engine.");
			Logger::Log(LOG_WARN, LOG_LEVEL, msg);

			return nullptr;
		}
	}

	auto it = levels.emplace(Info.name, new SceneryData()).first;
	SceneryData *data = it->second;

	data->id = GenerateID<Scenery>();
	data->Alloc(Info);
	data->level->info = data;

	return data->level;
}


Scene* ResourceManager::GetScene(const String &Name) {
	Scene *pScene = nullptr;
	
	String msg("Attempting to retrieve [%s] scene from engine.", ~Name);
	Logger::Log(LOG_INFO, LOG_SCENE, msg);

	auto it = scenes.find(Name);

	if (it != scenes.end())
		pScene = it->second->scene;

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

	auto it = shaders.find(Name);

	if (it != shaders.end())
		pShader = it->second->shader;

	if (!pShader) {
		msg.Write("Unable to retrieve [%s] shader from engine", ~Name);
		Logger::Log(LOG_ERR, LOG_SHADER, msg);
	} else {
		msg.SetString("Shader retrieved from engine!");
		Logger::Log(LOG_INFO, LOG_SHADER, msg);
	}
	
	return pShader;
}


Texture* ResourceManager::GetTexture(const String &Name) {
	Texture *pTexture = nullptr;

	String msg("Attempting to retrieve [%s] texture from engine.", ~Name);
	Logger::Log(LOG_INFO, LOG_TEXTURE, msg);

	auto it = textures.find(Name);

	if (it != textures.end())
		pTexture = it->second->texture;

	if (!pTexture) {
		msg.Write("Unable to retrieve [%s] texture from engine", ~Name);
		Logger::Log(LOG_ERR, LOG_TEXTURE, msg);
	} else {
		msg.SetString("Texture retrieved from engine!");
		Logger::Log(LOG_INFO, LOG_TEXTURE, msg);
	}

	return pTexture;
}


Material* ResourceManager::GetMaterial(const String &Name) {
	Material *pMaterial = nullptr;

	String msg("Attempting to retrieve [%s] material from engine.", ~Name);
	Logger::Log(LOG_INFO, LOG_MATERIAL, msg);

	auto it = materials.find(Name);

	if (it != materials.end())
		pMaterial = it->second->material;

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

	auto it = levels.find(Name);

	if (it != levels.end())
		pLevel = it->second->level;

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

	auto it = scenes.find(Name);

	if (it == scenes.end()) {
		msg.SetString("Unable to find scene with such name. Probably does not exist in engine's DB.");
		Logger::Log(LOG_ERR, LOG_SCENE, msg);
		return false;
	}

	delete it->second;
	scenes.erase(it);

	msg.SetString("Scene deleted!");
	Logger::Log(LOG_INFO, LOG_SCENE, msg);

	return true;
}


bool ResourceManager::DeleteShader(const String &Name) {
	String msg("Attempting to delete [%s] shader.", ~Name);
	Logger::Log(LOG_INFO, LOG_SHADER, msg);

	auto it = shaders.find(Name);

	if (it == shaders.end()) {
		msg.SetString("Unable to find shader with such name. Does not exist within engine.");
		Logger::Log(LOG_ERR, LOG_SHADER, msg);
		return false;
	}

	delete it->second;
	shaders.erase(it);

	msg.SetString("Shader deleted!");
	Logger::Log(LOG_INFO, LOG_SHADER, msg);

	return true;
}


bool ResourceManager::DeleteTexture(const String &Name) {
	String msg("Attempting to delete [%s] texture.", ~Name);
	Logger::Log(LOG_INFO, LOG_TEXTURE, msg);

	auto it = textures.find(Name);

	if (it == textures.end()) {
		msg.SetString("Unable to find texture with such name. Does not exist within engine.");
		Logger::Log(LOG_ERR, LOG_SHADER, msg);
		return false;
	}
	
	delete it->second;
	textures.erase(it);

	msg.SetString("Texture deleted!");
	Logger::Log(LOG_INFO, LOG_SHADER, msg);

	return false;
}


bool ResourceManager::DeleteMaterial(const String &Name) {
	String msg("Attempting to delete [%s] material", ~Name);
	Logger::Log(LOG_INFO, LOG_MATERIAL, msg);

	auto it = materials.find(Name);

	if (it == materials.end()) {
		msg.SetString("Unable to find material with such name. Does not exist within engine.");
		Logger::Log(LOG_ERR, LOG_MATERIAL, msg);
		return false;
	}

	delete it->second;
	materials.erase(it);

	msg.SetString("Material deleted!");
	Logger::Log(LOG_INFO, LOG_MATERIAL, msg);

	return true;
}


bool ResourceManager::DeleteLevel(const String &Name) {
	String msg("Attenmpting to delete [%s] level", ~Name);
	Logger::Log(LOG_INFO, LOG_LEVEL, msg);

	auto it = levels.find(Name);

	if (it == levels.end()) {
		msg.SetString("Unable to find level with such name. Does not exist within engine.");
		Logger::Log(LOG_ERR, LOG_LEVEL, msg);
		return false;
	}

	delete it->second;
	levels.erase(it);

	msg.SetString("Level deleted!");
	Logger::Log(LOG_INFO, LOG_LEVEL, msg);

	return true;
}


void ResourceManager::CleanResource() {
	String msg;
	msg.SetString("Deleting all resources from engine.");
	Logger::Log(LOG_INFO, LOG_MANAGER, msg);

	for (auto &pair : textures) {
		delete pair.second;
		pair.second = nullptr;
	}

	for (auto &pair : shaders) {
		delete pair.second;
		pair.second = nullptr;
	}

	for (auto &pair : scenes) {
		delete pair.second;
		pair.second = nullptr;
	}

	for (auto &pair : materials) {
		delete pair.second;
		pair.second = nullptr;
	}

	for (auto &pair : levels) {
		delete pair.second;
		pair.second = nullptr;
	}

	msg.SetString("Successfully removed all resources from engine.");
	Logger::Log(LOG_INFO, LOG_MANAGER, msg);
}

ResourceManager::ResourceManager() {
	String msg;
	msg.Write("Initialising Gravity's resource manager.");
	Logger::Log(LOG_INFO, LOG_MANAGER, msg);
}

ResourceManager::~ResourceManager() {
	CleanResource();
}