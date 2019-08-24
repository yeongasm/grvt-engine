#include "stdafx.h"


/**
* Window interaction variables are declared here.
*/
SceneData		*gHoveredScene = nullptr;
MaterialData	*gHoveredMat = nullptr;


/**
* Declare all windows here.
*/
WindowMenuBarTemplate			*WindowMenuBar			= nullptr;
WindowNewProjectTemplate		*WindowNewProject		= nullptr;
WindowCreateObjectTemplate		*WindowCreateObject		= nullptr;
WindowProjectExplorerTemplate	*windowProjectExplorer	= nullptr;
WindowNewSceneTemplate			*WindowNewScene			= nullptr;
WindowNewTextureTemplate		*WindowNewTexture		= nullptr;
WindowNewShaderTemplate			*WindowNewShader		= nullptr;
WindowNewMaterialTemplate		*WindowNewMaterial		= nullptr;
WindowNewSceneryTemplate		*WindowNewScenery		= nullptr;
WindowNewSceneInstanceTemplate	*WindowNewSceneInstance = nullptr;


/**
* WindowsHandler functions start here.
*/
WindowsHandler::WindowsHandler() : windows(), activeWindows() {}


WindowsHandler::WindowsHandler(const WindowsHandler &Other) { *this = Other; }


WindowsHandler& WindowsHandler::operator= (const WindowsHandler &Other) {
	_ASSERTE(this != &Other);

	if (this != &Other) {
		windows			= Other.windows;
		activeWindows	= Other.activeWindows;
	}

	return *this;
}


WindowsHandler::~WindowsHandler() { Release(); }


