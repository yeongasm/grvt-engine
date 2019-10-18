#include "stdafx.h"


LightCreationInfo::LightCreationInfo() :
	Position(0.0f), Colour(1.0f), Name(), Type(LIGHT_TYPE_NONE), Brightness(0.5f), 
	Constant(1.0f), Linear(0.0f), Quadratic(0.0f), Radius(0.0f) {}


LightCreationInfo::~LightCreationInfo() {}


LightSource::LightSource() :
	Position(0.0f), Colour(1.0f), Name(),
	Type(LIGHT_TYPE_NONE), Brightness(0.0f), Enable(true) {}


LightSource::LightSource(const LightSource &Other) { *this = Other; }


LightSource::LightSource(LightSource &&Other) { *this = std::move(Other); }


LightSource& LightSource::operator= (const LightSource &Other) {
	// Ensures that the object is not being assigned to itself. Only in debug.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Position	= Other.Position;
		Colour		= Other.Colour;
		Name		= Other.Name;
		Type		= Other.Type;
		Brightness	= Other.Brightness;
		Enable		= Other.Enable;
	}

	return *this;
}


LightSource& LightSource::operator= (LightSource &&Other) {
	// Ensures that the object is not being assigned to itself. Only in debug.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Position	= Other.Position;
		Colour		= Other.Colour;
		Name		= Other.Name;
		Type		= Other.Type;
		Brightness	= Other.Brightness;
		Enable		= Other.Enable;

		Other.Free();
	}

	return *this;
}


LightSource::~LightSource() {}


DirLight::DirLight() : LightSource() {}


DirLight::~DirLight() {}


DirLight::DirLight(const DirLight &Other) { *this = Other; }


DirLight::DirLight(DirLight &&Other) { *this = std::move(Other); }


DirLight& DirLight::operator= (const DirLight &Other) {
	LightSource::operator=(Other);

	return *this;
}


DirLight& DirLight::operator= (DirLight &&Other) {
	LightSource::operator=(std::move(Other));

	return *this;
}


void DirLight::Alloc(const LightCreationInfo &Info) {
	Position	= Info.Position;
	Colour		= Info.Colour;
	Brightness	= Info.Brightness;
	Type		= Info.Type;
	Name		= Info.Name;
}


void DirLight::Free() {
	Brightness	= 0.0f;
	Type		= LIGHT_TYPE_NONE;
	Position	= glm::vec3(0.0f);
	Colour		= glm::vec3(0.0f);
	Name.Release();
}


void DirLight::Compute(glm::mat4 &Buffer) {
	Buffer[0][0] = 0.0f;
	Buffer[0][1] = Brightness;
	Buffer[1][0] = Position.x;
	Buffer[1][1] = Position.y;
	Buffer[1][2] = Position.z;
	Buffer[2][0] = Colour.x;
	Buffer[2][1] = Colour.y;
	Buffer[2][2] = Colour.z;
}


PointLight::PointLight() :
	LightSource(), Constant(1.0f), Linear(0.0f),
	Quadratic(0.0f), Radius(0.0f), Simplified(false) {}


PointLight::~PointLight() {}


PointLight::PointLight(const PointLight &Other) { *this = Other; }


PointLight::PointLight(PointLight &&Other) { *this = std::move(Other); }


PointLight& PointLight::operator= (const PointLight &Other) {
	// Ensures that the object is not being assigned to itself. Only in debug.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Constant	= Other.Constant;
		Linear		= Other.Linear;
		Quadratic	= Other.Quadratic;
		Radius		= Other.Radius;
		Simplified	= Other.Simplified;

		LightSource::operator=(Other);
	}

	return *this;
}


PointLight& PointLight::operator= (PointLight &&Other) {
	// Ensures that the object is not being assigned to itself. Only in debug.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Constant	= Other.Constant;
		Linear		= Other.Linear;
		Quadratic	= Other.Quadratic;
		Radius		= Other.Radius;
		Simplified	= Other.Simplified;
		
		LightSource::operator=(std::move(Other));

		new (&Other) PointLight();
	}

	return *this;
}


void PointLight::Alloc(const LightCreationInfo &Info) {
	LightSource::Alloc(Info);

	if (Info.Radius)
		Simplified = true;

	Constant	= Info.Constant;
	Linear		= Info.Linear;
	Quadratic	= Info.Quadratic;
	Radius		= Info.Radius;
}


void PointLight::Free() {
	Simplified	= false;
	Constant	= 0.0f;
	Linear		= 0.0f;
	Quadratic	= 0.0f;
	Radius		= 0.0f;

	LightSource::Free();
}


void PointLight::UseRadiusForAttenuation(bool Enable) {
	if (Simplified != Enable)
		Simplified = Enable;
}


void PointLight::Compute(glm::mat4 &Buffer) {
	LightSource::Compute(Buffer);

	float kC = Constant;
	float kL = Linear;
	float kQ = Quadratic;

	if (Simplified) {
		kL = 2 / Radius;
		kQ = 1 / (Radius * Radius);
	}

	Buffer[0][0] = 1.0f;
	Buffer[3][0] = kC;
	Buffer[3][1] = kL;
	Buffer[3][2] = kQ;
}