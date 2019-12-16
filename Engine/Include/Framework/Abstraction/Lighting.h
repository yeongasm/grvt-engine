#pragma once

#ifndef GRAVITY_ABSTRACTION_LIGHTING
#define GRAVITY_ABSTRACTION_LIGHTING

#include "Minimal.h"
#include "Framebuffer.h"

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
		glm::vec3	Position	= glm::vec3(0.0f, 0.0f, 0.0f);
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
	* [1][0] - Position->x.
	* [1][1] - Position->y.
	* [1][2] - Position->z.
	* [1][3] - Empty.
	* [2][0] - Colour->r.
	* [2][1] - Colour->g.
	* [2][2] - Colour->b.
	* [2][3] - Brightness.
	* [3][0] - 1.0f if Directional light else 0.0f.
	* [3][1] - 1.0f if Point light else 0.0f.
	* [3][2] - Empty.
	* [3][3] - Empty.
	*/

	/**
	*/
	struct LightSource
	{
		GrvtFramebuffer* ShadowMap;
		glm::vec3		 Position;
		glm::vec3		 Colour;
		LightType		 Type;
		float			 Brightness;
		bool			 Enable;

		LightSource();
		virtual ~LightSource();

		LightSource(const LightSource& Other);
		LightSource& operator= (const LightSource& Other);

		LightSource(LightSource&& Other);
		LightSource& operator= (LightSource&& Other);

		virtual void	Alloc	(const LightCreationInfo& Info);
		virtual void	Free	();

		virtual void	Compute	(glm::mat4& Buffer);
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

		PointLight();
		~PointLight();

		PointLight(const PointLight& Other);
		PointLight& operator= (const PointLight& Other);

		PointLight(PointLight&& Other);
		PointLight& operator= (PointLight&& Other);

		void	Alloc	(const LightCreationInfo& Info) override;
		void	Free	() override;

		void	Compute	(glm::mat4& Buffer) override;

		/**
		* The algorithm compares the radius with each key value in the entry table.
		* It then takes the shortest one from the specified Radius and uses the values in there instead.
		*/
		void	UpdateByRadius(float32 Radius);
	};

}

#endif // !GRAVITY_ABSTRACTION_LIGHTING