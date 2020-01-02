#include "pch.h"
#include <Include/Core/ModelDefaults.h>
#include <Include/Manager/Manager.h>
#include <Include/Framework/Abstraction/Actor.h>
#include <Include/Framework/Abstraction/Scene.h>
#include <Include/Framework/Abstraction/Shader.h>
#include <Renderer/Renderer.h>
#include <Renderer/DeferredPBR.h>
#include "CameraSystem.h"
#include "ShaderLibrary/ShaderLib.h"

#include "DemoModule.h"
#include "Etc/ModuleFuncs.h"


Grvt::EngineIO*		m_IO		= nullptr;
Grvt::GrvtEngine*	m_Engine	= nullptr;
Grvt::GrvtScene*	DemoScene	= nullptr;
Grvt::DeferredPBR*	m_Renderer	= nullptr;

CameraSystem* CamSystem = nullptr;

extern "C"
{

	void OnStartUp(Grvt::GrvtEngine* EnginePtr)
	{
		m_Renderer = (Grvt::DeferredPBR*)Grvt::InitRenderer(new Grvt::DeferredPBR());

		// Register camera system.
		CamSystem = (CameraSystem*)EnginePtr->RegisterSystem(GRSYSTEMCRED(CameraSystem), new CameraSystem());
		Grvt::ResourceManager* Manager = Grvt::GetResourceManager();
		
		{
			Grvt::SceneCreationInfo Info;
			Info.Name = "DemoLevel";
			DemoScene = Manager->NewScene(Info);

			Grvt::SetActiveScene(DemoScene);
		}

		DemoScene->Camera = &CamSystem->Camera;
	}

	void OnLoad(Grvt::GrvtEngine* EnginePtr)
	{
		m_Engine = EnginePtr;
		m_IO = m_Engine->GetIO();
	}

	void ExecuteApplication()
	{
	
	}

	void OnUnload()
	{
		m_IO = nullptr;
		m_Engine = nullptr;
	}

}
