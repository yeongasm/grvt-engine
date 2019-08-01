#include "stdafx.h"


#define GetIOAndResourceHandle											\
	AppIO			*io			= &application->io;						\
	ResourceManager *resource	= application->GetResourceHandler();


GravityWindow::GravityWindow(const String &Name, GravityApp *Application) :
	isActive(false), imguiWindow(nullptr), application(Application),
	parentWindow(nullptr), childWindows(), name(Name) {}


//GravityWindow::GravityWindow(const GravityWindow &Other) { *this = Other; }


//GravityWindow::GravityWindow(GravityWindow &&Other) { *this = std::move(Other); }


//GravityWindow& GravityWindow::operator= (const GravityWindow &Other) {
//	_ASSERTE(this != &Other);
//
//	if (this != &Other) {
//		isActive	= Other.isActive;
//		imguiWindow = Other.imguiWindow;
//		application = Other.application;
//	}
//
//	return *this;
//}


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
	parentWindow	= nullptr;

	childWindows.Release();
}


void GravityWindow::AddChildWindow(GravityWindow *Window) {
	childWindows.Push(Window);
	Window->parentWindow = this;
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