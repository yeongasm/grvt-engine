#include "stdafx.h"
#include <string>


int main() {

	GravityApp *app = NewApplication("Gravity Engine v2.0", 800, 600, 4, 0);
	
	app->Init();
	app->EnableVSync(true);

	ResourceManager *manager	= app->GetResourceHandler();
	Renderer		*renderer	= app->GetRenderer();
	EulerCamera		*camera		= new EulerCamera();

	Scene		*witch		= nullptr;
	Scene		*cube		= nullptr;
	Shader		*diffuse	= nullptr;
	Shader		*rawColour	= nullptr;
	Material	*material	= nullptr;
	Light		*light		= nullptr;

	renderer->SCR_WIDTH = app->width;
	renderer->SCR_HEIGHT = app->height;


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

		info.name			= "Simple Raw Colour Shader";
		info.fragmentShader = "RawColour.fs";

		rawColour = manager->NewShader(info);
	}

	SceneInstanceCreation info;
	
	info.shader = diffuse;
	SceneInstance *witchInst = witch->CreateInstance(info);
	
	info.shader = rawColour;
	SceneInstance *cubeInst	= cube->CreateInstance(info);

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

		bodyMat->SetTexture("diffuseTexture", body);
		objMat->SetTexture("diffuseTexture", object);

		witchInst->GetNode(0)->PushMaterial(bodyMat);
		witchInst->GetNode(1)->PushMaterial(objMat);
		witchInst->GetNode(2)->PushMaterial(objMat);
		witchInst->GetNode(3)->PushMaterial(objMat);
		witchInst->GetNode(4)->PushMaterial(objMat);

		mat.name = "Raw Cube Colour";
		mat.shader = rawColour;
		mat.textures.Release();

		Material *cubeMat = manager->NewMaterial(mat);
		cubeMat->SetVector("colour", glm::vec3(1.0f, 1.0f, 1.0f));

		cubeInst->GetNode(0)->PushMaterial(cubeMat);
	}

	witchInst->renderState.DefaultModelRenderState();
	cubeInst->renderState.DefaultModelRenderState();

	//Scenery *level = manager->NewLevel("Level 01");

	//LightCreationInfo lightInfo;
	//lightInfo.name = "Directional light";
	//lightInfo.type = LIGHT_TYPE_DIRECTIONAL;

	//DirLight *dirlight = level->AddDirectionalLight(lightInfo);

	//lightInfo.name = "Point light";
	//lightInfo.type = LIGHT_TYPE_POINTLIGHT;
	//lightInfo.position = glm::vec3(5.0f, 5.0f, 2.0f);

	//PointLight *pointlight = level->AddPointLight(lightInfo);

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

	bool	showUI		= false;
	float	deltaTime	= 0.0f;

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
		

		cubeInst->SetPosition(glm::vec3(15.0f, 0.0f, -1.0f));
		cubeInst->SetScale(glm::vec3(10.0f));

		witchInst->SetPosition(glm::vec3(-5.0f, 2.0f, 0.0f));

		renderer->AttachCamera(camera);
		renderer->PushSceneForRender(witchInst);
		renderer->PushSceneForRender(cubeInst);

		renderer->Render();

		{
			if (io.IsKeyPressed(GLFW_KEY_GRAVE_ACCENT))
				showUI ^= true;

			if (showUI)
				ImGui::ShowDemoWindow();
		}

		app->SwapBuffer();
	}

	TerminateApplication(app);

	delete camera;

	DumpLogIntoFile

#if GRAVITY_LEAK_DEBUG
	_CrtDumpMemoryLeaks();
#endif


	return 0;
}