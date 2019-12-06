#include "stdafx.h"



int main()
{
	Grvt::GrvtEngine* Engine = Grvt::InitialiseEngine("Hello World!", 1920, 1080, 4, 3);

	//xcopy "$(SolutionDir)bin\$(PlatformTarget)\$(Cofiguration)\Engine\Engine.lib" "$(SolutionDir)\lib" / e / y / q / i

	printf("Hello World!\n");

	return 0;
}