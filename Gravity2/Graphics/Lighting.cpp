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


void Light::Compute(Shader *Shader) {}


DirLight::DirLight() : Light() {}


DirLight::DirLight(const DirLight &Other) { *this = Other; }


DirLight::DirLight(DirLight &&Other) { *this = std::move(Other); }


DirLight& DirLight::operator= (const DirLight &Other) {
	Light::operator=(Other);

	return *this;
}


DirLight& DirLight::operator= (DirLight &&Other) {
	Light::operator=(std::move(Other));

	return *this;
}


DirLight::~DirLight() {
	Free();
}


void DirLight::Compute(Shader *Shader) {
	// NOTE(Afiq):
	// Be sure to use this naming scheme inside the shader as well.
	// In the future, this function would be use to compute the light's view matrix for shadow calculation.
	Shader->SetFloat("DirLight.brightness", brightness);
	Shader->SetVector("DirLight.position", position);
	Shader->SetVector("DirLight.colour", lightColour);
}


PointLight::PointLight() :
	Light(),
	simplified(false),
	constant(1.0f),
	linear(0.0f),
	quadratic(0.0f),
	radius(0.0f) {}


PointLight::PointLight(const PointLight &Other) { *this = Other; }


PointLight::PointLight(PointLight &&Other) { *this = std::move(Other); }


PointLight& PointLight::operator= (const PointLight &Other) {
	// Ensures that the object is not being assigned to itself. Only in debug.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		simplified	= Other.simplified;
		constant	= Other.constant;
		linear		= Other.linear;
		quadratic	= Other.quadratic;
		radius		= Other.radius;

		Light::operator=(Other);
	}

	return *this;
}


PointLight& PointLight::operator= (PointLight &&Other) {
	// Ensures that the object is not being assigned to itself. Only in debug.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		simplified	= Other.simplified;
		constant	= Other.constant;
		linear		= Other.linear;
		quadratic	= Other.quadratic;
		radius		= Other.radius;
		
		Light::operator=(std::move(Other));

		new (&Other) PointLight();
	}

	return *this;
}


PointLight::~PointLight() {
	simplified	= false;
	constant	= 0.0f;
	linear		= 0.0f;
	quadratic	= 0.0f;
	radius		= 0.0f;

	Free();
}


void PointLight::UseRadiusForAttenuation(bool Enable) {
	if (simplified != Enable)
		simplified = Enable;
}


void PointLight::Compute(Shader *Shader) {
	float kC = constant;
	float kL = linear;
	float kQ = quadratic;

	if (simplified) {
		kL = 2 / radius;
		kQ = 1 / (radius * radius);
	}

	Shader->SetFloat("DirLight.brightness", brightness);
	Shader->SetFloat("PointLight.constant", kC);
	Shader->SetFloat("PointLight.linear", kL);
	Shader->SetFloat("PointLight.quadratic", kQ);
	Shader->SetVector("PointLight.position", position);
	Shader->SetVector("PointLight.colour", lightColour);
}