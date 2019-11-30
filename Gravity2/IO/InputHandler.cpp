#include "stdafx.h"


InputHandler::InputHandler() :
	PrevState(GLFW_RELEASE), 
	CurrState(0), 
	PressTime(0.0f) {}


bool InputHandler::OnPress() 
{
	bool pressed = false;

	if (CurrState == GLFW_PRESS && PrevState == GLFW_RELEASE)
		pressed	= true;
	
	PrevState = CurrState;
	
	return pressed;
}


bool InputHandler::OnHold(float MinDuration, float *Buf) 
{
	bool hold = false;

	if (CurrState == GLFW_PRESS) 
	{
		float duration = 0.0f;

		if (!PressTime)
			PressTime = (float)glfwGetTime();

		float now = (float)glfwGetTime();

		duration = min(now - PressTime, 1.0f);

		if (duration >= MinDuration)
			hold = true;

		if (Buf)
			*Buf = duration;
	}

	if (PressTime && CurrState == GLFW_RELEASE) 
	{
		PressTime = 0.0f;
		PrevState = GLFW_PRESS;
	}

	return hold;
}


bool InputHandler::OnRelease() 
{
	bool released = false;

	if (CurrState == GLFW_RELEASE && PrevState == GLFW_PRESS)
		released = true;
	
	PrevState = CurrState;

	return released;
}