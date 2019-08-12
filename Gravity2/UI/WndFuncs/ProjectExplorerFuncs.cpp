#include "stdafx.h"


void UIProjectExplorer::ShowSceneList(GravityApp *Application) {
	static bool		init		= true;
	static size_t	selected	= -1;
	SceneData		*pData		= nullptr;
	ResourceManager *manager	= Application->GetResourceHandler();
	
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


void UIProjectExplorer::ShowTextureList(GravityApp *Application) {
	static bool		init = true;
	static size_t	selected = -1;
	TextureData		*pData = nullptr;
	ResourceManager *manager = Application->GetResourceHandler();

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


void UIProjectExplorer::ShowShaderList(GravityApp *Application) {
	static bool		init = true;
	static size_t	selected = -1;
	ShaderData		*pData = nullptr;
	ResourceManager *manager = Application->GetResourceHandler();

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
		params[2].Value = (pData->vertexFile.Length())	 ? pData->vertexFile.c_str() : "NULL";
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


void UIProjectExplorer::ShowMaterialList(GravityApp *Application) {
	static bool		init = true;
	static size_t	selected = -1;
	MaterialData	*pData = nullptr;
	ResourceManager *manager = Application->GetResourceHandler();

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

		ImGuiCustomColumnPairToolTip(params, 2);
		ImGui::NextColumn();
		ImGui::Text(pData->name.c_str());		ImGui::NextColumn();
	}
	ImGui::EndColumns();
	ImGui::EndChild();
	ImGui::PopStyleColor();
}


void UIProjectExplorer::ShowSceneryList(GravityApp *Application) {
	static bool		init = true;
	static size_t	selected = -1;
	SceneryData		*pData	 = nullptr;
	ResourceManager *manager = Application->GetResourceHandler();

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