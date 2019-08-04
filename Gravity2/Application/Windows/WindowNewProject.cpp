#include "stdafx.h"


void WindowNewProjectTemplate::Events() { return; }


void WindowNewProjectTemplate::Draw() {
	WindowsHandler &ui = application->ui;
	bool open = true;

	if (ImGui::BeginPopupModal(name.c_str(), &open)) {
		if (!open)
			ui.Hide(this);

		isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
		
		if (!imguiWindow)
			imguiWindow = ImGui::FindWindowByName(name.c_str());

		static bool		sameName = true;
		static String	nameString;
		static String	fileString;
		static String	dirString;

		nameString.Reserve(512, false);
		fileString.Reserve(512, false);
		dirString.Reserve(512, false);

		ImGui::Text("New project details");
		ImGui::NewLine();
		ImGui::Checkbox("File and project share same name?", &sameName);
		ImGui::Separator();

		ImGui::PushItemWidth(-1.0f);
		ImGui::InputText("Project name", nameString.First(), 512);

		if (sameName)
			fileString = nameString;

		ImGui::InputText("File name", fileString.First(), 512);
		ImGui::InputText("Directory", dirString.First(), 512);
		ImGui::PopItemWidth();
		ImGui::EndPopup();
	}
}