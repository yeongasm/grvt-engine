#include "stdafx.h"


AppIO::AppIO() : 
	mouseWheel					(0.0f), 
	mouseWheelH					(0.0f), 
	minDurationForHold			(1.0f), 
	mouseDoubleClickTime		(0.5f), 
	mouseDoubleClickMaxDist		(5.0f),
	keyDoubleTapTime			(0.5f), 
	mouseState					{IO_INPUT_NONE}, 
	keyState					{IO_INPUT_NONE}, 
	clickTime					{0.0f}, 
	keyPressTime				{0.0f}, 
	mouseHoldDuration			{0.0f}, 
	keyHoldDuration				{0.0f}, 
	mouseClickPos				(0.0f, 0.0f), 
	mousePos					(0.0f, 0.0f), 
	keys						(), 
	mouseButton					() {}


AppIO::AppIO(const AppIO &Other) {
	*this = Other;
}


AppIO& AppIO::operator= (const AppIO &Other) {
	if (this != &Other) {
		mouseWheel				= Other.mouseWheel;
		mouseWheelH				= Other.mouseWheelH;
		minDurationForHold		= Other.minDurationForHold;
		mouseDoubleClickTime	= Other.mouseDoubleClickTime;
		mouseDoubleClickMaxDist = Other.mouseDoubleClickMaxDist;
		keyDoubleTapTime		= Other.keyDoubleTapTime;
		
		mousePos				= Other.mousePos;
		mouseClickPos			= Other.mouseClickPos;

		for (uint i = 0; i < MOUSE_BUTTON_MAX; i++) {
			clickTime[i]		= Other.clickTime[i];
			keyPressTime[i]		= Other.keyPressTime[i];
			mouseState[i]		= Other.mouseState[i];
			mouseButton[i]		= Other.mouseButton[i];
			mouseHoldDuration[i] = Other.mouseHoldDuration[i];
		}

		for (uint i = 0; i < GRVT_ARRAY_LENGTH(keys); i++) {
			keys[i] = Other.keys[i];
			keyState[i] = Other.keyState[i];
			keyHoldDuration[i] = Other.keyHoldDuration[i];
		}
	}

	return *this;
}


AppIO::~AppIO() {}


bool AppIO::IsKeyPressed(int Key) {
	return keyState[Key] == IO_INPUT_PRESSED;
}


bool AppIO::IsKeyHeld(int Key) {
	return keyState[Key] == IO_INPUT_HELD;
}


bool AppIO::IsKeyDoubleTapped(int Key) {
	return keyState[Key] == IO_INPUT_REPEAT;
}


bool AppIO::IsKeyReleased(int Key) {
	return keyState[Key] == IO_INPUT_RELEASED;
}


bool AppIO::IsMouseClicked(uint Button) {
	if (Button < 0 || Button > MOUSE_BUTTON_MAX)
		return false;

	return mouseState[Button] == IO_INPUT_PRESSED;
}


bool AppIO::IsMouseDoubleClicked(uint Button) {
	if (Button < 0 || Button > MOUSE_BUTTON_MAX)
		return false;

	return mouseState[Button] == IO_INPUT_REPEAT;
}


bool AppIO::IsMouseHeld(uint Button) {
	if (Button < 0 || Button > MOUSE_BUTTON_MAX)
		return false;

	return mouseState[Button] == IO_INPUT_HELD;
}


bool AppIO::IsMouseReleased(uint Button) {
	if (Button < 0 || Button > MOUSE_BUTTON_MAX)
		return false;

	return mouseState[Button] == IO_INPUT_RELEASED;
}


bool AppIO::MouseDragDelta(uint Button, glm::vec2 *Buf) {
	if (!IsMouseHeld(Button))
		return false;

	if (Buf)
		*Buf = mousePos - mouseClickPos;

	return true;
}


bool AppIO::IsMouseDragging(uint Button) {
	glm::vec2 dragDelta(0.0f);

	if (MouseDragDelta(Button, &dragDelta))
		if (dragDelta.x != 0.0f && dragDelta.y != 0.0f)
			return true;

	return false;
}


bool AppIO::Ctrl() {
	return IsKeyHeld(GLFW_KEY_RIGHT_CONTROL) || IsKeyHeld(GLFW_KEY_LEFT_CONTROL);
}


bool AppIO::Alt() {
	return IsKeyHeld(GLFW_KEY_RIGHT_ALT) || IsKeyHeld(GLFW_KEY_LEFT_ALT);
}


bool AppIO::Shift() {
	return IsKeyHeld(GLFW_KEY_RIGHT_SHIFT) || IsKeyHeld(GLFW_KEY_LEFT_SHIFT);
}