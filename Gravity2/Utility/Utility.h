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


/**
* Opens a file at the specified path and pushes it's contents into a buffer.
* @param [REQUIRED] (String) Buffer - Buffer to stream in file contents.
* @param [REQUIRED] (String) Path   - Path to file.
*/
bool OpenFile(String &Buffer, const String &Path);

/**
* Root directory is the path to the solution's directory.
* @param [REQUIRED] Path - Path to file.
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

#endif // !GRAVITY_UTILITY