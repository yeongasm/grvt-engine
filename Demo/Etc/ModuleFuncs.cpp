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

	Floor->Scale = glm::vec3(10.0f);
	Floor->Position.x = Grvt::GetActiveScene()->Camera->Position.x;
	Floor->Position.z = Grvt::GetActiveScene()->Camera->Position.z;

	const float32 Width = Grvt::GetActiveScene()->Camera->Width;
	const float32 Far = Grvt::GetActiveScene()->Camera->Far;

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
	Floor->Material.SetVector("ViewPos", Grvt::GetActiveScene()->Camera->Position);
	Floor->Material.SetFloat("Near", Grvt::GetActiveScene()->Camera->Near);
	Floor->Material.SetFloat("Far", Grvt::GetActiveScene()->Camera->Far);
}