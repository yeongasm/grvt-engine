#include "stdafx.h"


LightCreationInfo::LightCreationInfo() :
	Position(0.0f), Colour(1.0f), Name(), Type(GrvtLight_Type_None), Brightness(0.5f), 
	Constant(1.0f), Linear(0.0f), Quadratic(0.0f), Radius(0.0f) {}


LightCreationInfo::~LightCreationInfo() {}


ShadowMap::ShadowMap() :
	Handle(), DepthTexture(), Type(GrvtShadowMap_None), Bias(0.0f) {}


ShadowMap::~ShadowMap() {}


LightSource::LightSource() :
	Position(0.0f), Colour(1.0f), Type(GrvtLight_Type_None), Brightness(0.0f), Enable(true) {}


LightSource::~LightSource() {}


void LightSource::Alloc(const LightCreationInfo& Info) {
	Brightness	= Info.Brightness;
	Type		= Info.Type;
	Position	= Info.Position;
	Colour		= Info.Colour;
}


void LightSource::Free() {
	Brightness	= 0.0f;
	Type		= GrvtLight_Type_None;
	Position	= glm::vec3(0.0f);
	Colour		= glm::vec3(0.0f);
	Shadow.Type = GrvtShadowMap_None;
}


void LightSource::Compute(glm::mat4& Buffer) {
	Buffer[0][0] = 0.0f;
	Buffer[0][1] = Brightness;
	Buffer[1][0] = Position.x;
	Buffer[1][1] = Position.y;
	Buffer[1][2] = Position.z;
	Buffer[2][0] = Colour.x;
	Buffer[2][1] = Colour.y;
	Buffer[2][2] = Colour.z;
}


DirLight::DirLight() : LightSource(), LightSpaceTransform() {}


DirLight::~DirLight() {}


//DirLight::DirLight(const DirLight &Other) { *this = Other; }


//DirLight::DirLight(DirLight &&Other) { *this = std::move(Other); }


void DirLight::Alloc(const LightCreationInfo& Info) {
	LightSource::Alloc(Info);
	//Position	= Info.Position;
	//Colour		= Info.Colour;
	//Brightness	= Info.Brightness;
	//Type		= Info.Type;
}


void DirLight::Free() {
	LightSource::Free();
	//Brightness	= 0.0f;
	//Type		= GrvtLight_Type_None;
	//Position	= glm::vec3(0.0f);
	//Colour		= glm::vec3(0.0f);
	//Shadow.Type = GrvtShadowMap_None;
}


void DirLight::Compute(glm::mat4& Buffer) {
	LightSource::Compute(Buffer);
	//Buffer[0][0] = 0.0f;
	//Buffer[0][1] = Brightness;
	//Buffer[1][0] = Position.x;
	//Buffer[1][1] = Position.y;
	//Buffer[1][2] = Position.z;
	//Buffer[2][0] = Colour.x;
	//Buffer[2][1] = Colour.y;
	//Buffer[2][2] = Colour.z;
}


PointLight::PointLight() :
	LightSource(), LightSpaceTransforms(6), Constant(1.0f), Linear(0.0f), Quadratic(0.0f), Radius(0.0f), Simplified(false) {}


PointLight::~PointLight() {}


//PointLight::PointLight(const PointLight &Other) { *this = Other; }


//PointLight::PointLight(PointLight &&Other) { *this = std::move(Other); }


//PointLight& PointLight::operator= (const PointLight &Other) {
//	// Ensures that the object is not being assigned to itself. Only in debug.
//	_ASSERTE(this != &Other);
//
//	if (this != &Other) {
//		Constant	= Other.Constant;
//		Linear		= Other.Linear;
//		Quadratic	= Other.Quadratic;
//		Radius		= Other.Radius;
//		Simplified	= Other.Simplified;
//
//		LightSource::operator=(Other);
//	}
//
//	return *this;
//}


//PointLight& PointLight::operator= (PointLight &&Other) {
//	// Ensures that the object is not being assigned to itself. Only in debug.
//	_ASSERTE(this != &Other);
//
//	if (this != &Other) {
//		Constant	= Other.Constant;
//		Linear		= Other.Linear;
//		Quadratic	= Other.Quadratic;
//		Radius		= Other.Radius;
//		Simplified	= Other.Simplified;
//		
//		LightSource::operator=(std::move(Other));
//
//		new (&Other) PointLight();
//	}
//
//	return *this;
//}


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
	
	LightSpaceTransforms.Release();

	LightSource::Free();
}


void PointLight::UpdateRadius(bool Simplify, float Value) {
	if (Simplified != Simplify)
		Simplified = Simplify;

	Radius = Value;
}


void PointLight::Compute(glm::mat4& Buffer) {
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