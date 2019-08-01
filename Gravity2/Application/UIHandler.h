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

	GravityApp				*application;
	Array<GravityWindow*>	windows;
	Array<GravityWindow*>	activeWindows;

public:

	WindowsHandler();

	WindowsHandler(const WindowsHandler &Other);
	WindowsHandler& operator= (const WindowsHandler &Other);

	WindowsHandler(WindowsHandler &&Other)						= delete;
	WindowsHandler& operator= (WindowsHandler &&Other)			= delete;

	~WindowsHandler();

	/**
	* Initialises ImGui and sets the default font and theme for the windows.
	*/
	void Init(GravityApp *Application);

	/**
	* Push a window into the active windows buffer.
	* Objects in the active windows buffer are the ones being drawned onto the screen.
	*/
	void Show(GravityWindow *Window);

	/**
	* Pops a window from the active windows buffer.
	* Objects not in the active windows buffer are not being drawned onto the screen.
	*/
	void Hide(GravityWindow *Window);

	/**
	* Draw all the windows that are currently being showned.
	* TODO(Afiq):
	* Set up main menu bar and also dockspace?
	* Maybe not do a dockspace for now as it is not that important.
	*/
	void Tick();

	/**
	* Free all the memory owned by the handler.
	*/
	void Release();
};