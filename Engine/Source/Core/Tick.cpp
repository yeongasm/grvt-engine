#include "GrvtPch.h"

namespace Grvt
{

	extern GrvtEngine* g_Engine;

	void Tick()
	{
		EngineIO* IO = g_Engine->GetIO();

		if (IO->IsKeyHeld(GLFW_KEY_A))
		{
			printf("A is pressed!");
		}
	}

}