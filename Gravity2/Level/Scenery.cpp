#include "stdafx.h"


LevelCreationInfo::LevelCreationInfo() :
	name{},
	directory{},
	filename{} {}


LevelCreationInfo::LevelCreationInfo(const LevelCreationInfo &Other) { *this = Other; }


LevelCreationInfo::LevelCreationInfo(LevelCreationInfo &&Other) { *this = std::move(Other); }


LevelCreationInfo& LevelCreationInfo::operator= (const LevelCreationInfo &Other) {
	// Check if it's copying to itself in debug.
	_ASSERTE(this != &Other);

	// Disable copying to itself in runtime.
	if (this != &Other) {
		name		= Other.name;
		directory	= Other.directory;
		filename	= Other.filename;
	}

	return *this;
}


LevelCreationInfo& LevelCreationInfo::operator= (LevelCreationInfo &&Other) {
	// Check if it's copying to itself in debug.
	_ASSERTE(this != &Other);

	// Disable copying to itself in runtime.
	if (this != &Other) {
		name		= Other.name;
		directory	= Other.directory;
		filename	= Other.filename;

		new (&Other) LevelCreationInfo();
	}

	return *this;
}


LevelCreationInfo::~LevelCreationInfo() {
	name.Release();
	directory.Release();
	filename.Release();
}


size_t Scenery::CheckLightName(const String &Name) {
	size_t idx = -1;

	for (size_t i = 0; i < lights.Length(); i++) {
		if (lights[i]->name != Name)
			continue;

		idx = i;
		break;
	}

	return idx;
}


Scenery::Scenery() :
	hasDirLight(false),
	dirLight{},
	camera{},
	lights{},
	renderInstances{},
	info{} {}


Scenery::Scenery(const Scenery &Other) { *this = Other; }


Scenery::Scenery(Scenery &&Other) { *this = std::move(Other); }


Scenery& Scenery::operator= (const Scenery &Other) {
	// Check in debug if it's copying to itself.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		hasDirLight		= Other.hasDirLight;
		dirLight		= Other.dirLight;
		camera			= Other.camera;
		lights			= Other.lights;
		renderInstances = Other.renderInstances;
		info			= Other.info;
	}

	return *this;
}


Scenery& Scenery::operator= (Scenery &&Other) {
	// Check in debug if it's moving to itself.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		hasDirLight		= Other.hasDirLight;
		dirLight		= Other.dirLight;
		camera			= Other.camera;
		lights			= Other.lights;
		renderInstances = Other.renderInstances;
		info			= Other.info;

		new (&Other) Scenery();
	}

	return *this;
}


Scenery::~Scenery() {
	Free();
}


void Scenery::Free() {
	hasDirLight = false;

	for (Light *light : lights)
		delete light;
	
	for (SceneInstance *instance : renderInstances)
		instance = nullptr;

	info = nullptr;
}


void Scenery::PushSceneInstance(SceneInstance *Instance) {
	renderInstances.Push(Instance);
}


void Scenery::AttachCamera(EulerCamera *Camera) {
	if (camera != Camera)
		camera = Camera;
}


void Scenery::DetachCamera() {
	camera = nullptr;
}


SceneInstance* Scenery::CreateSceneInstance(Scene *Scene, const SceneInstanceCreation &Info) {
	SceneInstance *instance = nullptr;
	
	instance = Scene->CreateInstance(Info);
	
	if (instance)
		renderInstances.Push(instance);

	return instance;
}


DirLight* Scenery::AddDirectionalLight(const LightCreationInfo &Info) {
	Light *light = nullptr;

	if (!hasDirLight) {
		if (Info.type != LIGHT_TYPE_DIRECTIONAL)
			return nullptr;

		size_t idx = CheckLightName(Info.name);
		
		// A light with such a name already exist.
		if (idx != -1)
			return nullptr;

		light = lights.Insert(new DirLight());
		light->Alloc(Info);
		hasDirLight = true;
		dirLight = dynamic_cast<DirLight*>(light);
	} else {
		light = dirLight;
	}

	return dynamic_cast<DirLight*>(light);
}


PointLight* Scenery::AddPointLight(const LightCreationInfo &Info) {
	Light *light = nullptr;

	size_t idx = CheckLightName(Info.name);

	if (Info.type != LIGHT_TYPE_POINTLIGHT)
		return nullptr;

	if (idx == -1) {
		light = lights.Insert(new PointLight());
		light->Alloc(Info);
	} else {
		light = lights[idx];
	}

	return dynamic_cast<PointLight*>(light);
}


DirLight* Scenery::GetDirectionalLight() {
	return dirLight;
}


PointLight* Scenery::GetPointLight(const String &Name) {
	PointLight *pLight = nullptr;

	for (Light *light : lights) {
		if (light->type == LIGHT_TYPE_DIRECTIONAL)
			continue;

		if (light->name != Name)
			continue;

		pLight = dynamic_cast<PointLight*>(light);
		break;
	}

	return pLight;
}


bool Scenery::PopSceneInstance(const SceneInstance *Instance) {
	size_t idx = -1;

	for (size_t i = 0; i < renderInstances.Length(); i++) {
		if (renderInstances[i] != Instance)
			continue;

		idx = i;
		break;
	}

	if (idx != -1)
		renderInstances.PopAt(idx);

	return (idx == -1) ? false : true;
}


bool Scenery::PopAllInstanceWithScene(const Scene *Scene) {
	size_t firstEmpty = -1;

	for (size_t i = 0; i < renderInstances.Length(); i++) {
		if (renderInstances[i]->GetScene() != Scene)
			continue;

		if (firstEmpty == -1)
			firstEmpty = i;

		renderInstances[i] = nullptr;
	}

	if (firstEmpty != -1)
		renderInstances.PopAt(firstEmpty);

	return (firstEmpty == -1) ? false : true;
}


bool Scenery::PopLight(const String &Name) {
	size_t idx = -1;

	for (size_t i = 0; i < lights.Length(); i++) {
		if (lights[i]->name != Name)
			continue;

		idx = i;
		break;
	}

	if (idx != -1) {
		hasDirLight = lights[idx]->type == LIGHT_TYPE_DIRECTIONAL ? false : true;
		lights.PopAt(idx);
	}

	return (idx == -1) ? false : true;
}