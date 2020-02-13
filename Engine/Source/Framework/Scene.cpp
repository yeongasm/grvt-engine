#include "GrvtPch.h"

#include "API/Graphics/GraphicsInterface.h"

#include "Framework/Scene.h"
#include "Renderer/RenderFoundation.h"

#include "Manager/Manager.h"


Grvt::GrvtScene* g_ActiveScene = nullptr;


namespace Grvt
{

	GrvtScene::GrvtScene() :
		Sky(),
		Name(), 
		Camera(nullptr), 
		Renderer(nullptr), 
		Actors(), 
		PointLights(), 
		DirectionalLight(nullptr) {}


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
		if (DirectionalLight)
		{
			return DirectionalLight;
		}

		DirectionalLight = new DirLight();
		DirectionalLight->Alloc(Info);

		if (Info.Shadows)
		{
			// Set up shadow map for directional lighting.
			Gfl::Pair<uint32, ObjHandle>& DepthAttachment = DirectionalLight->DepthMap.DepthAttachment;

			// This currently has no purpose.
			DepthAttachment.Key = RenderTarget_AttachPoint_Depth;

			BaseAPI::FramebufferBuildData FBuild;
			BaseAPI::TextureBuildData TBuild;

			TBuild.Type = GL_FLOAT;
			TBuild.Format = GL_DEPTH_COMPONENT;
			TBuild.InternalFormat = GL_DEPTH_COMPONENT;
			TBuild.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_NEAREST});
			TBuild.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_NEAREST});
			TBuild.Parameters.Push({GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER});
			TBuild.Parameters.Push({GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER});
			TBuild.BorderColour[0] = 1.0f;
			TBuild.BorderColour[1] = 1.0f;
			TBuild.BorderColour[2] = 1.0f;
			TBuild.BorderColour[3] = 1.0f;

			FBuild.Attachments.Push(BaseAPI::TextureAttachment(&DepthAttachment.Value, GL_DEPTH_ATTACHMENT, TBuild));
			Middleware::GetBuildQueue()->QueueFramebufferForBuild(&DirectionalLight->DepthMap.Handle, FBuild);
		}

		return DirectionalLight;
	}

	
	PointLight* GrvtScene::AddNewPointLight(const LightCreationInfo& Info)
	{
		if (PointLights.Length() == MAX_POINT_LIGHTS)
		{
			return nullptr;
		}
		
		PointLight* LightPtr = PointLights.Insert(new PointLight());
		LightPtr->Alloc(Info);

		if (Info.Shadows)
		{
			Gfl::Pair<uint32, ObjHandle>& DepthAttachment = LightPtr->DepthMap.DepthAttachment;
			DepthAttachment.Key = RenderTarget_AttachPoint_Depth;
			
			BaseAPI::FramebufferBuildData FBuild;
			BaseAPI::TextureBuildData TBuild;

			LightPtr->DepthMap.Width  = 1024;
			LightPtr->DepthMap.Height = 1024;

			TBuild.Target = GL_TEXTURE_CUBE_MAP;
			TBuild.Type = GL_FLOAT;
			TBuild.Format = GL_DEPTH_COMPONENT;
			TBuild.InternalFormat = GL_DEPTH_COMPONENT;
			TBuild.Parameters.Push({GL_TEXTURE_MAG_FILTER, GL_NEAREST});
			TBuild.Parameters.Push({GL_TEXTURE_MIN_FILTER, GL_NEAREST});
			TBuild.Parameters.Push({GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE});
			TBuild.Parameters.Push({GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE});
			TBuild.Parameters.Push({GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE});

			FBuild.Width  = LightPtr->DepthMap.Width;
			FBuild.Height = LightPtr->DepthMap.Height;
			FBuild.Attachments.Push(BaseAPI::TextureAttachment(&DepthAttachment.Value, GL_DEPTH_ATTACHMENT, TBuild));

			Middleware::GetBuildQueue()->QueueFramebufferForBuild(&LightPtr->DepthMap.Handle, FBuild);
		}

		return LightPtr;
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
	bool GrvtScene::DeleteDirLight()
	{
		if (!DirectionalLight)
			return false;

		RenderTarget& Framebuffer = DirectionalLight->DepthMap;

		if (Framebuffer.DepthAttachment.Value.Id)
			Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Framebuffer.DepthAttachment.Value), Middleware::GrvtGfx_Type_Texture);

		if (Framebuffer.DepthStencilAttachment.Value.Id)
			Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Framebuffer.DepthStencilAttachment.Value), Middleware::GrvtGfx_Type_Texture);

		for (Gfl::Pair<uint32, ObjHandle>& ColourAttachment : Framebuffer.ColourAttachments)
		{
			Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(ColourAttachment.Value), Middleware::GrvtGfx_Type_Texture);
		}

		if (Framebuffer.Handle.Id)
			Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Framebuffer.Handle), Middleware::GrvtGfx_Type_Framebuffer);

		delete DirectionalLight;
		DirectionalLight = nullptr;

		return true;
	}


	bool GrvtScene::DeletePointLight(PointLight** PointLight)
	{
		size_t Index = PointLights.Find(*PointLight);

		if (Index == -1)
		{
			return false;
		}

		RenderTarget& Framebuffer = (*PointLight)->DepthMap;

		if (Framebuffer.DepthAttachment.Value.Id)
			Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Framebuffer.DepthAttachment.Value), Middleware::GrvtGfx_Type_Texture);

		if (Framebuffer.DepthStencilAttachment.Value.Id)
			Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Framebuffer.DepthStencilAttachment.Value), Middleware::GrvtGfx_Type_Texture);

		for (Gfl::Pair<uint32, ObjHandle>& ColourAttachment : Framebuffer.ColourAttachments)
		{
			Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(ColourAttachment.Value), Middleware::GrvtGfx_Type_Texture);
		}

		if (Framebuffer.Handle.Id)
			Middleware::GetBuildQueue()->QueueHandleForDelete(Gfl::Move(Framebuffer.Handle), Middleware::GrvtGfx_Type_Framebuffer);

		delete PointLights[Index];
		PointLights[Index] = nullptr;

		PointLight = nullptr;

		PointLights.PopAt(Index);

		return true;
	}


	void GrvtScene::Alloc(const SceneCreationInfo& Info)
	{
		Name = Info.Name;
		Actors.Reserve(Info.ActorReserves);
		PointLights.Reserve(Info.PointLightReserves);
	}


	void GrvtScene::Free()
	{
		Camera = nullptr;

		DeleteAllLights();

		if (Sky.SrcModel)
		{
			RemoveSkyBox();
		}

		Name.Release();
		Actors.Release();
		PointLights.Release();
	}


	void GrvtScene::DeleteAllActors()
	{
		Actors.Empty();
	}


	void GrvtScene::DeleteAllPointLights()
	{
		PointLight* Temp = nullptr;

		if (PointLights.Length())
			Temp = PointLights[0];

		for (size_t i = PointLights.Length() - 1; PointLights.Length(); i--)
		{
			Temp = PointLights[i];
			DeletePointLight(&Temp);
		}

		PointLights.Empty();
	}


	void GrvtScene::DeleteAllLights()
	{
		DeleteDirLight();
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

		if (DirectionalLight)
		{
			if (DirectionalLight->Enable)
			{
				DirectionalLight->Compute(Buffer.DirectionalLight);

				Buffer.DepthMap = &DirectionalLight->DepthMap;

				glm::mat4 Projection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, DirectionalLight->ShadowNear, DirectionalLight->ShadowFar);

				// The multiplier would be a value that keeps the target inside of the projection's frustrum.
				glm::vec3 Target = Camera->Position + Camera->Forward * 20.0f;
				glm::mat4 View	 = glm::lookAt(Target - DirectionalLight->Orientation * 50.0f, Target, glm::vec3(0.0f, 1.0f, 0.0f));

				Buffer.DirLightSpaceTransform = Projection * View;
			}
		}

		//GrvtActor* DebugSphere = nullptr;

		GrvtModel* Sphere = GetResourceManager()->GetModel("Sphere");

		for (PointLight* LightPtr : PointLights)
		{
			if (!LightPtr->Enable)
				continue;

			glm::mat4& PointLight = Buffer.PointLights.Insert(glm::mat4(0.0f));
			LightPtr->Compute(PointLight);

			Buffer.OmniDepthMaps.Push(&LightPtr->DepthMap);
			
			glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 1024.0f / 1024.0f, LightPtr->ShadowNear, LightPtr->ShadowFar);

			Buffer.PointLightSpaceTransforms.emplace_back(Projection * glm::lookAt(LightPtr->Position, LightPtr->Position + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
			Buffer.PointLightSpaceTransforms.emplace_back(Projection * glm::lookAt(LightPtr->Position, LightPtr->Position + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
			Buffer.PointLightSpaceTransforms.emplace_back(Projection * glm::lookAt(LightPtr->Position, LightPtr->Position + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
			Buffer.PointLightSpaceTransforms.emplace_back(Projection * glm::lookAt(LightPtr->Position, LightPtr->Position + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
			Buffer.PointLightSpaceTransforms.emplace_back(Projection * glm::lookAt(LightPtr->Position, LightPtr->Position + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
			Buffer.PointLightSpaceTransforms.emplace_back(Projection * glm::lookAt(LightPtr->Position, LightPtr->Position + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
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

			Command.State	 = Actor.DrawingState;
			Command.Material = &Actor.Material;
			Command.Sort	 = Actor.Sort;

			glm::mat3 TrInvModel = glm::transpose(glm::inverse(Model));

			// Set the model matrix in here.
			Command.Material->SetMatrix("Model", Model);
			Command.Material->SetMatrix("TrInvModel", TrInvModel);

			if (DirectionalLight)
			{
				Command.Material->SetTexture("DepthMap", GrvtTexture_Type_ShadowMap);
			}

			Gfl::String Uniform;
			for (size_t i = 0; i < PointLights.Length(); i++)
			{
				Uniform.Format("OmniDepthMaps[%d]", i);
				Command.Material->SetTextures(Uniform, (int32)(GrvtTexture_Type_OmniShadowMap + i));
				Uniform.Empty();
			}

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

			if (Actor.Shadow)
			{
				Command.Transform = Model;
				Buffer.ShadowCommands.Push(Command);
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