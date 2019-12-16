#include "pch.h"
#include <Include/Core/ModelDefaults.h>
#include <Include/Manager/Manager.h>
#include <Include/Framework/Abstraction/Actor.h>
#include <Renderer/Renderer.h>
#include <Renderer/DeferredPBR.h>

#include "DemoModule.h"


Grvt::EngineIO*		m_IO		= nullptr;
Grvt::GrvtEngine*	m_Engine	= nullptr;
Grvt::GrvtScene*	DemoScene	= nullptr;
Grvt::DeferredPBR*	m_Renderer	= nullptr;

extern "C"
{

	void OnStartUp(Grvt::GrvtEngine* EnginePtr)
	{
		m_Renderer = (Grvt::DeferredPBR*)Grvt::InitRenderer(new Grvt::DeferredPBR());

		Grvt::ResourceManager* Manager = Grvt::GetResourceManager();

		{
			Grvt::SceneCreationInfo Info;
			Info.Name = "Demo Level";
			DemoScene = Manager->NewScene(Info);
		}

		Grvt::ActorCreationInfo Info;
		Info.Identifier = "Sphere Actor";
		Info.SrcModel = Manager->GetModel("Sphere");

		DemoScene->AddNewActor(Info);

		Info.Identifier = "Cube Actor";
		Info.SrcModel = Manager->GetModel("Cube");

		DemoScene->AddNewActor(Info);

		Info.Identifier = "Floor";
		Info.SrcModel = Manager->GetModel("Plane");

		DemoScene->AddNewActor(Info);
	}

	void OnLoad(Grvt::GrvtEngine* EnginePtr)
	{
		m_Engine = EnginePtr;
		m_IO = m_Engine->GetIO();
		DemoScene = Grvt::GetResourceManager()->GetScene("Demo Level");
	}

	void ExecuteApplication()
	{
		/**
		* Transform the actors here.
		*/

		Grvt::CommandBuffer Buffer;
		DemoScene->CreateSceneCommandBuffer(Buffer);

		/**
		* TODO(Afiq):
		* Allow a mechanism to push command buffer into the renderer.
		* This needs to be done every frame.
		*/
	}

	void OnUnload()
	{
		m_IO = nullptr;
		m_Engine = nullptr;
	}

}
