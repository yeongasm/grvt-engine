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
			Info.Identifier		= "SphereActor";
			Info.SrcModel		= Manager->GetModel("Sphere");

			DemoScene->AddNewActor(Info);

			Info.Identifier = "PlaneActor";
			Info.SrcModel	= Manager->GetModel("Plane");

			DemoScene->AddNewActor(Info);
		}

		DemoScene->Camera = &CamSystem->Camera;
	}

	void OnLoad(Grvt::GrvtEngine* EnginePtr)
	{
		m_Engine = EnginePtr;
		m_IO = m_Engine->GetIO();

		Grvt::ResourceManager* Manager = Grvt::GetResourceManager();
		DemoScene = Manager->GetScene("DemoLevel");

		Grvt::GrvtMaterial* TestMaterial = nullptr;

		{
			Grvt::MaterialCreationInfo Info;
			Info.Name	= "TestMaterial";
			Info.Shader = Manager->GetShader("TestShader");

			TestMaterial = Manager->NewMaterial(Info);
			DemoScene->GetActor("SphereActor")->MaterialPtr = TestMaterial;
			DemoScene->GetActor("PlaneActor")->MaterialPtr = TestMaterial;
		}
	}

	void ExecuteApplication()
	{
		static bool init = true;
		static uint32 count = 0;

		// TODO(Afiq):
		// There needs to be a way to streamline this.
		// Updating the scene's camera pointer on every frame would be a hassle.

		Grvt::GrvtActor* Sphere = DemoScene->GetActor("SphereActor");
		//Grvt::GrvtActor* Plane = DemoScene->GetActor("PlaneActor");

		if (init)
		{
			printf("Actor position x:%.3f y:%.3f z:%.3f\n", Sphere->Position.x, Sphere->Position.y, Sphere->Position.z);
			printf("Camera position x:%.3f y:%.3f z:%.3f\n", DemoScene->Camera->Position.x, DemoScene->Camera->Position.y, DemoScene->Camera->Position.z);
			init = false;
		}
		
		count++;

		//printf("Yaw: %.3f\n", DemoScene->Camera->Yaw);
		//printf("Pitch: %.3f\n\n", DemoScene->Camera->Pitch);

		if (count == 128)
		{
			//printf("Yaw: %.3f\n", DemoScene->Camera->Yaw);
			//printf("Pitch: %.3f\n", DemoScene->Camera->Pitch);
			//printf("Camera position x:%.3f y:%.3f z:%.3f\n", DemoScene->Camera->Forward.x, DemoScene->Camera->Forward.y, DemoScene->Camera->Forward.z);
			//printf("Camera position x:%.3f y:%.3f z:%.3f\n", DemoScene->Camera->Right.x, DemoScene->Camera->Right.y, DemoScene->Camera->Right.z);
			//printf("Camera position x:%.3f y:%.3f z:%.3f\n", DemoScene->Camera->Up.x, DemoScene->Camera->Up.y, DemoScene->Camera->Up.z);

			count = 0;
		}

	}

	void OnUnload()
	{
		m_IO = nullptr;
		m_Engine = nullptr;
	}

}