void WindowsHandler::Init(GravityApp *Application) {
	ImGuiStyle &style	= ImGui::GetStyle();
	ImVec4 *colours		= style.Colors;
	ImFont *pFont		= nullptr;
	ImGuiIO &io			= ImGui::GetIO();

	String fontPath		= "Data/Editor/Fonts/OpenSans/OpenSans-Regular.ttf";
	String iconPath("Data/Editor/Fonts/FontAwesome/%s", FONT_ICON_FILE_NAME_FAS);

	const ImWchar iconRange[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
	ImFontConfig iconConf;
	iconConf.MergeMode	= true;
	iconConf.PixelSnapH = true;

	application = Application;

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Viewports causes an error for now. Something to do with handlers.
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	io.Fonts->Clear();
	pFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 15.0f);

	if (pFont) {
		pFont->FontSize = 15.0f;
		io.FontDefault = pFont;
	} else {
		io.Fonts->AddFontDefault();
	}

	io.Fonts->AddFontFromFileTTF(iconPath.c_str(), 15.0f, &iconConf, iconRange);

	io.Fonts->Build();
	ImGui::SetCurrentFont(io.Fonts->Fonts.front());

	style.FrameRounding = 4.0f;
	style.GrabRounding	= 4.0f;

	/**
	* NOTE(Afiq):
	* Default theme and colour scheme for Gravity.
	* 
	* TODO(Afiq):
	* Check for a theme that has been set upon loading a project file.
	*/
	colours[ImGuiCol_Text]					= ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colours[ImGuiCol_TextDisabled]			= ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colours[ImGuiCol_WindowBg]				= ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
//	colours[ImGuiCol_ChildBg]				= ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
//	colours[ImGuiCol_ChildBg]				= ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colours[ImGuiCol_PopupBg]				= ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colours[ImGuiCol_Border]				= ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colours[ImGuiCol_BorderShadow]			= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colours[ImGuiCol_FrameBg]				= ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colours[ImGuiCol_FrameBgHovered]		= ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colours[ImGuiCol_FrameBgActive]			= ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colours[ImGuiCol_TitleBg]				= ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
	colours[ImGuiCol_TitleBgActive]			= ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colours[ImGuiCol_TitleBgCollapsed]		= ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colours[ImGuiCol_MenuBarBg]				= ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colours[ImGuiCol_ScrollbarBg]			= ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colours[ImGuiCol_ScrollbarGrab]			= ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colours[ImGuiCol_ScrollbarGrabHovered]	= ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colours[ImGuiCol_ScrollbarGrabActive]	= ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
	colours[ImGuiCol_CheckMark]				= ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colours[ImGuiCol_SliderGrab]			= ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colours[ImGuiCol_SliderGrabActive]		= ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colours[ImGuiCol_Button]				= ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colours[ImGuiCol_ButtonHovered]			= ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colours[ImGuiCol_ButtonActive]			= ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colours[ImGuiCol_Header]				= ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
	colours[ImGuiCol_HeaderHovered]			= ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colours[ImGuiCol_HeaderActive]			= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colours[ImGuiCol_Separator]				= ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colours[ImGuiCol_SeparatorHovered]		= ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colours[ImGuiCol_SeparatorActive]		= ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colours[ImGuiCol_ResizeGrip]			= ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colours[ImGuiCol_ResizeGripHovered]		= ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colours[ImGuiCol_ResizeGripActive]		= ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colours[ImGuiCol_Tab]					= ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colours[ImGuiCol_TabHovered]			= ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colours[ImGuiCol_TabActive]				= ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colours[ImGuiCol_TabUnfocused]			= ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colours[ImGuiCol_TabUnfocusedActive]	= ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colours[ImGuiCol_PlotLines]				= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colours[ImGuiCol_PlotLinesHovered]		= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colours[ImGuiCol_PlotHistogram]			= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colours[ImGuiCol_PlotHistogramHovered]	= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colours[ImGuiCol_TextSelectedBg]		= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colours[ImGuiCol_DragDropTarget]		= ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colours[ImGuiCol_NavHighlight]			= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colours[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colours[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colours[ImGuiCol_ModalWindowDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	/**
	* Initialize all windows in this function.
	*/
	InitNewWindow(WindowMenuBar,			"Menu Bar",				Application);
	InitNewWindow(WindowNewProject,			"New Project",			Application);
	InitNewWindow(WindowCreateObject,		"Create Object",		Application);
	InitNewWindow(windowProjectExplorer,	"Project Explorer",		Application);
	InitNewWindow(WindowNewScene,			"New Scene",			Application);
	InitNewWindow(WindowNewTexture,			"New Texture",			Application);
	InitNewWindow(WindowNewShader,			"New Shader",			Application);
	InitNewWindow(WindowNewMaterial,		"New Material",			Application);
	InitNewWindow(WindowNewScenery,			"New Scenery",			Application);
	InitNewWindow(WindowNewSceneInstance,	"New Scene Instance",	Application);
}


void WindowsHandler::Show(GravityWindow *Window) {
	for (GravityWindow *showned : activeWindows)
		if (showned == Window)
			return;

	activeWindows.Push(Window);
}


void WindowsHandler::Hide(GravityWindow *Window) {
	for (size_t i = 0; i < activeWindows.Length();) {
		if (activeWindows[i] == Window) {
			activeWindows.PopAt(i);
			break;
		} else {
			i++;
		}
	}	
}


void WindowsHandler::Tick() {
	static bool init = true;

	WindowMenuBar->Events();
	WindowMenuBar->Draw();
	
	/**
	* NOTE(Afiq):
	* Hacky way of starting enabling the windows on init first. 
	* Need to find a better way than this.
	*/
	if (init) {
		if (application->GetCurrentProject()) {
			Show(WindowCreateObject);
			Show(windowProjectExplorer);
		}

		init = false;
	}

	for (size_t i = 0; i < activeWindows.Length(); i++) {
		activeWindows[i]->Events();

		// NOTE(Afiq):
		// This needs to be done because closing the window by triggering the close event in the Events() function will cause pointer invalidation.
		// This is ugly! We'll need to find another way to handle this.
		if (activeWindows[i])
			activeWindows[i]->Draw();
	}
}


void WindowsHandler::Release() {
	// Remove all the windows from showing.
	for (size_t i = 0; i < activeWindows.Length(); i++)
		activeWindows[i] = nullptr;

	activeWindows.Release();

	for (size_t i = 0; i < windows.Length(); i++) {
		delete windows[i];
		windows[i] = nullptr;
	}

	windows.Release();
}


/**
* ---------------------------------------------- GravityWindows definitions here ----------------------------------------------
*/

/**
* Menu Bar definition.
*/
void WindowMenuBarTemplate::Events() {
	WindowsHandler *ui = &application->ui;
	AppIO &io = application->io;
	
	if (io.Ctrl() && io.IsKeyPressed(GLFW_KEY_N))
		ui->Show(WindowNewProject);

	if (io.Ctrl() && io.Alt() && io.IsKeyPressed(GLFW_KEY_N))
		ui->Show(WindowCreateObject);

	if (io.Ctrl() && io.Alt() && io.IsKeyPressed(GLFW_KEY_L))
		ui->Show(windowProjectExplorer);
}


void WindowMenuBarTemplate::Draw() {
	WindowsHandler *ui = &application->ui;

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File")) {
		if (ImGui::BeginMenu("New")) {
			bool enableResourceLoading = false;

			if (application->GetCurrentProject())
				enableResourceLoading = true;

			if (ImGui::MenuItem("Project", "CTRL+N", false))
				ui->Show(WindowNewProject);

			//if (ImGui::MenuItem("Scene",	nullptr, false, enableResourceLoading))
			//	ui->Show(WindowNewScene);

			//if (ImGui::MenuItem("Shader",	nullptr, false, enableResourceLoading))
			//	ui->Show(WindowNewShader);

			//if (ImGui::MenuItem("Texture",	nullptr, false, enableResourceLoading))
			//	ui->Show(WindowNewTexture);

			//if (ImGui::MenuItem("Material", nullptr, false, enableResourceLoading))
			//	ui->Show(WindowNewMaterial);

			//if (ImGui::MenuItem("Scenery",	nullptr, false, enableResourceLoading))
			//	ui->Show(WindowNewScenery);

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Open", "CTRL+O")) {
			/**
			* NOTE(Afiq):
			* Open a project. Projects are JSON files with the extension *.grvt.
			*/
			//String path;
			//OpenNativeFileDialog(NATIVE_DIALOG_OPEN_FILE, path, "txt");
		}

		if (ImGui::MenuItem("Save", "CTRL+S"))
			printf("Saving current project ...\n");

		if (ImGui::MenuItem("Close", "ALT+F4"))
			printf("Closing application ...\n");

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View")) {
		bool enable = (application->GetCurrentProject()) ? true : false;

		if (ImGui::MenuItem("Create Object", "CTRL+ALT+N", false, enable))
			ui->Show(WindowCreateObject);

		if (ImGui::MenuItem("Project Explorer", "CTRL+ALT+L", false, enable))
			ui->Show(windowProjectExplorer);

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}


/**
* New Project window definition.
*/
void WindowNewProjectTemplate::Events() { 
	if (!isActive)
		return;

	AppIO &io = application->io;
	WindowsHandler &ui = application->ui;

	if (io.IsKeyPressed(GLFW_KEY_ESCAPE))
		ui.Hide(this);

	keyEvent[WINDOW_ON_SUBMIT].onEvent = application->io.IsKeyPressed(GLFW_KEY_ENTER);
}


void WindowNewProjectTemplate::Draw() {
	WindowsHandler &ui = application->ui;
	bool open = true;

	ImGui::OpenPopup(name.c_str());

	ImVec2 buttonSize(60.0f, 0.0f);
	int32 windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	if (ImGui::BeginPopupModal(name.c_str(), &open, windowFlags)) {
		ImGui::SetWindowSize(ImVec2(470.0f, 190.0f), ImGuiCond_Always);
		isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

		if (!imguiWindow)
			imguiWindow = ImGui::FindWindowByName(name.c_str());

#if IMGUI_WINDOW_DEBUGGER
		if (isActive)
			WindowDebugger::window = imguiWindow;
#endif

		static bool		sameName = true;
		static String	nameString;
		static String	fileString;
		static String	dirString;

		if (!nameString.Size())
			nameString.Reserve(512);

		if (!fileString.Size())
			fileString.Reserve(512);

		if (!dirString.Size())
			dirString.Reserve(512);

		if (!nameString.Length())
			nameString = "New Project";

		if (!dirString.Length())
			dirString = __ROOT__;

		ImGui::Text("New project details");
		if (ImGui::Checkbox("Project and file share same name?", &sameName))
			fileString = nameString;

		ImGui::Separator();
		ImGui::Spacing();
		if (ImGui::BeginChild("Details", ImVec2(70.0f, 75.0f))) {
			ImGui::Text("Project name");
			ImGui::Spacing();
			ImGui::Text("File name");
			ImGui::Spacing();
			ImGui::Text("Directory");
		}
		ImGui::EndChild();
		ImGui::SameLine();
		if (ImGui::BeginChild("Input", ImVec2(-1.0f, 75.0f))) {
			ImGui::PushItemWidth(-1.0f);
			ImGui::InputText("Project name", nameString.First(), 512);

			if (sameName)
				fileString = nameString;

			if (ImGui::InputText("File name", fileString.First(), 512))
				sameName = false;

			ImGui::InputText("Directory", dirString.First(), 512);
			ImGui::PopItemWidth();
		}
		ImGui::EndChild();
		ImGui::Separator();

		ImGuiButtonFlags submitFlag = 0;
		keyEvent[WINDOW_ON_SUBMIT].enabled = true;

		if (!nameString.Length() || !fileString.Length()) {
			submitFlag = ImGuiButtonFlags_Disabled;
			keyEvent[WINDOW_ON_SUBMIT].enabled = false;
		}

		if (submitFlag) {
			ImGui::TextColored(ImVec4(0.8039f, 0.2039f, 0.2313f, 1.0f), "Project and file name can not be empty!");
			ImGui::SameLine();
		}

		ImGui::Indent(imguiWindow->Size.x - 145.0f);
		if (ImGui::Button("Cancel", buttonSize))
			open = false;

		ImGui::SameLine();
		if (submitFlag)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.47f, 0.47f, 0.47f, 1.0f));

		if (ImGui::ButtonEx("Create", buttonSize, submitFlag) || keyEvent[WINDOW_ON_SUBMIT]) {
			ProjectCreationInfo info;
			info.name		= nameString;
			info.filename	= fileString;
			info.directory	= dirString;

			application->NewProject(info);
			open = false;
			ui.Show(WindowCreateObject);
		}

		if (submitFlag)
			ImGui::PopStyleColor();

		ImGui::Unindent();
		ImGui::EndPopup();
	}

	if (!open) {
#if IMGUI_WINDOW_DEBUGGER
		WindowDebugger::window = nullptr;
#endif
		ui.Hide(this);
		ImGui::CloseCurrentPopup();
	}
}


/**
* Create object window definition.
*/
void WindowCreateObjectTemplate::Events() {
	if (!isActive)
		return;
}


void WindowCreateObjectTemplate::Draw() {
	const ResourceManager *manager	= application->GetResourceHandler();
	WindowsHandler	&ui	= application->ui;

	bool open = true;
	ImGui::Begin(name.c_str(), &open);
	isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
	if (!imguiWindow)
		imguiWindow = ImGui::FindWindowByName(name.c_str());

#if IMGUI_WINDOW_DEBUGGER
	if (isActive)
		WindowDebugger::window = imguiWindow;
#endif

	float childWidth = (imguiWindow->Size.x * 0.5f);
	
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.0f));
	if (ImGui::BeginChild("Left Icons", ImVec2(childWidth - 15.0f, 0.0f), false, ImGuiWindowFlags_NoScrollbar)) {
		if (ImGui::Button(ICON_FA_RUNNING"\t\tScene", ImVec2(-1.0f, 0.0f)))
			ui.Show(WindowNewScene);

		ImGuiCustomToolTip("A Scene is simply a model that can be placed into a level by converting it into a SceneInstance.");

		if (ImGui::Button(ICON_FA_IMAGES"\t  Texture", ImVec2(-1.0f, 0.0f)))
			ui.Show(WindowNewTexture);

		ImGuiCustomToolTip("Textures are required to make Material objects that will be attached onto SceneInstances.");

		if (ImGui::Button(ICON_FA_TREE"\t\tScenery", ImVec2(-1.0f, 0.0f)))
			ui.Show(WindowNewScenery);

		ImGuiCustomToolTip("A Scenery object is simply a level where all Light and SceneInstance objects can be placed.");
	}
	ImGui::EndChild();
	ImGui::SameLine();
	if (ImGui::BeginChild("Right Icons", ImVec2(childWidth - 7.0f, 0.0f), false, ImGuiWindowFlags_NoScrollbar)) {
		if (ImGui::Button(ICON_FA_CODE"\t   Shader", ImVec2(-1.0f, 0.0f)))
			ui.Show(WindowNewShader);

		ImGuiCustomToolTip("Shaders are required to be attached onto a SceneInstance in order for it to be rendered.");

		if (ImGui::Button(ICON_FA_FILE_CODE"\t\t Material", ImVec2(-1.0f, 0.0f)))
			ui.Show(WindowNewMaterial);

		ImGuiCustomToolTip("Materials are a dynamic way to attach 'images' onto SceneInstance objects.");

		if (ImGui::Button(ICON_FA_CROP_ALT"\t\tFramebuffer", ImVec2(-1.0f, 0.0f)))
			printf("Coming soon ...\n");

		ImGuiCustomToolTip("Framebuffers are coming soon!");
	}
	ImGui::EndChild();
	ImGui::PopStyleVar();
	ImGui::End();
	
	if (!open) {
#if IMGUI_WINDOW_DEBUGGER
		WindowDebugger::window = nullptr;
#endif
		ui.Hide(this);
	}
}


