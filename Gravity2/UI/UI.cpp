#include "stdafx.h"


#define GetIOAndResourceHandle											\
	AppIO			*io			= &application->io;						\
	ResourceManager *resource	= application->GetResourceHandler();


WindowIOEvent::WindowIOEvent() : enabled(true), onEvent(false) {}


WindowIOEvent::operator bool() {
	if (!enabled)
		return false;

	return onEvent;
}


GravityWindow::GravityWindow() :
	isActive(false), imguiWindow(nullptr), application(nullptr), keyEvent{}, name() {}


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


void GravityWindow::Init(const String &Name, GravityApp *&Application) {
	name = Name;
	application = Application;
}


void GravityWindow::Release() {
	isActive		= false;
	imguiWindow		= nullptr;
	application		= nullptr;
}


void GravityWindow::Draw() {}


void GravityWindow::Events() {}


ImGuiWindow *WindowDebugger::window = nullptr;


void WindowDebugger::RenderDebugger() {
	static float colour[4];
	ImGui::Begin("ImGui Debugger");
	if (window) {
		ImGui::Text("Name: %s", window->Name);
		ImGui::Text("ID: %d", window->ID);
		ImGui::Text("Size X: %.2f", window->Size.x);
		ImGui::Text("Size Y: %.2f", window->Size.y);
	}
	ImGui::ColorPicker4("##", colour, ImGuiColorEditFlags_Float, colour);
	ImGui::End();
}