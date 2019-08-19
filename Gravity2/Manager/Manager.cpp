#include "stdafx.h"

/**
* TODO(Afiq):
* Some of the logging functionalities inside of the functions below need to be properly sentenced.
*/



TextureData::TextureData() : id{}, name{}, file{},
	directory{}, texture{}, references{} {}


TextureData::TextureData(const TextureCreationInfo &Info) : id(0), name(Info.name), file(Info.files[0]), 
	directory(Info.directory), texture(new Texture()), references{} {}


void TextureData::Alloc(const TextureCreationInfo &Info) {
	id			= 0;
	name		= Info.name;
	file		= Info.files[0];
	directory	= Info.directory;
	texture		= new Texture();
}


void TextureData::Free() {
	for (size_t i = 0; i < references.Length(); i++)
		*references[i] = nullptr;

	if (texture) {
		delete texture;
		texture = nullptr;
	}

	id = 0;
	name.Release();
	file.Release();
	directory.Release();
	references.Length();
}


TextureData::~TextureData() { Free(); }


ShaderData::ShaderData() : id{}, name{}, vertexFile{}, fragmentFile{}, 
	geometryFile{}, directory{}, shader{}, references{} {}


ShaderData::ShaderData(const ShaderCreationInfo &Info) : id(0), name(Info.name), vertexFile(Info.vertexShader), fragmentFile(Info.fragmentShader),
	geometryFile(Info.geometryShader), directory(Info.directory), shader(new Shader()), references{} {}


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


Texture* ResourceManager::NewTexture(const TextureCreationInfo &Info) {
	String msg;
	// 1. Check if specified files already exist inside Gravity as a texture.
	// NOTE: We are generating a texture and not a cubemap. Best to seggregate different features.
	for (TextureData *data : textures) {
		if (data->file == Info.files[0]) {
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
	for (TextureData *data : textures) {
		if (data->name == Info.name) {
			msg.Write("Texture with name [%s] already exist! Texture name must be unique", ~Info.name);
			Logger::Log(LOG_WARN, LOG_TEXTURE, msg);
			return data->texture;
		}
	}

	// 4. Ensure that only 1 file is specified for a texture.
	if (Info.files.Length() > 1) {
		msg.SetString("More than 1 texture file is specified! Unable to create a texture with multiple texture files.");
		Logger::Log(LOG_WARN, LOG_TEXTURE, msg);
		return nullptr;
	}

	size_t idx = textures.Push(new TextureData());
	TextureData *data = textures[idx];

	data->Alloc(Info);
	data->id = GenerateID<Texture>();
	data->texture->info = data;

	//db->texture = new Texture();
	if (!data->texture->Alloc(Info)) {
		msg.SetString("Unable to allocate new texture. Exiting operation");
		Logger::Log(LOG_ERR, LOG_TEXTURE, msg);
		//delete db->texture;
		delete data;
		textures[idx] = nullptr;

		textures.PopAt(idx);

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


Texture* ResourceManager::GetTexture(const String &Name) {
	Texture *pTexture = nullptr;

	String msg("Attempting to retrieve [%s] texture from engine.", ~Name);
	Logger::Log(LOG_INFO, LOG_TEXTURE, msg);

	for (size_t i = 0; i < textures.Length(); i++) {
		if (textures[i]->name != Name)
			continue;

		pTexture = textures[i]->texture;
		break;
	}

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


bool ResourceManager::DeleteTexture(const String &Name) {
	String msg("Attempting to delete [%s] texture.", ~Name);
	Logger::Log(LOG_INFO, LOG_TEXTURE, msg);

	size_t idx = -1;

	for (size_t i = 0; i < textures.Length(); i++) {
		if (textures[i]->name != Name)
			continue;

		idx = i;
		break;
	}


	if (idx == -1) {
		msg.SetString("Unable to find texture with such name. Does not exist within engine.");
		Logger::Log(LOG_ERR, LOG_SHADER, msg);
		return false;
	}

	//// Remove all reference to this Texture from Materials.
	//for (MaterialData *data : materials) {
	//	for (size_t i = 0; i < data->material->textures.Length(); i++) {
	//		if (textures[idx]->texture != data->material->textures[i])
	//			continue;

	//		data->material->textures[i] = nullptr;
	//	}
	//}
	
	delete textures[idx];
	textures.PopAt(idx);

	msg.SetString("Texture deleted!");
	Logger::Log(LOG_INFO, LOG_SHADER, msg);

	return false;
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
	String msg;
	msg.SetString("Deleting all resources from engine.");
	Logger::Log(LOG_INFO, LOG_MANAGER, msg);

	for (size_t i = 0; i < textures.Length(); i++) {
		delete textures[i];
		textures[i] = nullptr;
	}

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

	textures.Empty(); shaders.Empty(); scenes.Empty(); materials.Empty(); levels.Empty();

	msg.SetString("Successfully removed all resources from engine.");
	Logger::Log(LOG_INFO, LOG_MANAGER, msg);
}

ResourceManager::ResourceManager() {
	String msg;
	msg.Write("Initialising Gravity's resource manager.");
	Logger::Log(LOG_INFO, LOG_MANAGER, msg);

	textures.Reserve(256);
	shaders.Reserve(256);
	scenes.Reserve(256);
	materials.Reserve(256);
	levels.Reserve(256);
}

ResourceManager::~ResourceManager() {
	CleanResource();
}