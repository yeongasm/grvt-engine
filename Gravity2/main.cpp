#include "stdafx.h"


/**
* But if you don't love your work, you can never be a professional - Chef Tatsuya Sekiguchi
*/

/**
* Default values for default materials.
*
* Albedo - RGBA(255, 255, 255, 255)
* Normal - RGBA(127, 127, 255, 255)
* Metallic - RGBA(0, 0, 0, 0)
* Roughness - RGBA(57, 57, 57, 255)
*/


void GetDesktopResolution(int32& Width, int32& Height)
{
	RECT Size;
	const HWND Desktop = GetDesktopWindow();
	GetWindowRect(Desktop, &Size);
	Width = Size.right;
	Height = Size.bottom;
}


int main() {

	int32 Width = 0;
	int32 Height = 0;

	GetDesktopResolution(Width, Height);

	GrvtEngine* Engine = InitialiseEngine("Gravity Engine", Width, Height, 4, 3);

	while (Engine->Running())
	{
		Engine->NewFrame();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		Engine->EndFrame();
	}

	TerminateEngine();

	return 0;
}