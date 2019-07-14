#include "stdafx.h"


GravityApp *grApp = nullptr;


void GravityApp::GravtiyScrollCallback(GLFWwindow *Window, double OffsetX, double OffsetY) {
	ImGui_ImplGlfw_ScrollCallback(Window, OffsetX, OffsetY);

	grApp->io.mouseWheelH	= (float)OffsetY;
	grApp->io.mouseWheel	= (float)OffsetX;
}


void GravityApp::GravityMouseButtonCallback(GLFWwindow *Window, int Button, int Action, int Mods) {
	if (Action == GLFW_PRESS && Button >= 0 && Button <= ArrayLen(grApp->io.mouseButton))
		grApp->io.mouseButton[Button].currState = true;
	
	ImGui_ImplGlfw_MouseButtonCallback(Window, Button, Action, Mods);
}


void GravityApp::GravityKeyCallback(GLFWwindow *Window, int Key, int ScanCode, int Action, int Mods) {
	ImGui_ImplGlfw_KeyCallback(Window, Key, ScanCode, Action, Mods);
	
	if (Key >= 0 && Key < 512) {
		if (Action == GLFW_PRESS)
			grApp->io.keys[Key].currState = GLFW_PRESS;
		if (Action == GLFW_RELEASE)
			grApp->io.keys[Key].currState = GLFW_RELEASE;
	}
}


void GravityApp::GravityFramebufferCallback(GLFWwindow *Window, int Width, int Height) {
	grApp->width	= Width;
	grApp->height	= Height;

	glViewport(0, 0, Width, Height);
}


GravityApp::GravityApp() :
	window(nullptr),
	manager(nullptr),
	renderer(nullptr),
	vsyncEnabled(0),
	width(0), 
	height(0), 
	name{}, 
	glVersionMajor{}, 
	glVersionMinor{},
	deltaTime{},
	io{},
	fstats{} {}


GravityApp::~GravityApp() {}


void GravityApp::Init() {
	ConstructLogString

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(width, height, ~name, nullptr, nullptr);

	// Throw an assertion if we fail to create a window.
	// Should improvise on this.
	_ASSERTE(window);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		_ASSERTE(false);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	String glslVersion("#version %d%d0 core", glVersionMajor, glVersionMinor);

	ImGui_ImplGlfw_InitForOpenGL(window, false);
	ImGui_ImplOpenGL3_Init(~glslVersion);

	glfwSetScrollCallback(window, GravityApp::GravtiyScrollCallback);
	glfwSetKeyCallback(window, GravityApp::GravityKeyCallback);
	glfwSetMouseButtonCallback(window, GravityApp::GravityMouseButtonCallback);
	glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
	glfwSetFramebufferSizeCallback(window, GravityApp::GravityFramebufferCallback);

	PrintIntoLog(LOG_INFO, LOG_APP, "Vendor: %s",			glGetString(GL_VENDOR))
	PrintIntoLog(LOG_INFO, LOG_APP, "Graphics Card: %s",	glGetString(GL_RENDERER))
	PrintIntoLog(LOG_INFO, LOG_APP, "Version: %s",			glGetString(GL_VERSION))

	manager = new ResourceManager();
	renderer = new Renderer();
}


void GravityApp::Free() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	delete manager;
	delete renderer;
}


void GravityApp::EnableVSync(bool Enable) {
	vsyncEnabled = (Enable) ? 1 : 0;

	glfwSwapInterval(vsyncEnabled);
}


void GravityApp::NewFrame() {
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void GravityApp::SwapBuffer() {

	/**
	* NOTE(Afiq):
	* For now we include ImGui's render operations inside of Gravity's swap buffer operation.
	* It only make sense to draw UI right before we swap buffers.
	*/
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui::EndFrame();
	glfwSwapBuffers(window);
}


void GravityApp::SetInputMode(int Mode, int Value) {
	glfwSetInputMode(window, Mode, Value);
}


bool GravityApp::VSyncStatus() const {
	return vsyncEnabled;
}


float GravityApp::Tick() {
	static float lastFrameTime = 0.0f;
	// 1. Update application's delta time.
	float currentFrame	= (float)glfwGetTime();
	deltaTime			= min(currentFrame - lastFrameTime, 0.1f);
	lastFrameTime		= currentFrame;

	// 2. Update frame statistics.
	fstats.Tick(deltaTime);

	// NOTE(Afiq): 
	// I feel like IO related operations should be placed inside of the NewFrame() function rather than the Tick() function.
	// Further thought should be given into the idea.
	// 2. Update mouse position from ImGui's IO handler.
	ImGuiIO &guiIO = ImGui::GetIO();
	io.mousePos = glm::vec2(guiIO.MousePos.x, guiIO.MousePos.y);

	for (int i = 0; i < ArrayLen(this->io.mouseButton); i++)
		io.mouseButton[i].currState = glfwGetMouseButton(window, i);

	return deltaTime;
}


float GravityApp::FramesPerSecond() const {
	return fstats.Framerate();
}


float GravityApp::TimePerFrame() const {
	return fstats.TimePerFrame();
}


Renderer* GravityApp::GetRenderer() const {
	return renderer;
}


ResourceManager* GravityApp::GetResourceHandler() const {
	return manager;
}


bool GravityApp::CloseAplication() {
	// TODO(Afiq):
	// We should probaly remove this in the future as we might wanna use the escape key to exit operations instead.
	if (io.keys[GLFW_KEY_ESCAPE].OnPress() ||
		(io.keys[GLFW_KEY_LEFT_ALT].OnHold() && io.keys[GLFW_KEY_F4].OnPress()))
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	return glfwWindowShouldClose(window);
}


GravityApp* NewApplication(const char *AppName, int Width, int Height, int OpenGLVMajor, int OpenGLVMinor) {
	if (!grApp)
		grApp = new GravityApp();

	grApp->name				= AppName;
	grApp->width			= Width;
	grApp->height			= Height;
	grApp->glVersionMajor	= OpenGLVMajor;
	grApp->glVersionMinor	= OpenGLVMinor;

	return grApp;
}


GravityApp* GetApplication() {
	if (!grApp)
		return nullptr;

	return grApp;
}


void TerminateApplication(GravityApp* App) {
	App->Free();

	delete App;
}