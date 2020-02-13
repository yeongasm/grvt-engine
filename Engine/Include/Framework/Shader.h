#pragma once

#ifndef GRAVITY_ABSTRACTION_SHADER
#define GRAVITY_ABSTRACTION_SHADER

#include "Minimal.h"
#include "API/Graphics/GraphicsDriver.h"

namespace Grvt
{

	struct ShaderImportInfo
	{
		Gfl::String Name;
		Gfl::String PathToVertexShader;
		Gfl::String PathToGeometryShader;
		Gfl::String PathToFragmentShader;
	};


	/**
	*/
	enum AttrType : uint32 
	{
		Shader_Type_None = 0xFF, /** On first init only */
		Shader_Type_Boolean = 0x01,
		Shader_Type_Integer = 0x02,
		Shader_Type_Float = 0x03,
		Shader_Type_Sampler = 0x04,
		Shader_Type_Vector = 0x05,
		Shader_Type_Matrix = 0x06
	};


	/**
	*/
	enum AttrSubType : uint32 
	{
		Shader_SubType_None = 0xFF, /** On first init only */
		Shader_SubType_Sampler1D = 0x01,
		Shader_SubType_Sampler2D = 0x02,
		Shader_SubType_Sampler3D = 0x03,
		Shader_SubType_SamplerCube = 0x04,
		Shader_SubType_Vector2 = 0x05,
		Shader_SubType_Vector3 = 0x06,
		Shader_SubType_Vector4 = 0x07,
		Shader_SubType_Matrix2 = 0x08,
		Shader_SubType_Matrix3 = 0x09,
		Shader_SubType_Matrix4 = 0x0A
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
	};


	using UniformAttr = VertexAttr;


	struct UniformValue
	{
		union {
			bool		Boolean;
			int			Integer;
			float		Float;
			glm::vec2	Vec2;
			glm::vec3	Vec3;
			glm::vec4	Vec4;
			glm::mat2	Mat2;
			glm::mat3	Mat3;
			glm::mat4	Mat4 = glm::mat4(0.0f);
		};

		//template <class T>
		//inline T Cast() const { return *((T*)&Boolean); }

		template <typename T>
		inline void SetValue(const T& Value) { *((T*)&Boolean) = Value; }
	};


	class GrvtShader 
	{
	private:

		GrvtShader(const GrvtShader& Rhs)				= delete;
		GrvtShader& operator= (const GrvtShader& Rhs)	= delete;

		GrvtShader(GrvtShader&& Rhs)				= delete;
		GrvtShader& operator= (GrvtShader&& Rhs)	= delete;

	public:

		Gfl::String VertexShader;
		Gfl::String GeometryShader;
		Gfl::String FragmentShader;

		Gfl::Array<VertexAttr>	Attributes;
		Gfl::Array<UniformAttr> Uniforms;
		GfxHandle				Handle;
	};

}


#endif // !GRAVITY_ABSTRACTION_SHADER