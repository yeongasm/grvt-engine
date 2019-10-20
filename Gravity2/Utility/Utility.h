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


#define ArrayLen(Arr) ((int)(sizeof(Arr)/sizeof(*Arr)))


#define CopyRawArray(Dst, Src)					\
	for (size_t i = 0; i < ArrayLen(Dst); i++)	\
		Dst[i] = Src[i];						


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
	LOG_SHADER			= 0x00,
	LOG_TEXTURE			= 0x01,
	LOG_SCENE			= 0x02,
	LOG_MANAGER			= 0x03,
	LOG_OPENFILE		= 0x04,
	LOG_MATERIAL		= 0x05,
	LOG_SCENEINST		= 0x06,
	LOG_APP				= 0x07,
	LOG_LIGHT			= 0x08,
	LOG_LEVEL			= 0x09,
	LOG_POSTPROCESS		= 0x0A,
	LOG_RENDERBUFFER	= 0x0B
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

	/**
	* Returns the duration of time passed from the start of this struct in milliseconds.
	*/
	float Now();
};


#endif // !GRAVITY_UTILITY