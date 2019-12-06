#include "GrvtPch.h"
#include "Framework/Abstraction/Material.h"


namespace Grvt
{

	GrvtMaterial::GrvtMaterial() :
		Uniforms(), Textures(), ShaderHandle(nullptr) {}


	GrvtMaterial::~GrvtMaterial() {
		Free();
	}


	GrvtMaterial::GrvtMaterial(const GrvtMaterial& Other)
	{
		*this = Other;
	}


	GrvtMaterial::GrvtMaterial(GrvtMaterial&& Other)
	{
		*this = Gfl::Move(Other);
	}


	GrvtMaterial& GrvtMaterial::operator= (const GrvtMaterial& Other)
	{
		if (this != &Other)
		{
			Uniforms = Other.Uniforms;
			Textures = Other.Textures;
			ShaderHandle = Other.ShaderHandle;
		}

		return *this;
	}


	GrvtMaterial& GrvtMaterial::operator= (GrvtMaterial&& Other)
	{
		if (this != &Other)
		{
			Uniforms = Other.Uniforms;
			Textures = Other.Textures;
			ShaderHandle = Other.ShaderHandle;

			new (&Other) GrvtMaterial();
		}

		return *this;
	}


	size_t GrvtMaterial::FindUniform(const Gfl::String& Identifier)
	{
		for (UniformAttr& Uniform : Uniforms)
		{
			if (Uniform.Name == Identifier)
				return Uniforms.IndexOf(Uniform);
		}

		return -1;
	}


	void GrvtMaterial::Alloc(const MaterialCreationInfo& Info)
	{
		ShaderHandle = &Info.Shader->Handle;
		Textures = Info.Textures;
		Uniforms = Info.Shader->Uniforms;
	}


	void GrvtMaterial::Free()
	{
		ShaderHandle = nullptr;
		Uniforms.Release();
		Textures.Release();
	}


	bool GrvtMaterial::SetBool(const Gfl::String& Uniform, bool Value)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1)
			return false;

		if (Uniforms[Index].Type != GrvtShader_AttrType_Boolean)
			return false;

		Uniforms[Index].UpdateValue(Value);

		return true;
	}


	bool GrvtMaterial::SetInt(const Gfl::String& Uniform, int Value)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1)
			return false;

		if (Uniforms[Index].Type != GrvtShader_AttrType_Integer)
			return false;

		Uniforms[Index].UpdateValue(Value);

		return true;
	}


	bool GrvtMaterial::SetFloat(const Gfl::String& Uniform, float Value)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1)
			return false;

		if (Uniforms[Index].Type != GrvtShader_AttrType_Float)
			return false;

		Uniforms[Index].UpdateValue(Value);

		return true;
	}


	bool GrvtMaterial::SetVector(const Gfl::String& Uniform, glm::vec2 Value)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1)
			return false;

		if (Uniforms[Index].SubType != GrvtShader_AttrSubType_Vector2)
			return false;

		Uniforms[Index].UpdateValue(Value);

		return true;
	}


	bool GrvtMaterial::SetVector(const Gfl::String& Uniform, glm::vec3 Value)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1)
			return false;

		if (Uniforms[Index].SubType != GrvtShader_AttrSubType_Vector3)
			return false;

		Uniforms[Index].UpdateValue(Value);

		return true;
	}


	bool GrvtMaterial::SetVector(const Gfl::String& Uniform, glm::vec4 Value)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1)
			return false;

		if (Uniforms[Index].SubType != GrvtShader_AttrSubType_Vector4)
			return false;

		Uniforms[Index].UpdateValue(Value);

		return true;
	}


	bool GrvtMaterial::SetMatrix(const Gfl::String& Uniform, glm::mat2 Value)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1)
			return false;

		if (Uniforms[Index].SubType != GrvtShader_AttrSubType_Matrix2)
			return false;

		Uniforms[Index].UpdateValue(Value);

		return true;
	}


	bool GrvtMaterial::SetMatrix(const Gfl::String& Uniform, glm::mat3 Value)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1)
			return false;

		if (Uniforms[Index].SubType != GrvtShader_AttrSubType_Matrix3)
			return false;

		Uniforms[Index].UpdateValue(Value);

		return true;
	}


	bool GrvtMaterial::SetMatrix(const Gfl::String& Uniform, glm::mat4 Value)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1)
			return false;

		if (Uniforms[Index].SubType != GrvtShader_AttrSubType_Matrix4)
			return false;

		Uniforms[Index].UpdateValue(Value);

		return true;
	}


	bool GrvtMaterial::SetTexture(const Gfl::String& Uniform, GrvtTexture* Texture)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1)
			return false;

		if (Uniforms[Index].Type != GrvtShader_AttrType_Sampler)
			return false;

		Uniforms[Index].UpdateValue(Texture->Type);

		return true;
	}

}