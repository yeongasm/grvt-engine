// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <unordered_map>
#include <deque>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <nativefiledialog/nfd.h>

#include <GrvtFoundation.h>
#include <GrvtArray.h>
#include <GrvtString.h>
#include <GrvtPair.h>

using namespace Gfl;

#include <Framework/Foundations/Foundations.h>
#include <Framework/Foundations/LowLevelAPI.h>
#include <Framework/Foundations/Interface.h>

#include <Framework/Abstractions/Model.h>
#include <Framework/Abstractions/Shader.h>
#include <Framework/Abstractions/Texture.h>
#include <Framework/Abstractions/Framebuffer.h>
#include <Framework/Abstractions/Material.h>
#include <Framework/Abstractions/Lighting.h>

#include <Camera/Camera.h>

#include <Renderer/RenderCache.h>
#include <Renderer/RenderFoundation.h>
#include <Renderer/Renderer.h>
#include <Renderer/DeferredRenderer.h>

#include <Manager/Manager.h>
#include <IO/InputHandler.h>
#include <IO/IO.h>

#include <Engine/Engine.h>
#include "Application/DemoApplication.h"

#endif //PCH_H
