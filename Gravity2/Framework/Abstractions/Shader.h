#pragma once


/**
* Revamp Status: Completed!
*/


enum ShaderComponent : uint32 {
	GrvtShader_SourceType_None		= 0xFF, /** On first init only */
	GrvtShader_SourceType_Vertex	= 0x01,
	GrvtShader_SourceType_Fragment	= 0x02,
	GrvtShader_SourceType_Geometry	= 0x03
};


enum GrShaderType : uint32 {
	GrvtShader_None		= 0xFF, /** On first init only */
	GrvtShader_Import	= 0x01,
	GrvtShader_Custom	= 0x02
};


/**
* ShaderProps object.
* Stores information about a specific type of shader.
*/
struct ShaderProps {

	String			Code;
	String			Path;
	ShaderComponent	Component;
	GrShaderType	Type;

	ShaderProps(const String&, ShaderComponent);
	ShaderProps(const String&, const String&, ShaderComponent);

};


/**
*/
struct ShaderImportInfo {

	Array<ShaderProps>	Properties;
	String				Name;

	ShaderImportInfo();
	~ShaderImportInfo();

	ShaderProps& AddShaderToProgram(const String&, ShaderComponent);
	ShaderProps& AddShaderToProgram(const String&, const String&, ShaderComponent);

	void PopShaderProperty(ShaderProps&);
};


/**
*/
enum AttrType : uint32 {
	GrvtShader_AttrType_None	= 0xFF, /** On first init only */
	GrvtShader_AttrType_Boolean = 0x01,
	GrvtShader_AttrType_Integer = 0x02,
	GrvtShader_AttrType_Float	= 0x03,
	GrvtShader_AttrType_Sampler = 0x04,
	GrvtShader_AttrType_Vector	= 0x05,
	GrvtShader_AttrType_Matrix	= 0x06
};


/**
*/
enum AttrSubType : uint32 {
	GrvtShader_AttrSubType_None			= 0xFF, /** On first init only */
	GrvtShader_AttrSubType_Sampler1D	= 0x01,
	GrvtShader_AttrSubType_Sampler2D	= 0x02,
	GrvtShader_AttrSubType_Sampler3D	= 0x03,
	GrvtShader_AttrSubType_SamplerCube	= 0x04,
	GrvtShader_AttrSubType_Vector2		= 0x05,
	GrvtShader_AttrSubType_Vector3		= 0x06,
	GrvtShader_AttrSubType_Vector4		= 0x07,
	GrvtShader_AttrSubType_Matrix2		= 0x08,
	GrvtShader_AttrSubType_Matrix3		= 0x09,
	GrvtShader_AttrSubType_Matrix4		= 0x0A
};


/**
* Gravity's VertexAttr data strucutre. A shortform notation for Vertex Attributes.
*
* Details the variables specified in the vertex shader.
*/
struct VertexAttr {

	String		Name;
	AttrType	Type;
	AttrSubType SubType;
	uint32		Location;
	int32		Size;

	VertexAttr();
	~VertexAttr();

};


/**
*/
struct UniformAttr : public VertexAttr {
private:

	union {
		bool		Boolean;
		int			Integer;
		float		Float;
		glm::vec2	Vec2;
		glm::vec3	Vec3;
		glm::vec4	Vec4;
		glm::mat2	Mat2;
		glm::mat3	Mat3;
		glm::mat4	Mat4;
	};
	
public:

	UniformAttr();
	~UniformAttr();

	/**
	* Retrieve the value of the uniform by specifying it's type from the Shader.
	*/
	template <class T> 
	inline T Cast() const { return *((T*)&Boolean); }

	/**
	* Updates the value of the specified uniform.
	*/
	template <class T>
	bool UpdateValue(const T &Value) {
		if (Cast<T>() == Value)
			return false;

		*((T*)&Boolean) = Value;

		return true;
	}

};


/**
* Gravity engine's wrapper for OpenGL vertex attributes and shader uniforms.
* Vertex attributes from shader are not meant to be changed but is available for references.
* Shader uniforms are stored as a single union with types bool, int, float, vec2, vec3, vec4, mat2, mat3 and mat4.
* NOTE: Uniform types must be known!
*
* To update a value of a uniform, call UpdateValue() and specify the uniform's type inside the template specifier.
* To use a value of a uniform, call Cast() and specify the uniform's type inside the template specifier.
*/
struct ShaderVar {
	
	std::map<String, VertexAttr>	Attributes;
	std::map<String, UniformAttr>	Uniforms;

	ShaderVar();
	~ShaderVar();

	ShaderVar(const ShaderVar &Other);
	ShaderVar& operator= (const ShaderVar &Other);

	ShaderVar(ShaderVar &&Other);
	ShaderVar& operator= (ShaderVar &&Other);

};


/**
* Revamped GrvtShader object.
* 
* A layer of abstraction is removed to let GrvtShader object itself contain all information.
* Shaders in Gravity can be imported or custom made.
*/
class GrvtShader {
public:

	Array<ShaderProps>	Properties;
	ShaderVar			Variables;
	String				Name;
	ObjHandle			Handle;

	GrvtShader();
	~GrvtShader();

	void Alloc(const ShaderImportInfo&);
	void Free();

private:

	GrvtShader(const GrvtShader&)				= delete;
	GrvtShader& operator= (const GrvtShader&)	= delete;
	

	GrvtShader(GrvtShader&&)					= delete;
	GrvtShader& operator= (GrvtShader&&)		= delete;

	//void Use();
	//void SetInt(const char*, int);
	//void SetInt(const UniformAttr&);
	//void SetBool(const char*, bool);
	//void SetBool(const UniformAttr&);
	//void SetFloat(const char*, float);
	//void SetFloat(const UniformAttr&);
	//void SetVector(const char*, glm::vec2);
	//void SetVector(const char*, glm::vec3);
	//void SetVector(const char*, glm::vec4);
	//void SetVector(const UniformAttr &Uniform);
	//void SetMatrix(const char*, glm::mat2);
	//void SetMatrix(const char*, glm::mat3);
	//void SetMatrix(const char*, glm::mat4);
	//void SetMatrix(const UniformAttr&);
	//void SetUniform(const UniformAttr&);
};