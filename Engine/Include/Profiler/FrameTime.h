#pragma once

#ifndef GRAVITY_PROFILER_FRAMETIME
#define GRAVITY_PROFILER_FRAMETIME

#include "Minimal.h"

namespace Grvt
{

	class FrameTime
	{
	private:

		int32	FrameSecIndex;
		float32	FrameSecPerFrameAccum;
		float32 FrameSecPerFrame[120];

	public:

		FrameTime()		= default;
		~FrameTime()	= default;

		FrameTime(const FrameTime& Other)				= default;
		FrameTime& operator= (const FrameTime& Other)	= default;

		FrameTime(FrameTime&& Other)					= default;
		FrameTime& operator= (FrameTime&& Other)		= default;

		void	Update			(float32 DeltaTime);
		float32 Framerate		()		const;
		float32	TimePerFrame	()		const;

	};

}

#endif // !GRAVITY_PROFILER_FRAMETIME