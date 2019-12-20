#include "GrvtPch.h"
#include "Renderer/RenderCache.h"

namespace Grvt
{

	uint32 g_StateParams[32] = { GL_NONE, GL_ONE,
		GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR,
		GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA,
		GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA,
		GL_ALWAYS, GL_NEVER,
		GL_LESS, GL_EQUAL,
		GL_LEQUAL, GL_GREATER,
		GL_NOTEQUAL, GL_GEQUAL,
		GL_FRONT, GL_BACK,
		GL_FRONT_AND_BACK, GL_CW,
		GL_CCW, GL_POINT,
		GL_LINE, GL_FILL
	};

	RenderState::RenderState() :
		SrcBlend(BlendFunc_None),
		DstBlend(BlendFunc_None),
		DepthFunc(DepthFunc_Less),
		CullFace(CullFace_Back),
		FrontFace(FrontFace_CCW),
		PolygonFace(PolygonFace_Front_And_Back),
		PolygonMode(PolygonMode_Fill) {}

	RenderState::~RenderState()
	{
		SrcBlend = 0;
		DstBlend = 0;
		DepthFunc = 0;
		CullFace = 0;
		FrontFace = 0;
		PolygonFace = 0;
		PolygonMode = 0;
	}


	RenderState::RenderState(const RenderState& Other)
	{
		*this = Other;
	}


	RenderState& RenderState::operator= (const RenderState& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			SrcBlend = Other.SrcBlend;
			DstBlend = Other.DstBlend;
			DepthFunc = Other.DepthFunc;
			CullFace = Other.CullFace;
			FrontFace = Other.FrontFace;
			PolygonFace = Other.PolygonFace;
			PolygonMode = Other.PolygonMode;
		}

		return *this;
	}


	RenderState::RenderState(RenderState&& Other)
	{
		*this = Gfl::Move(Other);
	}


	RenderState& RenderState::operator= (RenderState&& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other) {
			SrcBlend = Other.SrcBlend;
			DstBlend = Other.DstBlend;
			DepthFunc = Other.DepthFunc;
			CullFace = Other.CullFace;
			FrontFace = Other.FrontFace;
			PolygonFace = Other.PolygonFace;
			PolygonMode = Other.PolygonMode;

			new (&Other) RenderState();
		}

		return *this;
	}

	
	void RenderCache::SetAlphaBlend(uint32 Source, uint32 Destination)
	{
		if (Source && Destination)
		{
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable(GL_BLEND);
		}

		if (SrcBlend != Source || DstBlend != Destination)
		{
			SrcBlend = Source;
			DstBlend = Destination;

			glBlendFunc(g_StateParams[SrcBlend], g_StateParams[DstBlend]);
		}
	}


	void RenderCache::SetDepthFunc(uint32 Func)
	{
		if (DepthFunc)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		if (DepthFunc != Func)
		{
			DepthFunc = Func;

			glDepthFunc(g_StateParams[DepthFunc]);
		}
	}


	void RenderCache::SetCullFace(uint32 Face)
	{
		if (FrontFace != Face)
		{
			FrontFace = Face;

			glFrontFace(g_StateParams[FrontFace]);
			glCullFace(g_StateParams[CullFace]);
		}	
	}


	void RenderCache::SetPolygonMode(uint32 Mode)
	{
		if (PolygonMode != Mode)
		{
			PolygonMode = Mode;
			glPolygonMode(g_StateParams[PolygonFace], g_StateParams[PolygonMode]);
		}
	}

}