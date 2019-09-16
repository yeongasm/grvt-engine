#include "stdafx.h"


PointLight	*pointLight = nullptr;

void LightWindowTest(const GravityApp *App) {
	bool open = true;
	ImGui::Begin("Manipulate light", &open);
	ImGui::SliderFloat("X", &pointLight->position.x, -50.0f, 50.0f);
	ImGui::SliderFloat("Y", &pointLight->position.y, -50.0f, 50.0f);
	ImGui::SliderFloat("Z", &pointLight->position.z, -50.0f, 50.0f);
	ImGui::SliderFloat("Brightness", &pointLight->brightness, 0.0f, 1.0f);
	ImGui::SliderFloat("Radius", &pointLight->radius, 0.0f, 100.0f);
	ImGui::End();
}


/**
* But if you don't love your work, you can never be a professional - Chef Tatsuya Sekiguchi
*/


int main() {

	GravityApp *app = NewApplication("Gravity Engine v2.0", 1366, 768, 4, 5);

	app->Init();
	app->EnableVSync(1);

	{
		ProjectCreationInfo info;
		info.name = "PBR Demo";
		info.filename = "PBR Demo";
		info.directory = RootDir("bin/");

		app->NewProject(info);
	}

	ResourceManager *manager	= app->GetResourceHandler();
	Renderer		*renderer	= app->GetRenderer();

	// NOTE(Afiq):
	// The camera class should really be inside of the Scenery class.
	EulerCamera		*camera		= new EulerCamera();

	Scene		*witch		= nullptr;
	Scene		*cube		= nullptr;
	Shader		*diffuse	= nullptr;
	Material	*material	= nullptr;
	Scenery		*level		= nullptr;

	{
		SceneCreationInfo info;
		info.name		= "Chibi witch";
		info.directory	= RootDir("Data/Models/witch/source/");
		info.file		= "03.obj";
		info.type		= SCENE_TYPE_CUSTOM; 

		witch = manager->NewScene(info);
	}

	{
		ShaderCreationInfo info;
		info.name			= "Simple shader";
		info.directory		= RootDir("Data/Shaders/");
		info.vertexShader	= "basic_diffused.vs";
		info.fragmentShader = "basic_diffused.fs";

		diffuse = manager->NewShader(info);
	}

	SceneInstanceCreation info;
	SceneInstance *witchInst = witch->CreateInstance(info);

	{
		TextureCreationInfo info;
		info.name			= "Witch_body_albedo";
		info.directory		= RootDir("Data/Models/witch/source/");
		info.files.Push("body.jpg");
		info.type			= TEXTURE_TYPE_ALBEDO;

		Texture *body = manager->NewTexture(info);

		info.files.Release();
		info.name			= "Witch_obj_albedo";
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
		
		//bodyMat->SetTexture("diffuseTexture", body);
		//objMat->SetTexture("diffuseTexture", object);

		bodyMat->SetVector("material.ambient",	glm::vec3(1.0f, 0.5f, 0.31f));
		bodyMat->SetVector("material.diffuse",	glm::vec3(1.0f, 0.5f, 0.31f));
		bodyMat->SetVector("material.specular", glm::vec3(0.5f, 0.5f, 0.5f ));
		bodyMat->SetFloat("material.shininess", 32.0f);

		objMat->SetVector("material.ambient",	glm::vec3(1.0f, 0.5f, 0.31f));
		objMat->SetVector("material.diffuse",	glm::vec3(1.0f, 0.5f, 0.31f));
		objMat->SetVector("material.specular",	glm::vec3(1.0f, 1.0f, 1.00f ));
		objMat->SetFloat("material.shininess",	32.0f);

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
		info.name		= "Point light 1";
		info.type		= LIGHT_TYPE_POINTLIGHT;
		info.position	= glm::vec3(1.2f, 1.0f, 2.0f);
		info.radius		= 10.0f;

		pointLight = level->AddPointLight(info);
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
	
	witchInst->renderState.cullFace	= true;
	int clicknum = 0;

	while (!app->CloseAplication()) {
		app->NewFrame();
		app->Tick();

		deltaTime = app->deltaTime;

		// NOTE(Afiq): Temporary placeholder for camera system before incorporating UI.
		{
			if (io.IsKeyPressed(GLFW_KEY_F1)) {
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

		//witchInst->SetPosition(glm::vec3(-5.0f, 0.0f, 0.0f));

		// Don't render anything if the window is minimised.
		if (app->width && app->height) {
			// Before drawing everything onto the screen, we pack the level's data and compute the necessities for the renderer.
			renderer->PreRenderLevel(level);

			// Draw onto the screen.
			renderer->Render();
		}

		{
			if (io.IsKeyPressed(GLFW_KEY_V)) {
				enableVSync ^= true;
				app->EnableVSync(enableVSync);
			}

			LightWindowTest(app);
		}

		app->ui.Tick();
		
#if IMGUI_WINDOW_DEBUGGER
		WindowDebugger::RenderDebugger();
#endif

		app->SwapBuffer();
	}

	TerminateApplication(app);

	delete camera;

//	DumpLogIntoFile
	Logger::Free();

#if 0
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}