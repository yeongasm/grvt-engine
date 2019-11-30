#pragma once


struct InputHandler {
	int			PrevState;
	int			CurrState;
	float		PressTime;


	InputHandler();


	/**
	* Returns true when key is pressed.
	* Standalone function and cannot be used with other OnEvent() functions.
	*/
	bool OnPress	();


	/**
	* Returns true when the duration of key being pressed exceeds the duration specified.
	* Calls OnPress() internally, hence we do not need to call OnPress() to activate this function.
	* Can be used alongside OnRelease();
	*
	* @param [OPTIONAL] (float)		Default = 0.01f	MinDuration - Minimum duration required for function to return true in seconds.
	* @param [OPTIONAL] (float*)	Buf - Placeholder to get total duration the button is held in seconds.
	*/
	bool OnHold		(float MinDuration = 0.01f, float *Buf = nullptr);


	/**
	* Returns true when key is released.
	* Tailor made to be used alongside OnHold().
	* Can be used as a standalone function to create OnRelease() events.
	*/
	bool OnRelease	();
};