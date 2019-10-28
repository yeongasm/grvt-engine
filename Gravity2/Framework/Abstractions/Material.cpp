#include "stdafx.h"


MaterialCreationInfo::MaterialCreationInfo() :
	Name(), Shader(nullptr), Textures() {}


MaterialCreationInfo::~MaterialCreationInfo() {
	Name.Release();
	Textures.Release();
	Shader = nullptr;
}


void MaterialCreationInfo::PushTexture(GrvtTexture* Texture) {
	Textures.Push({Texture->Type, &Texture->Handle});
}


void MaterialCreationInfo::PopTexture(GrvtTexture* Texture) {
	size_t idx = Textures.IndexOf({Texture->Type, &Texture->Handle});
	Textures.PopAt(idx);
}


void MaterialCreationInfo::SetShader(GrvtShader* Shader) {
	if (Shader != Shader)
		Shader = Shader;
}


void MaterialCreationInfo::RemoveShader() {
	Shader = nullptr;
}


GrvtMaterial::GrvtMaterial() :
	Uniforms(), Textures(), ShaderHandle(nullptr) {}


GrvtMaterial::~GrvtMaterial() { Free(); }


GrvtMaterial::GrvtMaterial(const GrvtMaterial& Other) { *this = Other; }


GrvtMaterial::GrvtMaterial(GrvtMaterial&& Other) { *this = std::move(Other); }


GrvtMaterial& GrvtMaterial::operator= (const GrvtMaterial& Other) {
	if (this != &Other) {
		Uniforms	= Other.Uniforms;
		Textures	= Other.Textures;
		ShaderHandle = Other.ShaderHandle;
	}

	return *this;
}


GrvtMaterial& GrvtMaterial::operator= (GrvtMaterial&& Other) {
	if (this != &Other) {
		Uniforms = Other.Uniforms;
		Textures = Other.Textures;
		ShaderHandle = Other.ShaderHandle;

		new (&Other) GrvtMaterial();
	}

	return *this;
}


void GrvtMaterial::Alloc(const MaterialCreationInfo &Info) {
	ShaderHandle = &Info.Shader->Handle;
	Textures = Info.Textures;
	Uniforms = Info.Shader->Uniforms;
}


void GrvtMaterial::Free() {
	ShaderHandle = nullptr;
	Uniforms.clear();
	Textures.Release();
}


bool GrvtMaterial::SetBool(const String& Uniform, bool Value) {
	auto it = Uniforms.find(Uniform);

	if (it == Uniforms.end())
		return false;

	if (it->second.Type != GrvtShader_AttrType_Boolean)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool GrvtMaterial::SetInt(const String& Uniform, int Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.Type != GrvtShader_AttrType_Integer)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool GrvtMaterial::SetFloat(const String& Uniform, float Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.Type != GrvtShader_AttrType_Float)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool GrvtMaterial::SetVector(const String& Uniform, glm::vec2 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.SubType != GrvtShader_AttrSubType_Vector2)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool GrvtMaterial::SetVector(const String& Uniform, glm::vec3 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.SubType != GrvtShader_AttrSubType_Vector3)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool GrvtMaterial::SetVector(const String& Uniform, glm::vec4 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.SubType != GrvtShader_AttrSubType_Vector4)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool GrvtMaterial::SetMatrix(const String& Uniform, glm::mat2 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.SubType != GrvtShader_AttrSubType_Matrix2)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool GrvtMaterial::SetMatrix(const String& Uniform, glm::mat3 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.SubType != GrvtShader_AttrSubType_Matrix3)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool GrvtMaterial::SetMatrix(const String& Uniform, glm::mat4 Value) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	if (it->second.SubType != GrvtShader_AttrSubType_Matrix4)
		return false;

	Uniforms[Uniform].UpdateValue(Value);

	return true;
}


bool GrvtMaterial::SetTexture(const String& Uniform, GrvtTexture* Texture) {
	auto it = Uniforms.find(Uniform);
	
	if (it == Uniforms.end())
		return false;

	Uniforms[Uniform].UpdateValue(Texture->Type);

	return true;
}