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
protected:
	bool					isActive;
	ImGuiWindow				*imguiWindow;
	GravityApp				*application;
	String					name;

	void Init(const String &Name, GravityApp *&Application);
	void Release();

	friend class WindowsHandler;
public:

	/**
	* The default constructor should always take a pointer to the main application in order to have access to the API.
	*/
	GravityWindow();

	GravityWindow(const GravityWindow &Other);
	GravityWindow& operator= (const GravityWindow &Other);

	GravityWindow(GravityWindow &&Other)					= delete;
	GravityWindow& operator= (GravityWindow &&Other)		= delete;

	~GravityWindow();

	/**
	* Draws a window on to the screen.
	*/
	virtual void Draw();

	/**
	* Listen to IO Events that are tied to this window.
	* The idea of this function was to enable keyboard shortcuts that are tied to this window.
	* Hence we'll need to make a set of keyboard shortcuts on the WindowsHandler for opening certain windows.
	*/
	virtual void Events();
};


/**
* Declare this only in UI.h and nowhere else.
*/
#define DeclNewWindow(Name)												\
	class Window##Name##Template : public GravityWindow {				\
	public:																\
		using GravityWindow::GravityWindow;								\
		void Draw();													\
		void Events();													\
	};


DeclNewWindow(MenuBar)
DeclNewWindow(NewProject)
//DeclNewWindow(AppStats)


struct WindowDebugger {
	static ImGuiWindow *window;

	static void RenderDebugger();
};