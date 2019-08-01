#pragma once


/**
* WindowsHandler data strucutre.
*
* Handles windows in Gravity and the main wrapper class around ImGui.
* NOTE(Afiq):
* This class is strictly meant for windows manipulation only. 
*/
class WindowsHandler {
private:

	Array<GravityWindow*> windows;
	Array<GravityWindow*> activeWindows;

public:

	WindowsHandler();

	WindowsHandler(const WindowsHandler &Other);
	WindowsHandler(WindowsHandler &&Other);

	WindowsHandler& operator= (const WindowsHandler &Other);
	WindowsHandler& operator= (WindowsHandler &&Other);

	~WindowsHandler();

	/**
	* Initialises ImGui and sets the default font and theme for the windows.
	*/
	void Init();

	/**
	* Show's a window onto the screen.
	*/
	void Show(GravityWindow *Window);

	/**
	* Adds a new window into the handler. 
	*/
	void AddNewWindow(GravityWindow *NewWindow);
};