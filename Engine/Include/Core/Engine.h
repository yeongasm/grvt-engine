#pragma once

#ifndef GRAVITY_CORE_ENGINE
#define GRAVITY_CORE_ENGINE

#include "Minimal.h"
#include "Core/ReloadModule.h"
#include "Manager/Manager.h"

namespace Grvt
{

	class GrvtApplication;

	class GrvtEngine
	{
	private:

		friend class GrvtApplication;

		EngineIO			IO;
		ReloadModule		Module;

		Gfl::String			Name;
		ResourceManager*	ResourceMgrPtr;
		GLFWwindow*			Window;
		
		float32				DeltaTime;
		int32				Width;
		int32				Height;
		int32				VersionMajor;
		int32				VersionMinor;

	public:

		bool				VSync;

		friend ENGINE_API GrvtEngine*	InitialiseEngine(const Gfl::String& Name, int32 Width, int32 Height, int32 OpenGLVMajor, int32 OpenGLVMinor);
		friend ENGINE_API void			TerminateEngine();

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

		ENGINE_API EngineIO* GetIO();

		void NewFrame();
		void EndFrame();

		/**
		* Checks if the Engine is still running.
		* Exits the program loop when false is returned.
		*/
		bool Running();

		/**
		* Initialises the currently loaded in the engine.
		* Called automatically inside of the global PreTick() function.
		*/
		void InitModule();

		/**
		* Executes the currently loaded module in the engine.
		* Called every frame inside of the global Tick() function.
		*/
		void ExecuteModule();

		/**
		* Shuts down the currently loaded module in the engine.
		* Called automatically inside of the global PostTick() function.
		*/
		void ShutdownModule();
	};


	ENGINE_API GrvtEngine*	GetEngine();


	ENGINE_API GrvtEngine*	InitialiseEngine(const Gfl::String& Name, int32 Width, int32 Height, int32 OpenGLVMajor, int32 OpenGLVMinor);
	ENGINE_API void			ExecuteEngine();
	ENGINE_API void			TerminateEngine();


}

#endif // !GRAVITY_CORE_ENGINE