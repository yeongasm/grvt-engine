#pragma once


/**
* Forward declare the WindowsHandler class.
*/
class WindowsHandler;
class GravityApp;


/**
* WindowIOFlags enum.
*/
enum WindowIOFlags : uint {
	WINDOW_ON_SUBMIT = 0x00,
	WINDOW_ON_CANCEL = 0x01,
	WINDOW_EVENT_MAX = 0x02
};


/**
* WindowIOEvent data structure.
*
* A wrapper around on submit and on cancel events. Any new events can be added into WindowIOFlags enum.
* NOTE(Afiq):
* Not really happy with this implementation. Need to figure out a different way.
*/
struct WindowIOEvent {
	bool enabled;
	bool onEvent;

	WindowIOEvent();

	operator bool();
};


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
	WindowIOEvent			keyEvent[WINDOW_EVENT_MAX];
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
DeclNewWindow(ObjectCreator)
//DeclNewWindow(AppStats)

/**
* ImGui support functions.
*/

/**
* Generic tooltip for different ImGui components.
* To use it, simply place the function right after the desired component to be used with.
*/
void ImGuiCustomToolTip(const char *Description);


struct WindowDebugger {
	static ImGuiWindow *window;

	static void RenderDebugger();
};