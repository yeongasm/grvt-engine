#include "GrvtPch.h"
#include "Framework/Abstraction/Scene.h"


namespace Grvt
{

	GrvtScene::GrvtScene() :
		LevelName(), Actors(), DirectionalLights(), PointLights() {}


	GrvtScene::~GrvtScene() {}


	GrvtScene::GrvtScene(const GrvtScene& Other)
	{
		*this = Other;
	}


	GrvtScene& GrvtScene::operator= (const GrvtScene& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			LevelName			= Other.LevelName;
			Actors				= Other.Actors;
			DirectionalLights	= Other.DirectionalLights;
			PointLights			= Other.PointLights;
		}

		return *this;
	}


	GrvtScene::GrvtScene(GrvtScene&& Other)
	{
		*this = Gfl::Move(Other);
	}


	GrvtScene& GrvtScene::operator= (GrvtScene&& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			LevelName			= Other.LevelName;
			Actors				= Other.Actors;
			DirectionalLights	= Other.DirectionalLights;
			PointLights			= Other.PointLights;

			new (&Other) GrvtScene();
		}

		return *this;
	}


	GrvtActor& GrvtScene::AddNewActor(const ActorCreationInfo& Info)
	{
		GrvtActor& Actor = Actors.Insert(GrvtActor());
		
		Actor.Alloc(Info);

		return Actor;
	}


	DirLight* GrvtScene::AddNewDirectionalLight(const LightCreationInfo& Info)
	{
		if (DirectionalLights.Length() == MAX_DIRECTIONAL_LIGHTS)
		{
			return nullptr;
		}

		DirLight& Light = DirectionalLights.Insert(DirLight());
		Light.Alloc(Info);

		return &Light;
	}

	
	PointLight* GrvtScene::AddNewPointLight(const LightCreationInfo& Info)
	{
		if (PointLights.Length() == MAX_POINT_LIGHTS)
		{
			return nullptr;
		}
		
		PointLight& Light = PointLights.Insert(PointLight());
		Light.Alloc(Info);

		return &Light;
	}


	GrvtActor* GrvtScene::GetActor(const Gfl::String& Name)
	{
		for (GrvtActor& Actor : Actors)
		{
			if (Actor.Name != Name)
			{
				continue;
			}

			return &Actor;
		}

		return nullptr;
	}


	bool GrvtScene::DeleteActor(GrvtActor& Actor)
	{
		size_t Index = Actors.IndexOf(Actor);

		if (Index == -1)
		{
			return false;
		}

		Actors.PopAt(Index);

		return true;
	}


	bool GrvtScene::DeleteActor(const Gfl::String& Identifier)
	{
		GrvtActor* Actor = GetActor(Identifier);

		if (!Actor)
		{
			return false;
		}

		size_t Index = Actors.IndexOf(*Actor);
		Actors.PopAt(Index);

		return true;
	}


	bool GrvtScene::DeleteDirLight(DirLight& DirectionalLight)
	{
		size_t Index = DirectionalLights.IndexOf(DirectionalLight);

		if (Index == -1)
		{
			return false;
		}

		DirectionalLights.PopAt(Index);

		return true;
	}


	bool GrvtScene::DeletePointLight(PointLight& PointLight)
	{
		size_t Index = PointLights.IndexOf(PointLight);

		if (Index == -1)
		{
			return false;
		}

		PointLights.PopAt(Index);

		return true;
	}


	void GrvtScene::Alloc(const SceneCreationInfo& Info)
	{
		LevelName = Info.Name;
		Actors.Reserve(Info.ActorReserves);
		DirectionalLights.Reserve(Info.DirLightReserves);
		PointLights.Reserve(Info.PointLightReserves);
	}


	void GrvtScene::Free()
	{
		Actors.Release();
		DirectionalLights.Release();
		PointLights.Release();
	}


	void GrvtScene::DeleteAllActors()
	{
		Actors.Empty();
	}

	
	void GrvtScene::DeleteAllDirLights()
	{
		DirectionalLights.Empty();
	}


	void GrvtScene::DeleteAllPointLights()
	{
		PointLights.Empty();
	}


	void GrvtScene::DeleteAllLights()
	{
		DeleteAllDirLights();
		DeleteAllPointLights();
	}

}