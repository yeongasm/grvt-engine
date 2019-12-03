#pragma once


/**
* Camera's projection mode.
*/
enum ProjectionMode : uint32 
{
	GrvtCamera_Projection_None = 0x00,	// On first init only.
	GrvtCamera_Projection_Perspective = 0x01,
	GrvtCamear_Projection_Orthographics = 0x02
};


/**
*/
struct ENGINE_API CameraCreationInfo 
{

	glm::vec3		CameraPosition = glm::vec3(0.0f, 0.0f, 1.0f);
	int32			ViewportWidth;
	int32			ViewportHeight;
	ProjectionMode	Projection = GrvtCamera_Projection_Perspective;
	float32			Near = 1.0f;
	float32			Far = 1000.0f;
	float32			FieldOfView = 45.0f;
	float32			Sensitivity = 1.0f;
	float32			MoveSpeed = 20.0f;

};


/**
* Gravity Engine's robust camera data structure.
*
* GravityCamera should be flexible enough to construct different types of camera fit for different needs.
* NOTE(Afiq):
* Not really sure if the camera system should be in the engine. Maybe it should maybe it shouldn't.
*/
class ENGINE_API BaseCamera 
{
protected:

	glm::mat4	Projection;
	glm::mat4	View;

	glm::quat	Orientation;
	glm::vec3	Position;

	glm::vec3	Forward;
	glm::vec3	Up;
	glm::vec3	Right;

	/**
	* Resets the camera to it's default state.
	*/
	virtual void Reset();

	/**
	* Updates the view matrix for the camera.
	*/
	virtual void UpdateViewMatrix();

	/**
	* Updates the different vectors of the camera.
	*/
	virtual void UpdateOrientation();

	/**
	* Updates the camera's projection matrix.
	*/
	virtual void UpdateProjectionMatrix();

public:

	/**
	* Moves the camera in world space.
	*/
	virtual void Translate(const glm::vec3& Direction, float DeltaTime);

	/**
	* Rotates the camera in world space.
	*/
	virtual void Rotate(const glm::vec3& Angle);

	/**
	* Zooms the camera's view. Only works when in perspective mode.
	*/
	virtual void Zoom(float Offset);

private:

	uint32		Mode;

public:

	/**
	* Properties for computing the projection matrix.
	*/
	float32		FieldOfView;
	float32		Width;
	float32		Height;
	float32		Near;
	float32		Far;

	/**
	* Camera's control feel.
	*/
	float32		MoveSpeed;
	float32		Sensitivity;

	/**
	* Euler angles that will be fed into a quaternion to handle the camera's orientation.
	*/
	float32		Yaw;
	float32		Pitch;
	float32		Roll;

	/**
	* Enable or disable the camera.
	*/
	bool		Enable;

	BaseCamera();
	~BaseCamera();

	BaseCamera(const BaseCamera& Other);
	BaseCamera& operator= (const BaseCamera& Other);

	BaseCamera(BaseCamera&& Other);
	BaseCamera& operator= (BaseCamera&& Other);

protected:

	bool		Dirty;

public:

	/**
	* Retrieves the view matrix.
	*/
	const glm::mat4& GetCameraView() const;

	/**
	* Retrieves the projection matrix.
	*/
	const glm::mat4& GetCameraProjection() const;

	/**
	* Initialises the camera object.
	*/
	void Init(const CameraCreationInfo& Info);

	/**
	* Camera's Tick() function. Called every single frame.
	*/
	virtual void Tick(float DeltaTime);

	/**
	* Changes the camera's projection mode.
	*/
	void SetCameraProjection(uint32 NewProjection);

};