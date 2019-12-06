#include "GrvtPch.h"
#include "Core/Engine.h"
#include "Utility/Utility.h"
#include "Framework/Foundation/Interface.h"

namespace Grvt
{

	GrvtEngine* Engine = nullptr;
	float		LastFrameTime = 0.0f;
	float		CurrentFrameTime = 0.0f;


	GrvtEngine* InitialiseEngine(const Gfl::String& Name, int32 Width, int32 Height, int32 OpenGLVMajor, int32 OpenGLVMinor)
	{
		if (!Engine)
			Engine = new GrvtEngine();

		Engine->Name = Name;
		Engine->Width = Width;
		Engine->Height = Height;
		Engine->VersionMajor = OpenGLVMajor;
		Engine->VersionMinor = OpenGLVMinor;

		Engine->Alloc();

		return Engine;
	}


	GrvtEngine* GetEngine()
	{
		return Engine;
	}


	void TerminateEngine()
	{
		Engine->Free();
		delete Engine;
	}


	void GrvtEngine::ScrollCallback(GLFWwindow* Window, float64 HorizontalOffset, float64 VerticalOffset)
	{
		Engine->IO.MouseWheel = *(float32*)&VerticalOffset;
		Engine->IO.MouseWheelH = *(float32*)&HorizontalOffset;
	}


	void GrvtEngine::MouseCallback(GLFWwindow* Window, int32 Button, int32 Action, int32 Mods)
	{
		if (Action == GLFW_PRESS && Button >= 0 && Button <= GRVT_ARRAY_LENGTH(Engine->IO.MouseButton))
		{
			Engine->IO.MouseButton[Button].CurrState = GLFW_PRESS;
		}
	}


	void GrvtEngine::KeyCallback(GLFWwindow* Window, int32 Key, int32 ScanCode, int32 Action, int32 Mods)
	{
		if (Key >= 0 && Key < 512)
		{
			if (Action == GLFW_PRESS)
			{
				Engine->IO.Keys[Key].CurrState = GLFW_PRESS;
			}

			if (Action == GLFW_RELEASE)
			{
				Engine->IO.Keys[Key].CurrState = GLFW_RELEASE;
			}
		}
	}


	void GrvtEngine::FramebufferCallback(GLFWwindow* Window, int32 Width, int32 Height)
	{
		Engine->Width = Width;
		Engine->Height = Height;
	}


	GrvtEngine::GrvtEngine() :
		IO(),
		Name(),
		Window(nullptr),
		Application(nullptr),
		DeltaTime(0.0f),
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
		//glfwWindowHint(GLFW_SAMPLES, 4);

		Window = glfwCreateWindow(Width, Height, Name.C_Str(), nullptr, nullptr);

		_ASSERTE(Window); // Failed to initialise window.

		glfwMakeContextCurrent(Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			_ASSERTE(false); // Failed to initialise GLAD;
		}

		glfwSetScrollCallback(Window, GrvtEngine::ScrollCallback);
		glfwSetKeyCallback(Window, GrvtEngine::KeyCallback);
		glfwSetMouseButtonCallback(Window, GrvtEngine::MouseCallback);
		glfwSetFramebufferSizeCallback(Window, GrvtEngine::FramebufferCallback);

		Middleware::InitialiseBuildQueue();
		//Engine->Application->Initialise(this);
	}


	void GrvtEngine::Free()
	{
		glfwDestroyWindow(Window);
		glfwTerminate();

		//Engine->Application->Shutdown();
		Middleware::ReleaseBuildQueue();
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

			IO.MousePos.x = *(float32*)&MouseX;
			IO.MousePos.y = *(float32*)&MouseY;
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

			if (IO.Keys[i].OnPress())
			{
				IO.KeyState[i] = Grvt_IoInput_Pressed;
			}

			if (IO.Keys[i].OnHold(IO.MinDurationForHold, &IO.KeyHoldDuration[i]))
			{
				IO.KeyState[i] = Grvt_IoInput_Held;
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
		glfwPollEvents();

		CurrentFrameTime = (float)glfwGetTime();
		DeltaTime = min(CurrentFrameTime - LastFrameTime, 0.1f);
		LastFrameTime = CurrentFrameTime;

		UpdateIO();

		// Listens for resources to be generated or deleted from the GPU.
		Middleware::GetBuildQueue()->Listen();
	}


	void GrvtEngine::EndFrame()
	{
		glfwSwapBuffers(Window);
	}


	EngineIO* GrvtEngine::GetIO()
	{
		return &IO;
	}


	GrvtApplication* GrvtEngine::GetApplication()
	{
		return Application;
	}


	bool GrvtEngine::Running()
	{
		if (IO.Alt() && IO.IsKeyPressed(GLFW_KEY_F4))
		{
			glfwSetWindowShouldClose(Window, GLFW_TRUE);
		}

		return !glfwWindowShouldClose(Window);
	}

}