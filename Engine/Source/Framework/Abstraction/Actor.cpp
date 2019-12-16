#include "GrvtPch.h"
#include "Framework/Abstraction/Actor.h"


namespace Grvt
{

	GrvtActor::GrvtActor() :
		Name(),
		DrawingState(),
		Position(),
		Rotation(),
		Scale(1.0f),
		MaterialPtr(nullptr), 
		ModelPtr(nullptr),
		Mode(GL_TRIANGLES),
		Instanced(false),
		Render(true) {}


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
			MaterialPtr = Other.MaterialPtr;
			ModelPtr = Other.ModelPtr;
			DrawingState = Other.DrawingState;
			Position = Other.Position;
			Rotation = Other.Rotation;
			Scale = Other.Scale;
			Instanced = Other.Instanced;
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
			MaterialPtr = Other.MaterialPtr;
			ModelPtr = Other.ModelPtr;
			DrawingState = Other.DrawingState;
			Position = Other.Position;
			Rotation = Other.Rotation;
			Scale = Other.Scale;
			Instanced = Other.Instanced;

			new (&Other) GrvtActor();
		}

		return *this;
	}


	void GrvtActor::Alloc(const ActorCreationInfo& Info)
	{
		MaterialPtr		= Info.SrcMaterial;
		ModelPtr		= Info.SrcModel;
		DrawingState	= Info.DrawingState;
		Position		= Info.Position;
		Instanced		= Info.Instanced;
	}


	void GrvtActor::Free()
	{
		Name.Release();
		ModelPtr = nullptr;
		MaterialPtr = nullptr;
	}

}