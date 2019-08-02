#include "stdafx.h"


#define GetIOAndResourceHandle											\
	AppIO			*io			= &application->io;						\
	ResourceManager *resource	= application->GetResourceHandler();


GravityWindow::GravityWindow(const String &Name, GravityApp *Application) :
	isActive(false), imguiWindow(nullptr), application(Application), name(Name) {}


GravityWindow::GravityWindow(const GravityWindow &Other) { *this = Other; }


//GravityWindow::GravityWindow(GravityWindow &&Other) { *this = std::move(Other); }


GravityWindow& GravityWindow::operator= (const GravityWindow &Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		isActive	= Other.isActive;
		imguiWindow = Other.imguiWindow;
		application = Other.application;
		name		= Other.name;
	}

	return *this;
}


//GravityWindow& GravityWindow::operator= (GravityWindow &&Other) {
//	_ASSERTE(this != &Other);
//
//	if (this != &Other) {
//		Release();
//
//		isActive	= Other.isActive;
//		imguiWindow = Other.imguiWindow;
//		application = Other.application;
//
//		Other.Release();
//	}
//
//	return *this;
//}


GravityWindow::~GravityWindow() { Release(); }


void GravityWindow::Release() {
	isActive		= false;
	imguiWindow		= nullptr;
	application		= nullptr;
}


void GravityWindow::Draw() {}


void GravityWindow::Events() {}


void WindowMenuBarTemplate::Draw() {
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("New Project", "CTRL+N", false))
			printf("Adding new project ...\n");

		if (ImGui::MenuItem("Open File", "CTRL+O", false))
			printf("Opening file ...\n");

		if (ImGui::MenuItem("Save", "CTRL+S", false))
			printf("Saving file ...\n");
		
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}


void WindowMenuBarTemplate::Events() {}


void WindowAppStatsTemplate::Draw() {
	WindowsHandler &ui = application->ui;
	bool open = true;
	ImGui::Begin("Render Info", &open);
	if (!open)
		ui.Hide(this);

	isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
	ImGui::Text("Delta time (seconds): %f", application->deltaTime);
	ImGui::Text("FPS: %.1f", application->FramesPerSecond());
	ImGui::Text("Single frame: %.4f ms/frame", application->TimePerFrame());
	ImGui::Text("VSync: %s", (application->VSyncStatus()) ? "Enabled" : "Disabled");
	ImGui::End();
}


void WindowAppStatsTemplate::Events() {
	AppIO &io = application->io;

	if (io.IsKeyPressed(GLFW_KEY_GRAVE_ACCENT))
		application->ui.Show(this);

	if (isActive && io.IsKeyPressed(GLFW_KEY_ESCAPE))
		application->ui.Hide(this);
}