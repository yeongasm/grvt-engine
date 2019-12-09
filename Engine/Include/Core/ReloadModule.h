#pragma once

#ifndef GRAVITY_CORE_RELOAD_MODULE
#define GRAVITY_CORE_RELOAD_MODULE

namespace Grvt
{

	class ReloadModule
	{
	private:

		friend class GrvtEngine;

		using ArgFuncPtr	= void (*)(GrvtEngine* EnginePtr);
		using FuncPtr		= void (*)();

		HINSTANCE	ModuleDll;
		FILETIME	DllLastWriteTime;

		Gfl::String	DllFile;
		Gfl::String	DllTempFile;

		ArgFuncPtr	StartUp;
		ArgFuncPtr	OnLoad;
		FuncPtr		Execute;
		FuncPtr		Unload;

		bool		IsValid;

	public:

		ReloadModule();
		~ReloadModule();

		FILETIME WatchFileChange();

		//void Init();
		bool LoadModuleDll(bool OnEngineInit = false);
		void UnloadModuleDll();

	};

}

#endif // !GRAVITY_CORE_RELOAD_MODULE