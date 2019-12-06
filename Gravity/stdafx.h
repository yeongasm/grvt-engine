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

#include <Minimal.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Include/Thirdparty/stb/stb_image.h>

#include <Include/Framework/Foundation/Foundations.h>
#include <Include/Framework/Foundation/LowLevelAPI.h>
#include <Include/Framework/Foundation/Interface.h>

#include <Include/Framework/Abstraction/Texture.h>
#include <Include/Framework/Abstraction/Shader.h>
#include <Include/Framework/Abstraction/Framebuffer.h>
#include <Include/Framework/Abstraction/Model.h>
#include <Include/Framework/Abstraction/Material.h>
#include <Include/Framework/Abstraction/Lighting.h>

#include <Include/Camera/Camera.h>

#include <Include/Renderer/RenderCache.h>
#include <Include/Renderer/RenderFoundation.h>
#include <Include/Renderer/Renderer.h>
#include <Include/Renderer/DeferredRenderer.h>

#include <Include/Utility/Utility.h>
#include <Include/Manager/Manager.h>
#include <Include/IO/InputHandler.h>
#include <Include/IO/IO.h>

#include <Include/Core/Application.h>
#include <Include/Core/Engine.h>

#endif // !PRECOMPILE_HEADER