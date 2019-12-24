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

		{
			Grvt::ShaderImportInfo Info;
			Info.Name = "TestShader";
			Info.AddShaderToProgram(TestShader::VertexShader,	Grvt::GrvtShader_SourceType_Vertex);
			Info.AddShaderToProgram(TestShader::FragmentShader, Grvt::GrvtShader_SourceType_Fragment);

			Manager->NewShaderProgram(Info);
		}

		{
			Grvt::ActorCreationInfo Info;
			Info.Identifier	= "SphereActorMiddle";
			Info.SrcModel	= Manager->GetModel("Sphere");

			DemoScene->AddNewActor(Info);

			Info.Identifier = "SphereActorUp";

			DemoScene->AddNewActor(Info);

			Info.Identifier = "SphereActorRight";

			DemoScene->AddNewActor(Info);

			Info.Identifier = "SphereActorForward";

			DemoScene->AddNewActor(Info);
		}

		DemoScene->Camera = &CamSystem->Camera;
	}

	void OnLoad(Grvt::GrvtEngine* EnginePtr)
	{
		m_Engine = EnginePtr;
		m_IO = m_Engine->GetIO();

		for (size_t i = 0; i < 5; i++)
		{
			m_IO->MouseHoldDuration[i] = 0.0f;
		}

		Grvt::ResourceManager* Manager = Grvt::GetResourceManager();
		DemoScene = Manager->GetScene("DemoLevel");

		Grvt::GrvtMaterial* TestMaterial = nullptr;

		{
			Grvt::MaterialCreationInfo Info;
			Info.Name	= "TestMaterial";
			Info.Shader = Manager->GetShader("TestShader");

			TestMaterial = Manager->NewMaterial(Info);
			// TODO(Afiq):
			// The Material in the actors, cannot be pointers.
			// They have to be by value.
			DemoScene->GetActor("SphereActorMiddle")->MaterialPtr = *TestMaterial;
			DemoScene->GetActor("SphereActorUp")->MaterialPtr = *TestMaterial;
			DemoScene->GetActor("SphereActorRight")->MaterialPtr = *TestMaterial;
			DemoScene->GetActor("SphereActorForward")->MaterialPtr = *TestMaterial;
		}
	}

	void ExecuteApplication()
	{	
		Grvt::GrvtActor* Mid = DemoScene->GetActor("SphereActorMiddle");
		Grvt::GrvtActor* X = DemoScene->GetActor("SphereActorRight");
		Grvt::GrvtActor* Y = DemoScene->GetActor("SphereActorUp");
		Grvt::GrvtActor* Z = DemoScene->GetActor("SphereActorForward");


		if (m_IO->IsKeyPressed(GLFW_KEY_F1))
		{
			printf("I came in here!");
			CameraSystem* CamSys = dynamic_cast<CameraSystem*>(m_Engine->GetSystem(GRSYSTEMCRED(CameraSystem)));
			CamSys->IsActive ^= true;
			CamSys->FirstMouse = true;
		}

		Mid->MaterialPtr.SetVector("Colour", glm::vec3(1.0f, 1.0f, 1.0f));
		X->MaterialPtr.SetVector("Colour", glm::vec3(1.0f, 0.0f, 0.0f));
		Y->MaterialPtr.SetVector("Colour", glm::vec3(0.0f, 1.0f, 0.0f));
		Z->MaterialPtr.SetVector("Colour", glm::vec3(0.0f, 0.0f, 1.0f));

		Mid->Position = glm::vec3(0.0f, 0.0f, 0.0f);
		X->Position = glm::vec3(10.0f, 0.0f, 0.0f);
		Y->Position = glm::vec3(0.0f, 10.0f, 0.0f);
		Z->Position = glm::vec3(0.0f, 0.0f, 10.0f);
	}

	void OnUnload()
	{
		m_IO = nullptr;
		m_Engine = nullptr;
	}

}
