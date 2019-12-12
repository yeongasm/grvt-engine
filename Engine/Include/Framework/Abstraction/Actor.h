#pragma once

#ifndef GRAVITY_ABSTRACTION_ACTOR
#define GRAVITY_ABSTRACTION_ACTOR

#include "Model.h"
#include "Material.h"
#include "Renderer/RenderCache.h"

namespace Grvt
{
	struct ActorCreationInfo
	{
		Gfl::String		Identifier;
		RenderState		DrawingState;
		glm::vec3		Position;
		GrvtMaterial*	SrcMaterial;
		GrvtModel*		SrcModel;
		bool			Instanced;
	};


	struct GrvtActor
	{
		Gfl::String		Name;
		RenderState		DrawingState;
		glm::vec3		Position;
		GrvtMaterial*	MaterialPtr;
		GrvtModel*		ModelPtr;
		bool			Instanced;

		GrvtActor();
		~GrvtActor();
		
		//GrvtActor(const Gfl::String& Name, GrvtModel* SrcModel, GrvtMaterial* SrcMaterial, const glm::vec3& DefaultPosition);
		//GrvtActor(const Gfl::String& Name, GrvtModel* SrcModel, GrvtMaterial* SrcMaterial, RenderState State, const glm::vec3& DefaultPosition);

		GrvtActor(const GrvtActor& Other);
		GrvtActor& operator= (const GrvtActor& Other);

		GrvtActor(GrvtActor&& Other);
		GrvtActor& operator= (GrvtActor&& Other);

		void Alloc(const ActorCreationInfo& Info);
		void Free();
	};

}

#endif // !GRAVITY_ABSTRACTION_ACTOR