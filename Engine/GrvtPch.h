#pragma once
#ifndef GRAVITY_PRECOMPILE_HEADER
#define GRAVITY_PRECOMPILE_HEADER

#if _WIN32
#include <Windows.h>
#endif

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "Minimal.h"

#endif // !GRAVITY_PRECOMPILE_HEADER