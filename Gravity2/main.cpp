#include "stdafx.h"


void InfoWindow(const GravityApp *App) {
	bool open = true;
	ImGui::Begin("Render Info", &open);
	ImGui::Text("Delta time (seconds): %f", App->deltaTime);
	ImGui::Text("FPS: %.1f", App->FramesPerSecond());
	ImGui::Text("Single frame: %.4f ms/frame", App->TimePerFrame());
	ImGui::Text("VSync: %s", (App->VSyncStatus()) ? "Enabled" : "Disabled");
	ImGui::End();
}


int main() {

	GravityApp *app = NewApplication("Gravity Engine v2.0", 800, 600, 4, 0);

	app->Init();
	app->EnableVSync(0);

	ResourceManager *manager	= app->GetResourceHandler();
	Renderer		*renderer	= app->GetRenderer();
	EulerCamera		*camera		= new EulerCamera();

	Scene		*witch		= nullptr;
	Scene		*cube		= nullptr;
	Shader		*diffuse	= nullptr;
	Material	*material	= nullptr;
	Scenery		*level		= nullptr;
	DirLight	*dirLight	= nullptr;

	{
		SceneCreationInfo info;
		info.name		= "Chibi witch";
		info.directory	= RootDir("Assets/Models/witch/source/");
		info.file		= "03.obj";
		info.type		= SCENE_TYPE_CUSTOM; 

		witch = manager->NewScene(info);

		info.name		= "Base Cube";
		info.directory	= RootDir("Assets/Primitives/Obj/");
		info.file		= "cube.obj";
		info.type		= SCENE_TYPE_BASIC_CUBE;

		cube = manager->NewScene(info);
	}

	{
		ShaderCreationInfo info;
		info.name			= "Simple shader";
		info.directory		= RootDir("Shaders/");
		info.vertexShader	= "basic_diffused.vs";
		info.fragmentShader = "basic_diffused.fs";

		diffuse = manager->NewShader(info);
	}

	SceneInstanceCreation info;
	
	info.shader = diffuse;
	SceneInstance *witchInst = witch->CreateInstance(info);

	{
		TextureCreationInfo info;
		GenerateGenericTextureInfo(info);
		info.name			= "Witch_body_albedo";
		info.directory		= RootDir("Assets/Models/witch/source/");
		info.files.Push("body.jpg");
		info.format			= 0;
		info.textureType	= TEXTURE_TYPE_ALBEDO;

		Texture *body = manager->NewTexture(info);

		info.name			= "Witch_obj_albedo";
		info.files.Release();
		info.files.Push("ob.jpg");

		Texture *object		= manager->NewTexture(info);

		MaterialCreationInfo mat;
		mat.name			= "Witch_body_material";
		mat.shader			= diffuse;
		mat.textures.Push(body);

		Material *bodyMat	= manager->NewMaterial(mat);

		mat.name			= "Witch_obj_material";
		mat.textures.Release();
		mat.textures.Push(object);

		Material *objMat	= manager->NewMaterial(mat);
		
		bodyMat->SetVector("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		bodyMat->SetVector("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		bodyMat->SetVector("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		bodyMat->SetFloat("material.shininess", 32.0f);

		objMat->SetVector("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		objMat->SetVector("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		objMat->SetVector("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		objMat->SetFloat("material.shininess", 32.0f);

		witchInst->GetNode(0)->PushMaterial(bodyMat);
		witchInst->GetNode(1)->PushMaterial(objMat);
		witchInst->GetNode(2)->PushMaterial(objMat);
		witchInst->GetNode(3)->PushMaterial(objMat);
		witchInst->GetNode(4)->PushMaterial(objMat);

	}

	witchInst->renderState.DefaultModelRenderState();

	{
		LevelCreationInfo info;
		info.name = "Level 01";

		level = manager->NewLevel(info);
	}

	{	
		LightCreationInfo info;
		info.name = "Directional light";
		info.type = LIGHT_TYPE_DIRECTIONAL;
		info.position = glm::vec3(100.0f, 50.0f, 0.0f);

		dirLight = level->AddDirectionalLight(info);
	}

	EulerCameraInitInfo camInfo;

	camInfo.position		= glm::vec3(0.0f, 20.0f, 60.0f);
	camInfo.worldUp			= glm::vec3(0.0f, 1.0f, 0.0f);
	camInfo.yaw				= -90.0f;
	camInfo.pitch			=  -5.0f;
	camInfo.movementSpeed	=  50.0f;
	camInfo.sensitivity		=   0.5f;
	camInfo.zoom			=  45.0f;
	camInfo.frameCenter		= glm::vec2(app->width / 2.0f, app->height / 2.0f);

	camera->Init(camInfo);

	level->AttachCamera(camera);
	level->PushSceneInstance(witchInst);

	bool	showUI		= true;
	float	deltaTime	= 0.0f;
	bool	enableVSync = app->VSyncStatus();

	AppIO &io = app->io;

	while (!app->CloseAplication()) {
		app->NewFrame();
		app->Tick();

		deltaTime = app->deltaTime;

		// NOTE(Afiq): Temporary placeholder for camera system before incorporating UI.
		{
			if (io.IsKeyPressed(GLFW_KEY_TAB)) {
				camera->isActive ^= true;

				int inputMode = (camera->isActive) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
				app->SetInputMode(GLFW_CURSOR, inputMode);
			}

			if (io.IsKeyHeld(GLFW_KEY_W))
				camera->ProcessKeyboard(CAMERA_MOVE_FORWARD, deltaTime);
			if (io.IsKeyHeld(GLFW_KEY_S))
				camera->ProcessKeyboard(CAMERA_MOVE_BACKWARD, deltaTime);
			if (io.IsKeyHeld(GLFW_KEY_A))
				camera->ProcessKeyboard(CAMERA_MOVE_LEFT, deltaTime);
			if (io.IsKeyHeld(GLFW_KEY_D))
				camera->ProcessKeyboard(CAMERA_MOVE_RIGHT, deltaTime);
			if (io.IsKeyHeld(GLFW_KEY_Q))
				camera->ProcessKeyboard(CAMERA_MOVE_DOWN, deltaTime);
			if (io.IsKeyHeld(GLFW_KEY_E))
				camera->ProcessKeyboard(CAMERA_MOVE_UP, deltaTime);

			camera->ProcessMouseMovement(io.mousePos.x, io.mousePos.y);
			camera->ProcessMouseScroll(io.mouseWheel);
		}

		renderer->screenWidth = app->width;
		renderer->screenHeight = app->height;

		witchInst->SetPosition(glm::vec3(-5.0f, 0.0f, 0.0f));

		// Don't render anything if the window is minimised.
		if (app->width && app->height) {
			// Before drawing everything onto the screen, we pack the level's data and compute the necessities for the renderer.
			renderer->PreRenderLevel(level);

			// Draw onto the screen.
			renderer->Render();
		}

		{
			if (io.IsKeyPressed(GLFW_KEY_GRAVE_ACCENT))
				showUI ^= true;

			if (io.IsKeyPressed(GLFW_KEY_V)) {
				enableVSync ^= true;
				app->EnableVSync(enableVSync);
			}

			if (showUI)
				InfoWindow(app);
		}

		app->SwapBuffer();
	}

	TerminateApplication(app);

	delete camera;

	DumpLogIntoFile

#if 0
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}