#include "stdafx.h"



void WindowProjectExplorerTemplate::ShowSceneList() {
	static bool		init		= true;
	static size_t	selected	= -1;
	SceneData		*pData		= nullptr;
	ResourceManager *manager	= application->GetResourceHandler();
	
	Pair<const char*, const char*> params[4];
	params[0].Key = "ID";
	params[1].Key = "Name";
	params[2].Key = "File";
	params[3].Key = "Directory";

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.10f, 0.12f, 1.00f));
	//ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.062f, 0.082f, 0.092f, 1.0f));

	// This needs to be done in order to enable the horizontal scrollbar.
	ImGui::SetNextWindowContentSize(ImVec2(1000.0f, 0.0f));
	ImGui::BeginChild("SceneDataInfo", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::BeginColumns("##", 4);
	// Set column with on init.
	if (init) {
		ImGui::SetColumnWidth(0,  80.0f);
		ImGui::SetColumnWidth(1, 200.0f);
		ImGui::SetColumnWidth(2, 200.0f);
		init = false;
	}

	ImGui::Text("ID");			ImGui::NextColumn();
	ImGui::Text("Name");		ImGui::NextColumn();
	ImGui::Text("File");		ImGui::NextColumn();
	ImGui::Text("Directory");	ImGui::NextColumn();
	ImGui::Separator();

	// Right click to open context menu for each Scene on the list.
	//SceneListContextMenu();

	for (size_t i = 0; i < manager->scenes.Length(); i++) {
		pData = manager->scenes[i];
		char id[4];
		char label[32];
		sprintf(id, "%d", pData->id);
		sprintf(label, "%04d", pData->id);

		params[0].Value = id;
		params[1].Value = pData->name.c_str();
		params[2].Value = pData->file.c_str();
		params[3].Value = pData->directory.c_str();

		if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_PressedOnClick))
			selected = i;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && keyEvent[WINDOW_ON_RGTCLK]) {
			action = EXPLORER_SCENE_CONTEXT_MENU;
			selected = i;
		}
		
		hoveredScene = (selected == i) ? pData : hoveredScene;

		ImGuiCustomColumnPairToolTip(params, 4);
		ImGui::NextColumn();
		ImGui::Text(pData->name.c_str());		ImGui::NextColumn();
		ImGui::Text(pData->file.c_str());		ImGui::NextColumn();
		ImGui::Text(pData->directory.c_str());	ImGui::NextColumn();
	}

	ImGui::EndColumns();
	ImGui::EndChild();
	ImGui::PopStyleColor();
}


void WindowProjectExplorerTemplate::SceneListContextMenu() {
	if (!hoveredScene)
		return;

	bool close = false;
	ResourceManager *manager = application->GetResourceHandler();
	
	if (action == EXPLORER_SCENE_CONTEXT_MENU)
		ImGui::OpenPopup("Scene Context Menu");

	if (ImGui::BeginPopup("Scene Context Menu")) {
		// NOTE(Afiq):
		// We have to set the main window to active in order to make clicking work perfectly.
		// ImGui does not consider Popups to be a child window! Hence why we needed to do this.
		isActive = true;
		ImGui::Text(hoveredScene->name.c_str());
		if (ImGui::Selectable("New Scene Instance")) {
			
		}
		if (ImGui::Selectable("Scene Info")) {
			close = true;
		}
		if (ImGui::Selectable("Delete Scene")) {
			manager->DeleteScene(hoveredScene->name.c_str());
			close = true;
		}
		ImGui::EndPopup();
	}

	if (close)
		ImGui::CloseCurrentPopup();
}


void WindowProjectExplorerTemplate::MaterialListContextMenu() {
	if (!hoveredMat)
		return;

	bool close = false;
	ResourceManager *manager = application->GetResourceHandler();

	if (action == EXPLORER_MATERIAL_CONTEXT_MENU)
		ImGui::OpenPopup("Material Context Menu");

	if (ImGui::BeginPopup("Material Context Menu")) {
		if (ImGui::Selectable("Delete Material")) {
			manager->DeleteMaterial(hoveredMat->name.c_str());
			close = true;
		}

		ImGui::EndPopup();
	}

	if (close)
		ImGui::CloseCurrentPopup();
}


