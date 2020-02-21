#include "GrvtPch.h"
#include "Profiler/FrameTime.h"

#include "Framework/Scene.h"
#include "Renderer/Renderer.h"


extern Grvt::GrvtScene*		g_ActiveScene	= nullptr;
extern Grvt::GrvtEngine*	g_Engine		= nullptr;


namespace Grvt
{

	GrvtEngine* g_Engine = nullptr;
	FrameTime	g_FrameTime;
	float		LastFrameTime = 0.0f;
	float		CurrentFrameTime = 0.0f;

	GrvtEngine* InitialiseEngine(const Gfl::String& Name, int32 Width, int32 Height, int32 OpenGLVMajor, int32 OpenGLVMinor)
	{
		if (!g_Engine)
			g_Engine = new GrvtEngine();

		g_Engine->Name = Name;
		g_Engine->Width = Width;
		g_Engine->Height = Height;
		g_Engine->VersionMajor = OpenGLVMajor;
		g_Engine->VersionMinor = OpenGLVMinor;

		g_Engine->Alloc();

		return g_Engine;
	}


	GrvtEngine* GetEngine()
	{
		return g_Engine;
	}


	void ExecuteEngine()
	{
		GrvtRenderer* RenderCtx = GetRenderer();
		GraphicsInterface* GfxInterface = GetGraphicsInterface();


		while (g_Engine->Running())
		{
			g_Engine->NewFrame();

			g_Engine->ExecuteModule();

			/**
			* NOTE(Afiq):
			*
			* Allow the scene to fill the contents of the renderer's back buffer when it is empty.
			* In a single threaded program, this scenario would never happen.
			* When the time comes to multithread the engine, we'll need to follow Dan's RenderService example.
			*/
			if (g_ActiveScene && RenderCtx->BackBuffer.IsEmpty)
			{
				g_ActiveScene->CreateSceneCommandBuffer(RenderCtx->BackBuffer);
			}

			// Build resources at the end of the frame.
			GfxInterface->Tick();

			// Render contents from back buffer.
			RenderCtx->Render();

			g_Engine->EndFrame();
		}

		g_Engine->ShutdownSystems();
		g_Engine->ShutdownModule();
	}


	void TerminateEngine()
	{
		ShutdownResourceManager();
		ShutdownRenderContext();

		g_Engine->Free();
		delete g_Engine;
	}


	void GrvtEngine::ScrollCallback(GLFWwindow* Window, float64 HorizontalOffset, float64 VerticalOffset)
	{
		g_Engine->IO.MouseWheel  += (float32)VerticalOffset;
		g_Engine->IO.MouseWheelH += (float32)HorizontalOffset;
	}


	void GrvtEngine::MouseCallback(GLFWwindow* Window, int32 Button, int32 Action, int32 Mods)
	{
		if (Action == GLFW_PRESS && Button >= 0 && Button <= GRVT_ARRAY_LENGTH(g_Engine->IO.MouseButton))
		{
			g_Engine->IO.MouseButton[Button].CurrState = GLFW_PRESS;
		}
	}


	void GrvtEngine::KeyCallback(GLFWwindow* Window, int32 Key, int32 ScanCode, int32 Action, int32 Mods)
	{
		if (Key >= 0 && Key < 512)
		{
			if (Action == GLFW_PRESS)
			{
				g_Engine->IO.Keys[Key].CurrState = GLFW_PRESS;
			}

			if (Action == GLFW_RELEASE)
			{
				g_Engine->IO.Keys[Key].CurrState = GLFW_RELEASE;
			}
		}
	}


	void GrvtEngine::FramebufferCallback(GLFWwindow* Window, int32 Width, int32 Height)
	{
		g_Engine->Width  = Width;
		g_Engine->Height = Height;
	}


	GrvtEngine::GrvtEngine() :
		IO(),
		GraphicsCtx(nullptr),
		Module(),
		Systems(),
		Name(),
		ResourceMgrPtr(nullptr),
		Window(nullptr),
		DeltaTime(0.0f),
		PosX(0),
		PosY(0),
		Width(0),
		Height(0),
		VersionMajor(0),
		VersionMinor(0),
		VSync(false) {}


	GrvtEngine::~GrvtEngine() {}


	void GrvtEngine::Alloc()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VersionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		Window = glfwCreateWindow(Width, Height, Name.C_Str(), nullptr, nullptr);

		_ASSERTE(Window);	// Failed to initialise window.

