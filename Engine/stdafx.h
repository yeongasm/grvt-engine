#pragma once

#ifndef PRECOMPILE_HEADER
#define PRECOMPILE_HEADER

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#ifdef GRAVITY_EXPORT
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#if _WIN32
#include <Windows.h>
#endif

#include <unordered_map>
#include <deque>

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

#include "Framework/Foundations/Foundations.h"
#include "Framework/Foundations/LowLevelAPI.h"
#include "Framework/Foundations/Interface.h"

#include "Framework/Abstractions/Shader.h"
#include "Framework/Abstractions/Texture.h"
#include "Framework/Abstractions/Framebuffer.h"
#include "Framework/Abstractions/Model.h"
#include "Framework/Abstractions/Material.h"
#include "Framework/Abstractions/Lighting.h"

#include "Camera/Camera.h"

#include "Renderer/RenderCache.h"
#include "Renderer/RenderFoundation.h"
#include "Renderer/Renderer.h"
#include "Renderer/DeferredRenderer.h"

#include "Utility/Utility.h"
#include "Manager/Manager.h"
#include "IO/InputHandler.h"
#include "IO/IO.h"

#include "Engine/Application.h"
#include "Engine/Engine.h"

#endif // !PRECOMPILE_HEADER