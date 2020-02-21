#pragma once

#ifndef GRAVITY_ABSTRACTION_MATERIAL
#define GRAVITY_ABSTRACTION_MATERIAL

#include "Minimal.h"
#include "API/Graphics/GraphicsDriver.h"

#include "Framework/Shader.h"
#include "Framework/Texture.h"

namespace Grvt
{

	using TexturePair = Gfl::Pair<TextureType, GfxHandle*>;
	using MatTextures = Gfl::Array<TexturePair>;


	struct MaterialCreationInfo
	{
		Gfl::String Name;
		GrvtShader* Shader = nullptr;
		MatTextures Textures;
	};


	/**
	*/
	class GrvtMaterial
	{
	private:

		using UniformMap = std::map<Gfl::HashString, UniformValue>;
		
	public:

		UniformMap	Uniforms;
		MatTextures Textures;
		GrvtShader* Shader;

		ENGINE_API GrvtMaterial();
		ENGINE_API ~GrvtMaterial();

		ENGINE_API GrvtMaterial(const GrvtMaterial&);
		ENGINE_API GrvtMaterial& operator= (const GrvtMaterial&);

		ENGINE_API GrvtMaterial(GrvtMaterial&&);
		ENGINE_API GrvtMaterial& operator= (GrvtMaterial&&);


		ENGINE_API void Alloc(const MaterialCreationInfo& Info);
		ENGINE_API void Free();


		/**
		* Sets a boolean value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API void SetBool(const Gfl::String& Name, bool Value);

		/**
		* Sets a integer value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API void SetInt(const Gfl::String& Name, int Value);

		/**
		* Sets a float value to a the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API void SetFloat(const Gfl::String& Name, float Value);

		/**
		* Sets a 2 component vector value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API void SetVector(const Gfl::String& Name, glm::vec2 Value);

		/**
		* Sets a 3 component vector value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API void SetVector(const Gfl::String& Name, glm::vec3 Value);

		/**
		* Sets a 4 component vector to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API void SetVector(const Gfl::String& Name, glm::vec4 Value);

		/**
		* Sets a 2x2 matrix value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API void SetMatrix(const Gfl::String& Name, glm::mat2 Value);

		/**
		* Sets a 3x3 matrix value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API void SetMatrix(const Gfl::String& Name, glm::mat3 Value);

		/**
		* Sets a 4x4 matrix value to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API void SetMatrix(const Gfl::String& Name, glm::mat4 Value);

		/**
		* Sets a texture with it's sampling unit to the specified uniform.
		* Does not update the value that's in the shader.
		* Returns false if uniform with the specified name is not found.
		*/
		ENGINE_API void SetTexture(const Gfl::String& Name, TextureType SamplerUnit);
	};

}

#endif // !GRAVITY_ABSTRACTION_MATERIAL