/**
* Project explorer window definition.
*/
void WindowProjectExplorerTemplate::Events() {
	if (!isActive)
		return;

	keyEvent[WINDOW_ON_RGTCLK].onEvent = application->io.IsMouseClicked(MOUSE_BUTTON_RIGHT);
	keyEvent[WINDOW_ON_LFTCLK].onEvent = application->io.IsMouseClicked(MOUSE_BUTTON_LEFT);

	if (keyEvent[WINDOW_ON_RGTCLK] || keyEvent[WINDOW_ON_LFTCLK])
		action = EXPLORER_ACTION_NONE;
}


void WindowProjectExplorerTemplate::Draw() {
	WindowsHandler &ui = application->ui;

	bool open = true;

	ImGui::Begin(name.c_str(), &open);
	isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
	if (!imguiWindow)
		imguiWindow = ImGui::FindWindowByName(name.c_str());

#if IMGUI_WINDOW_DEBUGGER
	if (isActive)
		WindowDebugger::window = imguiWindow;
#endif

	if (ImGui::BeginTabBar("Project Explorer Tab Bar", ImGuiTabBarFlags_Reorderable)) {

		if (ImGui::BeginTabItem("Scene")) {
			// Temporary placeholder for search feature.
			//char buf[1024] = {'\0'};
			//ImGui::PushItemWidth(-1.0f);
			//ImGui::InputText("##FindScene", buf, 1024, ImGuiInputTextFlags_None);
			//ImGui::PopItemWidth();
			ShowSceneList();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Texture")) {
			// Temporary placeholder for search feature.
			//char buf[1024] = {'\0'};
			//ImGui::PushItemWidth(-1.0f);
			//ImGui::InputText("##FindTexture", buf, 1024, ImGuiInputTextFlags_None);
			//ImGui::PopItemWidth();
			ShowTextureList();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Shader")) {
			// Temporary placeholder for search feature.
			//char buf[1024] = {'\0'};
			//ImGui::PushItemWidth(-1.0f);
			//ImGui::InputText("##FindShader", buf, 1024, ImGuiInputTextFlags_None);
			//ImGui::PopItemWidth();
			ShowShaderList();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Material")) {
			// Temporary placeholder for search feature.
			//char buf[1024] = {'\0'};
			//ImGui::PushItemWidth(-1.0f);
			//ImGui::InputText("##FindMaterial", buf, 1024, ImGuiInputTextFlags_None);
			//ImGui::PopItemWidth();
			ShowMaterialList();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Scenery")) {
			// Temporary placeholder for search feature.
			//char buf[1024] = {'\0'};
			//ImGui::PushItemWidth(-1.0f);
			//ImGui::InputText("##FindScenery", buf, 1024, ImGuiInputTextFlags_None);
			//ImGui::PopItemWidth();
			ShowSceneryList();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Framebuffer")) {
			ImGui::Text("Coming soon!");
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}


	gHoveredScene	= hoveredScene;
	gHoveredMat		= hoveredMat;

	SceneListContextMenu();
	MaterialListContextMenu();

	if (action == EXPLORER_NEW_SCENEISNT_WINDOW)
		ui.Show(WindowNewSceneInstance);

	ImGui::End();
	
	// Reset the action at the end of the function.
	action = EXPLORER_ACTION_NONE;

	if (!open) {
#if IMGUI_WINDOW_DEBUGGER
		WindowDebugger::window = nullptr;
#endif
		ui.Hide(this);
	}
}


/**
* New Scene window definition.
*/
void WindowNewSceneTemplate::Events() {
	if (!isActive)
		return;

	AppIO &io = application->io;
	WindowsHandler &ui = application->ui;

	if (io.IsKeyPressed(GLFW_KEY_ESCAPE))
		ui.Hide(this);

	keyEvent[WINDOW_ON_SUBMIT].onEvent = application->io.IsKeyPressed(GLFW_KEY_ENTER);
}


void WindowNewSceneTemplate::Draw() {
	WindowsHandler	&ui			= application->ui;
	ResourceManager *manager	= application->GetResourceHandler();

	bool open = true;
	ImVec2 buttonSize(60.0f, 0.0f);
	int32 windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin(name.c_str(), &open, windowFlags);
	ImGui::SetWindowSize(ImVec2(550.0f, 175.0f), ImGuiCond_Always);
	isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

	if (!imguiWindow)
		imguiWindow = ImGui::FindWindowByName(name.c_str());

#if IMGUI_WINDOW_DEBUGGER
	if (isActive)
		WindowDebugger::window = imguiWindow;
#endif

	static bool		flipUV		= false;
	static String	nameString;
	static String	path;
	static String	errMsg;

	ImVec4 maroon = ImVec4(0.8039f, 0.2039f, 0.2313f, 1.0f);

	if (!nameString.Size()) {
		nameString.Reserve(512);
		nameString = "New Scene";
	}

	if (!path.Size()) {
		path.Reserve(512);
		path = __ROOT__;
	}

	ImGui::Text("New Scene details");
	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::BeginChild("Details", ImVec2(90.0f, 75.0f))) {
		ImGui::Text("Scene name");
		ImGui::SameLine(); ImGui::TextColored(maroon, "*");
		ImGui::Spacing();
		ImGui::Text("Path");
		ImGui::SameLine(); ImGui::TextColored(maroon, "*");
		ImGui::Spacing();
		ImGui::Text("Flip UV");
	}
	ImGui::EndChild();
	ImGui::SameLine();
	if (ImGui::BeginChild("Input", ImVec2(-1.0f, 75.0f))) {
		ImGui::PushItemWidth(-1.0f);
		ImGui::InputText("##Scene name", nameString.First(), 512);
		ImGui::PopItemWidth();
		if (ImGui::Button("Open", buttonSize)) {
			OpenNativeFileDialog(NATIVE_DIALOG_OPEN_FILE, path, "obj;fbx");
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(-1.0f);
		ImGui::InputText("##File", path.First(), 512);
		if (path.Length())
			ImGuiCustomToolTip(path.c_str());

		ImGui::PopItemWidth();
		ImGui::Checkbox("##", &flipUV);
	}
	ImGui::EndChild();
	ImGui::Separator();
	ImGui::Spacing();

	ImGuiButtonFlags submitFlag = 0;
	keyEvent[WINDOW_ON_SUBMIT].enabled = true;

	if (!nameString.Length() || !path.Length()) {
		submitFlag = ImGuiButtonFlags_Disabled;
		keyEvent[WINDOW_ON_SUBMIT].enabled = false;
		errMsg.SetString("Scene name and path can not be empty!");
	}

	if (submitFlag) {
		ImGui::TextColored(maroon, errMsg.c_str());
		ImGui::SameLine();
	}

	ImGui::Indent(imguiWindow->Size.x - 145.0f);
	if (ImGui::Button("Cancel", buttonSize))
		open = false;

	ImGui::SameLine();
	if (submitFlag)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.47f, 0.47f, 0.47f, 1.0f));

	if (ImGui::ButtonEx("Create", buttonSize, submitFlag) || keyEvent[WINDOW_ON_SUBMIT]) {
		SceneCreationInfo info;
		info.name = nameString;
		info.flipUVs = flipUV;
		info.type = SCENE_TYPE_CUSTOM;

		GetFileNameFromPath(path, info.file, info.directory);

		if (manager->NewScene(info))
			open = false;
	}

	if (submitFlag)
		ImGui::PopStyleColor();

	ImGui::Unindent();
	ImGui::End();

	if (!open) {
#if IMGUI_WINDOW_DEBUGGER
		WindowDebugger::window = nullptr;
#endif
		ui.Hide(this);
	}
}


