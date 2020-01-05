#include "pch.h"
#include <Core/Engine.h>
#include <Manager/Manager.h>
#include "ModuleFuncs.h"


static bool MoveFloor = false;
constexpr float FloorMoveSpeed = 10.0f;

void RenderFloorGrid()
{
	Grvt::EngineIO*	 Io		= Grvt::GetEngine()->GetIO();
	Grvt::GrvtActor* Floor  = Grvt::GetActiveScene()->GetActor("FloorActor");

	// Create a floor if it has not been done.
	if (!Floor)
	{
		Grvt::ActorCreationInfo Info;
		Info.Identifier = "FloorActor";
		Info.SrcMaterial = Grvt::GetResourceManager()->GetMaterial("FloorMaterial");
		Info.SrcModel = Grvt::GetResourceManager()->GetModel("Plane");

		Floor = &Grvt::GetActiveScene()->AddNewActor(Info);
	}

	Floor->Scale = glm::vec3(1000.0f);

	//const float32 Width = Grvt::GetActiveScene()->Camera->Width;
	//const float32 Far = Grvt::GetActiveScene()->Camera->Far;

	// NOTE(Afiq):
	// This is not right! Since it does not take rotations into account.
	// 
	glm::vec3 CamPos  = Grvt::GetActiveScene()->Camera->Position;

	//float FloorFrontEdge = Floor->Position.z + 2000.0f;
	//float FloorSideEdge  = Floor->Position.x + 2000.0f;
	//
	//glm::vec2 Diff = glm::vec2(FloorSideEdge - CamPos.x, FloorFrontEdge - CamPos.z);
	//Diff.x -= Width * 0.5f;
	//Diff.y -= Far;

	//if (Diff.x != (2000.0f - Width * 0.5f) || Diff.y != 1000.0f)
	//{
	//	MoveFloor = true;
	//}

	//// Move the floor if the offsets are not 1000.0f.
	//if (MoveFloor)
	//{
	//	if ((int)Diff.x < (int)(2000.0f - (Width * 0.5f)))
	//	{
	//		Floor->Position.x += (FloorMoveSpeed * Grvt::GetEngine()->DeltaTime);
	//	}

	//	if ((int)Diff.x > (int)(2000.0f - (Width * 0.5f)))
	//	{
	//		Floor->Position.x -= (FloorMoveSpeed * Grvt::GetEngine()->DeltaTime);
	//	}

	//	if ((int)Diff.y < 1000)
	//	{
	//		Floor->Position.z += (FloorMoveSpeed * Grvt::GetEngine()->DeltaTime);
	//	}

	//	if ((int)Diff.y > 1000)
	//	{
	//		Floor->Position.z -= (FloorMoveSpeed * Grvt::GetEngine()->DeltaTime);
	//	}

	//	MoveFloor = false;
	//}

	// Toggle the floor for render.
	if (Io->IsKeyPressed(GLFW_KEY_G))
	{
		Floor->Render ^= true;
	}

	// We always want to render the grid hence face culling should be disabled.
	Floor->DrawingState.FrontFace = Grvt::CacheState_None;
	Floor->Rotation.x = -90.0f;
	Floor->Material.SetTexture("FloorTexture", Grvt::GrvtTexture_Type_Albedo);
	Floor->Material.SetVector("ScaleFactor", glm::vec2(Floor->Scale));
	Floor->Material.SetFloat("Near", Grvt::GetActiveScene()->Camera->Near);
	Floor->Material.SetFloat("Far", Grvt::GetActiveScene()->Camera->Far);
}