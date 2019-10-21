#include "stdafx.h"


float EulerCamera::GetHorizontalOffset(const float &Offset) {
	if (inverseX)
		return Offset + lastXPos;
	else
		return Offset - lastXPos;
}


float EulerCamera::GetVerticalOffset(const float &Offset) {
	if (inverseY)
		return lastYPos + Offset;
	else
		return lastYPos - Offset;
}


void EulerCamera::UpdateCameraVectors() {
	// Calculate the new Front vector.
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(newFront);

	// Also re-calculatethe Right and Up vector.
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


EulerCamera::EulerCamera() :
	inverseX(false),
	inverseY(false),
	lastXPos(0.0f),
	lastYPos(0.0f),
	isActive(false),
	yaw(0.0f),
	pitch(0.0f),
	movementSpeed(0.0f),
	sensitivity(0.0f),
	zoom(0.0f),
	position(0.0f, 0.0f, 0.0f),
	front(0.0f, 0.0f, 0.0f),
	up(0.0f, 0.0f, 0.0f),
	worldUp(0.0f, 0.0f, 0.0f) {}


EulerCamera::~EulerCamera() {}


void EulerCamera::Init(EulerCameraInitInfo &Info) {
	inverseX		= Info.inverseOnXAxis;
	inverseY		= Info.inverseOnYAxis;
	yaw				= Info.yaw;
	pitch			= Info.pitch;
	movementSpeed	= Info.movementSpeed;
	sensitivity		= Info.sensitivity;
	zoom			= Info.zoom;
	lastXPos		= Info.frameCenter.x;
	lastYPos		= Info.frameCenter.y;
	position		= Info.position;
	worldUp			= Info.worldUp;

	UpdateCameraVectors();
}


glm::mat4 EulerCamera::GetViewMatrix() {
	return glm::lookAt(position, position + front, up);
}


void EulerCamera::ProcessKeyboard(CameraMovement Direction, float DeltaTime) {
	float velocity = movementSpeed * DeltaTime;

	if (!isActive)
		return;

	if (Direction == CAMERA_MOVE_FORWARD)
		position += front * velocity;

	if (Direction == CAMERA_MOVE_BACKWARD)
		position -= front * velocity;

	if (Direction == CAMERA_MOVE_RIGHT)
		position += right * velocity;

	if (Direction == CAMERA_MOVE_LEFT)
		position -= right * velocity;

	if (Direction == CAMERA_MOVE_UP)
		position += up * velocity;

	if (Direction == CAMERA_MOVE_DOWN)
		position -= up * velocity;
}


void EulerCamera::ProcessMouseMovement(float XOffset, float YOffset, unsigned char ConstrainPitch) {
	static bool onInit = true;
	
	if (!isActive) {
		onInit = true;
		
		return;
	}

	if (onInit) {
		lastXPos = XOffset;
		lastYPos = YOffset;

		onInit = false;
	}
	
	float xOffFromOrig = GetHorizontalOffset(XOffset);
	float yOffFromOrig = GetVerticalOffset(YOffset);

	lastXPos = XOffset;
	lastYPos = YOffset;
	
	float horizontal = xOffFromOrig * sensitivity;
	float vertical	 = yOffFromOrig * sensitivity;

	yaw   += horizontal;
	pitch += vertical;

	// Ensures that screen doesn't flip when Pitch is out of bounds.
	if (ConstrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	// Update Front, Right, and Up Vectors using the updated Euler angles.
	UpdateCameraVectors();
}


void EulerCamera::ProcessMouseScroll(float YOffset) {
	if (!isActive)
		return;

	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= YOffset;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
}