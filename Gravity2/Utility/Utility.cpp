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
	path.Write("%s/%s", SolutionDir, ~Path);

	return path;
}

bool DoesFileExist(const String &Path) {
	struct stat buffer;
	return (stat(Path.c_str(), &buffer) == 0);
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

	stream = fopen(~logFileInstance, "w");

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
	printf("Time: %.6f ms\n\n", Now());
}


float Timer::Now() {
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;

	return duration.count() * 1000.0f;
}