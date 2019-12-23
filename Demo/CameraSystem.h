#pragma once

#ifndef DEMO_CAMERA_SYSTEM
#define DEMO_CAMERA_SYSTEM

#include "Camera/Camera.h"
#include "Systems/BaseSystem.h"

class CameraSystem : public Grvt::BaseSystem
{
public:

	Grvt::BaseCamera	Camera;
	
private:

	Grvt::EngineIO*		Io;
	glm::vec2			LastMousePos;

public:

	void Init();
	void Tick();
	void Shutdown();
};

#endif // !DEMO_CAMERA_SYSTEM