/**
* New Texture window definition.
*/
void WindowNewTextureTemplate::Events() {
	if (!isActive)
		return;

	AppIO &io = application->io;
	WindowsHandler &ui = application->ui;

	if (io.IsKeyPressed(GLFW_KEY_ESCAPE))
		ui.Hide(this);

	keyEvent[WINDOW_ON_SUBMIT].onEvent = application->io.IsKeyPressed(GLFW_KEY_ENTER);
}


void WindowNewTextureTemplate::Draw() {
	WindowsHandler	&ui = application->ui;
	ResourceManager *manager = application->GetResourceHandler();

	bool open = true;
	ImVec2 buttonSize(60.0f, 0.0f);
	int32 windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin(name.c_str(), &open, windowFlags);
	ImGui::SetWindowSize(ImVec2(550.0f, 175.0f), ImGuiCond_Always);
	isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

	if (!imguiWindow)
		imguiWindow = ImGui::FindWindowByName(name.c_str());

#if IMGUI_WINDOW_DEBUGGER
	if (isActive)
		WindowDebugger::window = imguiWindow;
#endif

	static int		type = 0;
	static String	nameString;
	static String	path;
	static String	errMsg;

	const char *textureTypes = "None\0Diffuse\0Albedo\0Specular\0Roughness\0Metallic\0AO\0Normal\0Height\0Displacement\0Reflection\0Emissive\0Opacity";
	
	ImVec4 maroon = ImVec4(0.8039f, 0.2039f, 0.2313f, 1.0f);

	if (!nameString.Size()) {
		nameString.Reserve(512);
		nameString = "New Texture";
	}

	if (!path.Size()) {
		path.Reserve(512);
		path = __ROOT__;
	}

	ImGui::Text("New Texture details");
	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::BeginChild("Details", ImVec2(90.0f, 75.0f))) {
		ImGui::Text("Texture name");
		ImGui::SameLine(); ImGui::TextColored(maroon, "*");
		ImGui::Spacing();
		ImGui::Text("Path");
		ImGui::SameLine(); ImGui::TextColored(maroon, "*");
		ImGui::Spacing();
		ImGui::Text("Texture type");
		ImGuiCustomToolTip("It is important to provide the proper type that the texture will be used for!");
		ImGui::SameLine(); ImGui::TextColored(maroon, "*");
	}
	ImGui::EndChild();
	ImGui::SameLine();
	if (ImGui::BeginChild("Input", ImVec2(-1.0f, 75.0f))) {
		ImGui::PushItemWidth(-1.0f);
		ImGui::InputText("##TextureName", nameString.First(), 512);
		ImGui::PopItemWidth();

		if (ImGui::Button("Open", buttonSize)) {
			OpenNativeFileDialog(NATIVE_DIALOG_OPEN_FILE, path, "png,jpg,jpeg");
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(-1.0f);
		ImGui::InputText("##File", path.First(), 512);

		if (path.Length())
			ImGuiCustomToolTip(path.c_str());

		ImGui::Combo("##Type", &type, textureTypes);
		ImGui::PopItemWidth();
		ImGui::SameLine();
	}
	ImGui::EndChild();
	ImGui::Separator();
	ImGui::Spacing();

	ImGuiButtonFlags submitFlag = 0;
	keyEvent[WINDOW_ON_SUBMIT].enabled = true;

	if (!nameString.Length() || !path.Length() || type == 0) {
		submitFlag = ImGuiButtonFlags_Disabled;
		keyEvent[WINDOW_ON_SUBMIT].enabled = false;
		errMsg.SetString("Name and path can not be empty! Texture type can not be set to None");
	}

	if (submitFlag) {
		ImGui::TextColored(maroon, errMsg.c_str());
		ImGui::SameLine();
	}

	ImGui::Indent(imguiWindow->Size.x - 145.0f);
	if (ImGui::Button("Cancel", buttonSize))
		open = false;

	ImGui::SameLine();
	if (submitFlag)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.47f, 0.47f, 0.47f, 1.0f));

	if (ImGui::ButtonEx("Create", buttonSize, submitFlag) || keyEvent[WINDOW_ON_SUBMIT]) {
		TextureCreationInfo info;
		GenerateGenericTextureInfo(info);
		info.name = nameString;
		info.files.Push(String());
		info.textureType = static_cast<TextureType>(type);
		GetFileNameFromPath(path, info.files.Last(), info.directory);

		if (manager->NewTexture(info))
			open = false;
	}

	if (submitFlag)
		ImGui::PopStyleColor();

	ImGui::Unindent();
	ImGui::End();

	if (!open) {
#if IMGUI_WINDOW_DEBUGGER
		WindowDebugger::window = nullptr;
#endif
		ui.Hide(this);
	}
}


