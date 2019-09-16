#include "stdafx.h"


bool OpenFile(String &Buffer, const String &Path) {
	String	msg;
	FILE	*stream	= nullptr;
	fpos_t	pos;

	stream = fopen(~Path, "rb");

	if (!stream) {
		msg.Write("Unable to open file at path: %s", ~Path);
		Logger::Log(LOG_ERR, LOG_OPENFILE, msg);
		return false;
	}

	fseek(stream, 0, SEEK_END);
	fgetpos(stream, &pos);

	rewind(stream);
	Buffer.Reserve((size_t)(pos + 1), false);
	fread(Buffer.First(), sizeof(char), pos, stream);
	fclose(stream);

	Buffer[pos] = '\0';
	Buffer.UpdateHash();
	Buffer.ShrinkToFit();

	return true;
}


String RootDir(const String &Path) {
	String path;
	path.Write("%s/%s", __ROOT__, ~Path);

	return path;
}

bool DoesFileExist(const String &Path) {
	struct stat buffer;
	return (stat(Path.c_str(), &buffer) == 0);
}


bool GetFileNameFromPath(String &Path, String &File, String &Directory) {
	const char backSlash = '\\';
	const char forwardSlash = '/';
	size_t lastIdx = -1;

	for (size_t i = Path.Length() - 1; i >= 0; i--) {
		// Get first back slash or forward slash.
		if (Path[i] == backSlash || Path[i] == forwardSlash) {
			lastIdx = i;
			break;
		}
	}

	if (lastIdx == -1 || lastIdx == Path.Length() - 1)
		return false;

	Iterator<char> it = Iterator<char>(&Path[0]);

	// Set the directory name.
	Directory.Append(it, lastIdx + 1);
	Directory[lastIdx + 1] = '\0';
	
	// Set the file name.
	it = it + lastIdx + 1;
	size_t length = Path.Length() - lastIdx;
	File.Append(it, length);
	File[length - 1] = '\0';

	return true;
}


bool OpenNativeFileDialog(WindowNativeDialogMode Mode, String &Buffer, const char *Format) {
	nfdchar_t *outPath = NULL;
	nfdresult_t result;

	if (Mode == NATIVE_DIALOG_OPEN_FILE) {
		if (Format) {
			result = NFD_OpenDialog(Format, __ROOT__, &outPath);
		}
	} else {
		result = NFD_PickFolder(__ROOT__, &outPath);
	}

	if (result == NFD_OKAY) {
		Buffer.SetString(outPath);
		puts("Success!");
		puts(outPath);
		free(outPath);
	} else if (result == NFD_CANCEL) {
		puts("User pressed cancel.");
	} else {
		Logger::Log(LOG_WARN, LOG_OPENFILE, NFD_GetError());
		return false;
	}

	return true;
}


bool OpenNativeFileDialog(Array<String> &Buffer, const char *Format) {
	nfdpathset_t pathSet;
	nfdresult_t result = NFD_OpenDialogMultiple(Format, __ROOT__, &pathSet);

	if (result == NFD_OKAY) {
		size_t i;
		for (i = 0; i < NFD_PathSet_GetCount(&pathSet); ++i) {
			nfdchar_t *path = NFD_PathSet_GetPath(&pathSet, i);
			Buffer.Push(path);
		}
		NFD_PathSet_Free(&pathSet);
	} else if (result == NFD_CANCEL) {
		puts("User pressed cancel.");
	} else {
		Logger::Log(LOG_WARN, LOG_OPENFILE, NFD_GetError());
		return false;
	}

	return true;
}


static String logErrorTypes[] = {"WARNING", "ERROR", "INFO"};
String *Logger::logType = logErrorTypes;


static String logComponentTypes[] = {"SHADER", "TEXTURE", "SCENE", 
	"MANAGER", "OPEN FILE", "MATERIAL", "SCENE INSTANCE", "APP",
	"LIGHT", "LEVEL"};
String *Logger::logComponent = logComponentTypes;


String Logger::logFileInstance = RootDir("Log/Gravity.log");
Array<String> Logger::logData = Array<String>();


void Logger::Log(LogType Type, LogComponent Component, const String &Message) {	
	static bool onInit = true;
	String log;
	log.Write("[%s] %s::%s -> %s\n", __TIME__, ~logType[Type], ~logComponent[Component], /*now*/ /*__TIME__,*/ ~Message);
	log.Print(false);
	logData.Push(log);
}


void Logger::DumpLogData() {
	FILE *stream = nullptr;

	stream = fopen(logFileInstance.c_str(), "a");

	if (!stream) 
		stream = fopen(logFileInstance.c_str(), "w+");

	if (!stream) { 
		printf("Unable to open log file for writing"); 
		return; 
	}

	for (String &data : logData)
		fprintf(stream, ~data);

	fclose(stream);
}


void Logger::Free() {
	logType->Release();
	logComponent->Release();
	logFileInstance.Release();
	logData.Release();
}


Timer::Timer() {
	start = std::chrono::high_resolution_clock::now();
}


Timer::~Timer() {
	//printf("Time: %.6f ms\n\n", Now());
}


float Timer::Now() {
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;

	return duration.count() * 1000.0f;
}