#include "stdafx.h"


void WindowMenuBarTemplate::Events() {

}


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