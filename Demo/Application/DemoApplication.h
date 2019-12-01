#pragma once


#ifndef GRVT_APPLICATON
#define GRVT_EXPORT __declspec(dllexport)
#else
#define GRVT_EXPORT __declspec(dllimport)
#endif


extern "C" GRVT_EXPORT void ModuleTick(GrvtEngine* Engine);