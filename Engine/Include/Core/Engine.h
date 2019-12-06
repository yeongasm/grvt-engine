#pragma once

#ifndef GRAVITY_CORE_ENGINE
#define GRAVITY_CORE_ENGINE

#include "Minimal.h"

namespace Grvt
{

	class GrvtApplication;

	class GrvtEngine
	{
	private:

		friend class GrvtApplication;

		EngineIO			IO;
		Gfl::String			Name;
		GLFWwindow*			Window;
		GrvtApplication*	Application;
		int32				Width;
		int32				Height;
		int32				VersionMajor;
		int32				VersionMinor;
		bool				VSync;

	public:

		float32				DeltaTime;

		friend ENGINE_API GrvtEngine* InitialiseEngine(const Gfl::String& Name, int32 Width, int32 Height, int32 OpenGLVMajor, int32 OpenGLVMinor);
		friend ENGINE_API void		TerminateEngine();

	private:

		static void ScrollCallback(GLFWwindow* Window, float64 HorizontalOffset, float64 VerticalOffset);
		static void MouseCallback(GLFWwindow* Window, int32 Button, int32 Action, int32 Mods);
		static void KeyCallback(GLFWwindow* Window, int32 Key, int32 ScanCode, int32 Action, int32 Mods);
		static void	FramebufferCallback(GLFWwindow* Window, int32 Width, int32 Height);

		void Alloc();
		void Free();
		void UpdateIO();

	public:

		ENGINE_API GrvtEngine();
		ENGINE_API	~GrvtEngine();

		GrvtEngine(const GrvtEngine& Other) = delete;
		GrvtEngine& operator= (const GrvtEngine& Other) = delete;

		GrvtEngine(GrvtEngine&& Other) = delete;
		GrvtEngine& operator= (GrvtEngine&& Other) = delete;

		ENGINE_API void				NewFrame();
		ENGINE_API void				EndFrame();
		ENGINE_API EngineIO*		GetIO();
		ENGINE_API GrvtApplication* GetApplication();
		ENGINE_API bool				Running();
	};

	ENGINE_API GrvtEngine*	InitialiseEngine(const Gfl::String& Name, int32 Width, int32 Height, int32 OpenGLVMajor, int32 OpenGLVMinor);
	ENGINE_API GrvtEngine*	GetEngine();
	ENGINE_API void			TerminateEngine();

	extern "C" ENGINE_API void			EngineTick();

}

#endif // !GRAVITY_CORE_ENGINE