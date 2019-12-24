#pragma once

#ifndef GRAVITY_ABSTRACTION_MATERIAL
#define GRAVITY_ABSTRACTION_MATERIAL

#include "Minimal.h"
#include "Framework/Abstraction/Shader.h"
#include "Framework/Abstraction/Texture.h"
#include "Framework/Foundation/Foundations.h"

namespace Grvt
{

	using TexturePair = Gfl::Pair<TextureType, ObjHandle*>;


	/**
	*/
	struct MaterialCreationInfo
	{
		Gfl::String	Name;
		GrvtShader* Shader;
		Gfl::Array<TexturePair>	Textures;
	};


	/**
	*/
	class GrvtMaterial
	{
	private:

		size_t FindUniform(const Gfl::String& Identifier);

	public:

		Gfl::Array<UniformAttr>	Uniforms;
		Gfl::Array<TexturePair>	Textures;
		GrvtShader* Shader;

		ENGINE_API GrvtMaterial();
		ENGINE_API ~GrvtMaterial();

		ENGINE_API GrvtMaterial(const GrvtMaterial&);
		ENGINE_API GrvtMaterial& operator= (const GrvtMaterial&);

		ENGINE_API GrvtMaterial(GrvtMaterial&&);
		ENGINE_API GrvtMaterial& operator= (GrvtMaterial&&);


		/**
		* Assigns the shaders and textures for this material.
		*/
		void Alloc(const MaterialCreationInfo& Info);

		/**
		* Releases all the textures and the shader that is assigned to this material.
		*/
		void Free();


		/**
		* Sets a boolean value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API bool SetBool(const Gfl::String& Uniform, bool Value);

		/**
		* Sets a integer value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API bool SetInt(const Gfl::String& Uniform, int Value);

		/**
		* Sets a float value to a the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API bool SetFloat(const Gfl::String& Uniform, float Value);

		/**
		* Sets a 2 component vector value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API bool SetVector(const Gfl::String& Uniform, glm::vec2 Value);

		/**
		* Sets a 3 component vector value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API bool SetVector(const Gfl::String& Uniform, glm::vec3 Value);

		/**
		* Sets a 4 component vector to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API bool SetVector(const Gfl::String& Uniform, glm::vec4 Value);

		/**
		* Sets a 2x2 matrix value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API bool SetMatrix(const Gfl::String& Uniform, glm::mat2 Value);

		/**
		* Sets a 3x3 matrix value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API bool SetMatrix(const Gfl::String& Uniform, glm::mat3 Value);

		/**
		* Sets a 4x4 matrix value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API bool SetMatrix(const Gfl::String& Uniform, glm::mat4 Value);

		/**
		* Sets a texture with it's sampling unit to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API bool SetTexture(const Gfl::String& Uniform, GrvtTexture* Texture);
	};

}

#endif // !GRAVITY_ABSTRACTION_MATERIAL