#include "stdafx.h"


GrvtEngine* Engine = nullptr;
float		LastFrameTime = 0.0f;
float		CurrentFrameTime = 0.0f;


GrvtEngine* InitialiseEngine(const String& Name, int32 Width, int32 Height, int32 OpenGLVMajor, int32 OpenGLVMinor)
{
	if (!Engine)
		Engine = new GrvtEngine();

	Engine->Name	= Name;
	Engine->Width	= Width;
	Engine->Height	= Height;
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
	Engine->IO.MouseWheel	= *(float32*)&VerticalOffset;
	Engine->IO.MouseWheelH	= *(float32*)&HorizontalOffset;
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
	Engine->Width	= Width;
	Engine->Height	= Height;
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
}


void GrvtEngine::Free()
{
	glfwDestroyWindow(Window);
	glfwTerminate();

	Middleware::ReleaseBuildQueue();
}


void GrvtEngine::NewFrame()
{
	glfwPollEvents();
	CurrentFrameTime	= (float)glfwGetTime();
	DeltaTime			= min(CurrentFrameTime - LastFrameTime, 0.1f);
	LastFrameTime		= CurrentFrameTime;

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


bool GrvtEngine::Running()
{
	if (IO.Alt() && IO.IsKeyPressed(GLFW_KEY_F4))
	{
		glfwSetWindowShouldClose(Window, GLFW_TRUE);
	}

	return !glfwWindowShouldClose(Window);
}