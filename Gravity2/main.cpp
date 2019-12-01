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


typedef void (*FuncPtr)(GrvtEngine*);

void LoadDll()
{
	HINSTANCE hDLL;               // Handle to DLL
	FuncPtr Function;    // Function pointer
	
	hDLL = LoadLibrary("Demo");
	if (!hDLL) 
	{
		_ASSERTE(false); // Unable to load dll.
	}

	Function = (FuncPtr)GetProcAddress(hDLL, "ModuleTick");
}


int main() {

	int32 Width = 0;
	int32 Height = 0;

	HMODULE Dll;
	FuncPtr Function;

	Dll = LoadLibrary("Demo");
	if (!Dll)
	{
		_ASSERTE(false); // Unable to load dll.
	}
	Function = (FuncPtr)GetProcAddress(Dll, "ModuleTick");

	GetDesktopResolution(Width, Height);

	GrvtEngine* Engine = InitialiseEngine("Gravity Engine", Width, Height, 4, 3);
	EngineIO* IO = Engine->GetIO();

	LoadDll();

	while (Engine->Running())
	{
		Engine->NewFrame();

		Function(Engine);

		Engine->EndFrame();
	}

	FreeLibrary(Dll);
	TerminateEngine();

	return 0;
}