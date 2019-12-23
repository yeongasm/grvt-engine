#include "pch.h"
#include <Renderer/Renderer.h>
#include "CameraSystem.h"

//CameraSystem::CameraSystem() : Camera() {}
//
//
//CameraSystem::~CameraSystem() {}
//
//
//CameraSystem::CameraSystem(const CameraSystem& Other)
//{
//	*this = Other;
//}
//
//
//CameraSystem& CameraSystem::operator= (const CameraSystem& Other)
//{
//	_ASSERTE(this != &Other);
//
//	if (this != &Other)
//	{
//		Camera = Other.Camera;
//	}
//
//	return *this;
//}
//
//
//CameraSystem::CameraSystem(CameraSystem&& Other)
//{
//	*this = Gfl::Move(Other);
//}
//
//
//CameraSystem& CameraSystem::operator= (CameraSystem&& Other)
//{
//	_ASSERTE(this != &Other);
//
//	if (this != &Other)
//	{
//		Camera = Gfl::Move(Other.Camera);
//	}
//
//	return *this;
//}


void CameraSystem::Init()
{
	REGISTERSYSTEM(CameraSystem)

	Grvt::CameraCreationInfo Info;
	Info.CameraPosition = glm::vec3(0.0f, 5.0f, 5.0f);
	Info.Sensitivity = 0.001f;
	Info.Near = 0.1f;
	Info.Far = 1000.0f;
	Info.ViewportWidth  = Grvt::GetRenderer()->Width;
	Info.ViewportHeight = Grvt::GetRenderer()->Height;

	LastMousePos = glm::vec2(0.0f, 0.0f);

	Camera.Init(Info);
	Io = Grvt::GetEngine()->GetIO();
}


void CameraSystem::Tick()
{
	// TODO(Afiq):
	// Need to find a way to not convert the width and height into a float.
	if (Camera.Width != (float32)Grvt::GetRenderer()->Width)
	{
		Camera.Width = (float32)Grvt::GetRenderer()->Width;
	}

	if (Camera.Height != (float32)Grvt::GetRenderer()->Height)
	{
		Camera.Height = (float32)Grvt::GetRenderer()->Height;
	}

	float32 DeltaTime = Grvt::GetEngine()->DeltaTime;

	glm::vec2 AngleOffset = Io->MousePos - LastMousePos;
	LastMousePos = Io->MousePos;

	Camera.Rotate(glm::vec3(AngleOffset.x, AngleOffset.y, 0.0f), DeltaTime);
	Camera.Zoom(Io->MouseWheel);

	if (Io->IsKeyHeld(GLFW_KEY_Q))
	{
		Camera.Translate(Camera.Up, DeltaTime);
	}

	if (Io->IsKeyHeld(GLFW_KEY_W))
	{
		Camera.Translate(Camera.Forward, DeltaTime);
	}

	if (Io->IsKeyHeld(GLFW_KEY_E))
	{
		Camera.Translate(-Camera.Up, DeltaTime);
	}

	if (Io->IsKeyHeld(GLFW_KEY_A))
	{
		Camera.Translate(-Camera.Right, DeltaTime);
	}

	if (Io->IsKeyHeld(GLFW_KEY_S))
	{
		Camera.Translate(-Camera.Forward, DeltaTime);
	}

	if (Io->IsKeyHeld(GLFW_KEY_D))
	{
		Camera.Translate(Camera.Right, DeltaTime);
	}

	Camera.Tick(DeltaTime);
}


void CameraSystem::Shutdown()
{
	printf("Shutting down CameraSystem!\n");
}