#pragma once

#ifndef GRAVITY_BASE_CAMERA
#define GRAVITY_BASE_CAMERA

#include "Minimal.h"

namespace Grvt
{

	/**
	* Camera's projection mode.
	*/
	enum ProjectionMode : uint32
	{
		GrvtCamera_Projection_None			= 0x00,	// On first init only.
		GrvtCamera_Projection_Perspective   = 0x01,
		GrvtCamera_Projection_Orthographic  = 0x02
	};


	/**
	*/
	struct CameraCreationInfo
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
	* TODO(Afiq):
	* Make the BaseCamera a struct instead.
	*/
	class BaseCamera
	{
	protected:

		glm::mat4	Projection;
		glm::mat4	View;

	public:

		glm::quat	Orientation;
		glm::vec3	Position;
		glm::vec3	Forward;
		glm::vec3	Up;
		glm::vec3	Right;
		glm::vec3	MoveDelta;

	protected:

		/**
		* Resets the camera to it's default state.
		*/
		virtual void Reset();

		/**
		* Updates the view matrix for the camera.
		*/
		virtual void UpdateViewMatrix();

	public:

		/**
		* Updates the different vectors of the camera.
		*/
		virtual void UpdateOrientation();

	protected:

		/**
		* Updates the camera's projection matrix.
		*/
		virtual void UpdateProjectionMatrix();

	public:

		/**
		* Moves the camera in world space.
		*/
		ENGINE_API virtual void Translate(const glm::vec3& Direction, float DeltaTime);

		/**
		* Rotates the camera in world space.
		*/
		ENGINE_API virtual void Rotate(const glm::vec3& Angle, float DeltaTime);

		/**
		* Zooms the camera's view. Only works when in perspective mode.
		*/
		ENGINE_API virtual void Zoom(float Offset);

	public:

		/**
		* Properties for computing the projection matrix.
		*/
		uint32		Mode;
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

		ENGINE_API BaseCamera();
		ENGINE_API ~BaseCamera();

		ENGINE_API BaseCamera(const BaseCamera& Other);
		ENGINE_API BaseCamera& operator= (const BaseCamera& Other);

		ENGINE_API BaseCamera(BaseCamera&& Other);
		ENGINE_API BaseCamera& operator= (BaseCamera&& Other);

	public:

		/**
		* Retrieves the view matrix.
		*/
		const glm::mat4& GetCameraView() const;

		/**
		* Retrieves the projection matrix.
		*/
		ENGINE_API const glm::mat4& GetCameraProjection() const;

		/**
		* Initialises the camera object.
		*/
		ENGINE_API void Init(const CameraCreationInfo& Info);

		/**
		* Camera's Tick() function. Called every single frame.
		*/
		ENGINE_API virtual void Tick(float DeltaTime);

		/**
		* Changes the camera's projection mode.
		*/
		ENGINE_API void SetCameraProjection(uint32 NewProjection);

	};

}

#endif // !GRAVITY_BASE_CAMERA