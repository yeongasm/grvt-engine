#include "pch.h"
#include <Core/ModelDefaults.h>
#include <Manager/Manager.h>
#include <Framework/Actor.h>
#include <Framework/Scene.h>
#include <Framework/Shader.h>
#include <Renderer/Renderer.h>
#include "CameraSystem.h"

#include "DemoModule.h"
#include "Etc/ModuleFuncs.h"


Grvt::EngineIO*		m_IO		= nullptr;
Grvt::GrvtEngine*	m_Engine	= nullptr;
Grvt::GrvtScene*	DemoScene	= nullptr;

glm::vec3 StartPos = glm::vec3(0.0f, 20.0f, 50.0f);
glm::vec3 EndPos = glm::vec3(-30.0f, 5.0f, 0.0f);
float Time = 0.0f;

using namespace Grvt;

extern "C"
{

	void OnStartUp(Grvt::GrvtEngine* EnginePtr)
	{
		ResourceManager* Manager = Grvt::GetResourceManager();

		m_Engine = EnginePtr;
		m_IO = m_Engine->GetIO();

		// Register camera system.
		m_Engine->RegisterSystem(GRSYSTEMCRED(CameraSystem), new CameraSystem());

		{
			ModelImportInfo Info;

			Info.Name = "Plane";
			Info.Path = Gfl::String("Data\\Model\\Plane.fbx");
			Manager->NewImportModel(Info);

			Info.Name = "Quad";
			Info.Path = Gfl::String("Data\\Model\\Quad.fbx");
			Manager->NewImportModel(Info);

			Info.Name = "Cube";
			Info.Path = Gfl::String("Data\\Model\\Cube.fbx");
			Manager->NewImportModel(Info);
			
			/*Info.Name = "Sphere";
			Info.Path = Gfl::String("Data\\Model\\Sphere.fbx");
			ResourceMgrPtr->NewImportModel(Info);

			Info.Name = "Cylinder";
			Info.Path = Gfl::String("Data\\Model\\Cylinder.fbx");
			ResourceMgrPtr->NewImportModel(Info);

			Info.Name = "Cone";
			Info.Path = Gfl::String("Data\\Model\\Cone.fbx");
			ResourceMgrPtr->NewImportModel(Info);

			Info.Name = "IcoSphere";
			Info.Path = Gfl::String("Data\\Model\\IcoSphere.fbx");
			ResourceMgrPtr->NewImportModel(Info);

			Info.Name = "Torus";
			Info.Path = Gfl::String("Data\\Model\\Torus.fbx");
			ResourceMgrPtr->NewImportModel(Info);

			Info.Name = "Suzanne";
			Info.Path = Gfl::String("Data\\Model\\Monkey.fbx");
			ResourceMgrPtr->NewImportModel(Info);*/

			Info.Name = "Floor";
			Info.Path = Gfl::String("Data\\Model\\Floor.fbx");
			Manager->NewImportModel(Info);
		}
	}

	void OnLoad(Grvt::GrvtEngine* EnginePtr)
	{

	}

	void ExecuteApplication()
	{

		// NOTE(Afiq):
		// This part here needs to be a system instead.
		/*if (Animate)
		{
			if (InitAnim)
			{
				Time = 0.0f;
				Camera->Yaw = 0.0f;
				Camera->Position.x = StartPos.x;
				//DemoScene->DirectionalLight->Orientation = InitOrientation;
				InitAnim = false;
			}

			glm::vec3 Distance = glm::normalize(EndPos - Camera->Position);
			float Velocity = Camera->MoveSpeed * m_Engine->DeltaTime;
			float T = (Time * m_Engine->DeltaTime) / (5.0f * m_Engine->DeltaTime);
			
			if (DemoScene->DirectionalLight)
			{
				DemoScene->DirectionalLight->Orientation = (1.0f - T) * InitOrientation + T * EndOrientation;
			}
			Camera->Position = (1.0f - T) * StartPos + T * EndPos;
			Camera->Yaw = (1.0f - T) * 0.0f + T * 90.0f;
			Camera->Pitch = (1.0f - T) * 25.0f + T * 5.0f;
			Camera->UpdateOrientation();

			Time += 1.0f * m_Engine->DeltaTime; 

			if (Camera->Position.x <= EndPos.x)
				InitAnim = true;

			//if (DemoScene->DirectionalLight)
			//	if (DemoScene->DirectionalLight->Orientation.x <= -1.0f)
			//		InitAnim = true;
		}*/
	}

}
