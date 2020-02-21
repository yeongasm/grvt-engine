#pragma once

#ifndef GRAVITY_ABSTRACTION_LIGHTING
#define GRAVITY_ABSTRACTION_LIGHTING

#include "Minimal.h"
#include "Renderer/RenderFoundation.h"

namespace Grvt
{

	/**
	* Revamp Status: Completed?
	*/


	/**
	* Types of light.
	* To be used upon creating a light light object onto the world.
	*/
	enum LightType : uint32
	{
		GrvtLight_Type_None			= 0xFF,
		GrvtLight_Type_Directional	= 0x00,
		GrvtLight_Type_Pointlight	= 0x01
	};


	/**
	*/
	struct LightCreationInfo
	{
		glm::vec3	Orientation = glm::vec3(0.0f);
		glm::vec3	Position	= glm::vec3(0.0f);
		glm::vec3	Colour		= glm::vec3(1.0f, 1.0f, 1.0f);
		LightType	Type		= GrvtLight_Type_Directional;
		float		Brightness	= 1.0f;
		float		Constant	= 1.0f;
		float		Linear		= 0.0f;
		float		Quadratic	= 0.0f;
		bool		Shadows		= true;
	};

	/**
	* The following is how light data is stored in a 4x4 Matrix.
	*
	* [0][0] - Brightness.
	* [0][1] - Constant.
	* [0][2] - Linear.
	* [0][3] - Quadratic.
	* [1][0] - Position / Orientation->x.
	* [1][1] - Position / Orientation->y.
	* [1][2] - Position / Orientation->z.
	* [1][3] - PCF.
	* [2][0] - Colour->r.
	* [2][1] - Colour->g.
	* [2][2] - Colour->b.
	* [2][3] - Empty.
	* [3][0] - Empty.
	* [3][1] - Empty.
	* [3][2] - Shadow Near.
	* [3][3] - Shadow Far.
	*/

	/**
	*/
	struct LightSource
	{
		RenderTarget*	DepthMap;
		glm::vec3		Orientation;
		glm::vec3		Position;
		glm::vec3		Colour;
		LightType		Type;
		float32			Brightness;
		float32			Bias;
		float32			ShadowNear;
		float32			ShadowFar;
		bool			Enable;
		bool			Shadows;

		ENGINE_API LightSource();
		ENGINE_API virtual ~LightSource();

		LightSource(const LightSource& Other)				= delete;
		LightSource& operator= (const LightSource& Other)	= delete;

		LightSource(LightSource&& Other);
		LightSource& operator= (LightSource&& Other);

		virtual void	Alloc		(const LightCreationInfo& Info);
		virtual void	Free		();

		virtual void	Compute		(glm::mat4& Buffer);
	};


	/**
	* Directional light.
	*/
	struct DirLight : public LightSource
	{

	};


	/**
	* Point light.
	*/
	struct PointLight : public LightSource
	{
		float	Constant;
		float	Linear;
		float	Quadratic;

		ENGINE_API PointLight();
		ENGINE_API ~PointLight();

		PointLight(const PointLight& Other)				= delete;
		PointLight& operator= (const PointLight& Other)	= delete;

		PointLight(PointLight&& Other);
		PointLight& operator= (PointLight&& Other);

		void	Alloc	(const LightCreationInfo& Info) override;
		void	Free	() override;

		void	Compute	(glm::mat4& Buffer) override;

		/**
		* The algorithm compares the radius with each key value in the entry table.
		* It then takes the shortest one from the specified Radius and uses the values in there instead.
		*/
		ENGINE_API void	UpdateByRadius(float32 Radius);
	};

}

#endif // !GRAVITY_ABSTRACTION_LIGHTING