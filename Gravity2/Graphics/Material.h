#pragma once


/**
* Material creation data structure.
* All materials created required a name for identification. Failing to provide a name would cause Gravity to abort it's material generation process.
* If no shader is specified into the struct, Gravity simply does not retrieve uniforms associated with the shader. This however can be done manually later.
* 
* @param [REQUIRED] (String)		name		- Identification of the specified material.
* @param [OPTIONAL] (Shader*)		shader		- Shader associated with this material. Gravity automatically retrieves uniforms associated with this shader when specified.
* @param [OPTIONAL] (TextureArr)	textures	- List of textures for this material. However, mapping textures to it's unit to the mesh needs to be done manually. 
*/
struct MaterialCreationInfo {

	String				Name;
	ShaderObj			*Shader;
	Array<TextureObj*>	Textures;

	MaterialCreationInfo();
	~MaterialCreationInfo();
	
	MaterialCreationInfo(const MaterialCreationInfo &Other);
	MaterialCreationInfo& operator= (const MaterialCreationInfo &Other);

	MaterialCreationInfo(MaterialCreationInfo &&Other);
	MaterialCreationInfo& operator= (MaterialCreationInfo &&Other);

};


// Forward declare data struct.
struct MaterialData;


/**
* Material data structure.
* 
* TODO(Afiq):
* Filter out the uniforms to only contain uniforms that are relevant to materials.
* One approach to this is to use a fix naming scheme for our materials in glsl.
*/
class MaterialObj {
public:

	String							Name;
	ShaderObj						*Shader;
	MaterialData					*Info;
	Array<TextureObj*>				Textures;
	std::map<String, UniformAttr>	Uniforms;

	MaterialObj();
	~MaterialObj();
	
	MaterialObj(const MaterialObj &Other);
	MaterialObj& operator= (const MaterialObj &Other);

	MaterialObj(MaterialObj &&Other);
	MaterialObj& operator= (MaterialObj &&Other);

	/**
	* Allocates a new material.
	*/
	void Alloc		(const MaterialCreationInfo &Info);

	/**
	* Released all data from the Material struct.
	*/
	void Free		();

	/**
	* Sets a boolean value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetBool	(const String &Uniform, bool Value);

	/**
	* Sets a integer value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetInt		(const String &Uniform, int Value);
	
	/**
	* Sets a float value to a the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetFloat	(const String &Uniform, float Value);
	
	/**
	* Sets a 2 component vector value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetVector	(const String &Uniform, glm::vec2 Value);
	
	/**
	* Sets a 3 component vector value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetVector	(const String &Uniform, glm::vec3 Value);
	
	/**
	* Sets a 4 component vector to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetVector	(const String &Uniform, glm::vec4 Value);
	
	/**
	* Sets a 2x2 matrix value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetMatrix	(const String &Uniform, glm::mat2 Value);
	
	/**
	* Sets a 3x3 matrix value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetMatrix	(const String &Uniform, glm::mat3 Value);
	
	/**
	* Sets a 4x4 matrix value to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetMatrix	(const String &Uniform, glm::mat4 Value);
	
	/**
	* Sets a texture with it's sampling unit to the specified uniform.
	* Does not update the value that's in the shader.
	* Returns false if uniform with the specified name is not found.
	*/
	bool SetTexture	(const String &Uniform, TextureObj *Texture);
};