#include "GrvtPch.h"
#include "Framework/Foundation/Interface.h"
#include "Framework/Abstraction/Scene.h"
#include "Renderer/RenderFoundation.h"
#include "Manager/Manager.h"


Grvt::GrvtScene* g_ActiveScene = nullptr;


namespace Grvt
{

	GrvtScene::GrvtScene() :
		Name(), Camera(nullptr), Renderer(nullptr), Actors(), DirectionalLights(), PointLights() {}


	GrvtScene::~GrvtScene() {}


	/*GrvtScene::GrvtScene(const GrvtScene& Other)
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
	}*/


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

		/*if (Info.Shadows)
		{
			Gfl::Pair<uint32, ObjHandle>& DepthAttachment = Light.ShadowMap.DepthAttachment;
			DepthAttachment.Key = RenderTarget_AttachPoint_Depth;
			
			BaseAPI::FramebufferBuildData FBuild;
			BaseAPI::TextureBuildData TBuild;

			TBuild.Target = GL_TEXTURE_CUBE_MAP;
			TBuild.Type = GL_FLOAT;
			TBuild.Format = GL_DEPTH_COMPONENT;
			TBuild.InternalFormat = GL_DEPTH_COMPONENT;
			TBuild.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_NEAREST});
			TBuild.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_NEAREST});
			TBuild.Parameters.Push({GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE});
			TBuild.Parameters.Push({GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE});
			TBuild.Parameters.Push({GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE});

			FBuild.Attachments.Push(BaseAPI::TextureAttachment(&DepthAttachment.Value, GL_DEPTH_ATTACHMENT, TBuild));

			Middleware::GetBuildQueue()->QueueFramebufferForBuild(&Light.ShadowMap.Handle, FBuild);
		}*/

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

	// TODO(Afiq):
	// Delete Framebuffer when we're deleting the light source.
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
		Camera = nullptr;
		Name.Release();
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


	void GrvtScene::AddSkyBox(GrvtMaterial* MaterialPtr, bool Render)
	{
		Sky.SrcModel	= GetResourceManager()->GetModel("Cube");
		Sky.Render		= Render;
		Sky.SrcMaterial = *MaterialPtr;
		Sky.SrcMaterial.SetTexture("CubeMap", GrvtTexture_Type_Cubemap);
	}


	void GrvtScene::RenderSkyBox(bool Render)
	{
		Sky.Render = Render;
	}


	void GrvtScene::RemoveSkyBox()
	{
		Sky.SrcModel = nullptr;
		Sky.Render = false;
		Sky.SrcMaterial.Free();
	}


