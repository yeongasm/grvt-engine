#pragma once


/**
* TODO(Afiq):
* The main application class should be cleaned up.
* Right now it is a mess.
*/
class GravityApp {
private:
	ResourceManager		*manager;
	GLFWwindow			*window;
	Renderer			*renderer;
	FrameStatistics		fstats;
	
	static void GravtiyScrollCallback			(GLFWwindow *Window, double OffsetX, double OffsetY);
	static void GravityMouseButtonCallback		(GLFWwindow *Window, int Button, int Action, int Mods);
	static void GravityKeyCallback				(GLFWwindow *Window, int Key, int ScanCode, int Action, int Mods);
	static void GravityFramebufferCallback		(GLFWwindow *Window, int Width, int Height);

public:
	int					vsyncEnabled;
	int					glVersionMajor;
	int					glVersionMinor;
	int					width;
	int					height;
	float				deltaTime;
	AppIO				io;
	WindowsHandler		ui;
	String				name;

	GravityApp();
	GravityApp(const GravityApp &Other)				= delete;
	GravityApp(GravityApp &&Other)					= delete;

	GravityApp& operator= (const GravityApp &Other) = delete;
	GravityApp& operator= (GravityApp &&Other)		= delete;

	~GravityApp();


	void				Init				();
	void				Free				();
	void				EnableVSync			(bool Enable);
	void				NewFrame			();
	void				SwapBuffer			();
	void				SetInputMode		(int Mode, int Value);
	
	bool				VSyncStatus			() const;

	float				Tick				();
	
	float				FramesPerSecond		()	const;
	float				TimePerFrame		()	const;

	Renderer*			GetRenderer			()	const;
	ResourceManager*	GetResourceHandler	()	const;

	/**
	* TODO(Afiq): 
	* This function needs to be expanded upon. For now for simplicity sake, we map the ESC key to close the application.
	* If the plans to have an editor to accompany the editor does happen, then we need to expand for it to do several things such as:
	* 1. Close application if everything is saved.
	* 2. Prompt a pop up box if there are no saved objects.
	*/
	bool				CloseAplication		();
};



GravityApp* NewApplication(const char *AppName, int Width, int Height, int OpenGLVMajor, int OpenGLVMinor);
GravityApp* GetApplication();
void		TerminateApplication(GravityApp *&App);