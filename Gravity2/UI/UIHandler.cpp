#include "stdafx.h"


/**
* Declare all windows here.
*/
WindowMenuBarTemplate			*WindowMenuBar			= nullptr;
WindowNewProjectTemplate		*WindowNewProject		= nullptr;
WindowProjectExplorerTemplate	*WindowProjectExplorer	= nullptr;


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
	
	application = Application;

	io.Fonts->Clear();
	pFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 15.0f);

	if (pFont) {
		pFont->FontSize = 15.0f;
		io.FontDefault = pFont;
	} else {
		io.Fonts->AddFontDefault();
	}

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
	InitNewWindow(WindowMenuBar, "Menu Bar", Application);
	InitNewWindow(WindowNewProject, "New Project", Application);
	InitNewWindow(WindowProjectExplorer, "Project Explorer", Application);
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

	WindowMenuBar->Events();
	WindowMenuBar->Draw();
	
	for (GravityWindow *window : activeWindows) {
		window->Events();
		window->Draw();
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
void WindowMenuBarTemplate::Events() { return; }


void WindowMenuBarTemplate::Draw() {
	WindowsHandler *ui = &application->ui;

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File")) {
		if (ImGui::BeginMenu("New")) {
			bool enableNewLevel = false;

			if (application->GetCurrentProject())
				enableNewLevel = true;

			if (ImGui::MenuItem("Project", "CTRL+Shift+N", false))
				ui->Show(WindowNewProject);


			if (ImGui::MenuItem("Scenery", "CTRL+N", false, enableNewLevel))
				printf("Adding new level ...\n");

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Open", "CTRL+O"))
			printf("Opening an existing project ...\n");

		if (ImGui::MenuItem("Save", "CTRL+S"))
			printf("Saving current project ...\n");

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

	if (application->io.IsKeyPressed(GLFW_KEY_ENTER))
		keyEvent[WINDOW_ON_SUBMIT].onEvent = true;
	else
		keyEvent[WINDOW_ON_SUBMIT].onEvent = false;

	if (application->io.IsKeyPressed(GLFW_KEY_ESCAPE))
		keyEvent[WINDOW_ON_CANCEL].onEvent = true;
	else 
		keyEvent[WINDOW_ON_CANCEL].onEvent = false;
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
		static String	nameString = "New Project";
		static String	fileString;
		static String	dirString = __ROOT__;

		if (!nameString.Size())
			nameString.Reserve(512);

		if (!fileString.Size())
			fileString.Reserve(512);

		if (!dirString.Size())
			dirString.Reserve(512);

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
		if (ImGui::Button("Cancel", buttonSize) || keyEvent[WINDOW_ON_CANCEL])
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
			ui.Show(WindowProjectExplorer);
		}

		if (submitFlag)
			ImGui::PopStyleColor();

		ImGui::Unindent();
		ImGui::EndPopup();
	}

	if (!open) {
#if IMGUI_WINDOW_DEBUGGER
		//WindowDebugger::window = nullptr;
#endif
		ui.Hide(this);
		ImGui::CloseCurrentPopup();
	}
}


/**
* Project explorer window definition.
*/
void WindowProjectExplorerTemplate::Events() { return; }


void WindowProjectExplorerTemplate::Draw() {
	WindowsHandler &ui = application->ui;

	bool open = true;

	ImGui::Begin(name.c_str(), &open);
	if (!open)
		ui.Hide(this);

	isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
	ImGui::End();
		
}