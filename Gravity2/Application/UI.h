#pragma once


/**
* Gravity Engine's Window data structure.
*
* The base class for all other windows.
* TODO(Afiq):
* Create a global windows handler and have each window get pointer to that handle.
* Global windows handler would contain a stack/queue that stores all windows.
* Any windows that are open would be pushed into the open window stack/queue.
*/
class GravityWindow {
private:
	bool				isActive;
	ImGuiWindow			*imguiWindow;
	/**
	* NOTE(Afiq):
	* Not sure if this needs to be const. Will make it const for now.
	*/
	const GravityApp	*application;

	void Release();

public:

	/**
	* The default constructor should always take a pointer to the main application in order to have access to the API.
	*/
	GravityWindow(const GravityApp *Application);

	GravityWindow(const GravityWindow &Other);
	GravityWindow(GravityWindow &&Other);

	GravityWindow& operator= (const GravityWindow &Other);
	GravityWindow& operator= (GravityWindow &&Other);

	~GravityWindow();

	virtual void Draw();
	virtual void Events();
};


#define DeclNewWindow(Name)									\
	class Window##Name##Template : public GravityWindow {	\
	public:													\
		using GravityWindow::GravityWindow;					\
		void Draw();										\
	};									


DeclNewWindow(MenuBar);