#pragma once

#ifndef PRECOMPILE_HEADER
#define PRECOMPILE_HEADER

#define _CRT_SECURE_NO_WARNINGS 1 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 

#if _WIN32
#include <Windows.h>
#endif

#include <sys/stat.h>

#include <map>
#include <queue>
#include <deque>
#include <thread>
#include <chrono>
#include <functional>

//#if defined(_DEBUG)
//#define _CRTDBG_MAP_ALLOC		// Enable heap memory leak detection
//#include <stdlib.h>
//#undef _malloca
//#include <crtdbg.h>
//#else
//#include <stdlib.h>
//#endif

// Required OpenGL header files.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

// GLM functions and data structures.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Custom array and string class.
#include <gravitystl.h>

// Include ImGui headers.
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Assimp library header files.
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb/stb_image.h"

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#ifdef _DEBUG
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
//#endif

// Utility header file.
#include "Utility/Utility.h"
#include "GravityOpenAPI/LowLevelAPI.h"
#include "GravityOpenAPI/Middleware.h"

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Scene.h"
#include "Graphics/Material.h"
#include "Graphics/SceneInstace.h"
#include "Graphics/Lighting.h"


#include "Camera/Camera.h"

#include "Level/Scenery.h"
#include "Manager/Manager.h"

#include "Renderer/RenderNode.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderBuffer.h"
#include "Renderer/Renderer.h"

#define IMGUI_WINDOW_DEBUGGER 1

#include "IO/InputHandler.h"
#include "IO/IO.h"

#include "UI/UI.h"
#include "UI/WndFuncs/WndFuncs.h"
#include "UI/UIHandler.h"

#include "Application/Statistics.h"
#include "Application/Project.h"
#include "Application/Application.h"

#endif // !PRECOMPILE_HEADER