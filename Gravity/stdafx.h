#pragma once

#ifndef PRECOMPILE_HEADER
#define PRECOMPILE_HEADER

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1


#if _WIN32
#include <Windows.h>
#endif

#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <EngineAPI.h>
#include <IO/IO.h>
#include <Core/Engine.h>
#include <Core/Tick.h>

#endif // !PRECOMPILE_HEADER