#pragma once
#ifndef GRAVITY_IO
#define GRAVITY_IO

#include "Minimal.h"
#include "IO/InputHandler.h"

namespace Grvt
{

	enum MouseButton : uint32
	{
		Grvt_MouseButton_Left	= 0x00,
		Grvt_MouseButton_Right	= 0x01,
		Grvt_MouseButton_Middle = 0x02,
		Grvt_MouseButton_Extra1 = 0x03,
		Grvt_MouseButton_Extra2 = 0x04,
		Grvt_MouseButton_Max	= 0x05
	};


	enum IOInputState : uint32
	{
		Grvt_IoInput_None		= 0x00,
		Grvt_IoInput_Pressed	= 0x01,
		Grvt_IoInput_Held		= 0x02,
		Grvt_IoInput_Released	= 0x03,
		Grvt_IoInput_Repeat		= 0x04
	};


	class GrvtEngine;


	/**
	* Gravity Engine's IO data structure.
	*
	* Responsible for handling all user interactions with the IO.
	*/
	class EngineIO
	{
	private:

		friend class GrvtEngine;

		InputHandler	MouseButton[5];
		InputHandler	Keys[512];

	public:

		glm::vec2		MouseClickPos;
		glm::vec2		MousePos;

		IOInputState	MouseState[5];
		IOInputState	KeyState[512];

		float			MouseWheel;
		float			MouseWheelH;
		float			MinDurationForHold;
		float			MouseDoubleClickTime;
		float			MouseDoubleClickMaxDist;
		float			ClickTime[5];

		float			KeyDoubleTapTime;
		float			KeyPressTime[5];

		float			MouseHoldDuration[5];
		float			KeyHoldDuration[512];

		EngineIO();
		~EngineIO();

		EngineIO(const EngineIO& Other) = delete;
		EngineIO& operator= (const EngineIO& Other) = delete;

		EngineIO(EngineIO&& Other) = delete;
		EngineIO& operator= (EngineIO&& Other) = delete;


		/**
		* Returns true upon pressing the key supplied into the argument.
		*
		* @param [REQUIRED] (int) Key - GLFW's key macro.
		*/
		ENGINE_API bool	IsKeyPressed(int32 Key);

		/**
		* Returns true upon double tapping the key supplied into the argument.
		*
		* @param [REQUIRED] (int) Key - GLFW's key macro.
		*/
		ENGINE_API bool IsKeyDoubleTapped(int32 Key);

		/**
		* Returns true upon holding the key supplied into the argument.
		*
		* @param [REQUIRED] (int) Key - GLFW's key macro.
		*/
		ENGINE_API bool IsKeyHeld(int32 Key);

		/**
		* Returns true upon releasing the key supplied into the argument.
		*
		* @param [REQUIRED] (int) Key - GLFW's key macro.
		*/
		ENGINE_API bool IsKeyReleased(int32 Key);

		/**
		* Returns true upon pressing the mouse button supplied into the argument.
		* Updates mouse click position within the application's IO.
		*
		* @param [OPTIONAL] (uint32)	Default = 0		Button - Mouse button index.
		*/
		ENGINE_API bool IsMouseClicked(uint32 Button = 0);

		/**
		* Returns true upon pressing the mouse button supplied into the argument twice within the set interval.
		* Updates mouse click position within the application's IO.
		*
		* @param [OPTIONAL] (uint32)	Default = 0		Button - Mouse button index.
		*/
		ENGINE_API bool IsMouseDoubleClicked(uint32 Button = 0);

		/**
		* Returns true upon holding the mouse button supplied into the argument.
		* Updates mouse click position within the application's IO.
		*
		* @param [OPTIONAL] (uint32)	Default = 0		Button - Mouse button index.
		*/
		ENGINE_API bool IsMouseHeld(uint32 Button = 0);

		/**
		* Returns true upon releasing the mouse button supplied into the argument.
		* Does not update mouse click position within the application's IO.
		*
		* @param [OPTIONAL] (uint32)	Default = 0		Button - Mouse button index.
		*/
		ENGINE_API bool IsMouseReleased(uint32 Button = 0);

		/**
		* Updates mouse's drag delta into a buffer when the specified mouse button is being held.
		* Returns false when specified mouse button is not being held.
		*
		* @param [OPTIONAL] (uint32)		Default = 0			Button - Mouse button index.
		* @param [OPTIONAL] (glm::vec2) Default = nullptr	Buf - Buffer variable to update mouse drag delta.
		*/
		ENGINE_API bool MouseDragDelta(uint32 Button = 0, glm::vec2* Buf = nullptr);

		/**
		* Returns true when the supplied mouse button is held and mouse is dragging.
		*
		* @param [OPTIONAL] (uint32)	Default = 0		Button - Mouse button index.
		*/
		ENGINE_API bool IsMouseDragging(uint32 Button = 0);

		/**
		* Returns true when either Ctrl keys are held.
		*/
		ENGINE_API bool Ctrl();

		/**
		* Returns true when either Alt keys are held.
		*/
		ENGINE_API bool Alt();

		/**
		* Returns true when either Shift keys are held.
		*/
		ENGINE_API bool Shift();
	};

}

#endif // !GRAVITY_IO