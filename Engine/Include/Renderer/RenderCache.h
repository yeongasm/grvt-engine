#pragma once

#ifndef GRAVITY_RENDER_CACHE
#define GRAVITY_RENDER_CACHE

#include "Minimal.h"


namespace Grvt
{
	/**
	* When implementing sorting, the bit layout will be as follow (starting from the LSB):
	* Source Blend		- 14 bits.
	* Destination Blend - 14 bits.
	* Depth Testing		- 8 bits.
	* Winding order		- 4 bits.
	* Cull face			- 4 bits.
	* Polygon Mode		- 4 bits.
	* Polygon Face		- 4 bits.
	* Shader			- 12 bits.
	*/
	enum BlendFuncs : uint32
	{
		BlendFunc_None						= 0x00,
		BlendFunc_Zero						= 0x00,
		BlendFunc_One						= 0x01,
		BlendFunc_Src_Colour				= 0x02,
		BlendFunc_One_Minus_Src_Colour		= 0x03,
		BlendFunc_Dst_Colour				= 0x04,
		BlendFunc_One_Minus_Dst_Colour		= 0x05,
		BlendFunc_Src_Alpha					= 0x06,
		BlendFunc_One_Minus_Src_Alpha		= 0x07,
		BlendFunc_Dst_Alpha					= 0x08,
		BlendFunc_One_Minus_Dst_Alpha		= 0x09,
		BlendFunc_Constant_Colour			= 0x0A,
		BlendFunc_One_Minus_Constant_Colour = 0x0B,
		BlendFunc_Constant_Alpha			= 0x0C,
		BlendFunc_One_Minus_Constant_Alpha	= 0x0D
	};

	enum DepthFuncs : uint32
	{
		DepthFunc_None		= 0x00,
		DepthFunc_Always	= 0x0E,
		DepthFunc_Never		= 0x0F,
		DepthFunc_Less		= 0x10,
		DepthFunc_Equal		= 0x11,
		DepthFunc_LEqual	= 0x12,
		DepthFunc_Greater	= 0x13,
		DepthFunc_Not_Equal = 0x14,
		DepthFunc_GEqual	= 0x15
	};

	enum CullFaces : uint32
	{
		CullFace_Front			= 0x16,
		CullFace_Back			= 0x17,
		CullFace_Front_And_Back = 0x18,
	};

	enum FrontFaces : uint32
	{
		FrontFace_CW	= 0x19,
		FrontFace_CCW	= 0x1A,
	};

	enum PolygonFaces : uint32
	{
		PolygonFace_Front			= 0x16,
		PolygonFace_Back			= 0x17,
		PolygonFace_Front_And_Back	= 0x18
	};

	enum PolygonModes : uint32
	{
		PolygonMode_Point	= 0x1B,
		PolygonMode_Line	= 0x1C,
		PolygonMode_Fill	= 0x1D
	};

	/**
	* RenderState data structure.
	*
	* NOTE(Afiq):
	* Gravity currently does not support blending.
	* A RenderState is enabled when the variable related to that state is no longer 0.
	*
	* TODO(Afiq):
	* Work on blending once Order Independent Transparency is implemented.
	*/
	struct RenderState
	{
		uint32	SrcBlend;
		uint32	DstBlend;
		uint32	DepthFunc;		/** OpenGL Depth functions. */
		uint32	CullFace;		/** Face to cull, front, back or front and back */
		uint32	FrontFace;		/** Front face's winding order. */
		uint32	PolygonFace;
		uint32	PolygonMode;	/** Mesh's polygon mode */

		ENGINE_API RenderState();
		ENGINE_API ~RenderState();

		RenderState(const RenderState& Other);
		RenderState& operator= (const RenderState& Other);

		RenderState(RenderState&& Other);
		RenderState& operator= (RenderState&& Other);
	};


	/**
	* RenderCache data structure.
	* TODO(Afiq):
	* This needs to be restructured.
	*/
	struct RenderCache : public RenderState
	{
		using RenderState::RenderState;

		void SetAlphaBlend(uint32 Source, uint32 Destination);
		void SetDepthFunc(uint32 Func);
		void SetCullFace(uint32 Face);
		void SetPolygonMode(uint32 Mode);
	};

}

#endif // !GRAVITY_RENDER_CACHE