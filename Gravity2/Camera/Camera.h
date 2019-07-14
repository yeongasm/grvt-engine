#pragma once


enum CameraMovement {
	CAMERA_MOVE_FORWARD,
	CAMERA_MOVE_BACKWARD,
	CAMERA_MOVE_LEFT,
	CAMERA_MOVE_RIGHT,
	CAMERA_MOVE_UP,
	CAMERA_MOVE_DOWN
};



/**
* Euler Angle camera creation data structure. Temporary fix before moving onto a Quaternion based camera.
* 
* @param [OPTIONAL] (bool)		inverseOnXAxis	- When enabled, moving the mouse right pans the camera to the left and vice versa.
* @param [OPTIONAL] (bool)		inverseOnYAxis	- When enabled, moving the mouse down pans the camera upwards and vice versa.
* @param [OPTIONAL] (float)		yaw				- Magnitude looking from left or right.
* @param [OPTIONAL] (float)		pitch			- Magnitude looking from top or bottom.
* @param [REQUIRED] (float)		movementSpeed	- Speed at which camera moves. Not specifying would make the camera unable to move.
* @param [REQUIRED] (float)		sensitivity		- Camera's sensitivity when moving the mouse.
* @param [REQUIRED] (glm::vec2) frameCenter		- Always provide the frame's center coordinate.
* @param [OPTIONAL] (glm::vec3) position		- Camera's initial position.
* @param [OPTIONAL] (glm::vec3) worldUp			- No idea what this is.
*/
struct EulerCameraInitInfo {
	bool		inverseOnXAxis	= false;
	bool		inverseOnYAxis	= false;
	float		yaw;
	float		pitch;
	float		movementSpeed;
	float		sensitivity;
	float		zoom;
	glm::vec2	frameCenter;
	glm::vec3	position		= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3	worldUp			= glm::vec3(0.0f, 0.0f, 0.0f);
};



class EulerCamera {
private:
	bool		inverseX;
	bool		inverseY;
	float		lastXPos;
	float		lastYPos;

	void		UpdateCameraVectors		();
	float		GetHorizontalOffset		(const float &Offset);
	float		GetVerticalOffset		(const float &Offset);

public:
	bool		isActive;
	float		yaw;
	float		pitch;
	float		movementSpeed;
	float		sensitivity;
	float		zoom;

	glm::vec3	position;
	glm::vec3	front;
	glm::vec3	up;
	glm::vec3	right;
	glm::vec3	worldUp;

	EulerCamera();
	EulerCamera(const EulerCamera &Other)				= delete;
	EulerCamera(EulerCamera &&Other)					= delete;

	EulerCamera& operator= (const EulerCamera &Other)	= delete;
	EulerCamera& operator= (EulerCamera &&Other)		= delete;

	~EulerCamera();

	void		Init					(EulerCameraInitInfo &Info);
	void		ProcessKeyboard			(CameraMovement Direction, float DeltaTime);
	void		ProcessMouseMovement	(float XOffset, float YOffset, unsigned char ConstrainPitch = true);
	void		ProcessMouseScroll		(float YOffset);
	glm::mat4	GetViewMatrix			();
};