/**
* New Shader window definition.
*/
void WindowNewShaderTemplate::Events() {
	if (!isActive)
		return;

	AppIO &io = application->io;
	WindowsHandler &ui = application->ui;

	if (io.IsKeyPressed(GLFW_KEY_ESCAPE))
		ui.Hide(this);

	keyEvent[WINDOW_ON_SUBMIT].onEvent = application->io.IsKeyPressed(GLFW_KEY_ENTER);
}


void WindowNewShaderTemplate::Draw() {
	WindowsHandler	&ui = application->ui;
	ResourceManager *manager = application->GetResourceHandler();

	bool open = true;
	ImVec2 buttonSize(60.0f, 0.0f);
	int32 windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin(name.c_str(), &open, windowFlags);
	ImGui::SetWindowSize(ImVec2(550.0f, 200.0f), ImGuiCond_Always);
	isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

	if (!imguiWindow)
		imguiWindow = ImGui::FindWindowByName(name.c_str());

#if IMGUI_WINDOW_DEBUGGER
	if (isActive)
		WindowDebugger::window = imguiWindow;
#endif

	static String	nameString;
	static String	path[3];
	static String	errMsg;

	ImVec4 maroon = ImVec4(0.8039f, 0.2039f, 0.2313f, 1.0f);

	if (!nameString.Size()) {
		nameString.Reserve(512);
		nameString = "New Shader";
	}

	if (!path[0].Size() && !path[1].Size() && !path[2].Size()) {
		path[0].Reserve(512); path[1].Reserve(512); path[2].Reserve(512);
		path[0] = path[1] = path[2] = __ROOT__;
	}

	ImGui::Text("New Shader details");
	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::BeginChild("Details", ImVec2(110.0f, 100.0f))) {
		ImGui::Text("Shader name");
		ImGui::SameLine(); ImGui::TextColored(maroon, "*");
		ImGui::Spacing();
		ImGui::Text("Vertex shader");
		ImGui::SameLine(); ImGui::TextColored(maroon, "*");
		ImGui::Spacing();
		ImGui::Text("Fragment shader");
		ImGui::SameLine(); ImGui::TextColored(maroon, "*");
		ImGui::Spacing();
		ImGui::Text("Geometry shader");
	}
	ImGui::EndChild();
	ImGui::SameLine();
	if (ImGui::BeginChild("Input", ImVec2(-1.0f, 100.0f))) {
		ImGui::PushItemWidth(-1.0f);
		ImGui::InputText("##ShaderName", nameString.First(), 512);
		ImGui::PopItemWidth();

		if (ImGui::Button("Open##Vertex", buttonSize)) {
			OpenNativeFileDialog(NATIVE_DIALOG_OPEN_FILE, path[0], "vs");
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(-1.0f);
		ImGui::InputText("##Vertex", path[0].First(), 512);
		ImGui::PopItemWidth();

		if (ImGui::Button("Open##Fragment", buttonSize)) {
			OpenNativeFileDialog(NATIVE_DIALOG_OPEN_FILE, path[1], "fs");
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(-1.0f);
		ImGui::InputText("##Fragment", path[1].First(), 512);
		ImGui::PopItemWidth();

		if (ImGui::Button("Open##Geometry", buttonSize)) {
			OpenNativeFileDialog(NATIVE_DIALOG_OPEN_FILE, path[2], "gs");
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(-1.0f);
		ImGui::InputText("##Geometry", path[2].First(), 512);
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
	ImGui::Separator();
	ImGui::Spacing();

	ImGuiButtonFlags submitFlag = 0;
	keyEvent[WINDOW_ON_SUBMIT].enabled = true;

	if (!nameString.Length() || !path[0].Length() || !path[1].Length() || !path[2].Length()) {
		submitFlag = ImGuiButtonFlags_Disabled;
		keyEvent[WINDOW_ON_SUBMIT].enabled = false;
		errMsg.SetString("Name, vertex shader and fragment shader can not be empty!");
	}

	if (submitFlag) {
		ImGui::TextColored(maroon, errMsg.c_str());
		ImGui::SameLine();
	}

	ImGui::Indent(imguiWindow->Size.x - 145.0f);
	if (ImGui::Button("Cancel", buttonSize))
		open = false;

	ImGui::SameLine();
	if (submitFlag)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.47f, 0.47f, 0.47f, 1.0f));

	if (ImGui::ButtonEx("Create", buttonSize, submitFlag) || keyEvent[WINDOW_ON_SUBMIT]) {
		ShaderCreationInfo info;
		info.name = nameString;
		GetFileNameFromPath(path[0], info.vertexShader, info.directory);
		GetFileNameFromPath(path[1], info.fragmentShader, info.directory);

		if (path[2].Length())
			GetFileNameFromPath(path[2], info.geometryShader, info.directory);

		if (manager->NewShader(info))
			open = false;
	}

	if (submitFlag)
		ImGui::PopStyleColor();

	ImGui::Unindent();
	ImGui::End();

	if (!open) {
#if IMGUI_WINDOW_DEBUGGER
		WindowDebugger::window = nullptr;
#endif
		ui.Hide(this);
	}
}


/**
* New Material window definition.
*/
void WindowNewMaterialTemplate::Events() {
	if (!isActive)
		return;

	AppIO &io = application->io;
	WindowsHandler &ui = application->ui;

	if (io.IsKeyPressed(GLFW_KEY_ESCAPE))
		ui.Hide(this);

	keyEvent[WINDOW_ON_SUBMIT].onEvent = application->io.IsKeyPressed(GLFW_KEY_ENTER);
}


void WindowNewMaterialTemplate::Draw() {
	WindowsHandler	&ui = application->ui;
	ResourceManager *manager = application->GetResourceHandler();

	bool open = true;
	ImVec2 buttonSize(60.0f, 0.0f);
	int32 windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin(name.c_str(), &open, windowFlags);
	ImGui::SetWindowSize(ImVec2(550.0f, 450.0f), ImGuiCond_Always);
	isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

	if (!imguiWindow)
		imguiWindow = ImGui::FindWindowByName(name.c_str());

#if IMGUI_WINDOW_DEBUGGER
	if (isActive)
		WindowDebugger::window = imguiWindow;
#endif

	static uint				textureCount = 0;
	static String			nameString;
	static String			errMsg;
	static Shader			*selectedShader;
	static Array<Texture*>	selectedTextures;

	ImVec4 maroon = ImVec4(0.8039f, 0.2039f, 0.2313f, 1.0f);

	if (!nameString.Size()) {
		nameString.Reserve(512);
		nameString = "New Material";
	}

	if (!selectedTextures.Size())
		selectedTextures.Reserve(16);

	ImGui::Text("New Material details");
	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::BeginChild("Details", ImVec2(110.0f, 350.0f))) {
		ImGui::Text("Material name");
		ImGui::SameLine(); ImGui::TextColored(maroon, "*");
		ImGui::Spacing();
		ImGui::Text("Shader");
		ImGui::Spacing();
		ImGui::Text("Textures");
	}
	ImGui::EndChild();
	ImGui::SameLine();
	if (ImGui::BeginChild("Input", ImVec2(-1.0f, 350.0f))) {
		ImGui::PushItemWidth(-1.0f);
		ImGui::InputText("##MaterialName", nameString.First(), 512);

		if (ImGui::BeginCombo("##ShaderList", (selectedShader) ? selectedShader->info->name.c_str() : nullptr)) {
			for (ShaderData *data : manager->shaders) {
				if (ImGui::Selectable(data->name.c_str(), (selectedShader) ? selectedShader == data->shader : false))
					selectedShader = data->shader;
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		ImGuiButtonFlags flag = (textureCount == 11) ? ImGuiButtonFlags_Disabled : ImGuiBackendFlags_None;

		if (ImGui::ButtonEx("Add", buttonSize, flag))
			textureCount++;

		UINewMaterial::ShowTextureSelectionList(textureCount, selectedTextures, *manager);
	}
	ImGui::EndChild();
	ImGui::Separator();
	ImGui::Spacing();

	ImGuiButtonFlags submitFlag = 0;
	keyEvent[WINDOW_ON_SUBMIT].enabled = true;

	if (!nameString.Length()) {
		submitFlag = ImGuiButtonFlags_Disabled;
		keyEvent[WINDOW_ON_SUBMIT].enabled = false;
		errMsg.SetString("Name can not be empty!");
	}

	if (submitFlag) {
		ImGui::TextColored(maroon, errMsg.c_str());
		ImGui::SameLine();
	}

	ImGui::Indent(imguiWindow->Size.x - 145.0f);
	if (ImGui::Button("Cancel", buttonSize))
		open = false;

	ImGui::SameLine();
	if (submitFlag)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.47f, 0.47f, 0.47f, 1.0f));

	if (ImGui::ButtonEx("Create", buttonSize, submitFlag) || keyEvent[WINDOW_ON_SUBMIT]) {
		MaterialCreationInfo info;
		info.name	= nameString;
		info.shader = selectedShader;
		
		if (selectedTextures.Length())
			info.textures = selectedTextures;

		if (manager->NewMaterial(info))
			open = false;
	}

	if (submitFlag)
		ImGui::PopStyleColor();

	ImGui::Unindent();
	ImGui::End();

	if (!open) {
#if IMGUI_WINDOW_DEBUGGER
		WindowDebugger::window = nullptr;
#endif
		ui.Hide(this);
	}
}


