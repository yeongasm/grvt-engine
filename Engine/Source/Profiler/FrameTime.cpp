#include "GrvtPch.h"
#include "Profiler/FrameTime.h"


namespace Grvt
{

	void FrameTime::Update(float32 DeltaTime)
	{
		FrameSecPerFrameAccum += DeltaTime - FrameSecPerFrame[FrameSecIndex];
		FrameSecPerFrame[FrameSecIndex] = DeltaTime;
		FrameSecIndex = ((int32)FrameSecIndex + 1) % (int32)(GRVT_ARRAY_LENGTH(FrameSecPerFrame));
	}

	float32 FrameTime::Framerate() const
	{
		return (FrameSecPerFrameAccum > 0.0f) ? (1.0f / (FrameSecPerFrameAccum / (float32)GRVT_ARRAY_LENGTH(FrameSecPerFrame))) : FLT_MAX;
	}

	float32 FrameTime::TimePerFrame() const
	{
		return 1000.0f / Framerate();
	}

}