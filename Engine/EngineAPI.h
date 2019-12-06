#pragma once
#ifndef GRAVITY_MACROS
#define GRAVITY_MACROS

#ifdef GRAVITY_EXPORT
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#endif // !GRAVITY_MACROS