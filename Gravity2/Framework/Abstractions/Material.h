#pragma once


using TexturePair = Pair<TextureType, ObjHandle*>;


/**
*/
class MaterialCreationInfo {
public:

	String				Name;
	
private:

	ObjHandle*			ShaderHandle;
	Array<TexturePair>	Textures;

public:

	MaterialCreationInfo();
	~MaterialCreationInfo();

	void PushTexture(GrvtTexture* Texture);
	void PopTexture(GrvtTexture* Texture);
	void SetShader(GrvtShader* Shader);
	void RemoveShader();

};


/**
*/
class GrvtMaterial {
public:

	std::unordered_map<String, UniformAttr>	Uniforms;
	
private:

	Array<TexturePair>				Textures;
	
public:

	ObjHandle*						ShaderHandle;

	GrvtMaterial();
	~GrvtMaterial();
	
	GrvtMaterial(const GrvtMaterial&);
	GrvtMaterial& operator= (const GrvtMaterial&);

	GrvtMaterial(GrvtMaterial&&);
	GrvtMaterial& operator= (GrvtMaterial&&);

	/**
	* Sets a boolean value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetBool	(const String& Uniform, bool Value);

	/**
	* Sets a integer value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetInt		(const String& Uniform, int Value);
	
	/**
	* Sets a float value to a the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetFloat	(const String& Uniform, float Value);
	
	/**
	* Sets a 2 component vector value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetVector	(const String& Uniform, glm::vec2 Value);
	
	/**
	* Sets a 3 component vector value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetVector	(const String& Uniform, glm::vec3 Value);
	
	/**
	* Sets a 4 component vector to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetVector	(const String& Uniform, glm::vec4 Value);
	
	/**
	* Sets a 2x2 matrix value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetMatrix	(const String& Uniform, glm::mat2 Value);
	
	/**
	* Sets a 3x3 matrix value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetMatrix	(const String& Uniform, glm::mat3 Value);
	
	/**
	* Sets a 4x4 matrix value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetMatrix	(const String& Uniform, glm::mat4 Value);
	
	/**
	* Sets a texture with it's sampling unit to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetTexture	(const String& Uniform, GrvtTexture *Texture);

};