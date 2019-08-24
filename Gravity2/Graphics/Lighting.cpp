#include "stdafx.h"


LightCreationInfo::LightCreationInfo() :
	shadows(true),
	brightness(0.5f),
	constant(1.0f),
	linear(0.0f),
	quadratic(0.0f),
	radius(0.0f),
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
		shadows		= Other.shadows;
		brightness	= Other.brightness;
		constant	= Other.constant;
		linear		= Other.linear;
		quadratic	= Other.quadratic;
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
		shadows		= Other.shadows;
		brightness	= Other.brightness;
		constant	= Other.constant;
		linear		= Other.linear;
		quadratic	= Other.quadratic;
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
	enable(true),
	shadows(true),
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
		enable		= Other.enable;
		shadows		= Other.shadows;
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
		enable		= Other.enable;
		shadows		= Other.shadows;
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


void Light::Compute(glm::mat4 &Buffer) {
	Buffer[0][0] = 0.0f;
	Buffer[0][1] = brightness;
	Buffer[1][0] = position.x;
	Buffer[1][1] = position.y;
	Buffer[1][2] = position.z;
	Buffer[2][0] = lightColour.x;
	Buffer[2][1] = lightColour.y;
	Buffer[2][2] = lightColour.z;
}


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
	Free();
}


void PointLight::Alloc(const LightCreationInfo &Info) {
	Light::Alloc(Info);

	if (Info.radius)
		simplified = true;

	constant	= Info.constant;
	linear		= Info.linear;
	quadratic	= Info.quadratic;
	radius		= Info.radius;
}


void PointLight::Free() {
	simplified	= false;
	constant	= 0.0f;
	linear		= 0.0f;
	quadratic	= 0.0f;
	radius		= 0.0f;

	Light::Free();
}


void PointLight::UseRadiusForAttenuation(bool Enable) {
	if (simplified != Enable)
		simplified = Enable;
}


void PointLight::Compute(glm::mat4 &Buffer) {
	Light::Compute(Buffer);

	float kC = constant;
	float kL = linear;
	float kQ = quadratic;

	if (simplified) {
		kL = 2 / radius;
		kQ = 1 / (radius * radius);
	}

	Buffer[0][0] = 1.0f;
	Buffer[3][0] = kC;
	Buffer[3][1] = kL;
	Buffer[3][2] = kQ;
}


RenderDirectionalLight::RenderDirectionalLight() :
	brightness(0.0f),
	position(0.0f),
	colour(0.0f) {}


RenderDirectionalLight::~RenderDirectionalLight() {}


RenderPointLight::RenderPointLight() :
	brightness{},
	constant{},
	linear{},
	quadratic{},
	position{},
	colour{} {}


RenderPointLight::~RenderPointLight() {
	brightness.Release();
	constant.Release();
	linear.Release();
	quadratic.Release();
	position.Release();
	colour.Release();
}