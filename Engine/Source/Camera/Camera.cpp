#include "GrvtPch.h"
#include "Camera/Camera.h"

namespace Grvt
{

	BaseCamera::BaseCamera() :
		Projection(1.0f), View(1.0f), Orientation(),
		Position(0.0f), Forward(), Up(),
		Right(), Mode(0), FieldOfView(0.0f),
		Width(0.0f), Height(0.0f), Near(0.0f),
		Far(0.0f), MoveSpeed(0.0f), Sensitivity(0.0f),
		Yaw(0.0f), Pitch(0.0f), Roll(0.0f),
		Enable(true), Dirty(false) {}


	BaseCamera::~BaseCamera()
	{
		Reset();
	}


	BaseCamera::BaseCamera(const BaseCamera& Other) { *this = Other; }


	BaseCamera& BaseCamera::operator= (const BaseCamera& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other) {
			Projection	= Other.Projection;
			View		= Other.View;
			Orientation = Other.Orientation;
			Position	= Other.Position;
			Forward		= Other.Forward;
			Up			= Other.Up;
			Right		= Other.Right;
			Mode		= Other.Mode;
			FieldOfView = Other.FieldOfView;
			Width		= Other.Width;
			Height		= Other.Height;
			Near		= Other.Near;
			Far			= Other.Far;
			MoveSpeed	= Other.MoveSpeed;
			Sensitivity = Other.Sensitivity;
			Yaw			= Other.Yaw;
			Pitch		= Other.Pitch;
			Roll		= Other.Roll;
			Enable		= Other.Enable;
		}

		return *this;
	}


	BaseCamera::BaseCamera(BaseCamera&& Other)
	{
		*this = Gfl::Move(Other);
	}


	BaseCamera& BaseCamera::operator= (BaseCamera&& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other) {
			Projection = Other.Projection;
			View = Other.View;
			Orientation = Other.Orientation;
			Position = Other.Position;
			Forward = Other.Forward;
			Up = Other.Up;
			Right = Other.Right;
			Mode = Other.Mode;
			FieldOfView = Other.FieldOfView;
			Width = Other.Width;
			Height = Other.Height;
			Near = Other.Near;
			Far = Other.Far;
			MoveSpeed = Other.MoveSpeed;
			Sensitivity = Other.Sensitivity;
			Yaw = Other.Yaw;
			Pitch = Other.Pitch;
			Roll = Other.Roll;
			Enable = Other.Enable;

			new (&Other) BaseCamera();
		}

		return *this;
	}


	void BaseCamera::Reset()
	{
		new (this) BaseCamera();
	}


	void BaseCamera::UpdateViewMatrix()
	{
		glm::mat4 Rotation = glm::mat4_cast(Orientation);
		glm::mat4 Translation = glm::mat4(1.0f);
		Translation = glm::translate(Translation, -Position);

		View = Rotation * Translation;
	}


	void BaseCamera::UpdateOrientation()
	{
		//glm::quat Quaternion = glm::quat(glm::vec3(glm::radians(Pitch), glm::radians(Yaw), glm::radians(Roll)));
		glm::quat Quaternion = glm::quat(glm::vec3(Pitch, Yaw, Roll));
		//printf_s("%.3f, %.3f\n", Yaw, Pitch);

		//Orientation = Quaternion * Orientation;
		//Orientation = glm::normalize(Orientation);
		Orientation = Quaternion;

		glm::mat4 Rot = glm::mat4_cast(Quaternion);
		Forward = glm::vec3(Rot[0][2], Rot[1][2], Rot[2][2]) * -1.0f;
		Up = glm::vec3(Rot[0][1], Rot[1][1], Rot[2][1]) * -1.0f;
		Right = glm::vec3(Rot[0][0], Rot[1][0], Rot[2][0]) * -1.0f;
		//Forward = Rotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
		//Up		= Rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		//Right	= Rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	}


	void BaseCamera::UpdateProjectionMatrix()
	{
		if (Mode == GrvtCamera_Projection_Perspective) 
		{
			Projection = glm::perspective(glm::radians(FieldOfView), Width / Height, Near, Far);
		} 
		else 
		{
			Projection = glm::ortho(0.0f, Width, 0.0f, Height, Near, Far);
		}
	}


	void BaseCamera::Translate(const glm::vec3& Direction, float DeltaTime)
	{
		float Velocity = MoveSpeed * DeltaTime;

		Position += Direction * Velocity;
		Dirty = true;
	}


	void BaseCamera::Rotate(const glm::vec3& Angle, float DeltaTime)
	{
		float Horizontal = Angle.x * Sensitivity;
		float Vertical = Angle.y * Sensitivity;

		//Yaw = glm::radians(Yaw + Horizontal);
		//Pitch = glm::radians(Pitch + Vertical);

		Yaw += Horizontal;
		Pitch += Vertical;

		UpdateOrientation();
		Dirty = true;
	}


	void BaseCamera::Zoom(float Offset)
	{
		if (FieldOfView >= 1.0f && FieldOfView <= 90.0f) 
		{
			FieldOfView -= Offset;
		}

		if (FieldOfView <= 1.0f) 
		{
			FieldOfView = 1.0f;
		}

		if (FieldOfView >= 90.0f) 
		{
			FieldOfView = 90.0f;
		}

		Dirty = true;
	}


	const glm::mat4& BaseCamera::GetCameraView() const
	{
		return View;
	}


	const glm::mat4& BaseCamera::GetCameraProjection() const
	{
		return Projection;
	}


	void BaseCamera::Init(const CameraCreationInfo& Info)
	{
		Position = Info.CameraPosition;
		Width = *(float32*)&Info.ViewportWidth;
		Height = *(float32*)&Info.ViewportHeight;
		Mode = Info.Projection;
		Near = Info.Near;
		Far = Info.Far;
		FieldOfView = Info.FieldOfView;
		Sensitivity = Info.Sensitivity;
		MoveSpeed = Info.MoveSpeed;

		Forward = glm::vec3(0.0f, 0.0f, -1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		Right = glm::vec3(1.0f, 0.0f, 0.0f);
	}


	void BaseCamera::Tick(float DeltaTime)
	{
		if (!Dirty)
			return;

		UpdateProjectionMatrix();
		UpdateViewMatrix();

		Dirty = false;
	}


	void BaseCamera::SetCameraProjection(uint32 NewProjection)
	{
		if (Mode != NewProjection) {
			Mode = NewProjection;
		}
	}

}