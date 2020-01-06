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

		//DemoScene->AddSkyBox(Grvt::GetResourceManager()->GetMaterial("CubeMapMaterial"));
	}

	void ExecuteApplication()
	{
		RenderFloorGrid();
	}

	void OnUnload()
	{
		m_IO = nullptr;
		m_Engine = nullptr;
	}

}
