#pragma once


using TexturePair = Pair<TextureType, ObjHandle*>;


/**
*/
struct ENGINE_API MaterialCreationInfo
{

	String				Name;
	GrvtShader* Shader;
	Array<TexturePair>	Textures;

};


/**
*/
class ENGINE_API GrvtMaterial
{
private:

	size_t FindUniform(const String& Identifier);

public:

	Array<UniformAttr>		Uniforms;
	Array<TexturePair>		Textures;
	ObjHandle* ShaderHandle;

	GrvtMaterial();
	~GrvtMaterial();

	GrvtMaterial(const GrvtMaterial&);
	GrvtMaterial& operator= (const GrvtMaterial&);

	GrvtMaterial(GrvtMaterial&&);
	GrvtMaterial& operator= (GrvtMaterial&&);


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
	bool SetBool(const String& Uniform, bool Value);

	/**
	* Sets a integer value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetInt(const String& Uniform, int Value);

	/**
	* Sets a float value to a the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetFloat(const String& Uniform, float Value);

	/**
	* Sets a 2 component vector value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetVector(const String& Uniform, glm::vec2 Value);

	/**
	* Sets a 3 component vector value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetVector(const String& Uniform, glm::vec3 Value);

	/**
	* Sets a 4 component vector to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetVector(const String& Uniform, glm::vec4 Value);

	/**
	* Sets a 2x2 matrix value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetMatrix(const String& Uniform, glm::mat2 Value);

	/**
	* Sets a 3x3 matrix value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetMatrix(const String& Uniform, glm::mat3 Value);

	/**
	* Sets a 4x4 matrix value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetMatrix(const String& Uniform, glm::mat4 Value);

	/**
	* Sets a texture with it's sampling unit to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetTexture(const String& Uniform, GrvtTexture* Texture);
};