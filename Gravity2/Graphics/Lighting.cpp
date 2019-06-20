#include "stdafx.h"


LightCreationInfo::LightCreationInfo() :
	brightness(0.5f),
	type(LIGHT_TYPE_NONE),
	position(0.0f),
	lightColour(1.0f),
	name{} {}


LightCreationInfo::LightCreationInfo(const LightCreationInfo &Other) { *this = Other; }


LightCreationInfo::LightCreationInfo(LightCreationInfo &&Other) { *this = std::move(Other); }


LightCreationInfo& LightCreationInfo::operator= (const LightCreationInfo &Other) {
	// Throw an assertion when somebody is assigning the object to itself.
	_ASSERTE(this != &Other);
	
	if (this != &Other) {
		brightness	= Other.brightness;
		type		= Other.type;
		position	= Other.position;
		lightColour = Other.lightColour;
		name		= Other.name;
	}

	return *this;
}


LightCreationInfo& LightCreationInfo::operator= (LightCreationInfo &&Other) {
	// Throw an assertion when somebody is assigning the object to itself.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		brightness	= Other.brightness;
		type		= Other.type;
		position	= Other.position;
		lightColour = Other.lightColour;
		name		= Other.name;

		new (&Other) LightCreationInfo();
	}

	return *this;
}


LightCreationInfo::~LightCreationInfo() {}


Light::Light() :
	brightness(0.0f),
	type(LIGHT_TYPE_NONE),
	position(0.0f),
	lightColour(0.0f) {}


Light::Light(const Light &Other) { *this = Other; }


Light::Light(Light &&Other) { *this = std::move(Other); }


Light& Light::operator= (const Light &Other) {
	// Ensures that the object is not being assigned to itself. Only in debug.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		brightness	= Other.brightness;
		type		= Other.type;
		position	= Other.position;
		lightColour = Other.lightColour;
		name		= Other.name;
	}

	return *this;
}


Light& Light::operator= (Light &&Other) {
	// Ensures that the object is not being assigned to itself. Only in debug.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Free();

		brightness	= Other.brightness;
		type		= Other.type;
		position	= Other.position;
		lightColour = Other.lightColour;
		name		= Other.name;

		Other.Free();
	}

	return *this;
}


Light::~Light() {
	Free();
}


void Light::Alloc(const LightCreationInfo &Info) {
	brightness	= Info.brightness;
	type		= Info.type;
	position	= Info.position;
	lightColour = Info.lightColour;
	name		= Info.name;
}


void Light::Free() {
	brightness	= 0.0f;
	type		= LIGHT_TYPE_NONE;
	position	= glm::vec3(0.0f);
	lightColour = glm::vec3(0.0f);
	name.Release();
}


void Light::Compute(Shader *Shader) {
	// NOTE(Afiq):
	// Be sure to use this naming scheme inside the shader as well.
	// In the future, this function would be use to compute the light's view matrix for shadow calculation.
	Shader->SetFloat("DirLight.brightness", brightness);
	Shader->SetVector("DirLight.position", position);
	Shader->SetVector("DirLight.colour", lightColour);
}