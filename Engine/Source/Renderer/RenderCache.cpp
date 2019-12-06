#include "glad/glad.h"
#include "Renderer/RenderCache.h"

namespace Grvt
{

	RenderState::RenderState() :
		DepthFunc(GL_CCW), FrontFace(GL_LESS), PolygonMode(GL_FILL) {}


	RenderState::~RenderState()
	{
		DepthFunc = FrontFace = PolygonMode = 0;
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
			DepthFunc = Other.DepthFunc;
			FrontFace = Other.FrontFace;
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
			DepthFunc = Other.DepthFunc;
			FrontFace = Other.FrontFace;
			PolygonMode = Other.PolygonMode;

			new (&Other) RenderState();
		}

		return *this;
	}


	void RenderCache::SetDepthFunc(uint32 Func)
	{
		if (DepthFunc)
		{
			if (DepthFunc != Func)
			{
				DepthFunc = Func;

				glEnable(GL_DEPTH_TEST);
				glDepthFunc(DepthFunc);
			}

			return;
		}

		glDisable(GL_DEPTH_TEST);
	}


	void RenderCache::SetCullFace(uint32 Face)
	{
		if (FrontFace)
		{
			if (FrontFace != Face)
			{
				FrontFace = Face;

				glEnable(GL_CULL_FACE);
				glCullFace(FrontFace);
			}

			return;
		}

		glDisable(GL_CULL_FACE);
	}


	void RenderCache::SetPolygonMode(uint32 Mode)
	{
		if (PolygonMode != Mode)
		{
			PolygonMode = Mode;
			glPolygonMode(GL_FRONT_AND_BACK, PolygonMode);
		}
	}

}