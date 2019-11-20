#pragma once


constexpr static glm::vec3 CameraWorldUp(0.0f, 1.0f, 0.0f);


class BaseCamera {
protected:

	glm::mat4	Projection;
	glm::mat4	View;
	glm::vec3	Position;
	glm::vec3	Forward;
	glm::vec3	Up;
	glm::vec3	Right;

public:

	float		MoveSpeed;
	float		Sensitivity;
	float		Zoom;
	bool		Enable;

	BaseCamera();
	~BaseCamera();

	BaseCamera(const BaseCamera& Other);
	BaseCamera& operator= (const BaseCamera& Other);

	BaseCamera(BaseCamera&& Other);
	BaseCamera& operator= (BaseCamera&& Other);


};