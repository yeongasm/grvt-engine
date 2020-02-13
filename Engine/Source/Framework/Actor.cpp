#include "GrvtPch.h"
#include "Framework/Actor.h"


namespace Grvt
{

	GrvtActor::GrvtActor() :
		Name(),
		DrawingState(),
		Position(),
		Rotation(),
		Scale(1.0f),
		Material(), 
		ModelPtr(nullptr),
		Mode(GL_TRIANGLES),
		Instanced(false),
		Render(true),
		Sort(true),
		Shadow(true) {}


	GrvtActor::~GrvtActor() {}


	//GrvtActor::GrvtActor(const Gfl::String& Name, GrvtModel* SrcModel, GrvtMaterial* SrcMaterial, const glm::vec3& DefaultPosition) :
	//	Name(Name), MaterialPtr(SrcMaterial), ModelPtr(SrcModel), Position(DefaultPosition) {}


	//GrvtActor::GrvtActor(const Gfl::String& Name, GrvtModel* SrcModel, GrvtMaterial* SrcMaterial, RenderState State, const glm::vec3& DefaultPosition) :
	//	Name(Name), MaterialPtr(SrcMaterial), ModelPtr(SrcModel), DrawingState(State), Position(DefaultPosition) {}


	GrvtActor::GrvtActor(const GrvtActor& Other)
	{
		*this = Other;
	}


	GrvtActor& GrvtActor::operator= (const GrvtActor& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Name = Other.Name;

			Material = Other.Material;
			ModelPtr = Other.ModelPtr;

			DrawingState = Other.DrawingState;

			Position = Other.Position;
			Rotation = Other.Rotation;
			Scale = Other.Scale;

			Instanced = Other.Instanced;

			Render	= Other.Render;
			Sort	= Other.Sort;
			Shadow	= Other.Shadow;
		}
		
		return *this;
	}


	GrvtActor::GrvtActor(GrvtActor&& Other)
	{
		*this = Gfl::Move(Other);
	}


	GrvtActor& GrvtActor::operator= (GrvtActor&& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Name = Other.Name;

			Material = Other.Material;
			ModelPtr = Other.ModelPtr;

			DrawingState = Other.DrawingState;

			Position = Other.Position;
			Rotation = Other.Rotation;
			Scale	= Other.Scale;

			Instanced = Other.Instanced;

			Render	= Other.Render;
			Sort	= Other.Sort;
			Shadow	= Other.Shadow;

			new (&Other) GrvtActor();
		}

		return *this;
	}


	void GrvtActor::Alloc(const ActorCreationInfo& Info)
	{
		Name			= Info.Identifier;

		// TODO(Afiq):
		// Remove this once everything is figured out.
		if (Info.SrcMaterial)
		{
			Material = *Info.SrcMaterial;
		}

		ModelPtr		= Info.SrcModel;
		DrawingState	= Info.DrawingState;
		Position		= Info.Position;
		Instanced		= Info.Instanced;
	}


	void GrvtActor::Free()
	{
		Name.Release();
		ModelPtr = nullptr;
	}

}