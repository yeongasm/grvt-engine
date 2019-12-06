#include "GrvtPch.h"


namespace Grvt
{

	EngineIO::EngineIO() :
		MouseWheel(0.0f),
		MouseWheelH(0.0f),
		MinDurationForHold(1.0f),
		MouseDoubleClickTime(0.5f),
		MouseDoubleClickMaxDist(5.0f),
		KeyDoubleTapTime(0.5f),
		MouseState{Grvt_IoInput_None},
		KeyState{Grvt_IoInput_None},
		ClickTime{0.0f},
		KeyPressTime{0.0f},
		MouseHoldDuration{0.0f},
		KeyHoldDuration{0.0f},
		MouseClickPos(0.0f, 0.0f),
		MousePos(0.0f, 0.0f),
		Keys(),
		MouseButton() {}


	//EngineIO::EngineIO(const EngineIO &Other) {
	//	*this = Other;
	//}
	//
	//
	//EngineIO& EngineIO::operator= (const EngineIO &Other) {
	//	if (this != &Other) {
	//		mouseWheel				= Other.mouseWheel;
	//		mouseWheelH				= Other.mouseWheelH;
	//		minDurationForHold		= Other.minDurationForHold;
	//		mouseDoubleClickTime	= Other.mouseDoubleClickTime;
	//		mouseDoubleClickMaxDist = Other.mouseDoubleClickMaxDist;
	//		keyDoubleTapTime		= Other.keyDoubleTapTime;
	//		
	//		mousePos				= Other.mousePos;
	//		mouseClickPos			= Other.mouseClickPos;
	//
	//		for (uint32 i = 0; i < MOUSE_BUTTON_MAX; i++) {
	//			clickTime[i]		= Other.clickTime[i];
	//			keyPressTime[i]		= Other.keyPressTime[i];
	//			mouseState[i]		= Other.mouseState[i];
	//			mouseButton[i]		= Other.mouseButton[i];
	//			mouseHoldDuration[i] = Other.mouseHoldDuration[i];
	//		}
	//
	//		for (uint32 i = 0; i < GRVT_ARRAY_LENGTH(keys); i++) {
	//			keys[i] = Other.keys[i];
	//			keyState[i] = Other.keyState[i];
	//			keyHoldDuration[i] = Other.keyHoldDuration[i];
	//		}
	//	}
	//
	//	return *this;
	//}


	EngineIO::~EngineIO() {}


	bool EngineIO::IsKeyPressed(int32 Key)
	{
		return KeyState[Key] == Grvt_IoInput_Pressed;
	}


	bool EngineIO::IsKeyHeld(int32 Key)
	{
		return KeyState[Key] == Grvt_IoInput_Held;
	}


	bool EngineIO::IsKeyDoubleTapped(int32 Key)
	{
		return KeyState[Key] == Grvt_IoInput_Repeat;
	}


	bool EngineIO::IsKeyReleased(int32 Key)
	{
		return KeyState[Key] == Grvt_IoInput_Released;
	}


	bool EngineIO::IsMouseClicked(uint32 Button)
	{
		if (Button < 0 || Button > Grvt_MouseButton_Max)
		{
			return false;
		}

		return MouseState[Button] == Grvt_IoInput_Pressed;
	}


	bool EngineIO::IsMouseDoubleClicked(uint32 Button)
	{
		if (Button < 0 || Button > Grvt_MouseButton_Max)
		{
			return false;
		}

		return MouseState[Button] == Grvt_IoInput_Repeat;
	}


	bool EngineIO::IsMouseHeld(uint32 Button)
	{
		if (Button < 0 || Button > Grvt_MouseButton_Max)
		{
			return false;
		}

		return MouseState[Button] == Grvt_IoInput_Held;
	}


	bool EngineIO::IsMouseReleased(uint32 Button)
	{
		if (Button < 0 || Button > Grvt_MouseButton_Max)
		{
			return false;
		}

		return MouseState[Button] == Grvt_IoInput_Released;
	}


	bool EngineIO::MouseDragDelta(uint32 Button, glm::vec2* Buf)
	{
		if (!IsMouseHeld(Button))
		{
			return false;
		}

		if (Buf)
		{
			*Buf = MousePos - MouseClickPos;
		}

		return true;
	}


	bool EngineIO::IsMouseDragging(uint32 Button)
	{
		glm::vec2 dragDelta(0.0f);

		if (MouseDragDelta(Button, &dragDelta))
		{
			if (dragDelta.x != 0.0f && dragDelta.y != 0.0f)
			{
				return true;
			}
		}

		return false;
	}


	bool EngineIO::Ctrl()
	{
		return IsKeyHeld(GLFW_KEY_RIGHT_CONTROL) || IsKeyHeld(GLFW_KEY_LEFT_CONTROL);
	}


	bool EngineIO::Alt()
	{
		return IsKeyHeld(GLFW_KEY_RIGHT_ALT) || IsKeyHeld(GLFW_KEY_LEFT_ALT);
	}


	bool EngineIO::Shift()
	{
		return IsKeyHeld(GLFW_KEY_RIGHT_SHIFT) || IsKeyHeld(GLFW_KEY_LEFT_SHIFT);
	}

}