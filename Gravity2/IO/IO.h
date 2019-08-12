#pragma once


enum MouseButton : uint {
	MOUSE_BUTTON_RIGHT	= 0x00,
	MOUSE_BUTTON_LEFT	= 0x01,
	MOUSE_BUTTON_MIDDLE = 0x02,
	MOUSE_BUTTON_EXTRA1 = 0x04,
	MOUSE_BUTTON_EXTRA2 = 0x05
};


struct AppIO {
	float			mouseWheel;
	float			mouseWheelH;
	glm::vec2		mouseClickPos;
	glm::vec2		mousePos;
	InputHandler	keys[512];
	InputHandler	mouseButton[5];

	AppIO();

	AppIO(const AppIO &Other);
	AppIO& operator= (const AppIO &Other);

	AppIO(AppIO &&Other)				= delete;
	AppIO& operator= (AppIO &&Other)	= delete;

	~AppIO();

	/**
	* Returns true upon pressing the key supplied into the argument.
	*
	* @param [REQUIRED] (int) Key - GLFW's key macro.
	*/
	bool		IsKeyPressed		(int Key);

	/**
	* Returns true upon holding the key supplied into the argument.
	*
	* @param [REQUIRED] (int) Key - GLFW's key macro.
	*/
	bool		IsKeyHeld			(int Key);
	
	/**
	* Returns true upon releasing the key supplied into the argument.
	*
	* @param [REQUIRED] (int) Key - GLFW's key macro.
	*/
	bool		IsKeyReleased		(int Key);
	
	/**
	* Returns true upon pressing the mouse button supplied into the argument.
	* Updates mouse click position within the application's IO.
	*
	* @param [OPTIONAL] (uint)	Default = 0		Button - Mouse button index.
	*/
	bool		IsMouseClicked		(uint Button = 0);
	
	/**
	* Returns true upon holding the mouse button supplied into the argument.
	* Updates mouse click position within the application's IO.
	*
	* @param [OPTIONAL] (uint)	Default = 0		Button - Mouse button index.
	*/
	bool		IsMouseHeld			(uint Button = 0);
	
	/**
	* Returns true upon releasing the mouse button supplied into the argument.
	* Does not update mouse click position within the application's IO.
	*
	* @param [OPTIONAL] (uint)	Default = 0		Button - Mouse button index.
	*/
	bool		IsMouseReleased		(uint Button = 0);
	
	/**
	* Updates mouse's drag delta into a buffer when the specified mouse button is being held.
	* Returns false when specified mouse button is not being held.
	*
	* @param [OPTIONAL] (uint)		Default = 0			Button - Mouse button index.
	* @param [OPTIONAL] (glm::vec2) Default = nullptr	Buf - Buffer variable to update mouse drag delta.
	*/
	bool		MouseDragDelta		(uint Button = 0, glm::vec2 *Buf = nullptr);

	/**
	* Returns true when the supplied mouse button is held and mouse is dragging.
	*
	* @param [OPTIONAL] (uint)	Default = 0		Button - Mouse button index.
	*/
	bool		IsMouseDragging		(uint Button = 0);

	/**
	* Returns true when either Ctrl keys are held.
	*/
	bool		Ctrl				();

	/**
	* Returns true when either Alt keys are held.
	*/
	bool		Alt					();

	/**
	* Returns true when either Shift keys are held.
	*/
	bool		Shift				();
};