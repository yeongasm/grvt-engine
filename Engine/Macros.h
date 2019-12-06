#pragma once
#ifndef GRAVITY_MACROS
#define GRAVITY_MACROS

#ifdef GRAVITY_EXPORT
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define GRVT_ARRAY_LENGTH(Arr) ((size_t)(sizeof(Arr) / sizeof(*Arr)))

#endif // !GRAVITY_MACROS