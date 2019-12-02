#pragma once

#ifndef PRECOMPILE_HEADER
#define PRECOMPILE_HEADER

#if _WIN32
#include <Windows.h>
#endif

#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GrvtArray.h>
#include <GrvtPair.h>
#include <GrvtString.h>


#endif // !PRECOMPILE_HEADER