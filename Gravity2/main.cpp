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

struct Timer {
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		printf("Time: %.6f ms\n\n", Now());
	}


	float Now()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		return duration.count() * 1000.0f;
	}
};


int main() {
	
	//{
		//Timer timer;
		String gfl;
		gfl.Write("Hello World!");
	//}

	{
		Timer timer;
		std::string std = "Hello World!";
	}

	printf("%zd\n", sizeof(WString));
	//GravityApp *app = NewApplication("Gravity Engine", 1366, 768, 4, 5);

	//app->Init();
	//app->EnableVSync(1);

	//while (!app->CloseAplication()) {
	//	app->NewFrame();
	//	app->Tick();

	//	app->ui.Tick();
	//	app->SwapBuffer();
	//}

	//TerminateApplication(app);

	//Logger::Free();

#if 0
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}