#include "GrvtPch.h"
#include "Camera/Camera.h"

namespace Grvt
{

	BaseCamera::BaseCamera() :
		Projection(1.0f), 
		View(1.0f), 
		Orientation(),
		Position(0.0f), 
		Forward(), 
		Up(),
		Right(), 
		Mode(0), 
		FieldOfView(0.0f),
		Width(0.0f), 
		Height(0.0f), 
		Near(0.0f),
		Far(0.0f), 
		MoveSpeed(0.0f), 
		Sensitivity(0.0f),
		Yaw(0.0f), 
		Pitch(0.0f), 
		Roll(0.0f) {}


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
			Projection  = Other.Projection;
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
		//glm::quat Quaternion = glm::quat(glm::vec3(Pitch, Yaw, Roll));
		glm::quat Quaternion = glm::quat(glm::vec3(glm::radians(Pitch), glm::radians(Yaw), glm::radians(Roll)));

		// Since we are not accumulating the values of Yaw, Pitch and Roll, the existing orientation has to take the new rotation into account.
		//Orientation = glm::normalize(Quaternion * Orientation);

		// If we are accumulating the values of Yaw, Pitch and Roll, we can't multiply the existing orientation with the new quaternion.
		// This is because of the fact that the new Quaternion is a result from the cumulation of those 3 angles.
		Orientation = glm::normalize(Quaternion);

		// Ideally this is what we should be going for but this method introduces new problems mainly,
		// if Roll was rotated, then Yaw and Pitch would be rotated as well. If Roll was rotated 90 degreed,
		// Yaw becomes Pitch and Pitch becomes Yaw.
		// 
		//glm::mat4 Rot = glm::mat4_cast(Quaternion);
		//Forward	= glm::vec3(Rot[0][2], Rot[1][2], Rot[2][2]);
		//Up		= glm::vec3(Rot[0][1], Rot[1][1], Rot[2][1]);
		//Right		= glm::vec3(Rot[0][0], Rot[1][0], Rot[2][0]);

		// The new Forward, Up and Right is calculated from the inverse of the new Orientation.
		// Why inverse? Because the Orientation was already used as the rotation in the view matrix.
		// To understand this, check 1Blue3Brown's explanation of Quaternions.
		// We multiply with the the world's space definition of Forward, Up and Right instead of the view space
		// because doing so would result in the Camera's Forward, Up and Right vector in world space which is what we want.
		// If we have calculated the new orientation of the Forward, Up and Right vector using the Camera's version of those coordinates,
		// then the result would've been a Forward, Up and Right vector in the view space instead.
		//
		Forward = glm::inverse(Orientation) * glm::vec3(0.0f, 0.0f, -1.0f);
		Up		= glm::inverse(Orientation) * glm::vec3(0.0f, 1.0f,  0.0f);
		Right	= glm::inverse(Orientation) * glm::vec3(1.0f, 0.0f,  0.0f);
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
	}


	void BaseCamera::Rotate(const glm::vec3& Angle, float DeltaTime)
	{
		float Horizontal = Angle.x * Sensitivity * DeltaTime;
		float Vertical = Angle.y * Sensitivity * DeltaTime;
		float ZRotation = Angle.z * Sensitivity * DeltaTime;

		//Yaw = glm::radians(Yaw + Horizontal);
		//Pitch = glm::radians(Pitch + Vertical);

		// Old code. Sort of works but not really. The rotation axis would be warped along the dimensions.
		// Meaning moving the mouse around x would change the pitch instead of the yaw and etc.
		//
		Yaw		+= Horizontal;
		Pitch	+= Vertical;
		Roll	+= ZRotation;

		//Yaw		= Horizontal;
		//Pitch	= Vertical;
		//Roll	= ZRotation;

		UpdateOrientation();

		// No reset previously.
		//Yaw = Pitch = Roll = 0;
	}


	void BaseCamera::Zoom(float Offset)
	{
		if (FieldOfView >= 1.0f && FieldOfView <= 90.0f) 
		{
			FieldOfView -= Offset;
		}

		if (FieldOfView < 1.0f) 
		{
			FieldOfView = 1.0f;
		}

		if (FieldOfView > 90.0f) 
		{
			FieldOfView = 90.0f;
		}
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
		Up		= glm::vec3(0.0f, 1.0f, 0.0f);
		Right	= glm::vec3(1.0f, 0.0f, 0.0f);
	}


	void BaseCamera::Tick(float DeltaTime)
	{
		UpdateProjectionMatrix();
		UpdateViewMatrix();
	}


	void BaseCamera::SetCameraProjection(uint32 NewProjection)
	{
		if (Mode != NewProjection) {
			Mode = NewProjection;
		}
	}

}