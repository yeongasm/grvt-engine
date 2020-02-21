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

		GfxVarType	Type;

		UniformValue() : 
			Type(GfxVarType::Gfx_Type_None) {}


		UniformValue(GfxVarType VarType) :
			Type(VarType) {}
		

		~UniformValue() { Type = GfxVarType::Gfx_Type_None; }

		//template <class T>
		//inline T Cast() const { return *((T*)&Boolean); }

		template <typename T>
		inline void SetValue(const T& Value) { *((T*)&Boolean) = Value; }
	};


	class GrvtShader 
	{
	private:

		using VertexAttributes	= std::map<Gfl::HashString, VertexAttr>;
		using UniformAttributes = std::map<Gfl::HashString, UniformAttr>;

		GrvtShader(const GrvtShader& Rhs)				= delete;
		GrvtShader& operator= (const GrvtShader& Rhs)	= delete;

		GrvtShader(GrvtShader&& Rhs)				= delete;
		GrvtShader& operator= (GrvtShader&& Rhs)	= delete;

	public:

		Gfl::String VertexShader;
		Gfl::String GeometryShader;
		Gfl::String FragmentShader;

		VertexAttributes	Attributes;
		UniformAttributes	Uniforms;
		GfxHandle			Handle;

		GrvtShader();
		~GrvtShader();
	};

}


#endif // !GRAVITY_ABSTRACTION_SHADER