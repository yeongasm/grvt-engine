#include "stdafx.h"


MaterialCreationInfo::MaterialCreationInfo() :
	name{},
	shader{},
	textures{} {}


MaterialCreationInfo::MaterialCreationInfo(const MaterialCreationInfo &Other) { *this = Other; }


MaterialCreationInfo::MaterialCreationInfo(MaterialCreationInfo &&Other) { *this = std::move(Other); }


MaterialCreationInfo& MaterialCreationInfo::operator= (const MaterialCreationInfo &Other) {
	// Prevent the structure from copying to itself.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		name		= Other.name;
		shader		= Other.shader;
		textures	= Other.textures;
	}

	return *this;
}


MaterialCreationInfo& MaterialCreationInfo::operator= (MaterialCreationInfo &&Other) {
	// Prevent the structure from executing a move semantic on itself.
	_ASSERTE(this != &Other);

	if (this != &Other) {
		name		= Other.name;
		shader		= Other.shader;
		textures	= Other.textures;

		new (&Other) MaterialCreationInfo();
	}

	return *this;
}


MaterialCreationInfo::~MaterialCreationInfo() {
	shader = nullptr;
	name.Release();
	textures.Release();
}


Material::Material() : name{}, shader{}, info{}, uniforms{}, textures{} {}


Material::Material(const MaterialCreationInfo &Info) { Alloc(Info); }


Material::Material(const Material &Other) { *this = Other; }


Material::Material(Material &&Other) { *this = std::move(Other); }


Material& Material::operator= (const Material &Other) {
	if (this != &Other) {
		name		= Other.name;
		shader		= Other.shader;
		info		= Other.info;
		uniforms	= Other.uniforms;
		textures	= Other.textures;
	}

	return *this;
}


Material& Material::operator= (Material &&Other) {
	if (this != &Other) {
		Free();

		shader		= Other.shader;
		info		= Other.info;
		uniforms	= Other.uniforms;
		textures	= Other.textures;

		Other.Free();
	}


	return *this;
}


Material::~Material() { Free(); }


void Material::Alloc(const MaterialCreationInfo &Info) {
	name = Info.name;
	
	if (Info.shader) {
		shader = Info.shader;
		uniforms = shader->attributes.uniforms;
		Info.shader->info->references.Push(&shader);
	}
	
	if (Info.textures.Length()) {
		// The downside of creating references to a texture is we can not just copy the array.
		for (size_t i = 0; i < Info.textures.Length(); i++) {
			textures.Push(Info.textures[i]);
			Info.textures[i]->info->references.Push(&textures[i]);
		}
	}
}


void Material::Free() {
	shader	= nullptr;
	info	= nullptr;

	name.Release();
	uniforms.clear();
	textures.Release();
}


bool Material::SetBool(const String &Uniform, bool Value) {
	auto it = uniforms.find(Uniform);

	if (it == uniforms.end())
		return false;

	if (it->second.type != ATTR_TYPE_BOOL)
		return false;

	uniforms[Uniform].UpdateValue(Value);
	uniforms[Uniform].inUse = true;

	return true;
}


bool Material::SetInt(const String &Uniform, int Value) {
	auto it = uniforms.find(Uniform);
	
	if (it == uniforms.end())
		return false;

	if (it->second.type != ATTR_TYPE_INT)
		return false;

	uniforms[Uniform].UpdateValue(Value);
	uniforms[Uniform].inUse = true;

	return true;
}


bool Material::SetFloat(const String &Uniform, float Value) {
	auto it = uniforms.find(Uniform);
	
	if (it == uniforms.end())
		return false;

	if (it->second.type != ATTR_TYPE_FLOAT)
		return false;

	uniforms[Uniform].UpdateValue(Value);
	uniforms[Uniform].inUse = true;

	return true;
}


bool Material::SetVector(const String &Uniform, glm::vec2 Value) {
	auto it = uniforms.find(Uniform);
	
	if (it == uniforms.end())
		return false;

	if (it->second.subType != ATTR_SUB_VEC2)
		return false;

	uniforms[Uniform].UpdateValue(Value);
	uniforms[Uniform].inUse = true;

	return true;
}


bool Material::SetVector(const String &Uniform, glm::vec3 Value) {
	auto it = uniforms.find(Uniform);
	
	if (it == uniforms.end())
		return false;

	if (it->second.subType != ATTR_SUB_VEC3)
		return false;

	uniforms[Uniform].UpdateValue(Value);
	uniforms[Uniform].inUse = true;

	return true;
}


bool Material::SetVector(const String &Uniform, glm::vec4 Value) {
	auto it = uniforms.find(Uniform);
	
	if (it == uniforms.end())
		return false;

	if (it->second.subType != ATTR_SUB_VEC4)
		return false;

	uniforms[Uniform].UpdateValue(Value);
	uniforms[Uniform].inUse = true;

	return true;
}


bool Material::SetMatrix(const String &Uniform, glm::mat2 Value) {
	auto it = uniforms.find(Uniform);
	
	if (it == uniforms.end())
		return false;

	if (it->second.subType != ATTR_SUB_MAT2)
		return false;

	uniforms[Uniform].UpdateValue(Value);
	uniforms[Uniform].inUse = true;

	return true;
}


bool Material::SetMatrix(const String &Uniform, glm::mat3 Value) {
	auto it = uniforms.find(Uniform);
	
	if (it == uniforms.end())
		return false;

	if (it->second.subType != ATTR_SUB_MAT3)
		return false;

	uniforms[Uniform].UpdateValue(Value);
	uniforms[Uniform].inUse = true;

	return true;
}


bool Material::SetMatrix(const String &Uniform, glm::mat4 Value) {
	auto it = uniforms.find(Uniform);
	
	if (it == uniforms.end())
		return false;

	if (it->second.subType != ATTR_SUB_MAT4)
		return false;

	uniforms[Uniform].UpdateValue(Value);
	uniforms[Uniform].inUse = true;

	return true;
}


bool Material::SetTexture(const String &Uniform, Texture *Texture) {
	auto it = uniforms.find(Uniform);
	
	if (it == uniforms.end())
		return false;

	if (!shader->IsUniformAMaterial(it->second))
		return false;

	uniforms[Uniform].UpdateValue(Texture->type);
	uniforms[Uniform].inUse = true;

	return true;
}