#include "pch.h"
#include <Renderer/Renderer.h>
#include "CameraSystem.h"
#include <Core/Engine.h>

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


void CameraSystem::MoveCamera()
{
	if (Io->IsKeyHeld(GLFW_KEY_Q))
	{
		Camera.Translate(-Camera.Up, EnginePtr->DeltaTime);
	}

	if (Io->IsKeyHeld(GLFW_KEY_W))
	{
		Camera.Translate(Camera.Forward, EnginePtr->DeltaTime);
	}

	if (Io->IsKeyHeld(GLFW_KEY_E))
	{
		Camera.Translate(Camera.Up, EnginePtr->DeltaTime);
	}

	if (Io->IsKeyHeld(GLFW_KEY_A))
	{
		Camera.Translate(-Camera.Right, EnginePtr->DeltaTime);
	}

	if (Io->IsKeyHeld(GLFW_KEY_S))
	{
		Camera.Translate(-Camera.Forward, EnginePtr->DeltaTime);
	}

	if (Io->IsKeyHeld(GLFW_KEY_D))
	{
		Camera.Translate(Camera.Right, EnginePtr->DeltaTime);
	}
}


void CameraSystem::MoveFBAndRotateLR()
{
	if (Offsets.y > 0)
	{
		Camera.Translate(-Camera.Forward, EnginePtr->DeltaTime);
	}

	if (Offsets.y < 0)
	{
		Camera.Translate(Camera.Forward, EnginePtr->DeltaTime);
	}

	Camera.Rotate(glm::vec3(Offsets.x, 0.0f, 0.0f), EnginePtr->DeltaTime);
}


void CameraSystem::RotateOnHold()
{
	Camera.Rotate(glm::vec3(Offsets.x, Offsets.y, 0.0f), EnginePtr->DeltaTime);
}


void CameraSystem::Init()
{
	REGISTERSYSTEM(CameraSystem)

	Grvt::CameraCreationInfo Info;
	Info.CameraPosition = glm::vec3(0.0f, 10.0f, 10.0f);
	Info.Sensitivity	= 5.0f;
	Info.MoveSpeed		= 50.0f;
	Info.Near			= 0.001f;
	Info.Far			= 1000.0f;
	Info.ViewportWidth  = Grvt::GetRenderer()->Width;
	Info.ViewportHeight = Grvt::GetRenderer()->Height;

	EnginePtr = Grvt::GetEngine();
	Io = Grvt::GetEngine()->GetIO();

	LastMousePos = glm::vec2(0.0f);
	Offsets		 = glm::vec2(0.0f);

	Mode		= CameraSystem_NoMode;
	Dirty		= true;
	FirstMouse	= true;

	Camera.Init(Info);
	Camera.Pitch = 45.0f;
	Camera.Rotate(glm::vec3(Camera.Pitch, Camera.Yaw, 0.0f), EnginePtr->DeltaTime);
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

	// Allow zoom if mouse is in viewport range.
	float32 HRange = Camera.Width  - Io->MousePos.x;
	float32 VRange = Camera.Height - Io->MousePos.y;

	if ((HRange >= 0 && HRange <= Camera.Width) && (VRange >= 0 && VRange <= Camera.Height))
	{
		Camera.Zoom(Io->MouseWheel);
		Dirty = true;
	}

	// NOTE(Afiq):
	// Remove this section when it is no longer needed.
	// Reset the camera to it's default position.
	if (Io->IsKeyPressed(GLFW_KEY_F1))
	{
		Camera.Yaw	 = 0.0f;
		Camera.Pitch = 45.0f;
		Camera.Roll  = 0.0f;
		Camera.Orientation  = glm::quat();
		Camera.Position		= glm::vec3(0.0f, 50.0f, 50.0f);
		Dirty = true;
	}

	if (Io->IsMouseHeld(Grvt::Grvt_MouseButton_Left))
	{
		Mode = CameraSystem_MoveFBAndRotateLR;
	}

	if (Io->IsMouseHeld(Grvt::Grvt_MouseButton_Right))
	{
		Mode = CameraSystem_RotateOnHold;
	}

	if (Io->IsMouseReleased(Grvt::Grvt_MouseButton_Right) || Io->IsMouseReleased(Grvt::Grvt_MouseButton_Left))
	{
		// Reset the mouse position to where it once was.
		EnginePtr->SetMousePosition(CapturedMousePos);
		Mode = CameraSystem_NoMode;
	}

	if (Mode != CameraSystem_NoMode)
	{
		if (FirstMouse)
		{
			// Set the mouse position to the center of the screen.
			CapturedMousePos = Io->MousePos;
			glm::vec2 ViewportCentre = glm::vec2(Camera.Width * 0.5f, Camera.Height * 0.5f);

			// Hide the cursor if camera is active;
			EnginePtr->SetMousePosition(ViewportCentre);
			EnginePtr->ShowCursor(false);

			LastMousePos = Io->MousePos;
			FirstMouse = false;
		}

		Offsets		 = Io->MousePos - LastMousePos;
		LastMousePos = Io->MousePos;
	}
	else
	{
		if (!FirstMouse)
		{
			EnginePtr->ShowCursor(true);
			EnginePtr->SetMousePosition(CapturedMousePos);
			FirstMouse = true;
			CapturedMousePos = glm::vec2(0.0f, 0.0f);
		}
	}

	if (Mode == CameraSystem_MoveFBAndRotateLR)
	{
		MoveFBAndRotateLR();
		MoveCamera();

		Dirty = true;
	} 
	else if (Mode == CameraSystem_RotateOnHold)
	{
		RotateOnHold();
		MoveCamera();
		Dirty = true;
	}

	if (Dirty)
	{
		Camera.Tick(EnginePtr->DeltaTime);
		Dirty = false;
	}
}


void CameraSystem::Shutdown()
{
	EnginePtr	= nullptr;
	Io			= nullptr;
}