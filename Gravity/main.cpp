#include "stdafx.h"


int main()
{
	int32 Width	 = 0;
	int32 Height = 0;
	RECT Size;
	const HWND Desktop = GetDesktopWindow();
	
	GetWindowRect(Desktop, &Size);

	Width	= Size.right;
	Height	= Size.bottom;

	Grvt::InitialiseEngine("Gravity Engine", Width, Height, 4, 3);

	Grvt::ExecuteEngine();

	Grvt::TerminateEngine();

	return 0;
}