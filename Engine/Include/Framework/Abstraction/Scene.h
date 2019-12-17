#pragma once

#ifndef GRAVITY_ABSTRACTION_SCENE
#define GRAVITY_ABSTRACTION_SCENE

#include "Minimal.h"
#include "Actor.h"
#include "Lighting.h"
#include "Renderer/RenderFoundation.h"

namespace Grvt
{

#define MAX_DIRECTIONAL_LIGHTS	8
#define MAX_POINT_LIGHTS		1024


	struct SceneCreationInfo
	{
		Gfl::String Name;
		size_t		ActorReserves		= 32;
		size_t		DirLightReserves	= 4;
		size_t		PointLightReserves	= 128;
	};


	/**
	* A GrvtScene represents a scene that will be rendered onto the screen.
	* Maximum number of directional lights allowed is 8.
	* Maximum number of point lights allowed is 1024.
	* Unlimited number of actors are allowed until memory runs out.
	*/
	class GrvtScene
	{
	public:

		Gfl::String	Name;

	private:

		friend class	ResourceManager;
		friend struct	BaseRenderer;
		friend class	DeferredPBR;
		friend ENGINE_API void	ExecuteEngine();

		Gfl::Array<GrvtActor>	Actors;
		Gfl::Array<DirLight>	DirectionalLights;
		Gfl::Array<PointLight>	PointLights;

		void	Alloc(const SceneCreationInfo& Info);
		void	Free();

		void	CreateSceneCommandBuffer(CommandBuffer& Buffer);

	public:

		GrvtScene();
		~GrvtScene();

		GrvtScene(const GrvtScene& Other);
		GrvtScene& operator= (const GrvtScene& Other);

		GrvtScene(GrvtScene&& Other);
		GrvtScene& operator= (GrvtScene&& Other);

		/**
		* Adds a new actor into the scene.
		*/
		ENGINE_API GrvtActor&	AddNewActor				(const ActorCreationInfo& Info);

		/**
		* Adds a new directional light into the scene.
		*/
		ENGINE_API DirLight*	AddNewDirectionalLight	(const LightCreationInfo& Info);
		
		/**
		* Adds a new point light into the scene.
		*/
		ENGINE_API PointLight* AddNewPointLight		(const LightCreationInfo& Info);

		/**
		* Retrieves an actor with the specified identifier from the scene.
		*/
		ENGINE_API GrvtActor*	GetActor				(const Gfl::String& Identifier);

		/**
		* Deletes an actor by reference from the scene.
		* Always prioritise this method of deleting by specifying an identifier.
		*/
		ENGINE_API bool		DeleteActor				(GrvtActor& Actor);

		/**
		* Deletes an actor by identifier from the scene.
		* Always prioritise deleting by reference over this method.
		*/
		ENGINE_API bool		DeleteActor				(const Gfl::String& Identifier);

		/**
		* Deletes a directional light by reference from the scene.
		*/
		ENGINE_API bool		DeleteDirLight			(DirLight& DirectionalLight);

		/**
		* Deletes a point light by reference from the scene.
		*/
		ENGINE_API bool		DeletePointLight		(PointLight& PointLight);

		/**
		* Clears all actors from the scene.
		* Does not release memory that was once allocated for it.
		*/
		ENGINE_API void		DeleteAllActors			();

		/**
		* Clears all directional lights from the scene.
		* Does not release memory that was once allocated for it.
		*/
		ENGINE_API void		DeleteAllDirLights		();

		/**
		* Clears all point lights from the scene.
		* Does not release memory that was once allocated for it.
		*/
		ENGINE_API void		DeleteAllPointLights	();

		/**
		* Clears all lights from the scene.
		* Does not release memory that was once allocated for them.
		*/
		ENGINE_API void		DeleteAllLights			();
	};

	/**
	* Sets an active scene to be rendered.
	* The current implementation in Gravity is that it only renders whatever scene that is set as the global active scene.
	* Because there always needs to be an active scene, a method to unset the active scene will not be provided.
	*/
	ENGINE_API void SetActiveScene(GrvtScene* ActiveScene);

	/**
	* Gets the currently global active scene.
	*/
	ENGINE_API GrvtScene* GetActiveScene();

}

#endif // !GRAVITY_ABSTRACTION_SCENE