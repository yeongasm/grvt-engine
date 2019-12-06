#pragma once

#ifndef GRAVITY_ABSTRACTION_SHADER
#define GRAVITY_ABSTRACTION_SHADER

#include "Minimal.h"
#include "Framework/Foundation/Foundations.h"

namespace Grvt
{

	/**
	* Revamp Status: Completed!
	*/


	enum ShaderComponent : uint32 
	{
		GrvtShader_SourceType_None = 0xFF, /** On first init only */
		GrvtShader_SourceType_Vertex = 0x01,
		GrvtShader_SourceType_Fragment = 0x02,
		GrvtShader_SourceType_Geometry = 0x03
	};


	/**
	* ShaderProps object.
	* Stores information about a specific type of shader.
	*/
	struct ShaderProps 
	{

		Gfl::String			Code;
		Gfl::String			Path;
		ShaderComponent	Component;

		ShaderProps();
		~ShaderProps();

		ShaderProps(const Gfl::String& Code, ShaderComponent Component);
		ShaderProps(const Gfl::String& Code, const Gfl::String& Path, ShaderComponent Component);

	};


	/**
	*/
	struct ShaderImportInfo 
	{

		Gfl::Array<ShaderProps>	Properties;
		Gfl::String				Name;

		ShaderImportInfo();
		~ShaderImportInfo();

		ShaderProps& AddShaderToProgram(const Gfl::String& SourceCode, ShaderComponent Component);
		ShaderProps& AddShaderToProgram(const Gfl::String& SourceCode, const Gfl::String& PathToFile, ShaderComponent Component);

		void PopShaderProperty(ShaderProps&);
	};


	/**
	*/
	enum AttrType : uint32 
	{
		GrvtShader_AttrType_None = 0xFF, /** On first init only */
		GrvtShader_AttrType_Boolean = 0x01,
		GrvtShader_AttrType_Integer = 0x02,
		GrvtShader_AttrType_Float = 0x03,
		GrvtShader_AttrType_Sampler = 0x04,
		GrvtShader_AttrType_Vector = 0x05,
		GrvtShader_AttrType_Matrix = 0x06
	};


	/**
	*/
	enum AttrSubType : uint32 
	{
		GrvtShader_AttrSubType_None = 0xFF, /** On first init only */
		GrvtShader_AttrSubType_Sampler1D = 0x01,
		GrvtShader_AttrSubType_Sampler2D = 0x02,
		GrvtShader_AttrSubType_Sampler3D = 0x03,
		GrvtShader_AttrSubType_SamplerCube = 0x04,
		GrvtShader_AttrSubType_Vector2 = 0x05,
		GrvtShader_AttrSubType_Vector3 = 0x06,
		GrvtShader_AttrSubType_Vector4 = 0x07,
		GrvtShader_AttrSubType_Matrix2 = 0x08,
		GrvtShader_AttrSubType_Matrix3 = 0x09,
		GrvtShader_AttrSubType_Matrix4 = 0x0A
	};


	/**
	* Gravity's VertexAttr data strucutre. A shortform notation for Vertex Attributes.
	*
	* Details the variables specified in the vertex shader.
	*/
	struct VertexAttr 
	{

		Gfl::String		Name;
		uint32			Type;
		uint32			SubType;
		uint32			Location;
		int32			Size;

		VertexAttr();
		~VertexAttr();

	};


	/**
	*/
	struct UniformAttr : public VertexAttr 
	{
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
		inline T Cast() const {
			return *((T*)&Boolean);
		}

		/**
		* Updates the value of the specified uniform.
		*/
		template <class T>
		bool UpdateValue(const T& Value) {
			if (Cast<T>() == Value)
				return false;

			*((T*)&Boolean) = Value;

			return true;
		}

	};


	/**
	* Revamped GrvtShader object.
	*
	* A layer of abstraction is removed to let GrvtShader object itself contain all information.
	* Shaders in Gravity can be imported or custom made.
	*/
	class GrvtShader 
	{
	public:

		Gfl::Array<UniformAttr>	Uniforms;
		Gfl::Array<ShaderProps>	Properties;
		ObjHandle				Handle;

		GrvtShader();
		~GrvtShader();

		void Alloc(const ShaderImportInfo& Import);
		void Free();

	private:

		GrvtShader(const GrvtShader& Rhs) = delete;
		GrvtShader& operator= (const GrvtShader& Rhs) = delete;


		GrvtShader(GrvtShader&& Rhs) = delete;
		GrvtShader& operator= (GrvtShader&& Rhs) = delete;

	};

}


#endif // !GRAVITY_ABSTRACTION_SHADER