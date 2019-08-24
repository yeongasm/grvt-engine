#include "stdafx.h"


AppIO::AppIO() :
	mouseWheel(0.0f),
	mouseWheelH(0.0f),
	mousePos(0.0f, 0.0f),
	keys{},
	mouseButton{} {}


AppIO::AppIO(const AppIO &Other) {
	*this = Other;
}


AppIO& AppIO::operator= (const AppIO &Other) {
	if (this != &Other) {
		mouseWheel	= Other.mouseWheel;
		mouseWheelH = Other.mouseWheelH;
		mousePos	= Other.mousePos;

		for (uint i = 0; i < ArrayLen(keys); i++)
			keys[i] = Other.keys[i];

		for (uint i = 0; i < ArrayLen(mouseButton); i++)
			mouseButton[i] = Other.mouseButton[i];
	}

	return *this;
}


AppIO::~AppIO() {}


bool AppIO::IsKeyPressed(int Key) {
	return keys[Key].OnPress();
}


bool AppIO::IsKeyHeld(int Key) {
	return keys[Key].OnHold();
}


bool AppIO::IsKeyReleased(int Key) {
	return keys[Key].OnRelease();
}


bool AppIO::IsMouseClicked(uint Button) {
	if (Button < 0 || Button > 5)
		return false;

	mouseClickPos = mousePos;

	return mouseButton[Button].OnPress();
}


bool AppIO::IsMouseHeld(uint Button) {
	if (Button < 0 || Button > 5)
		return false;

	if (mouseButton[Button].OnPress())
		mouseClickPos = mousePos;

	return mouseButton[Button].OnHold();
}


bool AppIO::IsMouseReleased(uint Button) {
	return mouseButton[Button].OnRelease();
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