void WindowProjectExplorerTemplate::ShowTextureList() {
	static bool		init = true;
	static size_t	selected = -1;
	TextureData		*pData = nullptr;
	ResourceManager *manager = application->GetResourceHandler();

	Pair<const char*, const char*> params[4];
	params[0].Key = "ID";
	params[1].Key = "Name";
	params[2].Key = "File";
	params[3].Key = "Directory";

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.10f, 0.12f, 1.00f));
	//ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.062f, 0.082f, 0.092f, 1.0f));

	// This needs to be done in order to enable the horizontal scrollbar.
	ImGui::SetNextWindowContentSize(ImVec2(1000.0f, 0.0f));
	ImGui::BeginChild("TextureDataInfo", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::BeginColumns("##", 4);
	// Set column with on init.
	if (init) {
		ImGui::SetColumnWidth(0, 80.0f);
		ImGui::SetColumnWidth(1, 200.0f);
		ImGui::SetColumnWidth(2, 200.0f);
		init = false;
	}

	ImGui::Text("ID");			ImGui::NextColumn();
	ImGui::Text("Name");		ImGui::NextColumn();
	ImGui::Text("File");		ImGui::NextColumn();
	ImGui::Text("Directory");	ImGui::NextColumn();
	ImGui::Separator();
	for (size_t i = 0; i < manager->textures.Length(); i++) {
		pData = manager->textures[i];
		char id[4];
		char label[32];
		sprintf(id, "%d", pData->id);
		sprintf(label, "%04d", pData->id);

		params[0].Value = id;
		params[1].Value = pData->name.c_str();
		params[2].Value = pData->file.c_str();
		params[3].Value = pData->directory.c_str();

		if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick))
			selected = i;

		ImGuiCustomColumnPairToolTip(params, 4);
		ImGui::NextColumn();
		ImGui::Text(pData->name.c_str());		ImGui::NextColumn();
		ImGui::Text(pData->file.c_str());		ImGui::NextColumn();
		ImGui::Text(pData->directory.c_str());	ImGui::NextColumn();
	}
	ImGui::EndColumns();
	ImGui::EndChild();
	ImGui::PopStyleColor();
}


void WindowProjectExplorerTemplate::ShowShaderList() {
	static bool		init = true;
	static size_t	selected = -1;
	ShaderData		*pData = nullptr;
	ResourceManager *manager = application->GetResourceHandler();

	Pair<const char*, const char*> params[6];
	params[0].Key = "ID";
	params[1].Key = "Name";
params[2].Key = "Vertex Shader";
params[3].Key = "Fragment Shader";
params[4].Key = "Geometry Shader";
params[5].Key = "Directory";

ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.10f, 0.12f, 1.00f));
//ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.062f, 0.082f, 0.092f, 1.0f));

// This needs to be done in order to enable the horizontal scrollbar.
ImGui::SetNextWindowContentSize(ImVec2(1000.0f, 0.0f));
ImGui::BeginChild("ShaderDataInfo", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
ImGui::BeginColumns("##", 6);
// Set column with on init.
if (init) {
	ImGui::SetColumnWidth(0, 80.0f);
	ImGui::SetColumnWidth(1, 150.0f);
	ImGui::SetColumnWidth(2, 150.0f);
	ImGui::SetColumnWidth(3, 150.0f);
	ImGui::SetColumnWidth(4, 150.0f);
	init = false;
}

ImGui::Text("ID");				ImGui::NextColumn();
ImGui::Text("Name");			ImGui::NextColumn();
ImGui::Text("Vertex Shader");	ImGui::NextColumn();
ImGui::Text("Fragment Shader");	ImGui::NextColumn();
ImGui::Text("Geometry Shader");	ImGui::NextColumn();
ImGui::Text("Directory");		ImGui::NextColumn();
ImGui::Separator();
for (size_t i = 0; i < manager->shaders.Length(); i++) {
	pData = manager->shaders[i];
	char id[4];
	char label[32];
	sprintf(id, "%d", pData->id);
	sprintf(label, "%04d", pData->id);

	params[0].Value = id;
	params[1].Value = pData->name.c_str();
	params[2].Value = (pData->vertexFile.Length()) ? pData->vertexFile.c_str() : "NULL";
	params[3].Value = (pData->fragmentFile.Length()) ? pData->fragmentFile.c_str() : "NULL";
	params[4].Value = (pData->geometryFile.Length()) ? pData->geometryFile.c_str() : "NULL";
	params[5].Value = pData->directory.c_str();

	if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick))
		selected = i;

	ImGuiCustomColumnPairToolTip(params, 6);
	ImGui::NextColumn();
	ImGui::Text(pData->name.c_str());		ImGui::NextColumn();
	ImGui::Text(params[2].Value);			ImGui::NextColumn();
	ImGui::Text(params[3].Value);			ImGui::NextColumn();
	ImGui::Text(params[4].Value);			ImGui::NextColumn();
	ImGui::Text(pData->directory.c_str());	ImGui::NextColumn();
}
ImGui::EndColumns();
ImGui::EndChild();
ImGui::PopStyleColor();
}


