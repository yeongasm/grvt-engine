#include "GrvtPch.h"
#include "Framework/Abstraction/Scene.h"
#include "Manager/Manager.h"


namespace Grvt
{

	GrvtScene::GrvtScene() :
		Name(), Actors(), DirectionalLights(), PointLights() {}


	GrvtScene::~GrvtScene() {}


	GrvtScene::GrvtScene(const GrvtScene& Other)
	{
		*this = Other;
	}


	GrvtScene& GrvtScene::operator= (const GrvtScene& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Name				= Other.Name;
			Actors				= Other.Actors;
			DirectionalLights	= Other.DirectionalLights;
			PointLights			= Other.PointLights;
		}

		return *this;
	}


	GrvtScene::GrvtScene(GrvtScene&& Other)
	{
		*this = Gfl::Move(Other);
	}


	GrvtScene& GrvtScene::operator= (GrvtScene&& Other)
	{
		_ASSERTE(this != &Other);

		if (this != &Other)
		{
			Name				= Other.Name;
			Actors				= Other.Actors;
			DirectionalLights	= Other.DirectionalLights;
			PointLights			= Other.PointLights;

			new (&Other) GrvtScene();
		}

		return *this;
	}


	GrvtActor& GrvtScene::AddNewActor(const ActorCreationInfo& Info)
	{
		GrvtActor& Actor = Actors.Insert(GrvtActor());
		
		Actor.Alloc(Info);

		return Actor;
	}


	DirLight* GrvtScene::AddNewDirectionalLight(const LightCreationInfo& Info)
	{
		if (DirectionalLights.Length() == MAX_DIRECTIONAL_LIGHTS)
		{
			return nullptr;
		}

		DirLight& Light = DirectionalLights.Insert(DirLight());
		Light.Alloc(Info);

		if (Info.Shadows)
		{
			/**
			* TODO(Afiq):
			* Make the shadow map's width and height be the same as the renderer's viewport size.
			*/
			FramebufferCreationInfo ShadowMapInfo;
			ShadowMapInfo.Name = Gfl::String().Format("%s_DShadowMap_%d", Name.C_Str(), DirectionalLights.Length());
			ShadowMapInfo.Width  = 800;
			ShadowMapInfo.Height = 600;
			ShadowMapInfo.AddAttachment(GrvtFramebuffer_AttachComponent_Texture, GrvtFramebuffer_Attachment_Depth);
			
			GetResourceManager()->NewFramebuffer(ShadowMapInfo);
		}

		return &Light;
	}

	
	PointLight* GrvtScene::AddNewPointLight(const LightCreationInfo& Info)
	{
		if (PointLights.Length() == MAX_POINT_LIGHTS)
		{
			return nullptr;
		}
		
		PointLight& Light = PointLights.Insert(PointLight());
		Light.Alloc(Info);

		if (Info.Shadows)
		{
			/**
			* TODO(Afiq):
			* Make the shadow map's width and height be the same as the renderer's viewport size.
			*/
			FramebufferCreationInfo ShadowMapInfo;
			ShadowMapInfo.Name = Gfl::String().Format("%s_PShadowMap_%d", Name.C_Str(), PointLights.Length());
			ShadowMapInfo.Width = 800;
			ShadowMapInfo.Height = 600;
			ShadowMapInfo.AddAttachment(GrvtFramebuffer_AttachComponent_Cubemap, GrvtFramebuffer_Attachment_Depth);

			GetResourceManager()->NewFramebuffer(ShadowMapInfo);
		}

		return &Light;
	}


	GrvtActor* GrvtScene::GetActor(const Gfl::String& Name)
	{
		for (GrvtActor& Actor : Actors)
		{
			if (Actor.Name != Name)
			{
				continue;
			}

			return &Actor;
		}

		return nullptr;
	}


	bool GrvtScene::DeleteActor(GrvtActor& Actor)
	{
		size_t Index = Actors.IndexOf(Actor);

		if (Index == -1)
		{
			return false;
		}

		Actors.PopAt(Index);

		return true;
	}


	bool GrvtScene::DeleteActor(const Gfl::String& Identifier)
	{
		GrvtActor* Actor = GetActor(Identifier);

		if (!Actor)
		{
			return false;
		}

		size_t Index = Actors.IndexOf(*Actor);
		Actors.PopAt(Index);

		return true;
	}


	bool GrvtScene::DeleteDirLight(DirLight& DirectionalLight)
	{
		size_t Index = DirectionalLights.IndexOf(DirectionalLight);

		if (Index == -1)
		{
			return false;
		}

		DirectionalLights.PopAt(Index);

		return true;
	}


	bool GrvtScene::DeletePointLight(PointLight& PointLight)
	{
		size_t Index = PointLights.IndexOf(PointLight);

		if (Index == -1)
		{
			return false;
		}

		PointLights.PopAt(Index);

		return true;
	}


	void GrvtScene::Alloc(const SceneCreationInfo& Info)
	{
		Name = Info.Name;
		Actors.Reserve(Info.ActorReserves);
		DirectionalLights.Reserve(Info.DirLightReserves);
		PointLights.Reserve(Info.PointLightReserves);
	}


	void GrvtScene::Free()
	{
		Actors.Release();
		DirectionalLights.Release();
		PointLights.Release();
	}


	void GrvtScene::DeleteAllActors()
	{
		Actors.Empty();
	}

	
	void GrvtScene::DeleteAllDirLights()
	{
		DirectionalLights.Empty();
	}


	void GrvtScene::DeleteAllPointLights()
	{
		PointLights.Empty();
	}


	void GrvtScene::DeleteAllLights()
	{
		DeleteAllDirLights();
		DeleteAllPointLights();
	}


	/**
	* TODO(Afiq):
	* Package lighting into the command buffer.
	* Package custom commands into the command buffer.
	*/
	void GrvtScene::CreateSceneCommandBuffer(CommandBuffer& Buffer)
	{
		// Prep for non instanced render command.
		for (GrvtActor& Actor : Actors)
		{
			if (!Actor.Render)
			{
				continue;
			}

			glm::mat4 Model = glm::mat4(1.0f);
			Model = glm::translate(Model, Actor.Position);
			Model = glm::scale(Model, Actor.Scale);

			if (Actor.Rotation.x)
			{
				Model = glm::rotate(Model, Actor.Position.x, glm::vec3(1.0f, 0.0f, 0.0f));
			}

			if (Actor.Rotation.y)
			{
				Model = glm::rotate(Model, Actor.Position.y, glm::vec3(0.0f, 1.0f, 0.0f));
			}

			if (Actor.Position.z)
			{
				Model = glm::rotate(Model, Actor.Position.z, glm::vec3(0.0f, 0.0f, 1.0f));
			}

			RenderCommand Command;
			Command.State = Actor.DrawingState;

			for (GrvtMesh& Mesh : Actor.ModelPtr->Meshes)
			{
				RenderNode Node;
				Node.Handle = &Mesh.Vao;
				Node.Size = Mesh.Size;
				Node.Mode = Actor.Mode;
				Node.Material = Actor.MaterialPtr;

				// This step is no longer necessary since we always default the amount to 1.
				//Node.Amount = 1;

				if (Mesh.Ebo.Id)
				{
					Node.Indexed = true;
				}

				Command.Nodes.Push(Node);
			}

			if (Actor.Instanced)
			{
				size_t Index = -1;

				for (RenderCommand& Command : Buffer.InstancedCommands)
				{
					if (Actor.ModelPtr->Meshes[0].Vao.Id != Command.Nodes[0].Handle->Id)
					{
						continue;
					}

					Index = Buffer.InstancedCommands.IndexOf(Command);
					break;
				}

				if (Index != -1)
				{
					size_t Length = Buffer.InstancedCommands[Index].Instances.Push(Model);
					Length++;

					for (RenderNode& Node : Buffer.InstancedCommands[Index].Nodes)
					{
						Node.Amount = Length;
					}
				}
				else
				{
					Buffer.InstancedCommands.Push(Command);
				}
			}
			else
			{
				Command.Transform = Model;
				Buffer.RenderCommands.Push(Gfl::Move(Command));
			}
		}

		/**
		* TODO(Afiq):
		* Package for lights and shadow maps.
		*/

		/**
		* TODO(Afiq):
		* Figure out a way to handle custom commands.
		*/

		Buffer.IsEmpty = false;
	}
}