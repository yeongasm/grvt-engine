#include "GrvtPch.h"
#include "Framework/Material.h"


namespace Grvt
{

	GrvtMaterial::GrvtMaterial() :
		Uniforms(), Textures(), Shader(nullptr) {}


	GrvtMaterial::~GrvtMaterial() 
	{
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

	void GrvtMaterial::Alloc(const MaterialCreationInfo& Info)
	{
		Shader	 = Info.Shader;
		Textures = Info.Textures;
	}


	void GrvtMaterial::Free()
	{
		Shader = nullptr;
		Uniforms.clear();
		Textures.Release();
	}


	void GrvtMaterial::SetBool(const Gfl::String& Name, bool Value)
	{
		Uniforms[Name].Boolean = Value;
	}


	void GrvtMaterial::SetInt(const Gfl::String& Name, int Value)
	{
		Uniforms[Name].Integer = Value;
	}


	void GrvtMaterial::SetFloat(const Gfl::String& Name, float Value)
	{
		Uniforms[Name].Float = Value;
	}


	void GrvtMaterial::SetVector(const Gfl::String& Name, glm::vec2 Value)
	{
		Uniforms[Name].Vec2 = Value;
	}


	void GrvtMaterial::SetVector(const Gfl::String& Name, glm::vec3 Value)
	{
		Uniforms[Name].Vec3 = Value;
	}


	void GrvtMaterial::SetVector(const Gfl::String& Name, glm::vec4 Value)
	{
		Uniforms[Name].Vec4 = Value;
	}


	void GrvtMaterial::SetMatrix(const Gfl::String& Name, glm::mat2 Value)
	{
		Uniforms[Name].Mat2 = Value;
	}


	void GrvtMaterial::SetMatrix(const Gfl::String& Name, glm::mat3 Value)
	{
		Uniforms[Name].Mat3 = Value;
	}


	void GrvtMaterial::SetMatrix(const Gfl::String& Name, glm::mat4 Value)
	{
		Uniforms[Name].Mat4 = Value;
	}


	void GrvtMaterial::SetTexture(const Gfl::String& Name, TextureType SamplerUnit)
	{
		Uniforms[Name].Integer = (int32)SamplerUnit;
	}


}