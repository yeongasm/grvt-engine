#pragma once

#ifndef DEMO_CAMERA_SYSTEM
#define DEMO_CAMERA_SYSTEM

#include "Camera/Camera.h"
#include "Systems/BaseSystem.h"

enum CamSystemMode : uint32 
{
	CameraSystem_NoMode				= 0x00,
	CameraSystem_MoveFBAndRotateLR	= 0x01,
	CameraSystem_RotateOnHold		= 0x02
};


/**
* NOTE(Afiq):
* Should we implement some sort of constraints to prevent Yaw and Pitch from exceeding 90 degrees?
*/
class CameraSystem : public Grvt::BaseSystem
{
public:

	Grvt::BaseCamera	Camera;

private:

	Grvt::GrvtEngine*	EnginePtr;
	Grvt::EngineIO*		Io;
	glm::vec2			CapturedMousePos;
	glm::vec2			LastMousePos;
	glm::vec2			Offsets;

	uint32 Mode;

	bool Dirty;

	/**
	* Moves camera forward, backward, right, left, up and down.
	*/
	void MoveCamera();
	void MoveFBAndRotateLR();
	void RotateOnHold();

public:

	bool FirstMouse;

	void Init();
	void Tick();
	void Shutdown();
};

#endif // !DEMO_CAMERA_SYSTEM