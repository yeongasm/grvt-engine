#pragma once
#ifndef GRAVITY_ABSTRACTION_LIGHTING
#define GRAVITY_ABSTRACTION_LIGHTING

#include "Minimal.h"
#include "Framework/Foundation/Foundations.h"

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

		glm::vec3	Position;
		glm::vec3	Colour;
		LightType	Type;
		float		Brightness;
		float		Constant;
		float		Linear;
		float		Quadratic;
		float		Radius;

		LightCreationInfo();
		~LightCreationInfo();

	};


	enum ShadowMapType : uint32
	{
		GrvtShadowMap_None				= 0xFF, /** On first init only */
		GrvtShadowMap_Directional		= 0x00,
		GrvtShadowMap_OmniDirectional	= 0x01
	};


	class ShadowMap
	{
	public:

		ObjHandle		Handle;
		ObjHandle		DepthTexture;
		ShadowMapType	Type;
		float			Bias;

		ShadowMap();
		~ShadowMap();

	private:

		ShadowMap(const ShadowMap&) = delete;
		ShadowMap& operator= (const ShadowMap&) = delete;

	};


	/**
	*/
	class LightSource
	{
	public:

		ShadowMap	Shadow;
		glm::vec3	Position;
		glm::vec3	Colour;
		LightType	Type;
		float		Brightness;
		bool		Enable;

		LightSource();
		virtual ~LightSource();

	private:

		LightSource(const LightSource&) = delete;
		LightSource& operator= (const LightSource&) = delete;

		LightSource(LightSource&&) = delete;
		LightSource& operator= (LightSource&&) = delete;

	public:

		virtual void	Alloc(const LightCreationInfo& Info);
		virtual void	Free();
		virtual void	Compute(glm::mat4& Buffer);

	};


	/**
	* Directional light data structure.
	*
	* We could easily just make the base Light object a directional light but it wouldn't seem proper in programming terms.
	* Compute method computes information required for directional light.
	*/
	class DirLight : public LightSource
	{
	public:

		glm::mat4 LightSpaceTransform;

		DirLight();
		~DirLight();

	private:

		DirLight(const DirLight&) = delete;
		DirLight& operator= (const DirLight&) = delete;

		DirLight(DirLight&&) = delete;
		DirLight& operator= (DirLight&&) = delete;

	public:

		void	Alloc(const LightCreationInfo& Info);
		void	Free();
		void	Compute(glm::mat4& Buffer);

	};


	/**
	* Point Light data structure.
	* Compute() method is overridden in this structure and is used to pass in data for Point Light calculations.
	*
	* TODO(Afiq):
	* Add framebuffers to enable shadow mapping and include it as part of the structure.
	* Add a function to generate debug sphere in wireframe mode.
	*/
	class PointLight : public LightSource
	{
	public:

		Gfl::Array<glm::mat4> LightSpaceTransforms;

		float	Constant;
		float	Linear;
		float	Quadratic;
		float	Radius;
		bool	Simplified;

		PointLight();
		~PointLight();

	private:

		PointLight(const PointLight&) = delete;
		PointLight& operator= (const PointLight&) = delete;

		PointLight(PointLight&&) = delete;
		PointLight& operator= (PointLight&&) = delete;

	public:

		void	Alloc(const LightCreationInfo& Info) override;
		void	Free() override;
		void	UpdateRadius(bool Simplify, float Value);
		void	Compute(glm::mat4& Buffer) override;

	};

}

#endif // !GRAVITY_ABSTRACTION_LIGHTING