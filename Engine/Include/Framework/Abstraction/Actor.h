#pragma once

#ifndef GRAVITY_ABSTRACTION_ACTOR
#define GRAVITY_ABSTRACTION_ACTOR

#include "Model.h"
#include "Material.h"
#include "Include/Renderer/RenderCache.h"

namespace Grvt
{
	struct ActorCreationInfo
	{
		Gfl::String		Identifier;
		glm::vec3		Position;
		RenderState		DrawingState;
		GrvtMaterial*	SrcMaterial = nullptr;
		GrvtModel*		SrcModel	= nullptr;
		uint32			Mode		= GL_TRIANGLES;
		bool			Instanced	= false;
	};


	struct GrvtActor
	{
		Gfl::String		Name;
		RenderState		DrawingState;
		glm::vec3		Position;
		glm::vec3		Rotation;
		glm::vec3		Scale;
		GrvtMaterial	Material;
		GrvtModel*		ModelPtr;
		uint32			Mode;

		bool			Instanced;
		bool			Render;
		bool			Sort;
		bool			Shadow;

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