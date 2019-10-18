#include "stdafx.h"


MaterialCreationInfo::MaterialCreationInfo() : Name{}, Shader{}, Textures{} {}


MaterialCreationInfo::MaterialCreationInfo(const MaterialCreationInfo &Other) { *this = Other; }


MaterialCreationInfo::MaterialCreationInfo(MaterialCreationInfo &&Other) { *this = std::move(Other); }


MaterialCreationInfo& MaterialCreationInfo::operator= (const MaterialCreationInfo &Other) {
	// Prevent the structure from copying to itself.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Name		= Other.Name;
		Shader		= Other.Shader;
		Textures	= Other.Textures;
	}

	return *this;
}


MaterialCreationInfo& MaterialCreationInfo::operator= (MaterialCreationInfo &&Other) {
	// Prevent the structure from executing a move semantic on itself.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		Name		= Other.Name;
		Shader		= Other.Shader;
		Textures	= Other.Textures;

		new (&Other) MaterialCreationInfo();
	}

	return *this;
}


MaterialCreationInfo::~MaterialCreationInfo() {
	Shader = nullptr;
	Name.Release();
	Textures.Release();
}


MaterialObj::MaterialObj() : Name(), Shader(nullptr), Info(nullptr), Textures(), Uniforms() {}


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
		Free();

		Shader		= Other.Shader;
		Info		= Other.Info;
		Uniforms	= Other.Uniforms;
		Textures	= Other.Textures;

		Other.Free();
	}

	return *this;
}


MaterialObj::~MaterialObj() {}


void MaterialObj::Alloc(const MaterialCreationInfo &Info) {
	Name = Info.Name;
	
	if (Info.Shader) {
		Shader = Info.Shader;
		Uniforms = Shader->Attributes.Uniforms;
		Info.Shader->Info->References.Push(&Shader);
	}
	
	if (Info.Textures.Length()) {
		// The downside of creating references to a texture is we can not just copy the array.
		for (size_t i = 0; i < Info.Textures.Length(); i++) {
			Textures.Push(Info.Textures[i]);
			Info.Textures[i]->Info->References.Push(&Textures[i]);
		}
	}
}


void MaterialObj::Free() {
	Shader	= nullptr;
	Info	= nullptr;

	Name.Release();
	Uniforms.clear();
	Textures.Release();
}


bool MaterialObj::SetBool(const String &Uniform, bool Value) {
	auto it = Uniforms.find(Uniform);

	if (it == Uniforms.end())
		return false;

	if (it->second.type != ATTR_TYPE_BOOL)
		return false;

	Uniforms[Uniform].UpdateValue(Value);
	Uniforms[Uniform].inUse = true;

	return true;
}


bool MaterialObj::SetInt(const String &Uniform, int Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.type != ATTR_TYPE_INT)
		return false;

	Uniforms[Uniform].UpdateValue(Value);
	Uniforms[Uniform].inUse = true;

	return true;
}


bool MaterialObj::SetFloat(const String &Uniform, float Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.type != ATTR_TYPE_FLOAT)
		return false;

	Uniforms[Uniform].UpdateValue(Value);
	Uniforms[Uniform].inUse = true;

	return true;
}


bool MaterialObj::SetVector(const String &Uniform, glm::vec2 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.subType != ATTR_SUB_VEC2)
		return false;

	Uniforms[Uniform].UpdateValue(Value);
	Uniforms[Uniform].inUse = true;

	return true;
}


bool MaterialObj::SetVector(const String &Uniform, glm::vec3 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.subType != ATTR_SUB_VEC3)
		return false;

	Uniforms[Uniform].UpdateValue(Value);
	Uniforms[Uniform].inUse = true;

	return true;
}


bool MaterialObj::SetVector(const String &Uniform, glm::vec4 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.subType != ATTR_SUB_VEC4)
		return false;

	Uniforms[Uniform].UpdateValue(Value);
	Uniforms[Uniform].inUse = true;

	return true;
}


bool MaterialObj::SetMatrix(const String &Uniform, glm::mat2 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.subType != ATTR_SUB_MAT2)
		return false;

	Uniforms[Uniform].UpdateValue(Value);
	Uniforms[Uniform].inUse = true;

	return true;
}


bool MaterialObj::SetMatrix(const String &Uniform, glm::mat3 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.subType != ATTR_SUB_MAT3)
		return false;

	Uniforms[Uniform].UpdateValue(Value);
	Uniforms[Uniform].inUse = true;

	return true;
}


bool MaterialObj::SetMatrix(const String &Uniform, glm::mat4 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.subType != ATTR_SUB_MAT4)
		return false;

	Uniforms[Uniform].UpdateValue(Value);
	Uniforms[Uniform].inUse = true;

	return true;
}


bool MaterialObj::SetTexture(const String &Uniform, TextureObj *Texture) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (!Shader->IsUniformAMaterial(it->second))
		return false;

	Uniforms[Uniform].UpdateValue(Texture->Type);
	Uniforms[Uniform].inUse = true;

	return true;
}