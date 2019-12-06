#pragma once

#ifndef GRAVITY_RENDER_CACHE
#define GRAVITY_RENDER_CACHE

#include "Minimal.h"


namespace Grvt
{

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
		uint32	DepthFunc;
		uint32	FrontFace;
		uint32	PolygonMode;

		RenderState();
		~RenderState();

		RenderState(const RenderState& Other);
		RenderState& operator= (const RenderState& Other);

		RenderState(RenderState&& Other);
		RenderState& operator= (RenderState&& Other);
	};


	/**
	* RenderCache data structure.
	*/
	struct RenderCache : public RenderState
	{
		using RenderState::RenderState;

		void SetDepthFunc(uint32 Func);
		void SetCullFace(uint32 Face);
		void SetPolygonMode(uint32 Mode);
	};

}

#endif // !GRAVITY_RENDER_CACHE