#include "GrvtPch.h"
#include "Core/ReloadModule.h"


namespace Grvt
{

	FILETIME ReloadModule::WatchFileChange()
	{

		WIN32_FIND_DATAA FindData;
		HANDLE FindHandle = FindFirstFileA(DllFile.C_Str(), &FindData);
		
		if (FindHandle == INVALID_HANDLE_VALUE)
		{
			return FILETIME();
		}

		FindClose(FindHandle);

		return FindData.ftLastWriteTime;
	}

	ReloadModule::ReloadModule() :
		ModuleDll(),
		DllLastWriteTime(),
		DllFile(),
		DllTempFile(),
		StartUp(nullptr),
		OnLoad(nullptr),
		Execute(nullptr),
		Unload(nullptr),
		IsValid(false) {}

	ReloadModule::~ReloadModule()
	{
		DllFile.Release();
	}

	//void ReloadModule::Init()
	//{

	//}

	bool ReloadModule::LoadModuleDll(bool OnEngineInit)
	{

		CopyFileA(DllFile.C_Str(), DllTempFile.C_Str(), FALSE);
		ModuleDll = LoadLibraryA(DllTempFile.C_Str());

		if (ModuleDll)
		{
			if (OnEngineInit)
			{
				StartUp = (ArgFuncPtr)GetProcAddress(ModuleDll, "OnStartUp");
			}

			OnLoad	= (ArgFuncPtr)GetProcAddress(ModuleDll, "OnLoad");
			Execute	= (FuncPtr)GetProcAddress(ModuleDll, "ExecuteApplication");
			Unload	= (FuncPtr)GetProcAddress(ModuleDll, "OnUnload");

			IsValid = true;
		}

		if (!IsValid)
		{
			StartUp = OnLoad = nullptr;
			Execute = Unload = nullptr;

			return false;
		}

		if (!OnEngineInit)
		{
			OnLoad(GetEngine());
		}

		return true;
	}

	void ReloadModule::UnloadModuleDll()
	{
		if (ModuleDll)
		{
			FreeLibrary(ModuleDll);
		}
		
		IsValid = false;
		StartUp = OnLoad = nullptr;
		Execute = Unload = nullptr;
	}

}