/**
* New Scenery window definition.
*/
void WindowNewSceneryTemplate::Events() {
	if (!isActive)
		return;

	AppIO &io = application->io;
	WindowsHandler &ui = application->ui;

	if (io.IsKeyPressed(GLFW_KEY_ESCAPE))
		ui.Hide(this);

	keyEvent[WINDOW_ON_SUBMIT].onEvent = application->io.IsKeyPressed(GLFW_KEY_ENTER);
}


void WindowNewSceneryTemplate::Draw() {
	WindowsHandler	&ui = application->ui;
	ResourceManager *manager = application->GetResourceHandler();

	bool open = true;
	ImVec2 buttonSize(60.0f, 0.0f);
	int32 windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin(name.c_str(), &open, windowFlags);
	ImGui::SetWindowSize(ImVec2(550.0f, 200.0f), ImGuiCond_Always);
	isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

	if (!imguiWindow)
		imguiWindow = ImGui::FindWindowByName(name.c_str());

#if IMGUI_WINDOW_DEBUGGER
	if (isActive)
		WindowDebugger::window = imguiWindow;
#endif

	static bool		sameName = true;
	static String	nameString;
	static String	fileString;
	static String	dirString;
	static String	errMsg;

	ImVec4 maroon = ImVec4(0.8039f, 0.2039f, 0.2313f, 1.0f);

	if (!nameString.Size()) {
		nameString.Reserve(512);
		nameString = "New Level";
	}

	if (!fileString.Size()) {
		fileString.Reserve(512);
		fileString = nameString;
	}

	if (!dirString.Size()) {
		dirString.Reserve(512);
		dirString = __ROOT__;
	}

	ImGui::Text("New Scenery details");
	ImGui::Checkbox("Scenery and File share the same name?", &sameName);
	ImGui::Separator();
	ImGui::Spacing();

	if (sameName)
		fileString = nameString;

	if (ImGui::BeginChild("Details", ImVec2(110.0f, 80.0f))) {
		ImGui::Text("Scenery name");
		ImGui::SameLine(); ImGui::TextColored(maroon, "*");
		ImGui::Spacing();
		ImGui::Text("File");
		ImGui::Spacing();
		ImGui::Text("Directory");
	}
	ImGui::EndChild();
	ImGui::SameLine();
	if (ImGui::BeginChild("Input", ImVec2(-1.0f, 80.0f))) {
		ImGui::PushItemWidth(-1.0f);
		ImGui::InputText("##SceneryName", nameString.First(), 512);

		if (sameName)
			fileString = nameString;

		if (ImGui::InputText("##FileName", fileString.First(), 512))
			sameName = false;
		ImGui::PopItemWidth();

		if (ImGui::Button("Open", buttonSize)) {
			OpenNativeFileDialog(NATIVE_DIALOG_OPEN_PATH, dirString, nullptr);
		}
		ImGui::SameLine();
		ImGui::PushItemWidth(-1.0f);
		ImGui::InputText("##Path", dirString.First(), 512);
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
	ImGui::Separator();
	ImGui::Spacing();

	ImGuiButtonFlags submitFlag = 0;
	keyEvent[WINDOW_ON_SUBMIT].enabled = true;

	if (!nameString.Length()) {
		submitFlag = ImGuiButtonFlags_Disabled;
		keyEvent[WINDOW_ON_SUBMIT].enabled = false;
		errMsg.SetString("Name can not be empty!");
	}

	if (submitFlag) {
		ImGui::TextColored(maroon, errMsg.c_str());
		ImGui::SameLine();
	}

	ImGui::Indent(imguiWindow->Size.x - 145.0f);
	if (ImGui::Button("Cancel", buttonSize))
		open = false;

	ImGui::SameLine();
	if (submitFlag)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.47f, 0.47f, 0.47f, 1.0f));

	if (ImGui::ButtonEx("Create", buttonSize, submitFlag) || keyEvent[WINDOW_ON_SUBMIT]) {
		LevelCreationInfo info;
		info.name = nameString;
		info.filename = fileString;
		info.directory = dirString;

		if (manager->NewLevel(info))
			open = false;
	}

	if (submitFlag)
		ImGui::PopStyleColor();

	ImGui::Unindent();
	ImGui::End();

	if (!open) {
#if IMGUI_WINDOW_DEBUGGER
		WindowDebugger::window = nullptr;
#endif
		ui.Hide(this);
	}
}


