#pragma once


/**
* Forward declare the WindowsHandler class.
*/
class WindowsHandler;
class GravityApp;


/**
* Gravity Engine's Window data structure.
*
* The base class for all other windows.
* TODO(Afiq):
* Some windows would contain child windows that can only exist inside the scope of a window.
* Should create a data structure for it.
*/
class GravityWindow {
private:
	bool					isActive;
	ImGuiWindow				*imguiWindow;
	GravityApp				*application;
	GravityWindow			*parentWindow;
	Array<GravityWindow*>	childWindows;
	String					name;

	void Release();

	void AddChildWindow(GravityWindow *ChildWindow);

	friend class WindowsHandler;
public:

	/**
	* The default constructor should always take a pointer to the main application in order to have access to the API.
	*/
	GravityWindow(const String &Name, GravityApp *Application);

	GravityWindow(const GravityWindow &Other)				= delete;
	GravityWindow(GravityWindow &&Other)					= delete;

	GravityWindow& operator= (const GravityWindow &Other)	= delete;
	GravityWindow& operator= (GravityWindow &&Other)		= delete;

	~GravityWindow();

	/**
	* Draws a window on to the screen.
	*/
	virtual void Draw();

	/**
	* Listen to IO Events that are tied to this window.
	*/
	virtual void Events();
};


#define DeclNewWindow(Name)									\
	class Window##Name##Template : public GravityWindow {	\
	public:													\
		using GravityWindow::GravityWindow;					\
		void Draw();					\
	};														\
	static GravityWindow *Window##Name = nullptr;
	


DeclNewWindow(MenuBar)