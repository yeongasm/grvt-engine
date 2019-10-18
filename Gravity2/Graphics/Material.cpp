#include "stdafx.h"


MaterialCreationInfo::MaterialCreationInfo() :
	Name(), Shader(nullptr), Textures() {}


MaterialCreationInfo::~MaterialCreationInfo() {
	Name.Release();
	Textures.Release();
	Shader = nullptr;
}


MaterialObj::MaterialObj() : 
	Name(), Shader(nullptr), Info(nullptr), Textures(), Uniforms() {}


MaterialObj::~MaterialObj() {
	Shader = nullptr;
	Info = nullptr;

	Name.Release();
	Uniforms.clear();
	Textures.Release();
}


MaterialObj::MaterialObj(const MaterialObj &Other) { *this = Other; }


MaterialObj::MaterialObj(MaterialObj &&Other) { *this = std::move(Other); }


MaterialObj& MaterialObj::operator= (const MaterialObj &Other) {
	if (this != &Other) {
		Name		= Other.Name;
		Shader		= Other.Shader;
		Info		= Other.Info;
		Uniforms	= Other.Uniforms;
		Textures	= Other.Textures;
	}

	return *this;
}


MaterialObj& MaterialObj::operator= (MaterialObj &&Other) {
	if (this != &Other) {

		Shader		= Other.Shader;
		Info		= Other.Info;
		Uniforms	= Other.Uniforms;
		Textures	= Other.Textures;

		Other.~MaterialObj();
	}

	return *this;
}


//void MaterialObj::Alloc(const MaterialCreationInfo &Info) {
//	Name = Info.Name;
//	
//	if (Info.Shader) {
//		Shader = Info.Shader;
//		Uniforms = Shader->Attributes.Uniforms;
//		Info.Shader->Info->References.Push(&Shader);
//	}
//	
//	if (Info.Textures.Length()) {
//		// The downside of creating references to a texture is we can not just copy the array.
//		for (size_t i = 0; i < Info.Textures.Length(); i++) {
//			Textures.Push(Info.Textures[i]);
//			Info.Textures[i]->Info->References.Push(&Textures[i]);
//		}
//	}
//}
//
//
//void MaterialObj::Free() {
//	Shader	= nullptr;
//	Info	= nullptr;
//
//	Name.Release();
//	Uniforms.clear();
//	Textures.Release();
//}


bool MaterialObj::SetBool(const String &Uniform, bool Value) {
	auto it = Uniforms.find(Uniform);

	if (it == Uniforms.end())
		return false;

	if (it->second.Type != ATTR_TYPE_BOOL)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool MaterialObj::SetInt(const String &Uniform, int Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.Type != ATTR_TYPE_INT)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool MaterialObj::SetFloat(const String &Uniform, float Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.Type != ATTR_TYPE_FLOAT)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool MaterialObj::SetVector(const String &Uniform, glm::vec2 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.SubType != ATTR_SUBTYPE_VEC2)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool MaterialObj::SetVector(const String &Uniform, glm::vec3 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.SubType != ATTR_SUBTYPE_VEC3)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool MaterialObj::SetVector(const String &Uniform, glm::vec4 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.SubType != ATTR_SUBTYPE_VEC4)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool MaterialObj::SetMatrix(const String &Uniform, glm::mat2 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.SubType != ATTR_SUBTYPE_MAT2)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool MaterialObj::SetMatrix(const String &Uniform, glm::mat3 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.SubType != ATTR_SUBTYPE_MAT3)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool MaterialObj::SetMatrix(const String &Uniform, glm::mat4 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.SubType != ATTR_SUBTYPE_MAT4)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool MaterialObj::SetTexture(const String &Uniform, TextureObj *Texture) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (!Shader->IsUniformAMaterial(it->second))
		return false;

	Uniforms[Uniform].UpdateValue(Texture->Type);

	return true;
}