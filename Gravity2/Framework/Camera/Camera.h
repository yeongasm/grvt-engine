#pragma once


/**
* Camera's projection mode.
*/
enum ProjectionMode : uint32 {
	GrvtCamera_Projection_Perspective	= 0x00,
	GrvtCamear_Projection_Orthographics = 0x01
};


/**
*/
struct CameraCreationInfo {

	glm::vec3		CameraPosition;
	glm::vec2		ViewportPosition;
	int32			ViewportWidth;
	int32			ViewportHeight;
	ProjectionMode	Projection;
	float32			Near;
	float32			Far;

};


/**
* Gravity Engine's robust camera data structure.
*
* GravityCamera should be flexible enough to construct different types of camera fit for different needs.
*/
class GravityCamera {
protected:

	glm::mat4	Projection;
	glm::mat4	View;
	glm::vec4	Viewport;

	glm::quat	Rotation;
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
	virtual void UpdateCameraVectors();

	/**
	* Moves the camera in world space.
	*/
	void Translate(const glm::vec3& Delta, float DeltaTime);

	/**
	* Rotates the camera in worls space.
	*/
	void Rotate(const glm::vec3& Angle, float DeltaTime);


public:

	float		FieldOfView;
	float		Near;
	float		Far;
	float		MoveSpeed;
	float		Sensitivity;
	float		Zoom;

	bool		Enable;

	GravityCamera();
	~GravityCamera();

	GravityCamera(const GravityCamera& Other);
	GravityCamera& operator= (const GravityCamera& Other);

	GravityCamera(GravityCamera&& Other);
	GravityCamera& operator= (GravityCamera&& Other);

private:

	bool		Dirty;

public:

	/**
	* Returns the camera view matrix.
	*/
	glm::mat4 GetViewMatrix() const;

	/**
	* Updates the state of the camera for every frame.
	*/
	virtual void Tick(float DeltaTime, const AppIO& IO);

	/**
	* Updates the camera's position in world space.
	*/
	virtual void SetPosition(glm::vec3 Source);

};