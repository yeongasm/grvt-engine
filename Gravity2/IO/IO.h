#pragma once


enum MouseButton : uint32 {
	MOUSE_BUTTON_LEFT	= 0x00,
	MOUSE_BUTTON_RIGHT	= 0x01,
	MOUSE_BUTTON_MIDDLE = 0x02,
	MOUSE_BUTTON_EXTRA1 = 0x03,
	MOUSE_BUTTON_EXTRA2 = 0x04,
	MOUSE_BUTTON_MAX	= 0x05
};


enum IOInputState : uint32 {
	IO_INPUT_NONE		= 0x00,
	IO_INPUT_PRESSED	= 0x01,
	IO_INPUT_HELD		= 0x02,
	IO_INPUT_RELEASED	= 0x03,
	IO_INPUT_REPEAT		= 0x04
};


/**
* Gravity Engine's IO data structure.
*
* Responsible for handling all user interactions with the IO.
* NEVER call the variables for detecting IO state changes. Only use the functions provided.
*
* Several variables can be altered to slightly alter the IO's behavior.
*
* @param (float) Default = 1.0f minDurationForHold		- Minimum time in seconds required to trigger an OnHold() event.
* @param (float) Default = 0.5f mouseDoubleClickTime		- Minimum time in seconds required for the second click to be registered as a double click.
* @param (float) Default = 5.0f mouseDoubleClickMaxDist	- Minimum distance in pixels from the initial click for the second click to be registered as a double click.
* @param (float) Default = 0.5f keyDoubleTapTime			- Minimum time in seconds required for the second key press to be registerd as a double tap.
*/
struct AppIO {

	float			mouseWheel;
	float			mouseWheelH;
	float			minDurationForHold;
	float			mouseDoubleClickTime;
	float			mouseDoubleClickMaxDist;
	float			keyDoubleTapTime;

	glm::vec2		mouseClickPos;
	glm::vec2		mousePos;

	float			clickTime[5];
	float			keyPressTime[5];
	float			mouseHoldDuration[5];

	IOInputState	mouseState[5];
	InputHandler	mouseButton[5];
	
	float			keyHoldDuration[512];
	IOInputState	keyState[512];
	InputHandler	keys[512];


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
	bool		IsKeyPressed		(int32 Key);

	/**
	* Returns true upon double tapping the key supplied into the argument.
	*
	* @param [REQUIRED] (int) Key - GLFW's key macro.
	*/
	bool		IsKeyDoubleTapped	(int32 Key);

	/**
	* Returns true upon holding the key supplied into the argument.
	*
	* @param [REQUIRED] (int) Key - GLFW's key macro.
	*/
	bool		IsKeyHeld			(int32 Key);
	
	/**
	* Returns true upon releasing the key supplied into the argument.
	*
	* @param [REQUIRED] (int) Key - GLFW's key macro.
	*/
	bool		IsKeyReleased		(int32 Key);
	
	/**
	* Returns true upon pressing the mouse button supplied into the argument.
	* Updates mouse click position within the application's IO.
	*
	* @param [OPTIONAL] (uint32)	Default = 0		Button - Mouse button index.
	*/
	bool		IsMouseClicked		(uint32 Button = 0);

	/**
	* Returns true upon pressing the mouse button supplied into the argument twice within the set interval.
	* Updates mouse click position within the application's IO.
	*
	* @param [OPTIONAL] (uint32)	Default = 0		Button - Mouse button index.
	*/
	bool		IsMouseDoubleClicked(uint32 Button = 0);
	
	/**
	* Returns true upon holding the mouse button supplied into the argument.
	* Updates mouse click position within the application's IO.
	*
	* @param [OPTIONAL] (uint32)	Default = 0		Button - Mouse button index.
	*/
	bool		IsMouseHeld			(uint32 Button = 0);
	
	/**
	* Returns true upon releasing the mouse button supplied into the argument.
	* Does not update mouse click position within the application's IO.
	*
	* @param [OPTIONAL] (uint32)	Default = 0		Button - Mouse button index.
	*/
	bool		IsMouseReleased		(uint32 Button = 0);
	
	/**
	* Updates mouse's drag delta into a buffer when the specified mouse button is being held.
	* Returns false when specified mouse button is not being held.
	*
	* @param [OPTIONAL] (uint32)		Default = 0			Button - Mouse button index.
	* @param [OPTIONAL] (glm::vec2) Default = nullptr	Buf - Buffer variable to update mouse drag delta.
	*/
	bool		MouseDragDelta		(uint32 Button = 0, glm::vec2 *Buf = nullptr);

	/**
	* Returns true when the supplied mouse button is held and mouse is dragging.
	*
	* @param [OPTIONAL] (uint32)	Default = 0		Button - Mouse button index.
	*/
	bool		IsMouseDragging		(uint32 Button = 0);

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