void WindowProjectExplorerTemplate::ShowMaterialList() {
	static bool		init = true;
	static size_t	selected = -1;
	MaterialData	*pData = nullptr;
	ResourceManager *manager = application->GetResourceHandler();

	Pair<const char*, const char*> params[2];
	params[0].Key = "ID";
	params[1].Key = "Name";

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.10f, 0.12f, 1.00f));
	//ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.062f, 0.082f, 0.092f, 1.0f));

	// This needs to be done in order to enable the horizontal scrollbar.
	//ImGui::SetNextWindowContentSize(ImVec2(300.0f, 0.0f));
	ImGui::BeginChild("MaterialDataInfo", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::BeginColumns("##", 2);
	// Set column with on init.
	if (init) {
		ImGui::SetColumnWidth(0, 80.0f);
		ImGui::SetColumnWidth(1, 150.0f);
		init = false;
	}

	ImGui::Text("ID");				ImGui::NextColumn();
	ImGui::Text("Name");			ImGui::NextColumn();
	ImGui::Separator();
	for (size_t i = 0; i < manager->materials.Length(); i++) {
		pData = manager->materials[i];
		char id[4];
		char label[32];
		sprintf(id, "%d", pData->id);
		sprintf(label, "%04d", pData->id);

		params[0].Value = id;
		params[1].Value = pData->name.c_str();

		if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick))
			selected = i;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && keyEvent[WINDOW_ON_RGTCLK]) {
			action = EXPLORER_MATERIAL_CONTEXT_MENU;
			selected = i;
		}

		hoveredMat = (selected == i) ? pData : hoveredMat;

		ImGuiCustomColumnPairToolTip(params, 2);
		ImGui::NextColumn();
		ImGui::Text(pData->name.c_str());		ImGui::NextColumn();
	}
	ImGui::EndColumns();
	ImGui::EndChild();
	ImGui::PopStyleColor();
}


void WindowProjectExplorerTemplate::ShowSceneryList() {
	static bool		init = true;
	static size_t	selected = -1;
	SceneryData		*pData	 = nullptr;
	ResourceManager *manager = application->GetResourceHandler();

	Pair<const char*, const char*> params[4];
	params[0].Key = "ID";
	params[1].Key = "Name";
	params[2].Key = "File";
	params[3].Key = "Directory";

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.10f, 0.12f, 1.00f));
	//ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.062f, 0.082f, 0.092f, 1.0f));

	// This needs to be done in order to enable the horizontal scrollbar.
	ImGui::SetNextWindowContentSize(ImVec2(1000.0f, 0.0f));
	ImGui::BeginChild("SceneryDataInfo", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::BeginColumns("##", 4);
	// Set column with on init.
	if (init) {
		ImGui::SetColumnWidth(0, 80.0f);
		ImGui::SetColumnWidth(1, 150.0f);
		init = false;
	}

	ImGui::Text("ID");				ImGui::NextColumn();
	ImGui::Text("Name");			ImGui::NextColumn();
	ImGui::Text("File");			ImGui::NextColumn();
	ImGui::Text("Directory");		ImGui::NextColumn();
	ImGui::Separator();
	for (size_t i = 0; i < manager->levels.Length(); i++) {
		pData = manager->levels[i];
		char id[4];
		char label[32];
		sprintf(id, "%d", pData->id);
		sprintf(label, "%04d", pData->id);

		params[0].Value = id;
		params[1].Value = pData->name.c_str();
		params[2].Value = (pData->filename.Length())  ? pData->filename.c_str()  : "NULL";
		params[3].Value = (pData->directory.Length()) ? pData->directory.c_str() : "NULL";

		if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick))
			selected = i;

		ImGuiCustomColumnPairToolTip(params, 4);
		ImGui::NextColumn();
		ImGui::Text(pData->name.c_str());	ImGui::NextColumn();
		ImGui::Text(params[2].Value);		ImGui::NextColumn();
		ImGui::Text(params[3].Value);		ImGui::NextColumn();
	}
	ImGui::EndColumns();
	ImGui::EndChild();
	ImGui::PopStyleColor();
}


void UINewMaterial::ShowTextureSelectionList(uint &Count, Array<Texture*> &Textures, ResourceManager &Manager) {
	for (uint i = 0; i < Count; i++) {
		char comboID[64];
		sprintf_s(comboID, "##selectedTexture_%d", i);
		ImGui::PushItemWidth(345.0f);
		if (ImGui::BeginCombo(comboID, (Textures[i]) ? Textures[i]->info->name.c_str() : nullptr)) {
			for (TextureData *data : Manager.textures)
				if (ImGui::Selectable(data->name.c_str(), (Textures.Length()) ? Textures[i] == data->texture : false))
					Textures[i] = data->texture;

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		char buttonLabel[64];
		sprintf_s(buttonLabel, "Remove##%d", i);
		if (ImGui::Button(buttonLabel, ImVec2(60.0f, 0.0f))) {
			Count--;

			if (Textures[i])
				Textures.PopAt(i);
		}
	}
}