#include "pch.h"

Grvt::EngineIO*		m_IO		= nullptr;
Grvt::GrvtEngine*	m_Engine	= nullptr;

extern "C"
{

	void OnStartUp(Grvt::GrvtEngine* EnginePtr)
	{

	}

	void OnLoad(Grvt::GrvtEngine* EnginePtr)
	{
		m_Engine = EnginePtr;
		m_IO = m_Engine->GetIO();
	}

	void ExecuteApplication()
	{
		if (m_IO->IsKeyPressed(GLFW_KEY_T))
		{
			printf("The quick brown fox jumps over the lazy dog!\n");
		}
	}

	void OnUnload()
	{
		m_IO = nullptr;
		m_Engine = nullptr;
	}

}
