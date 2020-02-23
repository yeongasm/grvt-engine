#include "GrvtPch.h"
#include "Framework/Lighting.h"


namespace Grvt
{

	LightSource::LightSource() :
		Orientation(0.0f),
		Position(0.0f),
		Colour(0.0f),
		Type(LightType::GrvtLight_Type_None),
		RenderTargetIdx(-1),
		Brightness(0.0f),
		Bias(0.005f),
		ShadowNear(0.0f),
		ShadowFar(0.0f),
		Enable(true),
		Shadows(true) {}


	LightSource::~LightSource() {}


	/*LightSource::LightSource(const LightSource& Other)
	{
		*this = Other;
	}


	LightSource& LightSource::operator= (const LightSource& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			ShadowMap	= Other.ShadowMap;
			Position	= Other.Position;
			Colour		= Other.Colour;
			Type		= Other.Type;
			Brightness	= Other.Brightness;
			Enable		= Other.Enable;
		}

		return *this;
	}*/


	LightSource::LightSource(LightSource&& Other)
	{
		*this = Gfl::Move(Other);
	}


	LightSource& LightSource::operator= (LightSource&& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Orientation = Gfl::Move(Other.Orientation);
			Position	= Gfl::Move(Other.Position);
			Colour		= Gfl::Move(Other.Colour);
			Type		= Gfl::Move(Other.Type);
			Brightness	= Gfl::Move(Other.Brightness);
			Bias		= Gfl::Move(Other.Bias);
			Enable		= Gfl::Move(Other.Enable);
			Shadows		= Gfl::Move(Other.Shadows);

			new (&Other) LightSource();
		}

		return *this;
	}


	void LightSource::Alloc(const LightCreationInfo& Info) 
	{
		Orientation = Info.Orientation;
		Position	= Info.Position;
		Colour		= Info.Colour;
		Type		= Info.Type;
		Brightness	= Info.Brightness;
		Shadows		= Info.Shadows;
	}


	void LightSource::Free() 
	{
		Brightness = 0.0f;
		Type = GrvtLight_Type_None;
		Orientation = glm::vec3(0.0f),
		Position	= glm::vec3(0.0f);
		Colour		= glm::vec3(0.0f);
	}


	void LightSource::Compute(glm::mat4& Buffer) 
	{
		Buffer[0][0] = Brightness;
		Buffer[0][1] = 1.0f;
		Buffer[1][0] = Orientation.x;
		Buffer[1][1] = Orientation.y;
		Buffer[1][2] = Orientation.z;
		Buffer[1][3] = Bias;
		Buffer[2][0] = Colour.x;
		Buffer[2][1] = Colour.y;
		Buffer[2][2] = Colour.z;
		Buffer[3][2] = ShadowNear;
		Buffer[3][3] = ShadowFar;
	}


	PointLight::PointLight() :
		LightSource(), 
		Constant(1.0f), 
		Linear(0.0f), 
		Quadratic(0.0f) {}


	PointLight::~PointLight() {}


	/*PointLight::PointLight(const PointLight &Other) 
	{ 
		*this = Other; 
	}

	PointLight& PointLight::operator= (const PointLight &Other) 
	{
		// Ensures that the object is not being assigned to itself. Only in debug.
		_ASSERTE(this != &Other);

		if (this != &Other) {
			Constant	= Other.Constant;
			Linear		= Other.Linear;
			Quadratic	= Other.Quadratic;
			
			LightSource::operator=(Other);
		}

		return *this;
	}*/


	PointLight::PointLight(PointLight &&Other) 
	{ 
		*this = Gfl::Move(Other); 
	}


	PointLight& PointLight::operator= (PointLight &&Other) 
	{
		// Ensures that the object is not being assigned to itself. Only in debug.
		_ASSERTE(this != &Other);
	
		if (this != &Other) {
			Constant	= Other.Constant;
			Linear		= Other.Linear;
			Quadratic	= Other.Quadratic;
			
			LightSource::operator=(Gfl::Move(Other));
	
			new (&Other) PointLight();
		}
	
		return *this;
	}


	void PointLight::Alloc(const LightCreationInfo& Info) 
	{
		LightSource::Alloc(Info);

		Constant	= Info.Constant;
		Linear		= Info.Linear;
		Quadratic	= Info.Quadratic;
	}


	void PointLight::Free() 
	{
		Constant	= 0.0f;
		Linear		= 0.0f;
		Quadratic	= 0.0f;

		LightSource::Free();
	}


	void PointLight::UpdateByRadius(float32 Radius) 
	{
		Linear		= 2.0f / Radius;
		Quadratic	= 1.0f / (Radius * Radius);
	}


	void PointLight::Compute(glm::mat4& Buffer) 
	{
		LightSource::Compute(Buffer);

		Buffer[0][1] = Constant;
		Buffer[0][2] = Linear;
		Buffer[0][3] = Quadratic;

		Buffer[1][0] = Position.x;
		Buffer[1][1] = Position.y;
		Buffer[1][2] = Position.z;
	}

}