#include "GrvtPch.h"
#include "Profiler/FrameTime.h"

namespace Grvt
{

	extern GrvtEngine*	g_Engine;
	extern FrameTime	g_FrameTime;

	void PreTick()
	{
		g_Engine->InitModule();
	}

	void Tick()
	{
		g_Engine->ExecuteModule();
	}

	void PostTick()
	{
		g_Engine->ShutdownModule();
	}

}