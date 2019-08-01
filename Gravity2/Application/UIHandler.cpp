#include "stdafx.h""


WindowsHandler::WindowsHandler() : windows(), activeWindows() {}


WindowsHandler::WindowsHandler(const WindowsHandler &Other) { *this = Other; }


WindowsHandler& WindowsHandler::operator= (const WindowsHandler &Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		windows			= Other.windows;
		activeWindows	= Other.activeWindows;
	}

	return *this;
}


WindowsHandler::~WindowsHandler() { Release(); }


void WindowsHandler::Init(GravityApp *Application) {

}


void WindowsHandler::Show(GravityWindow *Window) {
	for (GravityWindow *showned : activeWindows)
		if (showned == Window)
			return;

	activeWindows.Push(Window);
}


void WindowsHandler::Hide(GravityWindow *Window) {
	for (size_t i = 0; i < activeWindows.Length();) {
		if (activeWindows[i] == Window) {
			activeWindows.PopAt(i);
			break;
		} else {
			i++;
		}
	}
}


void WindowsHandler::Tick() {

	WindowMenuBar->Events();
	WindowMenuBar->Draw();

	for (GravityWindow *window : activeWindows) {
		window->Events();
		window->Draw();
	}
}


void WindowsHandler::Release() {
	// Remove all the windows from showing.
	for (size_t i = 0; i < activeWindows.Length(); i++)
		activeWindows[i] = nullptr;

	activeWindows.Release();

	for (size_t i = 0; i < windows.Length(); i++) {
		delete windows[i];
		windows[i] = nullptr;
	}

	windows.Release();
}