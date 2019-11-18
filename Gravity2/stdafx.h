#pragma once

#ifndef PRECOMPILE_HEADER
#define PRECOMPILE_HEADER

//#define _CRT_SECURE_NO_WARNINGS 1 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 

#if _WIN32
#include <Windows.h>
#endif

#include <sys/stat.h>

#include <string>
#include <unordered_map>
#include <deque>
#include <thread>
#include <chrono>

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

#include <nativefiledialog/nfd.h>

// Custom array and string class.
#include <GrvtArray.h>
#include <GrvtPair.h>
#include <GrvtString.h>

using namespace Gfl;

// Assimp library header files.
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ThirdParty/stb/stb_image.h"
#include "ThirdParty/FontAwesome/IconsFontAwesome5.h"
#include "ThirdParty/DearImGui/imgui.h"
#include "ThirdParty/DearImGui/imgui_internal.h"
#include "ThirdParty/DearImGui/imgui_impl_glfw.h"
#include "ThirdParty/DearImGui/imgui_impl_opengl3.h"
#include "ThirdParty/smhasher/MurmurHash3.h"

#include "Framework/Foundations/Foundations.h"
#include "Framework/Foundations/LowLevelAPI.h"
#include "Framework/Foundations/Interface.h"
#include "Framework/Abstractions/Shader.h"
#include "Framework/Abstractions/Texture.h"
#include "Framework/Abstractions/Postprocessing.h"
#include "Framework/Abstractions/Model.h"
#include "Framework/Abstractions/Material.h"
#include "Framework/Abstractions/Lighting.h"
#include "Framework/Camera/Camera.h"
#include "Framework/Renderer/RenderFoundation.h"
//#include "Framework/Renderer/RenderCommand.h"
//#include "Framework/Renderer/RenderInstance.h"
//#include "Framework/Renderer/Renderer.h"

#include "Engine/Utility/Utility.h"
#include "Engine/Manager/Manager.h"
#include "Engine/IO/InputHandler.h"
#include "Engine/IO/IO.h"
//#include "Engine/UI/UI.h"
//#include "Engine/UI/WindowFunctions.h"
//#include "Engine/UI/UIHandler.h"
//#include "Engine/Profiling/Statistics.h"
//#include "Engine/Project/Project.h"
//#include "Engine/Application/Application.h"

#endif // !PRECOMPILE_HEADER