void WindowNewSceneInstanceTemplate::Events() {
	if (!isActive)
		return;
}


void WindowNewSceneInstanceTemplate::Draw() {
	WindowsHandler &ui = application->ui;
	ResourceManager *manager = application->GetResourceHandler();

	bool open = true;
	ImVec2 buttonSize(60.0f, 0.0f);
	int32 windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin(name.c_str(), &open, windowFlags);
	ImGui::SetWindowSize(ImVec2(500.0f, 205.0f), ImGuiCond_Always);
	isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

	if (!imguiWindow)
		imguiWindow = ImGui::FindWindowByName(name.c_str());

#if IMGUI_WINDOW_DEBUGGER
	if (isActive)
		WindowDebugger::window = imguiWindow;
#endif

	static float	position[3]		= {0.0f};
	static float	scale[3]		= {1.0f};
	static float	rotation[3]		= {0.0f};
	static Shader	*selectedShader = nullptr;
	static String	errMsg;

	ImVec4 maroon = ImVec4(0.8039f, 0.2039f, 0.2313f, 1.0f);

	ImGui::Text("New Scene Instance details");
	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::BeginChild("Details", ImVec2(110.0f, 100.0f))) {
		ImGui::Text("Position");
		ImGui::Spacing();
		ImGui::Text("Scale");
		ImGui::Spacing();
		ImGui::Text("Rotation");
		ImGui::Spacing();
		ImGui::Text("Shader");
		ImGui::SameLine(); ImGui::TextColored(maroon, "*");
	}
	ImGui::EndChild();
	ImGui::SameLine();
	if (ImGui::BeginChild("Input", ImVec2(-1.0f, 100.0f))) {
		float childWidth = ImGui::GetWindowWidth();
		ImGui::PushItemWidth(-1.0f);
		ImGui::InputFloat3("##Position", position, 3);
		ImGui::InputFloat3("##Scale", scale, 3);
		ImGui::InputFloat3("##Rotation", rotation, 3);
		if (ImGui::BeginCombo("##ShaderList", (selectedShader) ? selectedShader->info->name.c_str() : nullptr)) {
			for (ShaderData *data : manager->shaders) {
				if (ImGui::Selectable(data->name.c_str(), (selectedShader) ? selectedShader == data->shader : false))
					selectedShader = data->shader;
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGuiButtonFlags submitFlag = 0;
	keyEvent[WINDOW_ON_SUBMIT].enabled = true;

	if (!selectedShader) {
		submitFlag = ImGuiButtonFlags_Disabled;
		keyEvent[WINDOW_ON_SUBMIT].enabled = false;
		errMsg.SetString("Shader can not be empty!");
	}

	if (submitFlag) {
		ImGui::TextColored(maroon, errMsg.c_str());
		ImGui::SameLine();
	}

	ImGui::Indent(imguiWindow->Size.x - 145.0f);
	if (ImGui::Button("Cancel", buttonSize))
		open = false;

	ImGui::SameLine();
	if (submitFlag)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.47f, 0.47f, 0.47f, 1.0f));

	if (ImGui::ButtonEx("Create", buttonSize, submitFlag) || keyEvent[WINDOW_ON_SUBMIT]) {
		SceneInstanceCreation info;
		info.position.x = position[0];
		info.position.y = position[1];
		info.position.z = position[2];
		info.scale.x	= scale[0];
		info.scale.y	= scale[1];
		info.scale.z	= scale[2];
		info.rotation.x = rotation[0];
		info.rotation.y = rotation[1];
		info.rotation.z = rotation[2];
		info.shader		= selectedShader;

		if (gHoveredScene->scene->CreateInstance(info))
			open = false;
	}

	if (submitFlag)
		ImGui::PopStyleColor();

	ImGui::Unindent();
	ImGui::End();

	if (!open) {
#if IMGUI_WINDOW_DEBUGGER
		WindowDebugger::window = nullptr;
#endif
		ui.Hide(this);
	}
}