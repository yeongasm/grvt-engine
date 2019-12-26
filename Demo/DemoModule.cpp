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

		{
			Grvt::ShaderImportInfo Test;
			Test.Name = "TestShader";
			Test.AddShaderToProgram(TestShader::VertexShader,	Grvt::GrvtShader_SourceType_Vertex);
			Test.AddShaderToProgram(TestShader::FragmentShader, Grvt::GrvtShader_SourceType_Fragment);

			Manager->NewShaderProgram(Test);

			Grvt::ShaderImportInfo Floor;

			Floor.Name = "FloorShader";
			Floor.AddShaderToProgram(FloorShader::VertexShader, Grvt::GrvtShader_SourceType_Vertex);
			Floor.AddShaderToProgram(FloorShader::FragmentShader, Grvt::GrvtShader_SourceType_Fragment);

			Manager->NewShaderProgram(Floor);
		}

		DemoScene->Camera = &CamSystem->Camera;
	}

	void OnLoad(Grvt::GrvtEngine* EnginePtr)
	{
		m_Engine = EnginePtr;
		m_IO = m_Engine->GetIO();

		Grvt::GrvtMaterial* SimpleMat = nullptr;
		Grvt::ResourceManager* Manager = Grvt::GetResourceManager();
		DemoScene = Manager->GetScene("DemoLevel");

		{
			if (!Manager->GetMaterial("TestMaterial"))
			{
				Grvt::MaterialCreationInfo Info;
				Info.Name = "TestMaterial";
				Info.Shader = Manager->GetShader("TestShader");

				SimpleMat = Manager->NewMaterial(Info);

				Info.Name = "FloorMaterial";
				Info.Shader = Manager->GetShader("FloorShader");

				Manager->NewMaterial(Info);
			}
		}

		{
			if (!DemoScene->GetActor("SphereActor"))
			{
				Grvt::ActorCreationInfo Info;
				Info.Identifier = "SphereActorMiddle";
				Info.SrcModel = Manager->GetModel("Sphere");
				Info.SrcMaterial = SimpleMat;

				DemoScene->AddNewActor(Info);

				Info.Identifier = "SphereActorUp";
				DemoScene->AddNewActor(Info);

				Info.Identifier = "SphereActorRight";
				DemoScene->AddNewActor(Info);

				Info.Identifier = "SphereActorForward";
				DemoScene->AddNewActor(Info);
			}
		}
	}

	void ExecuteApplication()
	{
		// Render the floor grid.
		RenderFloorGrid();

		Grvt::GrvtActor* Mid = DemoScene->GetActor("SphereActorMiddle");
		Grvt::GrvtActor* X = DemoScene->GetActor("SphereActorRight");
		Grvt::GrvtActor* Y = DemoScene->GetActor("SphereActorUp");
		Grvt::GrvtActor* Z = DemoScene->GetActor("SphereActorForward");

		Mid->Material.SetVector("Colour", glm::vec3(1.0f, 1.0f, 1.0f));
		X->Material.SetVector("Colour", glm::vec3(1.0f, 0.0f, 0.0f));
		Y->Material.SetVector("Colour", glm::vec3(0.0f, 1.0f, 0.0f));
		Z->Material.SetVector("Colour", glm::vec3(0.0f, 0.0f, 1.0f));

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
