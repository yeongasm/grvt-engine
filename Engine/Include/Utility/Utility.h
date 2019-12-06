#pragma once
#ifndef GRAVITY_UTILITY_FUNCTIONS
#define GRAVITY_UTILITY_FUNCTIONS

#include "Minimal.h"
#include "Framework/Abstraction/Model.h"

namespace Grvt
{

#define GRAVITY_LEAK_DEBUG 0


#ifdef _DEBUG
#if GRAVITY_LEAK_DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif


	enum WindowNativeDialogMode : uint32
	{
		GrvtNativeFileDialog_OpenFile = 0x00,
		GrvtNativeFileDialog_OpenPath = 0x01
	};


	namespace Util
	{


		/**
		* Opens a file at the specified path and pushes it's contents into a buffer.
		* @param [REQUIRED] (String) Buffer - Buffer to stream in file contents.
		* @param [REQUIRED] (String) Path   - Path to file.
		*/
		ENGINE_API bool OpenFile(Gfl::String& Buffer, const char* Path);

		/**
		* Checks if a file at the specified path exist or not.
		* @param [REQUIRED] (String) Path - Path to file.
		*/
		//bool DoesFileExist(const char* Path);


		/**
		* Gets the file name from path.
		* This function retrieves the file name by finding the last '/' from the path string.
		* @param [REQUIRED] (String) Path - Path to file.
		* @param [REQUIRED] (String) File - Buffer to place file.
		* @param [OPTIONAL] (String) Directory - Buffer to place directory.
		*/
		//bool GetFileNameFromPath(const char* Path, std::string& File, std::string& Directory);


		/**
		* Opens Window's native file dialog and returns the path string.
		* @param [REQUIRED] (WindowNativeDialogMode)	Mode - Native File Dialog's mode.
		* @param [REQUIRED] (String)					Buffer - Buffer to place resulting path.
		* @param [OPTIONAL] (const char*)				Format - Accepted file format WindowNativeDialogMode is set to NATIVE_DIALOG_OPEN_FILE.
		*/
		ENGINE_API bool OpenNativeFileDialog(WindowNativeDialogMode Mode, Gfl::String& Buffer, const char* Format = nullptr);


		/**
		* Opens Window's native file dialog and returns multiple path strings.
		* @param [REQUIRED] (WindowNativeDialogMode)	Mode - Native File Dialog's mode.
		* @param [REQUIRED] (String)					Buffer - Buffer to place resulting path.
		* @param [OPTIONAL] (const char*)				Format - Accepted file format WindowNativeDialogMode is set to NATIVE_DIALOG_OPEN_FILE.
		*/
		ENGINE_API bool OpenNativeFileDialog(Gfl::Array<Gfl::String>& Buffer, const char* Format = nullptr);


		/**
		* Imports the vertex model's data from the specified path.
		* @param [REQUIRED] (String)		Path - Path to the model's location.
		* @param [REQUIRED] (GrvtModel*)	Model - Model object.
		*/
		ENGINE_API void AssimpImportModelFromPath(const Gfl::String& Path, GrvtModel* Model);

	}

}

#endif // !GRAVITY_UTILITY_FUNCTIONS