	/**
	* TODO(Afiq):
	* Package lighting into the command buffer.
	* Package custom commands into the command buffer.
	*/
	void GrvtScene::CreateSceneCommandBuffer(CommandBuffer& Buffer)
	{
		if (!Buffer.IsEmpty)
		{
			return;
		}
		
		// Pass in the final projection and view matrix.
		Buffer.Projection = Camera->GetCameraProjection();
		Buffer.View = Camera->GetCameraView();

		RenderCommand Command;

		for (GrvtActor& Actor : Actors)
		{
			if (!Actor.Render)
			{
				continue;
			}

			// Do not pass the actor in for render if there is no shader or texture assigned to it.
			// Not sure if this is the right way but ideally, this should not happen.
			if (!Actor.Material.Shader)
			{
				continue;
			}

			glm::mat4 Model = glm::mat4(1.0f);
			Model = glm::translate(Model, Actor.Position);
			Model = glm::scale(Model, Actor.Scale);

			if (Actor.Rotation.x)
			{
				Model = glm::rotate(Model, glm::radians(Actor.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			}

			if (Actor.Rotation.y)
			{
				Model = glm::rotate(Model, glm::radians(Actor.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			}

			if (Actor.Position.z)
			{
				Model = glm::rotate(Model, glm::radians(Actor.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			}

			Command.State = Actor.DrawingState;
			Command.Material = &Actor.Material;

			glm::mat3 TrInvModel = glm::transpose(glm::inverse(Model));

			// Set the model matrix in here.
			Command.Material->SetMatrix("Model", Model);
			Command.Material->SetMatrix("TrInvModel", TrInvModel);

			RenderNode Node;

			for (GrvtMesh& Mesh : Actor.ModelPtr->Meshes)
			{
				Node.Handle = &Mesh.Vao;
				Node.Size = Mesh.Size;
				Node.Mode = Actor.Mode;

				// This step is no longer necessary since we always default the amount to 1.
				//Node.Amount = 1;

				if (Mesh.Ebo.Id)
				{
					Node.Indexed = true;
				}

				Command.Nodes.Push(Gfl::Move(Node));
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

			Command.Empty();
		}

		for (DirLight& Light : DirectionalLights)
		{
			if (!Light.Enable)
				continue;

			glm::mat4 LightSrc;
			Light.Compute(LightSrc);
			Buffer.DirectionalLights.Push(LightSrc);

			glm::mat4 LProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 1.0f, 100.0f);

			glm::vec3 NewPos = Light.Position + Camera->MoveDelta;
			printf("NewPos x: %.3f y: %.3f z: %.3f\n", NewPos.x, NewPos.y, NewPos.z);
			glm::vec3 NormalisedPos = glm::normalize(NewPos);

			glm::vec3 Right = glm::normalize(glm::cross(NormalisedPos, glm::vec3(0.0f, 1.0f, 0.0f)));
			glm::vec3 Up	= glm::normalize(glm::cross(Right, NormalisedPos));
			glm::vec3 Front = glm::normalize(glm::cross(Up, Right));

			glm::mat4 LView = glm::lookAt(NormalisedPos * 50.0f, glm::normalize(Camera->Position + Camera->Forward), glm::vec3(0.0f, 1.0f, 0.0f));

			Buffer.LightSpaceTransforms.Push(LProjection* LView);
		}

		for (PointLight& Light : PointLights)
		{
			if (!Light.Enable)
				continue;

			glm::mat4 LightSrc;
			Light.Compute(LightSrc);
			Buffer.PointLights.Push(LightSrc);

			glm::mat4 LProjection = glm::perspective(glm::radians(90.0f), Camera->Width / Camera->Height, Camera->Near, Camera->Far);
			Buffer.LightSpaceTransforms.Push(LProjection* glm::lookAt(Light.Position, Light.Position + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
			Buffer.LightSpaceTransforms.Push(LProjection* glm::lookAt(Light.Position, Light.Position + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
			Buffer.LightSpaceTransforms.Push(LProjection* glm::lookAt(Light.Position, Light.Position + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
			Buffer.LightSpaceTransforms.Push(LProjection* glm::lookAt(Light.Position, Light.Position + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
			Buffer.LightSpaceTransforms.Push(LProjection* glm::lookAt(Light.Position, Light.Position + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
			Buffer.LightSpaceTransforms.Push(LProjection* glm::lookAt(Light.Position, Light.Position + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
		}

		if (Sky.SrcModel && Sky.Render)
		{
			RenderNode Node;

			for (GrvtMesh& Mesh : Sky.SrcModel->Meshes)
			{
				Node.Handle = &Mesh.Vao;
				Node.Size	= Mesh.Size;
				Node.Mode	= GL_TRIANGLES;

				if (Mesh.Ebo.Id)
				{
					Node.Indexed = true;
				}

				Buffer.SkyBox.Nodes.Push(Gfl::Move(Node));
			}

			Buffer.SkyBox.Material = &Sky.SrcMaterial;
		}

		Buffer.IsEmpty = false;
	}

	
	void SetActiveScene(GrvtScene* ActiveScene)
	{
		if (g_ActiveScene != ActiveScene)
		{
			g_ActiveScene = ActiveScene;
		}
	}


	GrvtScene* GetActiveScene()
	{
		return g_ActiveScene;
	}
}