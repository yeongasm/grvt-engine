#pragma once
#ifndef GRAVITY_UTILITY
#define GRAVITY_UTILITY


#define GRAVITY_ENABLE_LOG 1
#define GRAVITY_LEAK_DEBUG 0


#if GRAVITY_ENABLE_LOG
#define ConstructLogString String msg;
#define PrintIntoLog(Type, Component, Format, ...) msg.Write(Format, __VA_ARGS__); Logger::Log(Type, Component, msg);
#define	DumpLogIntoFile Logger::DumpLogData(); Logger::Free();
#else
#define ConstructLogString
#define PrintIntoLog(Type, Component, Format, ...)
#define DumpLogIntoFile
#endif


#ifdef _DEBUG
#if GRAVITY_LEAK_DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif


#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define clamp(v, vmin, vmax)	(max(min(v, vmax), vmin))


#define ArrayLen(Arr) ((int)(sizeof(Arr)/sizeof(*Arr)))


#define CopyRawArray(Dst, Src)					\
	for (size_t i = 0; i < ArrayLen(Dst); i++)	\
		Dst[i] = Src[i];						


//typedef glm::vec4 Vector;


enum WindowNativeDialogMode : uint {
	NATIVE_DIALOG_OPEN_FILE		= 0x00,
	NATIVE_DIALOG_OPEN_PATH		= 0x01
};


/**
* Opens a file at the specified path and pushes it's contents into a buffer.
* @param [REQUIRED] (String) Buffer - Buffer to stream in file contents.
* @param [REQUIRED] (String) Path   - Path to file.
*/
bool OpenFile(String &Buffer, const String &Path);

/**
* Checks if a file at the specified path exist or not.
* @param [REQUIRED] (String) Path - Path to file.
*/
bool DoesFileExist(const String &Path);


/**
* Gets the file name from path.
* This function retrieves the file name by finding the last '/' from the path string.
* @param [REQUIRED] (String) Path - Path to file. 
* @param [REQUIRED] (String) File - Buffer to place file. 
* @param [OPTIONAL] (String) Directory - Buffer to place directory. 
*/
bool GetFileNameFromPath(String &Path, String &File, String &Directory);


/**
* Opens Window's native file dialog and returns the path string.
* @param [REQUIRED] (WindowNativeDialogMode)	Mode - Native File Dialog's mode.
* @param [REQUIRED] (String)					Buffer - Buffer to place resulting path.
* @param [OPTIONAL] (const char*)				Format - Accepted file format WindowNativeDialogMode is set to NATIVE_DIALOG_OPEN_FILE.
*/
bool OpenNativeFileDialog(WindowNativeDialogMode Mode, String &Buffer, const char *Format = nullptr);


/**
* Opens Window's native file dialog and returns multiple path strings.
* @param [REQUIRED] (WindowNativeDialogMode)	Mode - Native File Dialog's mode.
* @param [REQUIRED] (String)					Buffer - Buffer to place resulting path.
* @param [OPTIONAL] (const char*)				Format - Accepted file format WindowNativeDialogMode is set to NATIVE_DIALOG_OPEN_FILE.
*/
bool OpenNativeFileDialog(Array<String> &Buffer, const char *Format = nullptr);


/**
* Root directory is the path to the solution's directory.
* @param [REQUIRED] Path - A buffer to place the path.
*/
String RootDir(const String &Path);

enum LogType : size_t {
	LOG_WARN	= 0,
	LOG_ERR		= 1,
	LOG_INFO	= 2
};

enum LogComponent : size_t {
	LOG_SHADER		= 0,
	LOG_TEXTURE		= 1,
	LOG_SCENE		= 2,
	LOG_MANAGER		= 3,
	LOG_OPENFILE	= 4,
	LOG_MATERIAL	= 5,
	LOG_SCENEINST	= 6,
	LOG_APP			= 7,
	LOG_LIGHT		= 8,
	LOG_LEVEL		= 9
};

/**
* Static Logger class.
* Prints into the console while also dumping log contents into a file.
*
* TODO(Afiq):
* There needs to be a better way to handle logging.
* Latest idea is to put the logger inside of the Application class.
* Have the ResourceHandler (Manager class) have a variable to point to the Application class.
* Reason because the logger can only exist inside of the application and hence why we ought to put it inside of the Application class.
*/
struct Logger {
private:
	static String *logType;
	static String *logComponent;
	static String logFileInstance;
	static Array<String> logData;

public:
	static void	Log				(LogType Type, LogComponent Component, const String &Message);
	static void DumpLogData		();
	static void Free			();
};

struct Timer {
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	Timer();
	~Timer();

	float Now();
};

template <class _Type>
constexpr _Type&& Move(_Type &&Arg) {
	return static_cast<_Type&&>(Arg);
}

template <class _Type>
constexpr _Type&& Forward(_Type &Arg) {
	return static_cast<_Type&&>(Arg);
}

template <class _Type>
constexpr _Type&& Forward(_Type &&Arg) {
	return static_cast<_Type&&>(Arg);
}


/**
* NOTE(Afiq):
* Should we introduct a form of function data structure for abstraction?
* I can see the use of it when building our own thread wrapper but would it work? Not so sure ...
*/
//template <typename Type>
//struct Function {
//	using Callback = Type(*)(void*);
//
//	Callback Func;
//
//	Function(Callback Source) : Func(Source) {}
//
//	Type operator() () { return Func(); }
//};

#endif // !GRAVITY_UTILITY