#pragma once


class GrvtApplication;


class ENGINE_API GrvtEngine
{
private:

	friend class GrvtApplication;

	EngineIO			IO;
	String				Name;
	GLFWwindow*			Window;
	GrvtApplication*	Application;
	float32				DeltaTime;
	int32				Width;
	int32				Height;
	int32				VersionMajor;
	int32				VersionMinor;
	bool				VSync;

public:

	friend ENGINE_API GrvtEngine*	InitialiseEngine(const String& Name, int32 Width, int32 Height, int32 OpenGLVMajor, int32 OpenGLVMinor);
	friend ENGINE_API void			TerminateEngine();

private:

	static void ScrollCallback		(GLFWwindow* Window, float64 HorizontalOffset, float64 VerticalOffset);
	static void MouseCallback		(GLFWwindow* Window, int32 Button, int32 Action, int32 Mods);
	static void KeyCallback			(GLFWwindow* Window, int32 Key, int32 ScanCode, int32 Action, int32 Mods);
	static void	FramebufferCallback	(GLFWwindow* Window, int32 Width, int32 Height);

	void Alloc();
	void Free();
	void UpdateIO();

public:

	GrvtEngine();
	~GrvtEngine();

	GrvtEngine(const GrvtEngine& Other) = delete;
	GrvtEngine& operator= (const GrvtEngine& Other) = delete;

	GrvtEngine(GrvtEngine&& Other) = delete;
	GrvtEngine& operator= (GrvtEngine&& Other) = delete;

	void				NewFrame();
	void				EndFrame();
	EngineIO*			GetIO();
	GrvtApplication*	GetApplication();
	bool				Running();
};

ENGINE_API GrvtEngine*	InitialiseEngine(const String& Name, int32 Width, int32 Height, int32 OpenGLVMajor, int32 OpenGLVMinor);
ENGINE_API GrvtEngine*	GetEngine();
ENGINE_API void			TerminateEngine();