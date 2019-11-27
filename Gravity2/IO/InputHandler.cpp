#include "stdafx.h"


InputHandler::InputHandler() :
	prevState(GLFW_RELEASE), 
	currState(0), 
	pressTime(0.0f) {}


bool InputHandler::OnPress() {
	bool pressed = false;

	if (currState == GLFW_PRESS && prevState == GLFW_RELEASE)
		pressed	= true;
	
	prevState = currState;
	
	return pressed;
}


bool InputHandler::OnHold(float MinDuration, float *Buf) {
	bool hold = false;

	if (currState == GLFW_PRESS) {
		float duration = 0.0f;

		if (!pressTime)
			pressTime = (float)glfwGetTime();

		float now = (float)glfwGetTime();

		duration = min(now - pressTime, 1.0f);

		if (duration >= MinDuration)
			hold = true;

		if (Buf)
			*Buf = duration;
	}

	if (pressTime && currState == GLFW_RELEASE) {
		pressTime = 0.0f;
		prevState = GLFW_PRESS;
	}

	return hold;
}


bool InputHandler::OnRelease() {
	bool released = false;

	if (currState == GLFW_RELEASE && prevState == GLFW_PRESS)
		released = true;
	
	prevState = currState;

	return released;
}