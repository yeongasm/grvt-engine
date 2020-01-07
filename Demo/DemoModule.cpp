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

glm::vec3 StartPos = glm::vec3(0.0f, 20.0f, 50.0f);
glm::vec3 EndPos = glm::vec3(50.0f, 20.0f, 0.0f);
float Time = 0.0f;

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
			Grvt::ShaderImportInfo ShaderInfo;
			ShaderInfo.Name = "CubeMapShader";
			ShaderInfo.AddShaderToProgram(SkyboxShader::VertexShader, Grvt::GrvtShader_SourceType_Vertex);
			ShaderInfo.AddShaderToProgram(SkyboxShader::FragmentShader, Grvt::GrvtShader_SourceType_Fragment);

			Manager->NewShaderProgram(ShaderInfo);
		}

		{
			Grvt::ShaderImportInfo ShaderInfo;
			ShaderInfo.Name = "FloorShader";
			ShaderInfo.AddShaderToProgram(FloorShader::VertexShader, Grvt::GrvtShader_SourceType_Vertex);
			ShaderInfo.AddShaderToProgram(FloorShader::FragmentShader, Grvt::GrvtShader_SourceType_Fragment);

			Manager->NewShaderProgram(ShaderInfo);
		}

		{
			Grvt::ShaderImportInfo ShaderInfo;
			ShaderInfo.Name = "BasicColour";
			ShaderInfo.AddShaderToProgram(TestColourShader::VertexShader, Grvt::GrvtShader_SourceType_Vertex);
			ShaderInfo.AddShaderToProgram(TestColourShader::FragmentShader, Grvt::GrvtShader_SourceType_Fragment);

			Manager->NewShaderProgram(ShaderInfo);
		}


		{
			Grvt::TextureImportInfo CubemapInfo;
			CubemapInfo.Name = "DemoCubeMap";
			CubemapInfo.Type = Grvt::GrvtTexture_Type_Cubemap;
			CubemapInfo.Path.Push("Data\\Skybox\\LearnOpenGL\\right.jpg");
			CubemapInfo.Path.Push("Data\\Skybox\\LearnOpenGL\\left.jpg");
			CubemapInfo.Path.Push("Data\\Skybox\\LearnOpenGL\\top.jpg");
			CubemapInfo.Path.Push("Data\\Skybox\\LearnOpenGL\\bottom.jpg");
			CubemapInfo.Path.Push("Data\\Skybox\\LearnOpenGL\\front.jpg");
			CubemapInfo.Path.Push("Data\\Skybox\\LearnOpenGL\\back.jpg");
			
			Manager->NewImportCubemap(CubemapInfo);
		}

		{
			Grvt::TextureImportInfo Info;
			Info.Name = "FloorTileTexture";
			Info.Type = Grvt::GrvtTexture_Type_Albedo;
			Info.Path.Push("Data\\Texture\\FloorTile.png");

			Manager->NewImportTexture(Info);
		}

		DemoScene->Camera = &CamSystem->Camera;
	}

	void OnLoad(Grvt::GrvtEngine* EnginePtr)
	{
		m_Engine = EnginePtr;
		m_IO = m_Engine->GetIO();

		DemoScene = Grvt::GetResourceManager()->GetScene("DemoLevel");

		{
			Grvt::GrvtTexture* SkyBox = Grvt::GetResourceManager()->GetTexture("DemoCubeMap");

			Grvt::MaterialCreationInfo MaterialInfo;
			MaterialInfo.Name = "CubeMapMaterial";
			MaterialInfo.Shader = Grvt::GetResourceManager()->GetShader("CubeMapShader");
			MaterialInfo.Textures.Push({Grvt::GrvtTexture_Type_Cubemap, &SkyBox->Handle});

			Grvt::GetResourceManager()->NewMaterial(MaterialInfo);
		}

		{
			Grvt::GrvtTexture* FloorTile = Grvt::GetResourceManager()->GetTexture("FloorTileTexture");

			Grvt::MaterialCreationInfo FloorMat;
			FloorMat.Name = "FloorMaterial";
			FloorMat.Shader = Grvt::GetResourceManager()->GetShader("FloorShader");
			FloorMat.Textures.Push({Grvt::GrvtTexture_Type_Albedo, &FloorTile->Handle});

			Grvt::GetResourceManager()->NewMaterial(FloorMat);
		}

		{
			Grvt::MaterialCreationInfo BColourMat;
			BColourMat.Name = "BaseColourMaterial";
			BColourMat.Shader = Grvt::GetResourceManager()->GetShader("BasicColour");
			
			Grvt::GetResourceManager()->NewMaterial(BColourMat);
		}

		{
			Grvt::ActorCreationInfo ActorInfo;
			ActorInfo.Identifier = "Mid";
			ActorInfo.Position = glm::vec3(0.0f, 0.0f, 0.0f);
			ActorInfo.SrcModel = Grvt::GetResourceManager()->GetModel("Cube");
			ActorInfo.SrcMaterial = Grvt::GetResourceManager()->GetMaterial("BaseColourMaterial");

			DemoScene->AddNewActor(ActorInfo);

			ActorInfo.Identifier = "Right";
			ActorInfo.Position = glm::vec3(10.0f, 0.0, 0.0f);
			DemoScene->AddNewActor(ActorInfo);

			ActorInfo.Identifier = "Up";
			ActorInfo.Position = glm::vec3(0.0f, 10.0, 0.0f);
			DemoScene->AddNewActor(ActorInfo);

			ActorInfo.Identifier = "Front";
			ActorInfo.Position = glm::vec3(0.0f, 0.0, 10.0f);
			DemoScene->AddNewActor(ActorInfo);
		}

		//DemoScene->AddSkyBox(Grvt::GetResourceManager()->GetMaterial("CubeMapMaterial"));
	}

	void ExecuteApplication()
	{
		Grvt::BaseCamera* Camera = Grvt::GetActiveScene()->Camera;
		static bool Animate = false;
		static bool InitAnim = true;
		RenderFloorGrid();

		Grvt::GrvtActor* Mid	= Grvt::GetActiveScene()->GetActor("Mid");
		Grvt::GrvtActor* Right	= Grvt::GetActiveScene()->GetActor("Right");
		Grvt::GrvtActor* Up		= Grvt::GetActiveScene()->GetActor("Up");
		Grvt::GrvtActor* Front	= Grvt::GetActiveScene()->GetActor("Front");

		Mid->Material.SetVector("Colour", glm::vec3(1.0f, 1.0f, 1.0f));
		Mid->Material.SetFloat("Far", Grvt::GetActiveScene()->Camera->Far);
		Mid->Material.SetFloat("Near", Grvt::GetActiveScene()->Camera->Near);

		Right->Material.SetVector("Colour", glm::vec3(1.0f, 0.0f, 0.0f));
		Right->Material.SetFloat("Far", Grvt::GetActiveScene()->Camera->Far);
		Right->Material.SetFloat("Near", Grvt::GetActiveScene()->Camera->Near);

		Up->Material.SetVector("Colour", glm::vec3(0.0f, 1.0f, 0.0f));
		Up->Material.SetFloat("Far", Grvt::GetActiveScene()->Camera->Far);
		Up->Material.SetFloat("Near", Grvt::GetActiveScene()->Camera->Near);

		Front->Material.SetVector("Colour", glm::vec3(0.0f, 0.0f, 1.0f));
		Front->Material.SetFloat("Far", Grvt::GetActiveScene()->Camera->Far);
		Front->Material.SetFloat("Near", Grvt::GetActiveScene()->Camera->Near);

		if (m_IO->IsKeyPressed(GLFW_KEY_P))
			Animate ^= true;

		// NOTE(Afiq):
		// This part here needs to be a system instead.
		if (Animate)
		{
			if (InitAnim)
			{
				Time = 0.0f;
				Camera->Yaw = 0.0f;
				Camera->Position.x = StartPos.x;
				InitAnim = false;
			}

			glm::vec3 Distance = glm::normalize(EndPos - Camera->Position);
			float Velocity = Camera->MoveSpeed * m_Engine->DeltaTime;
			float T = (Time * m_Engine->DeltaTime) / (5.0f * m_Engine->DeltaTime);
			
			Camera->Position = (1.0f - T) * StartPos + T * EndPos;
			Camera->Yaw = (1.0f - T) * 0.0f + T * -90.0f;
			Camera->UpdateOrientation();

			Time += 1.0f * m_Engine->DeltaTime;

			if (Camera->Position.x >= EndPos.x)
				InitAnim = true;
		}
	}

	void OnUnload()
	{
		m_IO = nullptr;
		m_Engine = nullptr;
	}

}
