#include "GrvtPch.h"
#include "Framework/Abstraction/Material.h"


namespace Grvt
{

	GrvtMaterial::GrvtMaterial() :
		Uniforms(), Textures(), Shader(nullptr) {}


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
			Shader = Other.Shader;
		}

		return *this;
	}


	GrvtMaterial& GrvtMaterial::operator= (GrvtMaterial&& Other)
	{
		if (this != &Other)
		{
			Uniforms = Other.Uniforms;
			Textures = Other.Textures;
			Shader = Other.Shader;

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
		Shader = Info.Shader;
		Textures = Info.Textures;
		Uniforms = Info.Shader->Uniforms;
	}


	void GrvtMaterial::Free()
	{
		Shader = nullptr;
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


	bool GrvtMaterial::SetTexture(const Gfl::String& Uniform, TextureType Type)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1)
			return false;

		if (Uniforms[Index].Type != GrvtShader_AttrType_Sampler)
			return false;

		Uniforms[Index].UpdateValue(Type);

		return true;
	}


	bool GrvtMaterial::SetTexture(const Gfl::String& Uniform, int32 Unit)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1)
			return false;

		if (Uniforms[Index].Type != GrvtShader_AttrType_Sampler)
			return false;

		Uniforms[Index].UpdateValue(Unit);

		return true;
	}

	bool GrvtMaterial::SetTextures(const Gfl::String& Uniform, int32 Unit)
	{
		size_t Index = FindUniform(Uniform);

		if (Index == -1) {
			uint32 Location = BaseAPI::Shader::GrGetUniformLocation(Shader->Handle, Uniform.C_Str());

			if (Location == -1)
				return false;

			// Add the uniform in if it exists in shader.
			UniformAttr Temp;
			Temp.Name = Uniform;
			Temp.Location = Location;
			Temp.Type = GrvtShader_AttrType_Sampler;

			Index = Uniforms.Push(Temp);
		}

		if (Uniforms[Index].Type != GrvtShader_AttrType_Sampler)
			return false;

		Uniforms[Index].UpdateValue(Unit);

		return true;
	}
}