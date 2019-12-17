#pragma once

#ifndef GRAVITY_PRECOMPILE_HEADER
#define GRAVITY_PRECOMPILE_HEADER

#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "EngineAPI.h"
#include "Utility/Utility.h"
#include "IO/IO.h"
#include "Core/Engine.h"

#endif // !GRAVITY_PRECOMPILE_HEADER