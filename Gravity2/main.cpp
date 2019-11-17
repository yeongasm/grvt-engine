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


int main() {

	HashString<MurmurStringHash> testing;
	String testing2 = "Hello World!";

	testing = testing2;

#if 0
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}