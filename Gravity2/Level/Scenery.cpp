#include "stdafx.h"


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
	lights{},
	renderInstances{},
	name{} {}


Scenery::Scenery(const Scenery &Other) { *this = Other; }


Scenery::Scenery(Scenery &&Other) { *this = std::move(Other); }


Scenery& Scenery::operator= (const Scenery &Other) {
	// Check in debug if it's copying to itself.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		hasDirLight		= Other.hasDirLight;
		dirLight		= Other.dirLight;
		lights			= Other.lights;
		renderInstances = Other.renderInstances;
		name			= Other.name;
	}

	return *this;
}


Scenery& Scenery::operator= (Scenery &&Other) {
	// Check in debug if it's moving to itself.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		hasDirLight		= Other.hasDirLight;
		dirLight		= Other.dirLight;
		lights			= Other.lights;
		renderInstances = Other.renderInstances;
		name			= Other.name;

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

	name.Release();
}


void Scenery::PushSceneInstance(SceneInstance *Instance) {
	renderInstances.Push(Instance);
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
		dirLight = (DirLight*)light;
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

	if (idx != -1)
		lights.PopAt(idx);

	return (idx == -1) ? false : true;
}