		glfwMakeContextCurrent(Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			_ASSERTE(false); // Failed to initialise GLAD;
		}

		glfwSetScrollCallback(Window, GrvtEngine::ScrollCallback);
		glfwSetKeyCallback(Window, GrvtEngine::KeyCallback);
		glfwSetMouseButtonCallback(Window, GrvtEngine::MouseCallback);
		glfwSetFramebufferSizeCallback(Window, GrvtEngine::FramebufferCallback);

		//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		GraphicsCtx		= InitialiseRenderContext();
		ResourceMgrPtr	= InitialiseResourceManager();

		ResourceMgrPtr->Alloc(64);
		GraphicsCtx->Execute(this, (GLADloadproc)glfwGetProcAddress);

		// Put main thread to sleep when multi-threading is enabled to enable graphics
		// Sleep(10);
		ResourceMgrPtr->BindToInterface(&GraphicsCtx->GlInterface);

		Module.DllFile		= "Demo.dll";
		Module.DllTempFile	= "DemoReload.dll";

		Module.LoadModuleDll(true);
	}


	void GrvtEngine::Free()
	{
		glfwDestroyWindow(Window);
		glfwTerminate();
	}


	void GrvtEngine::UpdateIO()
	{
		const glm::vec2 MousePosBackup = IO.MousePos;
		IO.MousePos = glm::vec2(-FLT_MAX, -FLT_MAX);

		const bool Focused = glfwGetWindowAttrib(Window, GLFW_FOCUSED);

		if (Focused)
		{
			float64 MouseX, MouseY;
			glfwGetCursorPos(Window, &MouseX, &MouseY);

			IO.MousePos.x = (float32)MouseX;
			IO.MousePos.y = (float32)MouseY;
		}

		// Update the mouse button's state.
		for (int i = 0; i < Grvt_MouseButton_Max; i++)
		{
			IO.MouseButton[i].CurrState = glfwGetMouseButton(Window, i);
			IO.MouseState[i] = Grvt_IoInput_None;

			if (IO.MouseButton[i].OnPress())
			{
				IO.MouseState[i] = Grvt_IoInput_Pressed;
			}

			if (IO.MouseButton[i].OnHold(IO.MinDurationForHold, &IO.MouseHoldDuration[i]))
			{
				IO.MouseState[i] = Grvt_IoInput_Held;
			}

			if (IO.MouseButton[i].OnRelease())
			{
				IO.MouseState[i] = Grvt_IoInput_Released;
			}

			if (IO.MouseState[i] == Grvt_IoInput_Pressed)
			{
				IO.MouseClickPos = IO.MousePos;

				if (LastFrameTime - IO.ClickTime[i] < IO.MouseDoubleClickTime)
				{
					glm::vec2 deltaFromClickPos = IO.MousePos - IO.MouseClickPos;
					float SqrLength = deltaFromClickPos.x * deltaFromClickPos.x + deltaFromClickPos.y * deltaFromClickPos.y;

					if (SqrLength < IO.MouseDoubleClickMaxDist * IO.MouseDoubleClickMaxDist)
					{
						IO.MouseState[i] = Grvt_IoInput_Repeat;
					}
					// This is done to ensure that the third click isn't registered as a double click.
					IO.ClickTime[i] = -FLT_MAX;
				}
				else
				{
					IO.ClickTime[i] = LastFrameTime;
				}
			}
			else if (IO.MouseState[i] == Grvt_IoInput_Held)
			{
				IO.MouseClickPos = IO.MousePos;
			}
		}

		// Update the key's state.
		for (int i = 0; i < 512; i++)
		{
			IO.KeyState[i] = Grvt_IoInput_None;

			if (IO.Keys[i].OnHold(IO.MinDurationForHold, &IO.KeyHoldDuration[i]))
			{
				IO.KeyState[i] = Grvt_IoInput_Held;
			}

			if (IO.Keys[i].OnPress())
			{
				IO.KeyState[i] = Grvt_IoInput_Pressed;
			}

			if (IO.Keys[i].OnRelease())
			{
				IO.KeyState[i] = Grvt_IoInput_Released;
			}

			if (IO.KeyState[i] == Grvt_IoInput_Pressed)
			{
				if (LastFrameTime - IO.KeyPressTime[i] < IO.KeyDoubleTapTime)
				{
					IO.KeyState[i] = Grvt_IoInput_Repeat;
					IO.KeyPressTime[i] = -FLT_MAX;
				}
				else
				{
					IO.KeyPressTime[i] = LastFrameTime;
				}
			}
		}
	}


	void GrvtEngine::NewFrame()
	{
		CurrentFrameTime = (float32)glfwGetTime();
		DeltaTime = min(CurrentFrameTime - LastFrameTime, 0.1f);
		LastFrameTime = CurrentFrameTime;

		g_FrameTime.Update(DeltaTime);
		UpdateIO();

		// NOTE(Afiq):
		// Not feeling too good about this.
		// By default, the renderer should always start rendering from the window's position.
		// If the renderer position requires to be specific, it can be assigned inside of the module.
		glfwGetWindowPos(Window, &PosX, &PosY);
		
		if (GraphicsCtx->GlRenderer->PosX != PosX)
		{
			GraphicsCtx->GlRenderer->PosX = PosX;
		}

		if (GraphicsCtx->GlRenderer->PosY != PosY)
		{
			GraphicsCtx->GlRenderer->PosY = PosY;
		}

		if (GraphicsCtx->GlRenderer->Width != Width)
		{
			GraphicsCtx->GlRenderer->Width = Width;
		}

		if (GraphicsCtx->GlRenderer->Height != Height)
		{
			GraphicsCtx->GlRenderer->Height = Height;
		}

#if _DEBUG

		FILETIME NewDllLastWrite = Module.WatchFileChange();
		if (CompareFileTime(&NewDllLastWrite, &Module.DllLastWriteTime) != 0)
		{
			/**
			* NOTE(Afiq):
			* This should probably move into it's own thread.
			* Still needs to test if 100ms is significant enough to cause the frame to lag.
			*/
			Module.UnloadModuleDll();
			Module.LoadModuleDll();
			Module.DllLastWriteTime = NewDllLastWrite;
			Sleep(10);
		}

#endif
		
		// NOTE(Afiq): This part of the code still causes a crash from time to time.
		for (BaseSystem* System : Systems)
		{
			System->Tick();
		}
	}



	void GrvtEngine::EndFrame()
	{
		IO.MouseWheel = IO.MouseWheelH = 0.0f;
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}


	EngineIO* GrvtEngine::GetIO()
	{
		return &IO;
	}


	bool GrvtEngine::Running()
	{
		if (IO.Alt() && IO.IsKeyPressed(GLFW_KEY_F4))
		{
			glfwSetWindowShouldClose(Window, GLFW_TRUE);
		}

		return !glfwWindowShouldClose(Window);
	}


	void GrvtEngine::InitModule()
	{
		if (Module.StartUp)
		{
			Module.StartUp(this);
		}
	}


	void GrvtEngine::ExecuteModule()
	{
		if (Module.Execute)
		{
			Module.Execute();
		}
	}


	void GrvtEngine::ShutdownModule()
	{
		Module.UnloadModuleDll();
	}


	BaseSystem* GrvtEngine::RegisterSystem(const Gfl::String& Identity, BaseSystem* SrcSystem)
	{
		for (BaseSystem* System : Systems)
		{
			if (System->Name == Identity)
			{
				return System;
			}
		}

		BaseSystem* System = Systems.Insert(SrcSystem);
		System->Name = Identity;
		System->Init();

		return System;
	}


	BaseSystem* GrvtEngine::GetSystem(const Gfl::String& Identity)
	{
		for (BaseSystem* System : Systems)
		{
			if (System->Name == Identity)
			{
				return System;
			}
		}

		return nullptr;
	}


	void GrvtEngine::ShowCursor(bool Show)
	{
		if (!Show)
		{
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}


	void GrvtEngine::SetMousePosition(const glm::vec2& Position)
	{
		glfwSetCursorPos(Window, (float64)Position.x, (float64)Position.y);
		IO.MousePos = Position;
	}


	void GrvtEngine::DeleteSystem(const Gfl::String& Identity)
	{
		for (size_t i = 0; i < Systems.Length(); i++)
		{
			if (Systems[i]->Name != Identity)
			{
				continue;
			}

			Systems[i]->Shutdown();
			delete Systems[i];
			Systems.PopAt(i);
			break;
		}
	}


	void GrvtEngine::ShutdownSystems()
	{
		for (size_t i = 0; i < Systems.Length(); i++)
		{
			Systems[i]->Shutdown();
			delete Systems[i];
			Systems[i] = nullptr;
		}

		Systems.Release();
	}
}